#ifndef JERRY_H_
#define JERRY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Defs.h"

typedef struct Origin_T Origin;
typedef struct Jerry_T Jerry;
typedef struct Planet_T Planet;
typedef struct PhysicalCharacteristics_T PhysicalCharacteristics;


struct Planet_T {
    char* name;
    double X;
    double Y;
    double Z;
};

struct  Origin_T{
    Planet* planetP;
    char* DimensionName;
};

struct PhysicalCharacteristics_T {
    char *name;
    double value;
};

struct Jerry_T {
    char* ID;
    int happinessLevel;
    Origin* orignP;
    PhysicalCharacteristics** phyP;
    int countPhy;
};


Jerry* createJerry(char *ID,int happy,Origin *origin);
Planet* createPlanet(char *name,double x, double y, double z);
PhysicalCharacteristics* createPhysicalCharacteristics(char *name,double value);
Origin* createOrigin(char *name, Planet* p);
bool checkPhy(char *physical ,Jerry* J);
status insertPhy(PhysicalCharacteristics* p,Jerry* J);
status deletePhy(char* Pc ,Jerry* J);
status Delete_single_phy(PhysicalCharacteristics* Pc);
status deleteOrigin(Origin* o);
status deleteJerry(Jerry* j);
status deletePlanet(Planet* p);
void printJerry(Jerry* J);
void printPlanet(Planet* P);
void printPhyCOfJerry(Jerry* j);

int getCountPhy(Jerry* j);
status setHappiness(Jerry* j, int num);
int getHappiness(Jerry* j);
char* getJerryName(Jerry* j);

Element copy_id_H(Element id);
Element copy_jerry_H(Element j);
status free_key_H(Element name);
status print_id_H(Element id);
int transform_key_to_num_H(Element j);
status free_jerry_H(Element j);
status print_Jerry_H(Element j);
bool equal_key_H(Element ID_1, Element ID_2);
bool equal_jerry_to_name(Element jerry, Element ID);

status print_phy_H(Element phy);


#endif /* JERRY_H */
