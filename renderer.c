
#include <stdio.h>
#include <stdlib.h>

// TODO: split traffic sim into separate file
struct road {
    int exists;
    int length;
};

struct car {
    double speed; // in m/s
    double topSpeed; // in m/s
    double length; // of the car in meters
    double accelPerTick; // in m/s^2
    double brakePerTick; // in m/s^2
    void * road; // TODO: make the type the road type
    // TODO: front wheel and rear wheel to handle travelling across an intersection?
    // Current plan: car looks up and down the road, and then looks a single edge down an intersection.
    double positionAlongRoad; // travelled down in meters
};

int main(int argc, char const *argv[]) 
{
    struct road * intersectionsAndRoads;
    
    
    free(intersectionsAndRoads);
    return 0;
}


void step() {

}

struct road * buildRoundabout() {
    // remember, this being an adjacency matrix means that this is a directed graph. These roads only go one way.
    struct road * roundabout = calloc(20 * 20 * 2, sizeof(int));
    
    struct road * r1 = getRoad(roundabout, 20, 1, 2);
    struct road * r2 = getRoad(roundabout, 20, 2, 3);
    struct road * r3 = getRoad(roundabout, 20, 3, 4);
    struct road * r4 = getRoad(roundabout, 20, 4, 5);

    struct road * r5 = getRoad(roundabout, 20, 6, 7);
    struct road * r6 = getRoad(roundabout, 20, 7, 8);
    struct road * r7 = getRoad(roundabout, 20, 8, 9);
    struct road * r8 = getRoad(roundabout, 20, 9, 10);

    struct road * r9 = getRoad(roundabout, 20, 11, 12);
    struct road * r10 = getRoad(roundabout, 20, 12, 13);
    struct road * r11 = getRoad(roundabout, 20, 13, 14);
    struct road * r12 = getRoad(roundabout, 20, 14, 15);

    struct road * r13 = getRoad(roundabout, 20, 16, 17);
    struct road * r14 = getRoad(roundabout, 20, 17, 18);
    struct road * r15 = getRoad(roundabout, 20, 18, 19);
    struct road * r16 = getRoad(roundabout, 20, 19, 20);

    struct road * r17 = getRoad(roundabout, 20, 3, 8);
    struct road * r18 = getRoad(roundabout, 20, 8, 13);
    struct road * r19 = getRoad(roundabout, 20, 13, 18);
    struct road * r20 = getRoad(roundabout, 20, 18, 3);
    
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


    // rough estimates of dimensions we pulled from a roundabout in TH.
    r1->length = 100;
    r2->length = 40;
    r3->length = 40;
    r4->length = 100;

    r5->length = 100;
    r6->length = 40;
    r7->length = 40;
    r8->length = 100;

    r9->length= 100;
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

}

struct road * getRoad(struct road * intersections, int numIntersections, int intersection1, int intersection2) {
    return intersections + intersection1 + (numIntersections * intersection2);
}
