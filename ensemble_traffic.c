#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "traffic.h"

int main (int argc, char const *argv[]) {
    int numCars = 150;
    int ticks = 6000;
    int runs = 32;
    unsigned seedOne = 1;

    if (argc > 1) {
        numCars = readNumber(argv[1], numCars);
    }
    if (argc > 2) {
        ticks = readNumber(argv[2], ticks);
    }
    if (argc > 3) {
        runs = readNumber(argv[3], runs);
    }
    if (argc > 4) {
        seedOne = readNumber(argv[4], seedOne);
    }

    if (runs < 1) {
        runs = 1;
    }
    
    struct road * intersectionsAndRoads = buildRoundabout();
    int threads = omp_get_max_threads();
    
    long * serialTrips = malloc(sizeof(long) * runs);
    long * serialTicks = malloc(sizeof(long) * runs);
    long * parallelTrips = malloc(sizeof(long) * runs);
    long * parallelTicks = malloc(sizeof(long) * runs);

    // sequential runs (one after another)
    double t0 = omp_get_wtime();
    for (int r = 0; r < runs; r++) {
        serialTrips[r] = runSerial(intersectionsAndRoads, numCars, ticks, seedOne + r, &serialTicks[r]);
    }
    double serialSec = omp_get_wtime() - t0;

    // spread runs over threads
    t0 = omp_get_wtime();

    #pragma omp parallel for schedule(dynamic, 1)
    for (int r = 0; r < runs; r++) {
        parallelTrips[r] = runSerial(intersectionsAndRoads, numCars, ticks, seedOne + r, &parallelTicks[r]);

    }
    double parallelSec = omp_get_wtime() - t0;

    // check answers btwn runs
    int same = 1;
    long minTrips = serialTrips[0];
    long maxTrips = serialTrips[0];
    long sumTrips = 0;
    long sumTicks = 0;

    for (int r = 0; r < runs; r++) {
        if (serialTrips[r] != parallelTrips[r] || serialTicks[r] != parallelTicks[r]) {
            same = 0;
        }

        if (serialTrips[r] < minTrips) {
            minTrips = serialTrips[r];
        }

        if (serialTrips[r] > maxTrips) {
            maxTrips = serialTrips[r];
        }

        sumTrips = sumTrips + serialTrips[r];
        sumTicks = sumTicks + serialTicks[r];
    }

    double hrs =  ticks * secondsPerTick / 3600.0;
    double avgTrips = (double)sumTrips / runs;
    double avgTripSec = 0;
    if (sumTrips > 0) {
        avgTripSec = (double)sumTicks / sumTrips * secondsPerTick;
    }



    // check print statements (just a lot of sanity checking)
    printf("%d runs of %d cars for %.0f seconds\n", runs, numCars, ticks * secondsPerTick);
    printf("cars that go through the roundabout per hr: avg %.0f (with lowest run %.0f and highest run %.0f)\n", avgTrips / hrs, minTrips / hrs, maxTrips / hrs);
    printf("average trip %.1f s\n", avgTripSec);

    if (same == 1) {
        printf("match\n");
    } else {
        printf("serial and parallel dont match (BAD)\n");
    }

    double speedup = serialSec / parallelSec;
    printf("one after another  %.4f s\n", serialSec);
    printf("on %2d threads  %.4f s  (speedup %.2f)\n", threads, parallelSec, speedup);
    printf("RESULT %d %d %d %d %.6f %.6f %.3f\n", threads, numCars, ticks, runs, serialSec, parallelSec, speedup);



    free(serialTrips);
    free(serialTicks);
    free(parallelTrips);
    free(parallelTicks);
    free(intersectionsAndRoads);

    return 0;
}