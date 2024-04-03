// Copyright 2017-20 Eric Fedosejevs
//

#pragma once

#include "Utilogeny/source/core/lib/precomp.h"
//#include "source/core/globals.h"
#include "Utilogeny/source/core/lib/commandcontroller/commandqueue.h"
#include "Utilogeny/source/core/lib/console/termcolor.h"

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
	virtual void printhelp() { // Eventually allow overrides with custom help functions
		std::cout << termcolor::bold << termcolor::blue << termcolor::on_bright_white << keyword << termcolor::reset << "\n";
		std::cout << description << "\n";
	}
};

class cSetting {
public:
	const std::string keyword;
	bool(*set)(cQueuedCommand &, bool);
	cSetting(const std::string &, bool(*)(cQueuedCommand &, bool));
	cSetting(const std::string &, bool(*)(cQueuedCommand &, bool), const std::string &);
	std::string description;
	virtual void printhelp() { // Eventually allow overrides with custom help functions
		std::cout << termcolor::bold << termcolor::blue << termcolor::on_bright_white << keyword << termcolor::reset << "\n";
		std::cout << description << "\n";
	}
};

class cCommandController {
public:

	static cCommandController* get();

	static void init();
	static void terminate();
	static void wait_for_termination();

	~cCommandController();

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
	void printItemHelp(std::string); // Prints help for a specific command or settings
	void changeSetting(cQueuedCommand &); // Proc setting

	// Queue commands
	tCommandNum queueConsoleCommand(const std::string &); // Add console command to command queue
	tCommandNum queueScriptCommand(const std::string &); // Add run command to command queue
	tCommandNum queueGUICommand(const std::string&); // Add GUI command to command queue

	// Determine if command is complete, set dependencies
	bool isCommandComplete(const tCommandNum &) const; // Asks command queue whether a command has completed
	void setDependentCompletion(const tCommandNum &, const tCommandNum &);

private:
	friend class cGUIUpdater;

	cCommandController(void (*tmpSignalSettingFailurePtr)(cQueuedCommand&), void (*tmpSignalCommandFailurePtr)(cQueuedCommand&));

	std::mutex cmdControllerTerminationMutex;
	bool keepRunning = true;
	std::atomic<bool> procLoopRunning = false;

	static cCommandController* myInstance;

	void commandConsoleLoop(); // User command loop
	void commandProcLoop(); // Command processing loop

	void procLine(cQueuedCommand &); // Proc command line
	void commandConsoleStep(); // User command loop

	std::map<std::string, cCommand> _commands;
	std::map<std::string, cSetting> _settings;
	unsigned int scriptrecursiondepth;

	std::thread * commandConsoleLoopThread;
	std::thread * commandProcessLoopThread;

	cCommandQueue queuedCmds;
};