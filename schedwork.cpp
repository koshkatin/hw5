#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool recurse(
    const AvailabilityMatrix& avail,
    DailySchedule& sched,
    const size_t dailyNeed,
    const size_t maxShifts,
    std::vector<size_t>& numShifts,
    size_t day, 
    size_t slot
);


// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    
    // initialize sched
    sched.assign(avail.size(), std::vector<Worker_T>(dailyNeed, INVALID_ID));

    // keep track of the number of shifts taken by each worker
    std::vector<size_t> numShifts(avail[0].size(), 0);

    return recurse(avail, sched, dailyNeed, maxShifts, numShifts, 0, 0);
}

bool recurse(
    const AvailabilityMatrix& avail,
    DailySchedule& sched,
    const size_t dailyNeed,
    const size_t maxShifts,
    std::vector<size_t>& numShifts,
    size_t day, 
    size_t slot
) 
{
    // Reached the end of sched. Done!
    if (day == sched.size()) return true;

    // Wrap workers for next day if daily need is met
    if (slot == dailyNeed) {
        return recurse(avail, sched, dailyNeed, maxShifts, numShifts, day + 1, 0);
    }

    // try every worker for the day
    for (Worker_T w = 0; w < avail[0].size(); ++w) {
        
        // skip if worker unavailable on the day
        if (!avail[day][w]) continue;

        // skip if worker has filled their shifts
        if (numShifts[w] >= maxShifts) continue;
        
        // avoid double assigning worker for the day
        bool alreadyScheduled = false;
        for (size_t i = 0; i < slot; ++i) {
            if (sched[day][i] == w) alreadyScheduled = true;
        }
        if (alreadyScheduled) continue; 

        // otherwise, assign worker to slot
        sched[day][slot] = w;  
        numShifts[w]++;

        // recurse
        if (recurse(avail, sched, dailyNeed, maxShifts, numShifts, day, slot + 1)) {
            return true;
        }

        // backtrack
        numShifts[w]--;
        sched[day][slot] = INVALID_ID;
    }

    return false;
}