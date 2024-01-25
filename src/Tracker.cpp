/* `track' utility to display resource usage of processes, main source file.
   Copyright (C) 2024 Heitor Machado.

   Track was inspired on GNU Time
        by David Keppel <pardo@cs.washington.edu>.
        Heavily modified by David MacKenzie <djm@gnu.ai.mit.edu>.
        Heavily modified (again) by Assaf Gordon <assafgordon@gmail.com>.

   This file is part of Track.

   Track is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Track is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GNU Time.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Tracker.hpp"
#include "Reporter.hpp"
#include <string>
#include <iostream>
#include <chrono>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include "fmt/color.h"
#include "NeedsHelpException.hpp"
#include "ExecutionError.hpp"
#include <sstream>

using namespace std;
using namespace std::chrono;

/**
 * Parses the program input
 * @param inputParams struct used to store the parsed params
 */
void Tracker::parseCommandLine(InputParams& inputParams) {

	if(inputParams.argc < 2) {
		throw NeedsHelpException("Target command to track must be provided");
	}

	int startIdx = -1;
	for(int i = 1; i < inputParams.argc; i++) {
		if(startIdx < 0 && inputParams.argv[i][0] != '-') { // start index for the target command
			startIdx = i;
		}
		char* cur = &inputParams.argv[i][0];
		if(flags::hasFlag(cur, flags::help)) {
			throw NeedsHelpException();
		}
		if(flags::hasFlag(cur, flags::timer)) {
			inputParams.selective = true;
			inputParams.timer = true;
		} else
		if(flags::hasFlag(cur, flags::cpu)) {
			inputParams.selective = true;
			inputParams.cpu = true;
		} else
		if(flags::hasFlag(cur, flags::mem)) {
			inputParams.selective = true;
			inputParams.mem = true;
		} else
		if(flags::hasFlag(cur, flags::io)) {
			inputParams.selective = true;
			inputParams.io = true;
		} else if(inputParams.argv[i][0] == '-' && startIdx < 0) {
			cerr << "flag not found: " << cur << "\n";
			throw NeedsHelpException();
		};
	}

	inputParams.targetArgsSize = inputParams.argc - startIdx;
	inputParams.targetArgs = reinterpret_cast<char**>(new char[inputParams.targetArgsSize + 1][100]);
	for(int i = 0; i < inputParams.targetArgsSize; i++) {
		inputParams.targetArgs[i] = inputParams.argv[startIdx + i];
	}
	inputParams.targetArgs[inputParams.targetArgsSize] = NULL;

}

/**
 * Acquire target and proceed to tracking
 * @param inputParams parsed input params
 */
void Tracker::track(InputParams& inputParams) {

	int pipeId[2];
	int successPipe = pipe(pipeId);
	if(successPipe == -1) throw ExecutionError(strerror(errno));

	TargetCommandStats cmdStats;

	this->execute(pipeId, inputParams, cmdStats);

	this->gatherResults(pipeId, cmdStats);

	close(pipeId[0]);
	close(pipeId[1]);

	stringstream timer;
	Reporter::makeTimerLine(timer, cmdStats);

	Reporter::printResult(inputParams, timer, cmdStats);

}

/**
 * executes the target comand
 * @param pipeId the id to be used to put the start time into the pipe
 * @param inputParams input params
 * @param cmdStats struct with the results
 * @return the pid of the child process
 */
pid_t Tracker::execute(int pipeId[2], InputParams& inputParams, TargetCommandStats& cmdStats) {

	pid_t pid = fork();
	if (pid == 0) {

		// put start time in the pipe
		long startNanosPure = steady_clock::now().time_since_epoch().count();
		write(pipeId[1], &startNanosPure, sizeof(long));

		int res = execvp(inputParams.targetArgs[0], (char**) inputParams.targetArgs);

		if(res == -1) {
			perror(*inputParams.targetArgs);
			exit(EXIT_FAILURE);
		}
	} else if(pid < 0) { // cannot fork
		throw ExecutionError(strerror(errno));
	}

	// wait for process to end
	int childStatus;
	int res = waitpid(pid, &childStatus, 0);

	// get end time
	cmdStats.finish = steady_clock::now().time_since_epoch().count();

	if(WEXITSTATUS(childStatus) == EXIT_FAILURE) { // internal error on the child process
		throw ExecutionError();
	}

	if(res == -1) { // error waiting for the child process
		throw ExecutionError(strerror(errno));
	}

	return pid;
}

/**
 * collects the results into the TargetCommandStats struct
 * @param pipeId the id to be used get the start time from
 * @param cmdStats struct with the results
 */
void Tracker::gatherResults(int pipeId[2], TargetCommandStats& cmdStats) {

	read(pipeId[0], &cmdStats.start, sizeof(long));

	cmdStats.elapsed = cmdStats.finish - cmdStats.start;

	int retRu = getrusage(RUSAGE_CHILDREN, &cmdStats.usage);
	if(retRu == -1) {
		cout << strerror(errno) << "\n";
	}

}

/**
 * prints Track help
 */
void Tracker::printHelp() {

	fmt::print(fg(fmt::color::green), "Track utility to display resource usage of processes.\n\n");

	fmt::print(fg(fmt::color::aquamarine), "Usage: track [options] <command_to_be_tracked>\n\n");

	fmt::print(fg(fmt::color::cornflower_blue), "Options:\n");
	fmt::print(fg(fmt::color::cornflower_blue), "\t{}, {:<10} {}\n", flags::help[0], flags::help[1], flags::help[2]);
	fmt::print(fg(fmt::color::cornflower_blue), "\t{}, {:<10} {}\n", flags::timer[0], flags::timer[1], flags::timer[2]);
	fmt::print(fg(fmt::color::cornflower_blue), "\t{}, {:<10} {}\n", flags::mem[0], flags::mem[1], flags::mem[2]);
	fmt::print(fg(fmt::color::cornflower_blue), "\t{}, {:<10} {}\n", flags::cpu[0], flags::cpu[1], flags::cpu[2]);
	fmt::print(fg(fmt::color::cornflower_blue), "\t{}, {:<10} {}\n\n", flags::io[0], flags::io[1], flags::io[2]);

	fmt::print(fg(fmt::color::cornflower_blue), "{}\n", flags::section);
}




