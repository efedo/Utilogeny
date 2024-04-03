// Copyright 2017-20 Eric Fedosejevs
//

#include "Utilogeny/source/core/lib/precomp.h"
#include "Utilogeny/source/core/lib/commandcontroller/commandqueue.h"
//#include "source/core/utils/utilities.h"
#include "Utilogeny/source/core/lib/exceptions.h"
#include "Utilogeny/source/core/lib/utilities.h"

cQueuedCommand::cQueuedCommand(std::string tmpCmdStr, tCommandSource tmpCmdSource, const tCommandNum & tmpCommandNumber)
	: cQueuedCommand(tmpCmdStr, tmpCmdSource)
{
	commandNumber = tmpCommandNumber;
}

cQueuedCommand::cQueuedCommand(std::string tmpCmdStr, tCommandSource tmpCmdSource) 
: commandText(tmpCmdStr), cmdSource(tmpCmdSource), keyword("missing"), settingSuccess(false)
{
	splitCommand = splitCommandLineFast(commandText);
	if (splitCommand.size() > 0) {
		keyword = splitCommand.at(0);
		to_lower(keyword);
	}
}

cQueuedCommand::cQueuedCommand()
	: cQueuedCommand("placeholder", tCommandSource::console, 0)
{}

unsigned long cQueuedCommand::getCommandNumber() {
	return commandNumber;
}

void cQueuedCommand::setCommandNumber(const tCommandNum & tmpCmdNumber) {
	commandNumber = tmpCmdNumber;
}

// Check if you need to refresh this setting in the GUI
// (if it was a successful console command or a failed GUI command)
bool cQueuedCommand::needGUIrefresh() {
	//throwl("Needs to be fixed!")
	return ((settingSuccess && ((cmdSource == tCommandSource::console)||(cmdSource == tCommandSource::script)) ||
		(!settingSuccess && (cmdSource == tCommandSource::GUI))));
}

cCommandQueue::cCommandQueue() 
	: _nextCommandNumber(1)
{}

tCommandNum cCommandQueue::addConsoleCmdToQueue(const std::string & tmpStr) {
	return addCmdToQueue(tmpStr, tCommandSource::console);
}

tCommandNum cCommandQueue::addScriptCmdToQueue(const std::string & tmpStr) {
	return addCmdToQueue(tmpStr, tCommandSource::script);
}

tCommandNum cCommandQueue::addGUICmdToQueue(const std::string & tmpStr) {
//	std::cout << tmpStr << "\n>";
	return addCmdToQueue(tmpStr, tCommandSource::GUI);
}

tCommandNum cCommandQueue::addCmdToQueue(const std::string & tmpStr, const tCommandSource & tmpSource) {
	cmdQmutex.lock();
	tCommandNum tmpCmdNum = _nextCommandNumber;
	cQueuedCommand* const newCmd = new cQueuedCommand(tmpStr, tmpSource, _nextCommandNumber);
	queuedCmds.emplace(_nextCommandNumber, newCmd);
	++_nextCommandNumber;

	//if (newCmd->keyword == "exit") {
	//	exitTriggered = true; // Once the exit keyword has been triggered
	//}

	cmdQmutex.unlock();
	return tmpCmdNum;
}

bool cCommandQueue::getNextCmd(cQueuedCommand & tmpCmd) {
	bool haveCommand = false;
	cmdQmutex.lock();
	if (!queuedCmds.empty()) {
		runningCmds.insert(*queuedCmds.begin());
		tmpCmd = *queuedCmds.begin()->second;
		queuedCmds.erase(queuedCmds.begin());
		haveCommand = true;
	}
	cmdQmutex.unlock();
	return haveCommand;
}

bool cCommandQueue::isCommandComplete(const tCommandNum & tmpCmd) const {
	if (tmpCmd <= _lastCompletedCommand) {

		const_cast<std::mutex *>(&cmdQmutex)->lock();

		const bool isQueued = queuedCmds.count(tmpCmd);
		const bool _isRunning = runningCmds.count(tmpCmd);
		const bool isDeferredBool = isDeferred(tmpCmd);

		const_cast<std::mutex *>(&cmdQmutex)->unlock();

		if (isQueued || _isRunning || isDeferredBool) {
			return false;
		}
		else {
			return true;
		}
	}
	else {
		return false;
	}
}

/// Checks if a command is waiting on another
bool cCommandQueue::isDeferred(const tCommandNum & tmpCmd) const {
	if (defCompletion.byWaitingCmd.count(tmpCmd)) {
		return true;
	}
	else {
		return false;
	}
}

void cCommandQueue::completeCommand(cQueuedCommand & completedCmd) {

	cmdQmutex.lock();
	tCommandNum completedCmdNum = completedCmd.getCommandNumber();
	if (completedCmdNum > _lastCompletedCommand) {
		_lastCompletedCommand = completedCmdNum;
	}

	if (queuedCmds.count(completedCmdNum)) {
#ifdef DEBUG
		queuedCmds.erase(completedCmdNum);
		std::cerr << "Strange, a command completed without running.\n";
#endif //DEBUG
	}

	if (runningCmds.count(completedCmdNum)) {
		delete runningCmds.at(completedCmdNum); // delete command
		runningCmds.erase(completedCmdNum);
	}

	if (!isDeferred(completedCmdNum)) removeDelayingCmdHold(completedCmdNum);
	cmdQmutex.unlock();
}

void cCommandQueue::removeDelayingCmdHold(const tCommandNum & delayingCmd) {
	// Remove deferred command holds if this command is no longer held up
	while (defCompletion.byDelayingCmd.count(delayingCmd)) {
		if (isDeferred(delayingCmd)) break;
		std::multimap<tCommandNum, tCommandNum>::iterator found = defCompletion.byDelayingCmd.find(delayingCmd);
		tCommandNum delayedCmd = found->second;

		// Gets range for delayed command... needs to find this entry and remove it
		std::pair <std::multimap<tCommandNum, tCommandNum>::iterator, std::multimap<tCommandNum, tCommandNum>::iterator> delRange;
		delRange = defCompletion.byWaitingCmd.equal_range(delayedCmd);

		// Finds and removes this delaying cmd
		for (std::multimap<tCommandNum, tCommandNum>::iterator it = delRange.first; it != delRange.second; ++it) {
			if (it->second == delayingCmd) {
				std::multimap<tCommandNum, tCommandNum>::iterator oldit = it;
				++it;
				defCompletion.byWaitingCmd.erase(oldit);
				if (it == delRange.second) break;
			}
		}

		// Check if you erased the last hold on the command and the command was complete
		// in which case freeing should propagate
		if (!defCompletion.byWaitingCmd.count(delayedCmd)) {
			if (!(queuedCmds.count(delayedCmd) || runningCmds.count(delayedCmd))) {
				removeDelayingCmdHold(delayedCmd);
			}
		}

		defCompletion.byDelayingCmd.erase(found);
	}
}

/// First command does not complete until another queued command completes
void cCommandQueue::setDefCompletion(const tCommandNum & waitingCmd, const tCommandNum & delayingCmd) {
#ifdef DEBUG
	if (waitingCmd > _nextCommandNumber) throwl("Tried to delay a command that has not yet been queued");
#endif //DEBUG
	if (isCommandComplete(delayingCmd)) return;
	cmdQmutex.lock();
	defCompletion.byWaitingCmd.emplace(waitingCmd, delayingCmd);
	defCompletion.byDelayingCmd.emplace(delayingCmd, waitingCmd);
	cmdQmutex.unlock();
}
