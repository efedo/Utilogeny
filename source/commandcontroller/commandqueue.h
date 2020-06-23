// Copyright 2017-20 Eric Fedosejevs
//

#pragma once
#include "Utilogeny/source/precomp.h"
#include "Utilogeny/source/commandcontroller/typedefs.h"
//#include "source/core/globals.h"

/**
 * A container class for submitted commands.
 */
class cQueuedCommand {
public:
	unsigned long getCommandNumber();
	void setCommandNumber(const tCommandNum &);
	std::string commandText;
	std::string GUIspecific;
	std::string keyword;
	std::vector<std::string> splitCommand;
	tCommandSource cmdSource;
	bool needGUIrefresh(); // Check if you need to refresh this setting in the GUI
	bool settingSuccess;
	cQueuedCommand(std::string, tCommandSource, const tCommandNum &);
	cQueuedCommand();
private:
	unsigned long commandNumber;
	bool forceUpdateSet;
	bool forceUpdateValue;
	cQueuedCommand(std::string, tCommandSource);
};

/**
 * The Command Queue class contains a queue of commands waiting to be executed.
 */
class cCommandQueue {
public:
	tCommandNum addScriptCmdToQueue(const std::string &);
	tCommandNum addConsoleCmdToQueue(const std::string &);
	tCommandNum addGUICmdToQueue(const std::string &);
	bool getNextCmd(cQueuedCommand &);
	bool isCommandComplete(const tCommandNum &) const;
	//bool isCommandExit(const tCommandNum&) const;
	void completeCommand(cQueuedCommand &);
	void setDefCompletion(const tCommandNum &, const tCommandNum &); // First command does not complete until another queued command completes
	/// Checks if a command is waiting on another
	bool isDeferred(const tCommandNum &) const;
	cCommandQueue();
private:
	friend class cCommandController;
	tCommandNum _lastCompletedCommand = 0;
	tCommandNum addCmdToQueue(const std::string &, const tCommandSource &);
	//bool exitTriggered = false;
	void removeDelayingCmdHold(const tCommandNum &);
	std::map<tCommandNum, cQueuedCommand *> queuedCmds;
	std::map<tCommandNum, cQueuedCommand *> runningCmds;
	struct {
		std::multimap<tCommandNum, tCommandNum> byWaitingCmd;
		std::multimap<tCommandNum, tCommandNum> byDelayingCmd;
	} defCompletion;
	std::mutex cmdQmutex;
	tCommandNum _nextCommandNumber;
};