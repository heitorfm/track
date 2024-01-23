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

#ifndef TRACK_NEEDSHELPEXCEPTION_HPP
#define TRACK_NEEDSHELPEXCEPTION_HPP

#include <exception>
#include <utility>

using namespace std;

class NeedsHelpException: public exception {
private:
	string message;

public:
	NeedsHelpException() = default;
	explicit NeedsHelpException(string msg): message(std::move(msg)) {}
	string what() {
		return message;
	}

};


#endif //TRACK_NEEDSHELPEXCEPTION_HPP
