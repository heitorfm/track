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

#include <fmt/color.h>
#include <iostream>
#include "Reporter.hpp"

void Reporter::makeTimerLine(stringstream& timer, TargetCommandStats& cmdStats) {

	long nanos = cmdStats.elapsed;

	int micros = nanos / 1000;
	nanos %= 1000;

	int millis = micros / 1000;
	micros %= 1000;

	int seconds = millis / 1000;
	millis %= 1000;

	int minutes = seconds / 60;
	seconds %= 60;

	int hours = minutes / 60;
	minutes %= 60;

	int days = hours / 24;
	hours %= 24;

	auto timerColor = fg(fmt::rgb(180, 227, 61));

	if(days > 0) timer << fmt::format(timerColor, "{} days ", days);
	if(hours > 0) timer << fmt::format(timerColor, "{} hours ", hours);
	if(minutes > 0) timer << fmt::format(timerColor, "{} minutes ", minutes);
	if(seconds > 0) timer << fmt::format(timerColor, "{} seconds ", seconds);
	if(millis > 0) timer << fmt::format(timerColor, "{} millis ", millis);
	if(micros > 0) timer << fmt::format(timerColor, "{} micros ", micros);
	if(nanos > 0) timer << fmt::format(timerColor, "{} nanos", nanos);
	timer << '\n';

}

void Reporter::printRUsage(rusage ru) {

	fmt::print("ru_stime.tv_sec: {}\n", ru.ru_stime.tv_sec);
	fmt::print("ru_stime.tv_usec: {}\n", ru.ru_stime.tv_usec);
	fmt::print("ru_utime.tv_usec: {}\n", ru.ru_utime.tv_sec);
	fmt::print("ru_utime.tv_usec: {}\n", ru.ru_utime.tv_usec);
	fmt::print("ru_inblock: {}\n", ru.ru_inblock);
	fmt::print("ru_idrss: {}\n", ru.ru_idrss);
	fmt::print("ru_isrss: {}\n", ru.ru_isrss);
	fmt::print("ru_ixrss: {}\n", ru.ru_ixrss);
	fmt::print("ru_majflt: {}\n", ru.ru_majflt);
	fmt::print("ru_maxrss: {}\n", ru.ru_maxrss);
	fmt::print("ru_msgrcv: {}\n", ru.ru_msgrcv);
	fmt::print("ru_msgsnd: {}\n", ru.ru_msgsnd);
	fmt::print("ru_minflt: {}\n", ru.ru_minflt);
	fmt::print("ru_nivcsw: {}\n", ru.ru_nivcsw);
	fmt::print("ru_nsignals: {}\n", ru.ru_nsignals);
	fmt::print("ru_nswap: {}\n", ru.ru_nswap);
	fmt::print("ru_nvcsw: {}\n", ru.ru_nvcsw);
	fmt::print("ru_inblock: {}\n", ru.ru_inblock);
	fmt::print("ru_oublock: {}\n", ru.ru_oublock);

}

void Reporter::printResult(InputParams& inputParams, stringstream& timer, TargetCommandStats& cmdStats) {

	for(int i = 0; i < inputParams.targetArgsSize; i++) {
		cout << inputParams.targetArgs[i] << " ";
	}
	cout << '\n';

	if(!inputParams.selective || inputParams.timer) {
		fmt::print(fg(fmt::color::white), "-----------------------------------------------------------\n");
		cout << timer.str().c_str();
		fmt::print(fg(fmt::color::white), "-----------------------------------------------------------\n");
		fmt::print(fg(fmt::rgb(50, 250, 150)), "{: <15} {} micros\n", "System Time:", cmdStats.usage.ru_stime.tv_usec);
		fmt::print(fg(fmt::rgb(50, 250, 150)), "{: <15} {} micros\n", "User Time:", cmdStats.usage.ru_utime.tv_usec);
	}
	if(!inputParams.selective || inputParams.mem) {
		fmt::print(fg(fmt::color::white), "-----------------------------------------------------------\n");
		fmt::print(fg(fmt::rgb(50, 150, 250)), "{: <15} {} K\n", "Max Memory:", cmdStats.usage.ru_maxrss);
		fmt::print(fg(fmt::rgb(50, 150, 250)), "{: <15} {}\n", "Swaps:", cmdStats.usage.ru_nswap);
	}
	if(!inputParams.selective || inputParams.cpu) {
		fmt::print(fg(fmt::color::white), "-----------------------------------------------------------\n");
		fmt::print(fg(fmt::color::aquamarine), "{: <15} {}\n", "Involuntary context switches:", cmdStats.usage.ru_nivcsw);
		fmt::print(fg(fmt::color::aquamarine), "{: <15} {}\n", "Voluntary context switches:", cmdStats.usage.ru_nvcsw);
	}
	if(!inputParams.selective || inputParams.io) {
		fmt::print(fg(fmt::color::white), "-----------------------------------------------------------\n");
		fmt::print(fg(fmt::rgb(230, 175, 46)), "{: <15} {}\n", "Page Reclaims:", cmdStats.usage.ru_minflt);
		fmt::print(fg(fmt::rgb(230, 175, 46)), "{: <15} {}\n", "Page Faults:", cmdStats.usage.ru_majflt);
		fmt::print(fg(fmt::rgb(230, 175, 46)), "{: <15} {}\n", "Block input operations:", cmdStats.usage.ru_inblock);
		fmt::print(fg(fmt::rgb(230, 175, 46)), "{: <15} {}\n", "Block output operations:", cmdStats.usage.ru_oublock);
	}
	fmt::print(fg(fmt::color::white), "-----------------------------------------------------------\n");

}
