// test harness for the entire simulation (basically just a huge sanity check because everything kept breaking)
#include "traffic.h"
#include <omp.h>

int failures = 0;
struct carIdx * idx; // shared index for small tests

void check (int ok, char * huh) {
    if (ok == 1) {
        printf(" PASS %s\n", huh);
    } else {
        printf(" FAIL %s\n", huh);
        failures += 1;
    }
}


struct car makeCar(struct road * * path, int pathLength, int idx, double pos, double speed) {
    struct car c = {0};
    c.topSpeed = 10;
    c.length = 4.75;
    c.accelPerTick = 0.1;
    c.brakePerTick = 0.5;

    for (int k = 0; k < pathLength; k++) {
        c.path[k] = path[k];

    }

    c.pathLength = pathLength;
    c.pathIndex = idx;
    c.road = path[idx];
    c.destination = path[pathLength - 1];
    c.positionAlongRoad = pos;
    c.speed = speed;
    c.active = 1;
    c.foo = 1;

    return c;
}

// check roads
void testRoads(struct road * m) {
    printf("roads\n");
    int count = 0;
    int consistent = 1;

    for (int a = 0; a < numIntersections; a++) {
        for (int b = 0; b < numIntersections; b++) {
            struct road * r = getRoad(m, numIntersections, a, b);

            if ((*r).exists == 1) {
                count = count + 1;
                if ((*r).start != a || (*r).end != b || (*r).length <= 0 || (*r).speedLimit <= 0) {
                    consistent = 0;
                }
            }
        }
    }

    check(count == 20, "roundabout = 20 roads");
    check(consistent, "every road's start/end matches");
}

// find path
void testPathfind(struct road * m) {
    printf("find path\n");
    
    struct road * path[16];
    int len = findPath(m, getRoad(m, numIntersections, 1, 2), getRoad(m, numIntersections, 14, 15), path, 16);
    int nodes[7] = {1,2, 3, 8, 13, 14, 15}; // what nodes the route should go through

    int ok = 0;
    if (len == 6) {
        ok = 1;
        for (int k = 0; k < 6; k++) {
            if ((*path[k]).start != nodes[k]  || (*path[k]).end != nodes[k + 1]) {
                ok = 0;
            }
        }
    }

    check(ok, "1->2 to 14->15 goes 1-2-3-8-13-14-15" );
}

// a car going fast behind a stopped car
void testFollowing(struct road * m) {
    printf("following\n");
    struct road * path[16];
    int len = findPath(m, entryRoad(m, 0), exitRoad(m, 2), path, 16);

    // IT SHOYLD STOP
    struct car table[2];
    struct car out;
    table[0] = makeCar(path, len, 0, 50.0, 0.0);  // stops at 50 m
    table[1] = makeCar(path, len, 0, 40.0, 10.0); // speed up behind it
    buildIdx(m, idx, table, 2);
    updateCar(m, idx, table, 1, 0, &out);

    double room = (50.0 - 4.75) - out.positionAlongRoad; // space left
    check(room >= carGap, "fast car behind a stopped car keeps its gap");
}

// a car at the roundabout should wait if a car is close
void testYield(struct road * m) {
    printf("yield\n");

    struct road * merging[16];
    int mLen = findPath(m, getRoad(m, numIntersections, 2, 3), exitRoad(m, 1), merging, 16);
    struct road * ring[16];
    int rLen = findPath(m, getRoad(m, numIntersections, 18, 3), exitRoad(m, 2), ring, 16);

    struct car table[2];
    struct car out;
    table[0] = makeCar(merging, mLen, 0, 40.0, 0.0); // at the line
    table[1] = makeCar(ring,  rLen, 0, (*ring[0]).length - 10.0, 6.0); // 10 m away

    buildIdx(m, idx, table, 2);
    updateCar(m, idx, table, 0, 0, &out);

    check(out.road == merging[0], "car at the circle waits for the circle car");
}

// BIG RANDOM RUN
// a bunch of cars for 30 minutes and make sure nothing crashes
void testRandom(struct road * m) {
    printf("random run\n");
    int n = 100;
    
    struct car * s = malloc(sizeof(struct  car) * n);
    struct car * next = malloc(sizeof(struct car) * n);
    
    initCar(m, s, n, 1);
    struct carIdx * runIdx = newIdx(n);

    int overlaps = 0;
    for (int tick = 0; tick < 18000; tick++) {
        step(m, runIdx, s, next, n, tick);
        struct car * foo = s;
        
        s = next;
        next = foo;

        // two cars on the same road
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n;  j++) {
                
                if (i != j && s[i].active == 1 && s[j].active == 1 && s[i].road == s[j].road) {
                    double jRear =  s[j].positionAlongRoad - s[j].length;
                    
                    if (s[j].positionAlongRoad >= s[i].positionAlongRoad && jRear < s[i].positionAlongRoad) {
                        overlaps=overlaps + 1;
                    }
                }
            }

        }
    }

    long trips = 0;
    for (int i = 0; i < n; i++ ) {
        trips = trips + s[i].tripsCompleted;
    }

    check(overlaps == 0, "no cars on top of each other");
    check(trips > 500, "cars keep finishing trips (no gridlock)"); // its about 750 when things work

    free(s);
    free(next);
    freeIdx(runIdx);

}

// both tavles have every car at same position
int sameTables(struct car * a, struct car * b,  int n) {
    for (int i = 0; i < n; i++ ) {
        
        if (a[i].road != b[i].road ||  a[i].positionAlongRoad != b[i].positionAlongRoad || a[i].speed != b[i].speed || a[i].tripsCompleted != b[i].tripsCompleted) {
            return 0;

        }
    }
    return 1;
}

// should give the same answers (race cond)
void testThreads(struct road * m) {
    printf("threads\n");
    int n = 300;
    int ticks = 3000;
    omp_set_num_threads(4);

    struct car * s = malloc(sizeof(struct car) * n);
    struct car * sNext = malloc(sizeof(struct car) * n);
    struct car * p = malloc(sizeof(struct car) * n);
    struct car * pNext = malloc(sizeof(struct car) * n);
    struct car * r = malloc(sizeof(struct car) * n);
    struct car * rNext = malloc(sizeof(struct car) * n);
    
    initCar(m, s, n, 5);
    initCar(m, p, n, 5);
    initCar(m, r, n, 5);
    
    struct carIdx * threadIdx = newIdx(n);

    for (int tick = 0; tick < ticks; tick++) {
        step(m, threadIdx, s, sNext, n, tick);
        struct car * foo = s;
        
        s = sNext;
        sNext = foo;

        stepParallel(m, threadIdx, p, pNext, n, tick);
        foo = p;
        
        p = pNext;
        pNext = foo;
    }

    runParallel(m, threadIdx, &r, &rNext, n, 0, ticks);

    check(sameTables(s, p, n), "stepParallel matches step");
    check(sameTables(s, r, n), "runParallel matches step");

    free(s);
    free(sNext);
    free(p);
    free(pNext);
    free(r);
    free(rNext);
    freeIdx(threadIdx);
}

int main(void) {
    struct road * m = buildRoundabout();
    idx =  newIdx(10);
    
    testRoads(m);
    testPathfind(m);
    testFollowing(m);
    testYield(m);
    testRandom(m);
    testThreads(m);

    freeIdx(idx);
    free(m);

    printf("\n");
    if (failures == 0) {
        printf("PASS");
        return 0;
    }
    printf("FAIL (%d failures)\n", failures);
    return 1;
}

