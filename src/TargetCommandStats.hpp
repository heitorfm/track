
#ifndef TRACK_TARGETCOMMANDSTATS_HPP
#define TRACK_TARGETCOMMANDSTATS_HPP

#include "sys/resource.h"

/**
 * Stores the reults of the target command
 */
typedef struct TargetCommandStats {
	long start;
	long finish;
	long elapsed;
	rusage usage;
} TargetCommandStats;

#endif // TRACK_TARGETCOMMANDSTATS_HPP