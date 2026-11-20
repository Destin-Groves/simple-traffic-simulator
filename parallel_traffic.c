#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "traffic.h"

int sameTables(struct car * a, struct car * b, int n) {
    for (int i = 0; i < n; i++) {
        if (a[i].road != b[i].road) {
            return 0;
        }
        if (a[i].positionAlongRoad != b[i].positionAlongRoad) {
            return 0;
        }
        if (a[i].speed != b[i].speed) {
            return 0;
        }
        if (a[i].tripsCompleted != b[i].tripsCompleted) {
            return 0;
        }
    }
    return 1;

}

int main(int argc, char const *argv[]) {
    int numCars = 1000;
    int ticks = 2000;
    unsigned seed = 1;

    if (argc > 1) {
        numCars = readNumber(argv[1], numCars);
    }
    if (argc > 2) {
        ticks = readNumber(argv[2], ticks);
    }
    if (argc > 3) {
        seed = (unsigned)readNumber(argv[3], seed);
    }
    if (numCars < 1) {
        numCars = 1;
    }

    struct road * intersectionsAndRoads = buildRoundabout();
    int threads = omp_get_max_threads();
    struct carIdx * idx = newIdx(numCars);

    // serial
    struct car * s = malloc(sizeof(struct car) * numCars);
    struct car * sNext = malloc(sizeof(struct car) * numCars);
    initCar(intersectionsAndRoads, s, numCars, seed);

    double t0 = omp_get_wtime();
    for (int tick = 0; tick < ticks; tick++) {
        step(intersectionsAndRoads, idx, s, sNext, numCars, tick) ;
        struct car * foo = s;
        s = sNext;
        sNext = foo;
    }
    double serialSec = omp_get_wtime() - t0;

    // stepParalel per tick
    struct car * p = malloc(sizeof(struct car) * numCars);
    struct car * pNext = malloc(sizeof(struct car) * numCars);
    initCar(intersectionsAndRoads, p, numCars, seed);

    t0 = omp_get_wtime();
    for (int tick = 0; tick < ticks; tick++) {
        stepParallel(intersectionsAndRoads, idx, p, pNext, numCars, tick) ;
        struct car * foo = p;
        p = pNext;
        pNext = foo;
    }
    double stepParallelSec  = omp_get_wtime() - t0;

    // runParallel
    struct car * r = malloc(sizeof(struct car) * numCars);
    struct car * rNext = malloc(sizeof(struct car) * numCars);
    initCar(intersectionsAndRoads, r, numCars, seed);

    t0 = omp_get_wtime();
    runParallel(intersectionsAndRoads, idx, &r, &rNext, numCars, 0, ticks);
    double runParallelSec = omp_get_wtime() - t0;

    // check for same result btwn parallel and serial
    printResults(s, numCars, ticks);
    if (sameTables(s, p, numCars) == 1 && sameTables(s, r, numCars) == 1) {
        printf("match\n");
    } else {
        printf("serial and parallel dont match\n");
    }

    double best = stepParallelSec;
    if (runParallelSec < best) {
        best = runParallelSec;
    }
    double speedup = (serialSec / best);

    // due dilligance
    printf("serial  %.4f s\n", serialSec);
    printf("stepParallel  %.4f s  (speedup %.2f)\n", stepParallelSec, (serialSec / stepParallelSec));
    printf("runParallel   %.4f s  (speedup %.2f)\n", runParallelSec, (serialSec / runParallelSec));
    printf("threads   %d\n", threads);
    printf("RESULT %d %d %d %.6f %.6f %.6f %.3f\n", threads, numCars, ticks, serialSec, stepParallelSec, runParallelSec, speedup);


    free(s);
    free(sNext);
    free(p);
    free(pNext);
    free(r);
    free(rNext);
    freeIdx(idx);
    free(intersectionsAndRoads);

    return 0;

}