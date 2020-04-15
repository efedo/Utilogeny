// Copyright 2017-18 The Curators of the University of Missouri
//

#pragma once

#include "Utilogeny/source/precomp.h"
//#include "source/core/globals.h"
#include "Utilogeny/source/commandcontroller/commandqueue.h"

//namespace nn {
//	class cNNCommandManager;
//}

bool wrongArgNum(const std::vector<std::string> &, unsigned int, unsigned int);
bool wrongArgNum(const std::vector<std::string> &, unsigned int);

class cCommand {
public:
	const std::string keyword;
	void(*execute)(cQueuedCommand &);
	cCommand(const std::string &, void(*)(cQueuedCommand &));
	cCommand(const std::string &, void(*)(cQueuedCommand &), const std::string &);
	
	// Change from function pointer to member function
	void run(const std::vector<std::string> &);
	std::string description;
	void help();
};

class cSetting {
public:
	const std::string keyword;
	bool(*set)(cQueuedCommand &, bool);
	cSetting(const std::string &, bool(*)(cQueuedCommand &, bool));
	cSetting(const std::string &, bool(*)(cQueuedCommand &, bool), const std::string &);
	std::string description;
	void help();
};

class cCommandController {
public:

	static cCommandController* get();

	// For RNA-see, need to attach signals for command setting, failure, success
	///
	///
	//
	///
	//cCommandController(std::mutex* _cmdControllerTermMutex, void (*tmpSignalSettingFailurePtr)(cQueuedCommand&), void (*tmpSignalCommandFailurePtr)(cQueuedCommand&));

	void (*signalSettingFailurePtr)(cQueuedCommand&) = 0;
	void (*signalCommandFailurePtr)(cQueuedCommand&) = 0;

	// Setup
	void addCommand(std::string, void(*)(cQueuedCommand&));
	void addCommand(std::string, void(*)(cQueuedCommand&), const std::string&);
	void removeCommand(std::string);
	void removeCommandIfPresent(std::string);
	void addSetting(std::string, bool(*)(cQueuedCommand&, bool));
	void addSetting(std::string, bool(*)(cQueuedCommand&, bool), const std::string&);
	void removeSetting(std::string);
	void clearCommandsSettings(); // Clears list of available commands
	
	// Interaction
	void printCommands(); // Prints a list of commands
	void printSettings(); // Prints a list of commands
	void changeSetting(cQueuedCommand &); // Proc setting

	tCommandNum queueConsoleCommand(const std::string &); // Add console command to command queue
	tCommandNum queueScriptCommand(const std::string &); // Add run command to command queue
	bool isCommandComplete(const tCommandNum &) const; // Asks command queue whether a command has completed
	void setDependentCompletion(const tCommandNum &, const tCommandNum &);

private:
	friend class cGUIUpdater;

	cCommandController();
	static void create();

	std::mutex cmdControllerTerminationMutex;
	bool keepRunning = true;
	std::atomic<bool> procLoopRunning = false;

	static cCommandController* myInstance;

	//void initCommandsStartupOnly(); // Fill startup command list
	//void initCommandsPostData(); // Fill post-startup command list
	//void initCommandsAlwaysAvailable(); // Fill post-startup command list
	//void initSettingsStartupOnly(); // Fill post-startup command list
	//void initSettingsPostDataOnly(); // Fill post-startup command list
	//void initSettingsAlwaysAvailable(); // Fill post-startup command list

	void commandConsoleLoop(); // User command loop
	void commandProcLoop(); // Command processing loop

	tCommandNum _queueGUICommand(const std::string &); // Add GUI command to command queue

	void procLine(cQueuedCommand &); // Proc command line
	void commandConsoleStep(); // User command loop

	std::map<std::string, cCommand> _commands;
	std::map<std::string, cSetting> _settings;
	unsigned int scriptrecursiondepth;

	std::thread * commandConsoleLoopThread;
	std::thread * commandProcessLoopThread;

	cCommandQueue queuedCmds;

	~cCommandController();
};

//Test