#include "Jerry.h"


Jerry* createJerry(char *ID,int happy,Origin *origin){
	if (origin == NULL)
		return NULL;
	if (ID == NULL)
		return NULL;

    Jerry* j=(Jerry*) malloc(sizeof(struct Jerry_T));
    if (j==NULL) {
        printf("Memory Problem \n");
        return NULL;
    }

    j->ID = (char*) malloc(strlen(ID)+1);
    if (j->ID==NULL){
        printf("Memory Problem \n");
        free(j);
        return NULL;
    }


    strcpy(j->ID,ID);
    j->happinessLevel=happy;
    j->orignP = origin;
    j->countPhy=0;
    j->phyP=NULL;
    return j;
}

Planet* createPlanet(char *name,double x, double y, double z){
	if ( name == NULL)
		return NULL;

	Planet* P = (Planet*) malloc(sizeof(struct Planet_T));
    if (P==NULL) {
        printf("Memory Problem \n");
        return NULL;
    }
    P->name = (char*) malloc(strlen(name)+1);
    if (P->name==NULL){
        printf("Memory Problem \n");
        free(P);
        return NULL;
    }
    strcpy(P->name,name);
    P->X=x;
	P->Y=y;
	P->Z=z;
    return P;
}

PhysicalCharacteristics* createPhysicalCharacteristics(char* name, double value){
	if ( name == NULL )
		return NULL;

    PhysicalCharacteristics* pc = (PhysicalCharacteristics*)malloc(sizeof(struct PhysicalCharacteristics_T));
	if( pc==NULL ) {
        printf("Memory Problem \n");
        return NULL;
    }

	pc->name = (char*) malloc(strlen(name)+1);
    if (pc->name==NULL ){
        printf("Memory Problem \n");
	    free(pc);
	    pc=NULL;
	    return NULL;
    }
	strcpy(pc->name,name);
	pc->value=value;
	return pc;
}

Origin* createOrigin(char* name, Planet* p){
	if ( name == NULL)
		return NULL;
	if ( p == NULL)
		return NULL;

	Origin *O  = (Origin*) malloc(sizeof(struct Origin_T));
	if (O==NULL) {
        printf("Memory Problem \n");
        return NULL;
    }
    O->DimensionName = (char*) malloc(strlen(name)+1);
	if (O->DimensionName==NULL){
	    printf("Memory Problem \n");
	    free(O);
	    O = NULL;
	    return NULL;
	}
    strcpy(O->DimensionName,name);
    O->planetP = p;
    return O;
}

bool checkPhy(char* physical_name ,Jerry* J){
	if ( physical_name == NULL)
	    return false;
	if ( J == NULL)
	    return false;

	int i;
	for(i=0; i<J->countPhy; i++){
		if(strcmp(J->phyP[i]->name,physical_name)==0)
			return true;
	}
	return false;
}

status insertPhy(PhysicalCharacteristics* p ,Jerry* J){
    if ( p == NULL)
		return failure;
    if ( J == NULL)
	    return failure;

	//check if the attribute is exist in Jerry's attributes;
	if (checkPhy(p->name,J)==true)
		return failure;
    if (J->phyP==NULL)
    {
		 J->phyP = (PhysicalCharacteristics**)malloc(sizeof(PhysicalCharacteristics*));
		 if(J->phyP==NULL) {
             printf("Memory Problem \n");
             return failure;
         }
		 J->phyP[J->countPhy] = p;
		 J->countPhy++;
		 return success;
    }
	J->phyP = (PhysicalCharacteristics**)realloc(J->phyP, sizeof(PhysicalCharacteristics*) * (J->countPhy+1));
	if(J->phyP==NULL) {
        printf("Memory Problem \n");
        return failure;
    }

	J->phyP[J->countPhy] = p;
    J->countPhy++;
	return success;
}

status deletePhy(char* Pc ,Jerry* J){
	  if ( Pc == NULL)
			return success;
	  if ( J == NULL)
		    return success;
	  if (checkPhy(Pc,J) == true){
	      if (J->countPhy-1 > 0) {
              PhysicalCharacteristics **tmp = (PhysicalCharacteristics **) malloc(sizeof(PhysicalCharacteristics *) * (J->countPhy - 1));
              if (tmp == NULL) {
                  printf("Memory Problem \n");
                  return failure;
              }
              int i,co=0;
              for(i=0; i<J->countPhy; i++){
                  if(strcmp(J->phyP[i]->name,Pc) == 0)
                      Delete_single_phy(J->phyP[i]);
                  else{
                      tmp[co] = J->phyP[i];
                      co++;
                  }
              }
              free(J->phyP);
              J->phyP=NULL;
              J->phyP=tmp;
              J->countPhy--;
              return success;
          }
	      else{
              Delete_single_phy(J->phyP[0]);
              free(J->phyP);
              J->phyP=NULL;
              J->countPhy--;
              return success;
	      }
	  }
	  return failure;
}

void printPhyCOfJerry(Jerry* j){
	if (j->countPhy==0)
        return;

    int i;
	for(i=0; i<j->countPhy; i++){
		printf("%s : %.2lf", j->phyP[i]->name, j->phyP[i]->value);
		if(i == j->countPhy-1){
            printf(" \n");
		    break;
		}

		printf(" , ");
	}
}

void printJerry(Jerry* J){
	printf("Jerry , ID - %s : \nHappines level : %d \nOrigin : %s \n",J->ID,J->happinessLevel,J->orignP->DimensionName);
	printPlanet(J->orignP->planetP);
	if (J->countPhy==0)
        return;

	printf("Jerry's physical Characteristics available: \n\t");
	printPhyCOfJerry(J);
}

void printPlanet(Planet* P){
	printf("Planet : %s (%.2lf,%.2lf,%.2lf) \n",P->name,P->X,P->Y,P->Z);
}

status Delete_single_phy(PhysicalCharacteristics* Pc){
	if(Pc==NULL)
		return success;
	if(Pc->name==NULL){
		free(Pc);
		Pc=NULL;
		return success;
	}
	free(Pc->name);
	Pc->name=NULL;
	free(Pc);
	Pc=NULL;
	return success;
}

status deleteOrigin(Origin* o){
	if(o == NULL)
		return success;
	if(o->DimensionName != NULL){
		free(o->DimensionName);
		o->DimensionName=NULL;
	}
	free(o);
	o=NULL;
	return success;
}

status deleteJerry(Jerry* j){
	if(j == NULL)
		return success;

	if(j->ID != NULL){
		free(j->ID);
		j->ID=NULL;
	}
	if(j->orignP != NULL) {
        deleteOrigin(j->orignP);
        j->orignP=NULL;
    }

	if(j->phyP != NULL){
		int i;
		for(i=0; i < j->countPhy; i++){
			Delete_single_phy(j->phyP[i]);
            j->phyP[i]=NULL;
		}
		free(j->phyP);
		j->phyP=NULL;
	}
	j->countPhy=0;
	free(j);
	j=NULL;
	return success;
}

status deletePlanet(Planet* p){
    if ( p==NULL )
        return success;
    if (p->name != NULL){
        free(p->name);
        p->name=NULL;
    }
    free(p);
    p=NULL;
    return success;
}


int getCountPhy(Jerry *j) {
    if (j==NULL)
        return 0;
    return j->countPhy;
}

int getHappiness(Jerry* j) {
    if (j==NULL)
        return 0;
    return j->happinessLevel;
}

Element copy_id_H(Element id) {
    if (id==NULL)
        return NULL;
    char* name = (char*) malloc(strlen(id)+1);
    if (name==NULL) {
        printf("Memory Problem \n");
        return NULL;
    }
    strcpy(name,id);
    return name;
}

Element copy_jerry_H(Element j) {
    return j;
}

status free_key_H(Element name) {
    if (name==NULL)
        return failure;
    free(name);
    name = NULL;
    return success;
}

status print_id_H(Element id) {
    if (id==NULL)
        return failure;
    printf("%p \n",id);
    return success;
}

int transform_key_to_num_H(Element j) {
    char* name = (char*) j;
    int ascii_list[strlen(name)],i=0,num=0;
    while(name[i] != '\0') {
        ascii_list[i]=name[i];
        num+=ascii_list[i];
        i++;
    }
    return num;
}

status free_jerry_H(Element j) {
     return success;
}

status print_Jerry_H(Element j) {
    if (j==NULL)
        return failure;
    printJerry(j);
    return success;
}

bool equal_key_H(Element ID_1, Element ID_2) {
    if (ID_1==NULL || ID_2==NULL)
        return false;
    char* id1 = (char*) ID_1;
    char* id2 = (char*) ID_2;

    if (strcmp(id1,id2) == 0)
        return true;
    return false;
}

status print_phy_H(Element phy) {
    if (phy==NULL)
        return failure;
    char* p = (char*) phy;
    printf("%s : \n",p);
    return success;
}

char *getJerryName(Jerry *j) {
    if (j==NULL)
        return NULL;
    return j->ID;
}

status setHappiness(Jerry *j, int num) {
    if (num > 100) {
        j->happinessLevel = 100;
        return success;
    }
    if (num<0) {
        j->happinessLevel = 0;
        return success;
    }
    j->happinessLevel = num;
    return success;
}

bool equal_jerry_to_name(Element jerry, Element ID) {
    if (jerry==NULL || ID==NULL)
        return false;

    if (strcmp(getJerryName(jerry),ID) == 0)
        return true;
    return false;
}
