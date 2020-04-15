// Copyright 2017-18 The Curators of the University of Missouri
//


//#include "source/core/stdafx.h"
#include "Utilogeny/source/precomp.h"
#include "Utilogeny/source/Utilogeny.h"
#include "Utilogeny/source/exceptions.h"
#include "Utilogeny/source/utilities.h"
#include "Utilogeny/source/commandcontroller/commandqueue.h"
#include "Utilogeny/source/commandcontroller/commandcontroller.h"
#include "Utilogeny/source/commandcontroller/consolecontroller.h"

//#include "source/core/commandcontroller/commands/commands.h"
//#include "source/core/commandcontroller/settings/settings.h"
//#include "source/core/globals.h"
//#include "source/core/datatable/datatable.h"
//#include "source/core/utils/bioutils.h"
//#include "source/core/stats/statistics.h"
//#include "source/core/correlnet/correlnet_graph.h"

//#include "source/core/signal/signalrelay.h"

//#include "source/core/neuralnet/commands/nncmdmanager.h"

// Command/setting utility functions
bool wrongArgNum(const std::vector<std::string> & args, unsigned int min, unsigned int max) {
	if ((args.size() < min) || (args.size() > max)) {
		std::cerr << "Incorrect number of arguments.\n";
		return true;
	}
	return false;
}

bool wrongArgNum(const std::vector<std::string> & args, unsigned int num) {
	if (args.size() != num) {
		std::cerr << "Incorrect number of arguments.\n";
		return true;
	}
	return false;
}

cCommand::cCommand(const std::string & tmpKeyword, void(*tmpFuncPtr)(cQueuedCommand &) )
	: cCommand(tmpKeyword, tmpFuncPtr, "No description of command provided")
{}

cCommand::cCommand(const std::string & tmpKeyword, void(*tmpFuncPtr)(cQueuedCommand &),
	const std::string & tmpDescript)
	: keyword(tmpKeyword), execute(tmpFuncPtr), description(tmpDescript)
{}

cSetting::cSetting(const std::string & tmpKeyword, bool(*tmpFuncPtr)(cQueuedCommand &, bool))
	: cSetting(tmpKeyword, tmpFuncPtr, "No description of setting provided")
{}

cSetting::cSetting(const std::string & tmpKeyword, bool(*tmpFuncPtr)(cQueuedCommand &, bool), const std::string & tmpDescript)
	: keyword(tmpKeyword), set(tmpFuncPtr), description(tmpDescript)
{}

cCommandController* myInstance = 0;

cCommandController* cCommandController::get() {
	if (!myInstance) create(); 
	return myInstance;
}

void cCommandController::create() {
	myInstance = new cCommandController;
}

cCommandController::cCommandController()
	:	scriptrecursiondepth(0) , commandConsoleLoopThread(0), commandProcessLoopThread(0)
{

	//// If you try to load more than one command controller, throw
	//if (globals::commandControllerPtr) throw_line("Tried to load multiple command controllers");
	//globals::commandControllerPtr = this;

	// If you already have a command controller thread, something has gone wrong; throw
	if (commandConsoleLoopThread) {
		std::cerr << "Already have a command console thread!\n";
		throw_line("Bad thread flow!\n");
	}

	std::cout << "Initializing console.\n";

	// Make a new command input loop thread (gets user console inputs)
	commandConsoleLoopThread = new std::thread(&cCommandController::commandConsoleLoop, this);

	if (commandProcessLoopThread) {
		std::cerr << "Already have a command processing thread!\n";
		throw_line("Bad thread flow!\n");
	}

	// Make a new command processing loop thread
	commandProcessLoopThread = new std::thread(&cCommandController::commandProcLoop, this);

	// Need to wait until procLoop has the termination mutex lock to prevent premature termination
	while (!procLoopRunning) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

cCommandController::~cCommandController() {

	keepRunning = false;

	if (commandConsoleLoopThread) {
		commandConsoleLoopThread->join(); // Note that this will never happen unless you trigger cin
		delete commandConsoleLoopThread;
	}

	if (commandProcessLoopThread) {
		commandProcessLoopThread->join(); // Should be fine
		delete commandProcessLoopThread;
	}
}

// Clears list of available commands
void cCommandController::clearCommandsSettings() {
	_commands.clear();
	_settings.clear();
}


//// Fill command/settings lists
//void cCommandController::initStartup() {
//	clearCommandsSettings();
//	initCommandsAlwaysAvailable();
//	initSettingsAlwaysAvailable();
//	initCommandsStartupOnly();
//	initSettingsStartupOnly();
//}
//
//void cCommandController::initPostData() {
//	clearCommandsSettings();
//	initCommandsAlwaysAvailable();
//	initSettingsAlwaysAvailable();
//	initCommandsPostData();
//	initSettingsPostDataOnly();
//}

//// Fill startup command list
//void cCommandController::initCommandsStartupOnly() {
//	addCommand("load", &commands::load);
//
//	addCommand("badrun", &commands::badrun);
//	addCommand("goodrun", &commands::goodrun);
//	addCommand("remruns", &commands::remruns);
//	addCommand("gathersalmon", &commands::gathersalmon);
//	addCommand("combinetabs", &commands::combinetabs);
//}
//
//void cCommandController::initCommandsPostData() {
//	addCommand("close", &commands::close);
//	addCommand("find", &commands::find);
//	addCommand("findrun", &commands::findrun);
//
//	// Name genes, assign gene settings
//	addCommand("goi", &commands::goi);
//	addCommand("trp", &commands::trp);
//	addCommand("group", &commands::group);
//
//	addCommand("geneinfo", &commands::geneinfo);
//	addCommand("groupinfo", &commands::groupinfo);
//	addCommand("listgois", &commands::listgois);
//	addCommand("listgroups", &commands::listgroups);
//	addCommand("runinfo", &commands::runinfo);
//	addCommand("rundiff", &commands::rundiff);
//	addCommand("syn", &commands::syn);
//	addCommand("validate", &commands::validate);
//
//	// Comprehensive regression functions
//	addCommand("reg", &commands::reg);
//	addCommand("minireg", &commands::minireg);
//
//	// Pairwise analysis functions
//	addCommand("plot", &commands::plot);
//	addCommand("hill", &commands::hill);
//	addCommand("lin", &commands::lin);
//
//	// Correlation graph/cache commands
//
//	addCommand("correlgraph", &commands::correlgraph);
//	addCommand("correlgraphgois", &commands::correlgraphgois);
//	addCommand("precomp", &commands::precomp);
//
//	// Learning commands
//	addCommand("learn", &commands::learn);
//	addCommand("learnrem", &commands::learnrem);
//	addCommand("learnadd", &commands::learnadd);
//	addCommand("learnreset", &commands::learnreset);
//	addCommand("learndesc", &commands::learndesc);
//
//	// Print info commands
//	addCommand("printruns", &commands::printRunTable);
//
//	// Run info commands
//	addCommand("findclosest", &commands::closestRuns);
//
//	// Benchmarking commands
//	addCommand("bm_pearsoncsv", &commands::printPearsonCSV);
//
//	// Memory info commands
//	addCommand("meminfo", &commands::printMemInfo);
//}
//
//// Fill list with commands that are always available
//void cCommandController::initCommandsAlwaysAvailable() {
//	addCommand("exit", &commands::smoothexitargs);
//	addCommand("help", &commands::help);
//	addCommand("set", &commands::set);
//	addCommand("echo", &commands::echo);
//	addCommand("pause", &commands::pause);
//	addCommand("script", &commands::script);
//	addCommand("log", &commands::log);
////	addCommand("weitest", &commands::weitest);
//
//	#ifndef RNASEE_FINALRELEASE
//	addCommand("testphysics", &commands::testphysics);
//
//	// Add appropriate neural network commands
//	nn::cNNCommandManager::setAppropriateCommands();
//	#endif //RNASEE_FINALRELEASE
//
//}
//
//// Fill startup settings list
//void cCommandController::initSettingsStartupOnly() {
//	addSetting("datasource", &commands::cmdsettings::datasource);
//
//	addSetting("renorm_maxiters", &commands::cmdsettings::renorm_maxiters);
//
//	addSetting("warn_lowgenes", &commands::cmdsettings::warnLowGeneProp);
//	addSetting("warn_allgenes", &commands::cmdsettings::warnAllGeneProp);
//	addSetting("warn_scale", &commands::cmdsettings::warnScaleFactor);
//	addSetting("warn_housekeepingprop", &commands::cmdsettings::warnHousekeepingProp);
//	addSetting("warn_housekeepingdev", &commands::cmdsettings::warnHousekeepingDev);
//	addSetting("badruntreatment", &commands::cmdsettings::badRunTreatment);
//
//	addSetting("run_clustering", &commands::cmdsettings::runClustering);
////	addSetting("calc_expected", &commands::cmdsettings::calcExpectationCorrected);
//}
//
//// Fill post-startup settings list
//void cCommandController::initSettingsPostDataOnly() {
////	addSetting("mutimportance", &commands::settings::reg_mutualimportance);
//	addSetting("nl_miniiters", &commands::cmdsettings::nl_miniiters);
//	addSetting("nl_fulliters", &commands::cmdsettings::nl_fulliters);
//	addSetting("nl_talky", &commands::cmdsettings::nl_talky);
//
//	addSetting("correl_damping", &commands::cmdsettings::correlnet_importancedamping);
//	addSetting("deftrp", &commands::cmdsettings::deftrp);
//}
//
//void cCommandController::initSettingsAlwaysAvailable() {
//
//}

void cCommandController::addCommand(std::string tmpCommandString, 
	void(*tmpFunc)(cQueuedCommand &)) {
	_commands.emplace(tmpCommandString, cCommand(tmpCommandString, tmpFunc));
}

void cCommandController::addCommand(std::string tmpCommandString, 
	void(*tmpFunc)(cQueuedCommand &), const std::string & tmpDescript) {
	_commands.emplace(tmpCommandString, cCommand(tmpCommandString, tmpFunc, tmpDescript));
}

void cCommandController::removeCommand(std::string tmpCommandString) {
	if (_commands.count(tmpCommandString)) {
		_commands.erase(tmpCommandString);
	}
	else {
		std::cerr << "Could not remove command \"" << tmpCommandString << "\"\n";
	}
}

void cCommandController::removeCommandIfPresent(std::string tmpCommandString) {
	if (_commands.count(tmpCommandString)) {
		_commands.erase(tmpCommandString);
	}
}

void cCommandController::addSetting(std::string tmpCommandString, 
	bool(*tmpFunc)(cQueuedCommand &, bool)) {
	_settings.emplace(tmpCommandString, cSetting(tmpCommandString, tmpFunc));
}

void cCommandController::addSetting(std::string tmpCommandString, 
	bool(*tmpFunc)(cQueuedCommand &, bool), const std::string & tmpDescript) {
	_settings.emplace(tmpCommandString, cSetting(tmpCommandString, tmpFunc, tmpDescript));
}

void cCommandController::removeSetting(std::string tmpCommandString) {
	if (_settings.count(tmpCommandString)) _settings.erase(tmpCommandString);
}

// Prints a list of commands
void cCommandController::printCommands() {
	std::cout << "Available commands are currently as follows:" << std::endl;
	for (std::map<std::string, cCommand>::iterator it = _commands.begin(); it != _commands.end(); ++it) {
		std::cout << '\t' << it->first << std::endl;
	}
}

// Prints a list of settings
void cCommandController::printSettings() {
	std::cout << "Available settings are currently as follows:" << std::endl;
	for (std::map<std::string, cSetting>::iterator it = _settings.begin(); it != _settings.end(); ++it) {
		std::cout << '\t' << it->first << std::endl;
	}
}

// User command loop
void cCommandController::commandConsoleLoop() {
	std::cout << "\n>";
	while (keepRunning) {
		commandConsoleStep();
	}
}

// Command input loop
inline void cCommandController::commandConsoleStep() {
	std::string commandline;
	std::getline(std::cin, commandline); // Blocking!
	queueConsoleCommand(commandline);
}

// Command processing loop
void cCommandController::commandProcLoop() {

	// Lock the termination mutex to indicate that you are not terminated
	cmdControllerTerminationMutex.lock();

	procLoopRunning = true;

	cQueuedCommand nextCmd;
	while (keepRunning) {

		if (queuedCmds.getNextCmd(nextCmd)) {
			procLine(nextCmd);
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}

	// Lock the termination mutex to indicate that you are not terminated
	cmdControllerTerminationMutex.unlock();
}

void cCommandController::setDependentCompletion(const tCommandNum & delayedCmd, const tCommandNum & delayingCmd) {
	queuedCmds.setDefCompletion(delayedCmd, delayingCmd);
}

// Add GUI command to command queue
tCommandNum cCommandController::_queueGUICommand(const std::string & line) {
	return queuedCmds.addGUICmdToQueue(line);
}

// Add console command to command queue
tCommandNum cCommandController::queueConsoleCommand(const std::string & line) {
	return queuedCmds.addConsoleCmdToQueue(line);
}

// Add run command to command queue
tCommandNum cCommandController::queueScriptCommand(const std::string & line) {
	return queuedCmds.addScriptCmdToQueue(line);
}

bool cCommandController::isCommandComplete(const tCommandNum & tmpCmdNum) const {
	return queuedCmds.isCommandComplete(tmpCmdNum);
}

// Process command line
void cCommandController::procLine(cQueuedCommand & cmd) {

	if (cmd.splitCommand.size() > 0) {

		// If run command, determine whether to mirror based on setting

		std::cout << "\b#exec: " << cmd.commandText << "\n\n";

		if (_commands.count(cmd.keyword)) {
			try {
				_commands.at(cmd.keyword).execute(cmd);
			}
			catch (...) {
				std::cerr << "\nException in command:\n"
					<< "\tCommand num: " << cmd.getCommandNumber() << "\n"
					<< "\tCommand string: " << cmd.commandText << "\n";
				try {
					std::rethrow_exception(std::current_exception());
				}
				catch (const std::exception& e) {
					print_exception(e);
				} catch (...) {
					std::cerr << "Exception was not traceable.\n";
				}

				// Pop-up in GUI
				//if (globals::guiSignallerPtr) globals::guiSignallerPtr->sig_exceptOccurred();
				signalCommandFailurePtr(cmd);

			}
		}
		else {
			std::cerr << "Invalid command: " << cmd.keyword << ".\n";
		}
		queuedCmds.completeCommand(cmd); // Set as complete regardless so that execution can continue
		std::cout << "\n";
	}

	// Unmute the console (if muted for script input etc.) at the appropriate time
	if (Utilogeny::log::muteConsole && queuedCmds.isCommandComplete(Utilogeny::log::unmuteConsoleAfterCommand)) {
		unMuteConsole();
		std::cout << "\n";
	}

	std::cout << ">";
	std::cout.flush();
}

// Proc setting
void cCommandController::changeSetting(cQueuedCommand & cmd) {
	std::string settingKeyword = cmd.splitCommand.at(1);
	toLower(settingKeyword);
	if (_settings.count(settingKeyword)) {
		if (cmd.splitCommand.size() > 2) {
			if (!_settings.at(settingKeyword).set(cmd, true)) {
				// If command failed and it was a GUI command
				if (cmd.cmdSource == tCommandSource::GUI) {
					//if (globals::guiSignallerPtr) {
					//	globals::guiSignallerPtr->sig_settingFailure(cmd);
					//}
					signalSettingFailurePtr(cmd);
					//else {
					//	throw_line("GUI updater pointer missing."); // If it was a GUI command but you don't have a GUI updater pointer
					//	// something went horribly wrong
					//}
				}
			}
		}
		else {
			if (cmd.cmdSource == tCommandSource::GUI) throw_line("GUI should not be asking to output settings to console"); // GUI should not need to ask for settings to display
			_settings.at(cmd.splitCommand.at(1)).set(cmd, false);
		}
	}
	else {
		if (cmd.cmdSource == tCommandSource::GUI) {
			//if (globals::guiSignallerPtr) {
			//	globals::guiSignallerPtr->sig_settingFailure(cmd);
			//}
			//else {
			//	throw; // If it was a GUI command but you don't have a GUI updater pointer
			//	// something went horribly wrong
			//}
			signalSettingFailurePtr(cmd);
		}
		else {
			std::cerr << "Invalid setting." << std::endl;
		}
	}
}
