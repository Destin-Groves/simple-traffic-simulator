// DONE
#include <stdio.h>
#include <stdlib.h>
#include "traffic.h"

int main(int argc, char const *argv[]) {
    int numCars = 60;
    int numTicks = 6000;
    unsigned seed = 1;
    int print = 1000;

    if (argc > 1) {
        numCars = readNumber(argv[1], numCars);
    }
    if (argc > 2) {
        numTicks = readNumber(argv[2], numTicks);
    }
    if (argc > 3) {
        seed = readNumber(argv[3], seed);
    }
    if (argc > 4) {
        print = readNumber(argv[4], print);
    }
    if (numCars < 1) {
        numCars = 1;
    }

    struct road * intersectionsAndRoads = buildRoundabout();
    struct car * s = malloc(sizeof(struct car) * numCars);
    struct car * next = malloc(sizeof(struct car) * numCars);
    initCar(intersectionsAndRoads, s, numCars, seed);
    struct carIdx * idx = newIdx(numCars);

    for (int tick = 0; tick < numTicks; tick++) {
        step(intersectionsAndRoads, idx, s, next, numCars, tick);
        struct car * foo = s;
        s = next;
        next = foo;

        if (print > 0) {
            if ((tick + 1) % print == 0) {
                printResults(s, numCars, tick + 1);
            }
        }
    }

    printResults(s, numCars, numTicks);

    free(s);
    free(next);
    freeIdx(idx);
    free(intersectionsAndRoads);
    return 0;

}