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

#include <iostream>
#include "Tracker.hpp"
#include "NeedsHelpException.hpp"
#include "ExecutionError.hpp"

using namespace std;

int main(int argc, char** argv) {

	InputParams inputParams{.argc = argc, .argv = argv};

	Tracker tracker;

	try {

		tracker.parseCommandLine(inputParams);
		tracker.track(inputParams);

	} catch(NeedsHelpException& help) {
		if(!help.what().empty()) cout << help.what() << "\n";
		tracker.printHelp();
	} catch(ExecutionError& err) {
		cout << err.what() << "\n";
	}

	delete[] inputParams.targetArgs;


	return 0;
}
