
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "traffic.c"

#define NO_PATH 0

// pixels act for each intersection point 
int nodeX[21] = { 0, 408, 408, 400, 392, 392,  760, 560, 480, 560, 760,  392, 392, 400, 408, 408,  40, 240, 320, 240, 40 };
int nodeY[21] = { 0, 760, 560, 480, 560, 760,  392, 392, 400, 408, 408,  40, 240, 320, 240, 40,  408, 408, 400, 392, 392 };

int main(int argc, char const *argv[]) {
    int numCars = 60;
    unsigned seed = 1;

    // read stuff from the command line if its there
    if (argc > 1) {
        numCars = readNumber(argv[1], numCars);
    }
    if (argc > 2) {
        seed = readNumber(argv[2], seed);
    }
    if (numCars < 1) {
        numCars = 1;
    }
    
    // make the roundabout and the cars
    struct road * intersectionsAndRoads = buildRoundabout();
    struct car * s = malloc(sizeof(struct car) * numCars);
    struct car * next = malloc(sizeof(struct car) * numCars);
    
    initCar(intersectionsAndRoads, s, numCars, seed);
    struct carIdx * idx = newIdx(numCars);

    // one color per exit (makes my life easier)
    Color armColors[4] = {RED, GREEN, BLUE, ORANGE};

    InitWindow(800, 800, "simple traffic simulator");
    SetTargetFPS(60);

    int tick = 0;
    while (!WindowShouldClose()) {
        
        // move everyone one tick then swap the tables
        step(intersectionsAndRoads, idx, s, next, numCars, tick);
        
        struct car * foo = s;
        s = next;
        next = foo;
        tick = tick + 1;
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // gray line for every road that exists
        for (int a = 1; a < numIntersections; a++) {
            for (int b = 1; b < numIntersections; b++) {
                
                if ((*getRoad(intersectionsAndRoads, numIntersections, a, b)).exists == 1) {
                    DrawLine(nodeX[a], nodeY[a], nodeX[b], nodeY[b], GRAY);
                }
            }
        }

        // cars are dots
        long trips = 0;

        // count trips
        for (int i = 0; i < numCars; i++) {
            trips = trips + s[i].tripsCompleted;
            if (s[i].active == 0) {
                continue; // not on the road yet
            }

            struct road * r = s[i].road;
            double frac = s[i].positionAlongRoad / (*r).length; // 0 = start of road, 1 = end
            int x = nodeX[(*r).start] + frac * (nodeX[(*r).end] - nodeX[(*r).start]);
            int y = nodeY[(*r).start] + frac * (nodeY[(*r).end] - nodeY[(*r).start]);
            int arm = ((*s[i].destination).start - 4) / 5; // which exit

            DrawCircle(x, y, 4, armColors[arm]);
        }

        DrawText(TextFormat("time %.1f s   trips %ld", tick * secondsPerTick, trips), 10, 10, 20, BLACK);
        EndDrawing();
    }

    CloseWindow();

    // clean up
    free(s);
    free(next);
    freeIdx(idx);
    free(intersectionsAndRoads);
    printf("go home - fin\n");
    
    return 0;
}