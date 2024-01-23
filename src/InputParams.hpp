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

#ifndef TRACK_INPUTPARAMS_HPP
#define TRACK_INPUTPARAMS_HPP

#include <cstring>

using namespace std;


/**
 * Parsed input params
 */
typedef struct InputParams {

	int argc;
	char** argv;
	int targetArgsSize;
	char** targetArgs;

	bool selective = false;
	bool timer = false;
	bool cpu = false;
	bool mem = false;
	bool io = false;

} InputParams;

/**
 * aceptable flags in the input command
 */
const struct flags {
	constexpr static const char* help[] = {"-h", "--help", "Print help"};
	constexpr static const char* timer[] = {"-t", "--timer", "Add timer section to the response."};
	constexpr static const char* cpu[] = {"-c", "--cpu", "Add cpu section to the response."};
	constexpr static const char* mem[] = {"-m", "--memory", "Add memory section to the response."};
	constexpr static const char* io[] = {"-i", "--io", "Add IO section to the response."};

	constexpr static const char* section = "Adding section flags makes Track operate in 'selective mode': Any section must be added individually.";

	static bool hasFlag(const char* cur, const char* const* flag) {
		return memcmp(cur, flag[0], strlen(flag[0])) == 0 || memcmp(cur, flag[1], strlen(flag[1])) == 0;
	}

} flags;

#endif // TRACK_INPUTPARAMS_HPP