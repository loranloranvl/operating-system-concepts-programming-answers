#ifndef MANAGER
#define MANAGER

#include <stdbool.h>

// Data structure representing process identifier availability
typedef struct PidMap {
    bool* availabilities;
    int length;
    int lower_bound;
    int upper_bound;
} PidMap;

// Creates and initializes a data structure for representing pids; 
// returns -1 if unsuccessful, 1 if successful
// bounds are both inclusive
int allocate_map(
    PidMap* pmap, 
    int lower_bound, 
    int upper_bound
);

// Allocates and returns a pid; 
// returns -1 if unable to allocate a pid (all pids are in use)
int allocate_pid(PidMap* pmap);

// Releases pid
void release_pid(PidMap* pmap, int pid);

#endif