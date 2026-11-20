// DONE
#include <stdio.h>
#include <stdlib.h>

struct road {
    int start;
    int end;
    int exists;
    double length;
    double speedLimit;
    int yield;
};

struct car {
    double speed; // in m/s
    double topSpeed; // in m/s
    double length; // of the car in meters
    double accelPerTick; // in m/s^2
    double brakePerTick; // in m/s^2
    
    struct road * road; // road currently being travelled upon
    double positionAlongRoad; // travelled down in meters
    struct road * destination; // address of the destination road
    struct road * path[16]; // path of roads to be travelled
    
    int pathLength; // length of the path array
    int pathIndex; // current index in the path array

    int active; // 1 if the car is active, 0 if it has reached its destination
    int startTrip;
    long tripsCompleted;
    int totalTripTicks;
    unsigned foo; // random seed for the car's random number generator

};

// group cars together by road
struct carIdx {
    int numSlots;
    int capacity;
    int * groupStartIdx; // starting index of the group of cars on each road
    int * groupSize; // number of cars on each road
    int * carIdx;
    int * carActive; // driving cars
    int * frontQueIdx; // index of the front car in the queue on each road
};

// Fixed Values
static const int numIntersections = 21;
static const int numArmRoads = 4;
static const double secondsPerTick = 0.1; // 10 ticks per second
static const double carGap = 2.0; // minimum gap between cars in meters
static const double lookUpDistance = 80.0;

int readNumber(char const * text, int fallback);

// Internal Road Network Functions
struct road * buildRoundabout();
struct road * getRoad(struct road * intersections, int numIntersections, int intersection1, int intersection2);
struct road * entryRoad(struct road * intersections, int arm);
struct road * exitRoad(struct road * intersections, int arm);
int roundaboutIntersection(int node); // 1 if on the roundabout, 0 if on an arm

// Routes
int findPath(struct road * intersections, struct road * start, struct road * end, struct road * * pathOut, int maxPathLength);

void initCar(struct road * intersections, struct car * cars, int n, unsigned seed);
void startTrip(struct road * intersections, struct car * c, int tick);

struct carIdx * newIdx(int maxCars);
void freeIdx(struct carIdx * idx);
void buildIdx(struct road * intersections, struct carIdx*idx, struct car * old, int n);
void updateCar(struct road * intersections, struct carIdx * idx, struct car * old, int i, int tick, struct car * out);

// simulate in one tick
void step(struct road * intersections, struct carIdx * idx, struct car * old, struct car * new, int n, int tick);
void stepParallel(struct road * intersections, struct carIdx * idx, struct car * old, struct car * new, int n, int tick);

// simulate in many ticks
long runSerial(struct road * intersections, int numCars, int numTicks, unsigned seed, long * tripTicks);
void runParallel(struct road * intersections, struct carIdx * idx, struct car * * cars, struct car * * next, int n, int firstTick, int ticks);


void printResults(struct car * cars, int n, int tick);