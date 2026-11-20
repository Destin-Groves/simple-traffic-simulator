#include "traffic.h"

int readNumber(char const * text, int fallback) {
    char * end;
    long val = strtol(text, &end, 10);
    if (end == text || *end != '\0') {

        printf("%s is not a whole number, using %d\n", text, fallback);
        return fallback;
    }
    return (int)val;
}

// road network
struct road * getRoad(struct road * intersections, int numIntersections, int intersection1, int intersection2) {
    return (intersections + intersection1 + (numIntersections * intersection2));

}

struct road * buildRoundabout() {
    int n = numIntersections;
    struct road * roundabout = calloc(n * n * 2, sizeof(struct road));

    struct road * r1 = getRoad(roundabout, n, 1, 2);
    struct road * r2 = getRoad(roundabout, n, 2, 3);
    struct road * r3 = getRoad(roundabout, n, 3, 4);
    struct road * r4 = getRoad(roundabout, n, 4, 5);

    struct road * r5 = getRoad(roundabout, n, 6, 7);
    struct road * r6 = getRoad(roundabout, n, 7, 8);
    struct road * r7 = getRoad(roundabout, n, 8, 9);
    struct road * r8 = getRoad(roundabout, n, 9, 10);

    struct road * r9 = getRoad(roundabout, n, 11, 12);
    struct road * r10 = getRoad(roundabout, n, 12, 13);
    struct road * r11 = getRoad(roundabout, n, 13, 14);
    struct road * r12 = getRoad(roundabout, n, 14, 15);

    struct road * r13 = getRoad(roundabout, n, 16, 17);
    struct road * r14 = getRoad(roundabout, n, 17, 18);
    struct road * r15 = getRoad(roundabout, n, 18, 19);
    struct road * r16 = getRoad(roundabout, n, 19, 20);

    // roundabout circle
    struct road * r17 = getRoad(roundabout, n, 3, 8);
    struct road * r18 = getRoad(roundabout, n, 8, 13);
    struct road * r19 = getRoad(roundabout, n, 13, 18);
    struct road * r20 = getRoad(roundabout, n, 18, 3);

    r1->exists = 1;
    r2->exists = 1;
    r3->exists = 1;
    r4->exists = 1;
    r5->exists = 1;
    r6->exists = 1;
    r7->exists = 1;
    r8->exists = 1;
    r9->exists = 1;
    r10->exists = 1;
    
    r11->exists = 1;
    r12->exists = 1;
    r13->exists = 1;
    r14->exists = 1;
    r15->exists = 1;
    r16->exists = 1;
    r17->exists = 1;
    r18->exists = 1;
    r19->exists = 1;
    r20->exists = 1;

    // rough dimensions pulled from a roundabout in Terre Haute
    r1->length = 100;
    r2->length = 40;
    r3->length = 40;
    r4->length = 100;

    r5->length = 100;
    r6->length = 40;
    r7->length = 40;
    r8->length = 100;

    r9->length = 100;
    r10->length = 40;
    r11->length = 40;
    r12->length = 100;
    
    r13->length = 100;
    r14->length = 40;
    r15->length = 40;
    r16->length = 100;
    
    r17->length = 20 * 3.14159;
    r18->length = 20 * 3.14159;
    r19->length = 20 * 3.14159;
    r20->length = 20 * 3.14159;
    
    r1->start = 1;
    r2->start = 2;
    r3->start = 3;
    r4->start = 4;

    r5->start = 6;
    r6->start = 7;
    r7->start = 8;
    r8->start = 9;

    r9->start = 11;
    r10->start = 12;
    r11->start = 13;
    r12->start = 14;
    
    r13->start = 16;
    r14->start = 17;
    r15->start = 18;
    r16->start = 19;

    r17->start = 3;
    r18->start = 8;
    r19->start = 13;
    r20->start = 18;

    r1->end = 2;
    r2->end = 3;
    r3->end = 4;
    r4->end = 5;

    r5->end = 7;
    r6->end = 8;
    r7->end = 9;
    r8->end = 10;

    r9->end = 12;
    r10->end = 13;
    r11->end = 14;
    r12->end = 15;

    r13->end = 17;
    r14->end = 18;
    r15->end = 19;
    r16->end = 20;

    r17->end = 8;
    r18->end = 13;
    r19->end = 18;
    r20->end = 3;

    // speed limits (30 mph on long road, 20 mph near the roundabout circle, and 15 mph on the roundabout circle)
    r1->speedLimit = 13.4;
    r2->speedLimit = 8.9;
    r3->speedLimit = 8.9;
    
    r4->speedLimit = 13.4;
    r5->speedLimit = 13.4;
    r6->speedLimit = 8.9;
    r7->speedLimit = 8.9;
    
    r8->speedLimit = 13.4;
    r9->speedLimit = 13.4;
    r10->speedLimit = 8.9;
    r11->speedLimit = 8.9;
    r12->speedLimit = 13.4;
    
    r13->speedLimit = 13.4;
    r14->speedLimit = 8.9;
    r15->speedLimit = 8.9;
    r16->speedLimit = 13.4;
    
    r17->speedLimit = 6.7;
    r18->speedLimit = 6.7;
    r19->speedLimit = 6.7;
    r20->speedLimit = 6.7;

    // establish right of way
    r2->yield = 1;
    r6->yield = 1;
    r10->yield = 1;
    r14->yield = 1;

    return roundabout;
}

// cars on the arm
// appear
struct road * entryRoad(struct road * intersections, int arm) {
    int a = (5 * arm + 1);
    return getRoad(intersections, numIntersections, a, a + 1);

}

// disappear
struct road * exitRoad(struct road * intersections, int arm) {
    int a = (5 * arm + 4);
    return getRoad(intersections, numIntersections, a, a + 1);
}

int roundaboutIntersection(int node) {
    if (node == 3 || node == 8 || node == 13 || node == 18) {
        return 1;
    }
    return 0;
}


// pathfinding from one intersection to another
int findPath(struct road * intersections, struct road * start, struct road * end, struct road * * pathOut, int maxPathLength) {
    pathOut[0] = start;
    int length = 1;
    int node = start->end;
    
    while (node != end->start && length < maxPathLength - 1) {
        int nextNode = node + 1; // still on an arm, keep going toward the circle
        
        if (getRoad(intersections, numIntersections, node, end->start)->exists == 1) {
            nextNode = end->start; // the road off the circle to our exit
        } else if (roundaboutIntersection(node) == 1) {
            nextNode = (node + 5) % 20; // next spot around the circle (18 + 5 wraps to 3)
        }
        
        pathOut[length] = getRoad(intersections, numIntersections, node, nextNode);
        length = length + 1;
        node = nextNode;
    }
    
    pathOut[length] = end;
    return length + 1;
}

// each car has its own random seed (foo) so threads dont share one
unsigned nextRandom(unsigned * state) {
    *state = *state ^ (*state << 13);
    *state = *state ^ (*state >> 17);
    *state = *state ^ (*state << 5);
    
    return *state;
}

// random entry arm, different exit arm, then wait to get on the road
void startTrip(struct road * intersections, struct car * c, int tick) {
    int entryArm = nextRandom(&c->foo) % numArmRoads;
    int exitArm = (entryArm + 1 + nextRandom(&c->foo) % (numArmRoads - 1)) % numArmRoads;
    
    c->destination = exitRoad(intersections, exitArm);
    c->pathLength = findPath(intersections, entryRoad(intersections, entryArm), c->destination, c->path, 16);
    c->pathIndex = 0;
    c->road = c->path[0];
    c->positionAlongRoad = 0;
    c->speed = 0;
    c->active = 0;
    c->startTrip = tick;
}

void initCar(struct road * intersections, struct car * cars, int n, unsigned seed) {
    for (int i = 0; i < n; i++) {
        struct car c = {0};
        
        c.topSpeed = 10;
        c.length = 4.75;
        c.accelPerTick = 0.1;
        c.brakePerTick = 0.5;
        c.foo = seed ^ (0x9E3779B9u * (unsigned)(i + 1));
        
        if (c.foo == 0) {
            c.foo = 1; // seed cant be 0
        }
        
        startTrip(intersections, &c, 0);
        cars[i] = c;
    }
}

// sort the cars by road every tick so a car only has to check the cars near it
// cars on road slot s: carIdx[groupStartIdx[s]] up to carIdx[groupStartIdx[s + 1] - 1]
// frontQueIdx[s]: the car thats been waiting longest to get on road s (or -1)
struct carIdx * newIdx(int maxCars) {
    struct carIdx * idx = malloc(sizeof(struct carIdx));
    
    idx->numSlots = numIntersections * numIntersections;
    idx->capacity = maxCars;
    idx->groupStartIdx = malloc(sizeof(int) * (idx->numSlots + 1));
    idx->groupSize = malloc(sizeof(int) * idx->numSlots);
    idx->frontQueIdx = malloc(sizeof(int) * idx->numSlots);
    idx->carIdx = malloc(sizeof(int) * maxCars);
    idx->carActive = malloc(sizeof(int) * maxCars);
    
    return idx;
}

void freeIdx(struct carIdx * idx) {
    free(idx->groupStartIdx);
    free(idx->groupSize);
    free(idx->frontQueIdx);
    free(idx->carIdx);
    free(idx->carActive);
    free(idx);
}

int slotOf(struct road * intersections, struct road * r) {
    return (int)(r - intersections);

}

void buildIdx(struct road * intersections, struct carIdx * idx, struct car * old, int n) {
    for (int s = 0; s < idx->numSlots; s++) {
        idx->groupSize[s] = 0;
        idx->frontQueIdx[s] = -1;
    }
    
    int numActive = 0;
    
    for (int i = 0; i < n; i++) {
        int s = slotOf(intersections, old[i].path[0]);
        if (old[i].active == 1) {
            idx->groupSize[slotOf(intersections, old[i].road)] += 1; // count cars on each road
            idx->carActive[numActive] = i;
            numActive = numActive + 1;
        } else if (idx->frontQueIdx[s] == -1 || old[i].startTrip < old[idx->frontQueIdx[s]].startTrip) {
            idx->frontQueIdx[s] = i; // waited the longest so far

        }
    }
    
    idx->groupStartIdx[0] = 0;
    
    for (int s = 0; s < idx->numSlots; s++) {
        idx->groupStartIdx[s + 1] = idx->groupStartIdx[s] + idx->groupSize[s];
        idx->groupSize[s] = 0;
    }
    
    for (int k = 0; k < numActive; k++) {
        int s = slotOf(intersections, old[idx->carActive[k]].road);
        
        idx->carIdx[idx->groupStartIdx[s] + idx->groupSize[s]] = idx->carActive[k];
        idx->groupSize[s] += 1;
    }
}

// a waiting car gets on if its first in line and the start of the road is clear
int canEnter(struct road * intersections, struct carIdx * idx, struct car * old, int i) {
    int s = slotOf(intersections, old[i].path[0]);
    
    for (int k = idx->groupStartIdx[s]; k < idx->groupStartIdx[s + 1]; k++) {
        struct car * o = &old[idx->carIdx[k]];
        if (o->positionAlongRoad - o->length < old[i].length + carGap) {
            return 0;
        }
    }
    
    return idx->frontQueIdx[s] == i;
}

// distance from my front bumper to the back bumper of whoever is ahead of me
double gapAhead(struct road * intersections, struct carIdx * idx, struct car * old, int i) {
    struct car * c = &old[i];
    double gap = 1e9;
    double offset = -c->positionAlongRoad; // how far ahead road k starts
    
    for (int k = c->pathIndex; k < c->pathLength && offset <= lookUpDistance; k++) {
        struct road * r = c->path[k];
        // cars on this road
        int s = slotOf(intersections, r);
        
        for (int q = idx->groupStartIdx[s]; q < idx->groupStartIdx[s + 1]; q++) {
            struct car * o = &old[idx->carIdx[q]];
            double rear = o->positionAlongRoad - o->length;
            if (rear < 0) {
                rear = 0;
            }
            
            if (idx->carIdx[q] != i && offset + o->positionAlongRoad > 0 && offset + rear < gap) {
                gap = offset + rear;
            }
        }
        // cars that just drove off the end of this road but their back is still sticking out
        for (int v = 0; v < numIntersections; v++) {
            struct road * out = getRoad(intersections, numIntersections, r->end, v);
            int os = slotOf(intersections, out);
            
            for (int q = idx->groupStartIdx[os]; out->exists == 1 && q < idx->groupStartIdx[os + 1]; q++) {
                struct car * o = &old[idx->carIdx[q]];
                double rear = o->positionAlongRoad - o->length;
                
                if (idx->carIdx[q] != i && rear < 0 && o->path[o->pathIndex - 1] == r && offset + r->length + rear < gap) {
                    gap = offset + r->length + rear;
                }
                
                if (idx->carIdx[q] != i && rear < 0 && k == c->pathIndex && c->road->length - c->positionAlongRoad < gap) {
                    gap = c->road->length - c->positionAlongRoad; // its still in the intersection im heading into
                }
            }
        }
        offset = offset + r->length;
    }

    return gap;
}

// if im on a yield road and someone else is about 3 seconds from the same intersection, wait
int mustYield(struct road * intersections, struct carIdx * idx, struct car * old, int i) {
    struct car * c = &old[i];
    
    for (int v = 0; c->road->yield == 1 && v < numIntersections; v++) {
        struct road * in = getRoad(intersections, numIntersections, v, c->road->end);
        int s = slotOf(intersections, in);
        
        for (int q = idx->groupStartIdx[s]; in->exists == 1 && in != c->road && q < idx->groupStartIdx[s + 1]; q++) {
            struct car * o = &old[idx->carIdx[q]];
            if (in->length - o->positionAlongRoad < o->speed * 3.0 + 8.0) {
                return 1;

            }
        }
    }

    return 0;
}

// what car i does this tick: only reads old and only writes *out
void updateCar(struct road * intersections, struct carIdx * idx, struct car * old, int i, int tick, struct car * out) {
    struct car c = old[i];
    
    if (c.active == 0 && canEnter(intersections, idx, old, i) == 1) {
        c.active = 1;
        c.positionAlongRoad = c.length;
    }

    if (old[i].active == 1) {
        // room until the car ahead, or until the yield line if we have to wait
        double gap = gapAhead(intersections, idx, old, i);
        if (mustYield(intersections, idx, old, i) == 1 && c.road->length - c.positionAlongRoad + carGap < gap) {
            gap = c.road->length - c.positionAlongRoad + carGap;
        }

        double room = gap - carGap;
        if (room < 0) {
            room = 0;
        }

        // speed up (or slow down) toward the limit, but only if we could still stop in time
        double limit = c.topSpeed;
        if (c.road->speedLimit < limit) {
            limit = c.road->speedLimit;
        }

        double wanted = c.speed + c.accelPerTick;
        if (c.speed > limit) {
            wanted = c.speed - c.brakePerTick;
        }

        if ((c.speed > limit && wanted < limit) || (c.speed <= limit && wanted > limit)) {
            wanted = limit;
        }

        double brakeRate = c.brakePerTick / secondsPerTick;
        if (wanted * secondsPerTick + (wanted * wanted) / (2.0 * brakeRate) <= room) {
            c.speed = wanted;
        } else if (c.speed - c.brakePerTick > 0) {
            c.speed = c.speed - c.brakePerTick; // not enough room so brake

        } else {
            c.speed = 0;
        }

        // move (never into the car ahead) and go onto the next road if we went off the end
        double move = c.speed * secondsPerTick;
        if (move > room) {
            move = room;
            c.speed = move / secondsPerTick;
        }

        c.positionAlongRoad = c.positionAlongRoad + move;
        
        while (c.positionAlongRoad > c.road->length && c.pathIndex < c.pathLength - 1) {
            c.positionAlongRoad = c.positionAlongRoad - c.road->length;
            c.pathIndex = c.pathIndex + 1;
            c.road = c.path[c.pathIndex];
        }


        // made it to the end of the exit road: trip done, start a new one
        if (c.pathIndex == c.pathLength - 1 && c.positionAlongRoad >= c.road->length) {
            c.tripsCompleted = c.tripsCompleted + 1;
            c.totalTripTicks = c.totalTripTicks + (tick - c.startTrip);
            startTrip(intersections, &c, tick);
        }
    }

    *out = c;
}

// one tick, one car at a time
void step(struct road * intersections, struct carIdx * idx, struct car * old, struct car * new, int n, int tick) {
    buildIdx(intersections, idx, old, n);
    for (int i = 0; i < n; i++) {
        updateCar(intersections, idx, old, i, tick, &new[i]);
    }
}

// same thing on threads, every car only writes its own spot in new so no locks needed
void stepParallel(struct road * intersections, struct carIdx * idx, struct car * old, struct car * new, int n, int tick) {
    buildIdx(intersections, idx, old, n);
    
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; i++) {
        updateCar(intersections, idx, old, i, tick, &new[i]);
    }
}

// lots of ticks on threads, only starting the threads once
void runParallel(struct road * intersections, struct carIdx * idx, struct car * * cars, struct car * * next, int n, int firstTick, int ticks) {
    #pragma omp parallel
    for (int t = 0; t < ticks; t++) {
        
        #pragma omp single
        buildIdx(intersections, idx, *cars, n);
        struct car * old = *cars;
        struct car * new = *next;
        
        #pragma omp for schedule(static)
        for (int i = 0; i < n; i++) {
            updateCar(intersections, idx, old, i, firstTick + t, &new[i]);
        }
        
        #pragma omp single
        {
            *cars = new;
            *next = old;
        }
    }
}

// a whole simulation on one thread, returns how many trips got finished
long runSerial(struct road * intersections, int numCars, int numTicks, unsigned seed, long * tripTicks) {
    struct carIdx * idx = newIdx(numCars);
    struct car * s = malloc(sizeof(struct car) * numCars);
    struct car * next = malloc(sizeof(struct car) * numCars);
    initCar(intersections, s, numCars, seed);
    
    for (int tick = 0; tick < numTicks; tick++) {
        step(intersections, idx, s, next, numCars, tick);
        struct car * foo = s;
        s = next;
        next = foo;
    }
    long trips = 0;
    *tripTicks = 0;
    for (int i = 0; i < numCars; i++) {
        trips = trips + s[i].tripsCompleted;
        *tripTicks = *tripTicks + s[i].totalTripTicks;
    }

    free(s);
    free(next);
    freeIdx(idx);
    return trips;
}

void printResults(struct car * cars, int n, int tick) {
    long trips = 0;
    long ticks = 0;
    int onRoad = 0;
    
    for (int i = 0; i < n; i++) {
        trips = trips + cars[i].tripsCompleted;
        ticks = ticks + cars[i].totalTripTicks;
        onRoad = onRoad + cars[i].active;
    }
    
    double avgSec = 0;
    if (trips > 0) {
        avgSec = (double)ticks / trips * secondsPerTick;
    }
    
    printf("tick %7d (%7.1f s)  on road %4d  trips %7ld  avg trip %6.1f s\n", tick, tick * secondsPerTick, onRoad, trips, avgSec);
}