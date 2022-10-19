#include <stdio.h>
#include <stdlib.h>

#include "Jerry.h"
#include "MultiValueHashTable.h"


//VARIABLES OF GLOBAL LISTS
int numberOfPlanets=0;

Planet** G_Planet = NULL;
hashTable hash_of_jerrys = NULL;
MultiValueHashTable* hash_of_physicalC = NULL;
LinkedList* All_Jerries_Linked_list = NULL;
LinkedList* ALL_Physsical_Linked_list = NULL;

status take_Jerry_to_day_care(){
    char jerry_id[300];
    char planet_name[300];
    char origin_name[300];
    Planet *planet = NULL;
    Origin* origin = NULL;
    Jerry* jerry = NULL;
    int happiness;

    printf("What is your Jerry's ID ? \n");
    scanf("%s",jerry_id);
    if (getValue((KVP *) (lookupInHashTable(hash_of_jerrys, jerry_id) )) != NULL){
        printf("Rick did you forgot ? you already left him here ! \n");
        return failure;
    }
    printf("What planet is your Jerry from ? \n");
    scanf("%s",planet_name);
    for (int i = 0; i < numberOfPlanets; ++i) {
        if (strcmp(planet_name, G_Planet[i]->name) == 0) {
            planet = G_Planet[i];
            break;
        }
        if (i == numberOfPlanets-1) {
            printf("%s is not a known planet ! \n", planet_name);
            return failure;
        }
    }
    printf("What is your Jerry's dimension ? \n");
    scanf("%s",origin_name);
    printf("How happy is your Jerry now ? \n");
    scanf("%d",&happiness);
    origin = createOrigin(origin_name, planet);
    if (origin==NULL){
        printf("Memory Problem \n");
        return failure;
    }
    jerry = createJerry(jerry_id, happiness, origin);
    if (jerry==NULL){
        printf("Memory Problem \n");
        deleteOrigin(origin);
        return failure;
    }
    appendNode(All_Jerries_Linked_list,jerry);
    addToHashTable(hash_of_jerrys, jerry_id, jerry);
    printJerry(jerry);
    return success;
}

status insert_PhysicalCharacteristics_to_jerry(){
    char jerry_id[300];
    char phy[300];
    double value;
    printf("What is your Jerry's ID ? \n");
    scanf("%s",jerry_id);
    KVP* kvp_to_add = lookupInHashTable(hash_of_jerrys, jerry_id);
    if (kvp_to_add==NULL){
        printf("Rick this Jerry is not in the daycare ! \n");
        return failure;
    }
    printf("What physical characteristic can you add to Jerry - %s ? \n", jerry_id);
    scanf("%s",phy);
    if (checkPhy(phy, getValue(kvp_to_add))==true){
        printf("The information about his %s already available to the daycare ! \n",phy);
        return failure;
    }
    printf("What is the value of his %s ? \n", phy);
    scanf("%lf",&value);


    PhysicalCharacteristics* physicalCharacteristics = createPhysicalCharacteristics(phy, value);
    if (physicalCharacteristics==NULL){
        printf("Memory Problem \n");
        return failure;
    }

    insertPhy(physicalCharacteristics, getValue(kvp_to_add));

    //insert to multi value hash table - if p_c not exist - create new kvp with linked list as value inside
    if (addToMultiValueHashTable(hash_of_physicalC, phy, kvp_to_add)==failure)
        return failure; //todo memory exit

    displayMultiValueHashElementsByKey(hash_of_physicalC,physicalCharacteristics->name);
    return success;
}

status delete_PhysicalCharacteristics_from_jerry(){
    char jerry_id[300];
    char phy[300];
    printf("What is your Jerry's ID ? \n");
    scanf("%s",jerry_id);
    Jerry* jerry =  getValue(lookupInHashTable(hash_of_jerrys, jerry_id));
    if (jerry==NULL){
        printf("Rick this Jerry is not in the daycare ! \n");
        return failure;
    }
    printf("What physical characteristic do you want to remove from Jerry - %s ? \n", jerry_id);
    scanf("%s",phy);
    if (checkPhy(phy, jerry)==false){
        printf("The information about his %s not available to the daycare ! \n",phy);
        return failure;
    }
    if (deletePhy(phy, jerry)==success){
        if (removeFromMultiValueHashTable(hash_of_physicalC, phy, jerry_id) == success){
            printJerry(jerry);
            return success;
        }
    }
    return failure;
}

status take_Jerry_from_day_care(){
    char jerry_id[300];

    printf("What is your Jerry's ID ? \n");
    scanf("%s",jerry_id);
    Jerry* jerry =  getValue(lookupInHashTable(hash_of_jerrys, jerry_id));
    if (jerry==NULL){
        printf("Rick this Jerry is not in the daycare ! \n");
        return failure;
    }
    PhysicalCharacteristics** pPhysicalCharacteristics = jerry->phyP;
    for (int i = 0; i < jerry->countPhy; ++i) {
        removeFromMultiValueHashTable(hash_of_physicalC, pPhysicalCharacteristics[i]->name, jerry->ID);
    }
    removeFromHashTable(hash_of_jerrys, jerry_id);
    deleteNode(All_Jerries_Linked_list, jerry_id);
    printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
    return success;
}

status give_similar_jerry_by_PhysicalCharacteristics() {
    char phy[300];
    double value;
    printf("What do you remember about your Jerry ? \n");
    scanf("%s", phy);
    LinkedList *jerries = getValue(lookupInMultiValueHashTable(hash_of_physicalC, phy));
    if (jerries == NULL) {
        printf("Rick we can not help you - we do not know any Jerry's %s ! \n", phy);
        return failure;
    }
    printf("What do you remember about the value of his %s ? \n", phy);
    scanf("%lf", &value);

    Jerry *jerry, *returned_jerry;
    double jerry_phy_value;
    returned_jerry = getValue(getDataByIndex(jerries, 0));

    for (int i = 0; i < getCountPhy(returned_jerry); ++i) {
        if (strcmp(returned_jerry->phyP[i]->name, phy) == 0) {
            jerry_phy_value = returned_jerry->phyP[i]->value;
            break;
        }
    }

    // linked list of all the jerries in physical characteristic
    for (int i = 1; i < getLengthList(jerries); ++i) {
        jerry = getValue(getDataByIndex(jerries, i));
        for (int j = 0; j < jerry->countPhy; ++j) {
            if (strcmp(jerry->phyP[j]->name, phy) == 0) {
                if (abs(value - jerry_phy_value) > abs(value - jerry->phyP[j]->value)) {
                    returned_jerry = jerry;
                    jerry_phy_value = jerry->phyP[j]->value;
                    break;
                }
            }
        }
    }
    printf("Rick this is the most suitable Jerry we found : \n");
    printJerry(returned_jerry);
    printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");

    removeFromMultiValueHashTable(hash_of_physicalC, phy, returned_jerry->ID);
    removeFromHashTable(hash_of_jerrys, returned_jerry->ID);
    deleteNode(All_Jerries_Linked_list, returned_jerry->ID);
    return success;
}

status give_me_the_saddest_jerry(){
    if (getLengthList(All_Jerries_Linked_list)==0){
        printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
        return success;
    }
    Jerry* jerry;
    jerry = getDataByIndex(All_Jerries_Linked_list,0);
    int index_of_saddest_jerry = 0;
    for (int i = 1; i < getLengthList(All_Jerries_Linked_list); ++i) {
        if (jerry->happinessLevel > getHappiness(getDataByIndex(All_Jerries_Linked_list,i))) {
            jerry = getDataByIndex(All_Jerries_Linked_list, i);
            index_of_saddest_jerry = i;
        }
    }
    printf("Rick this is the most suitable Jerry we found : \n");
    printJerry(getDataByIndex(All_Jerries_Linked_list, index_of_saddest_jerry));
    for(int j=0; j < jerry->countPhy; j++ ){
    	removeFromMultiValueHashTable(hash_of_physicalC, jerry->phyP[j]->name, jerry->ID);
    }
    removeFromHashTable(hash_of_jerrys, getJerryName(getDataByIndex(All_Jerries_Linked_list, index_of_saddest_jerry)));
    deleteNode(All_Jerries_Linked_list, getJerryName(getDataByIndex(All_Jerries_Linked_list,index_of_saddest_jerry)));
    printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
    return success;
}

status show_jerries_info(){
    char phy[300];
    char ch[300];
    memset(ch,0,300);

    printf("What information do you want to know ? \n");
    printf("1 : All Jerries \n");
    printf("2 : All Jerries by physical characteristics \n");
    printf("3 : All known planets \n");

    scanf("%s", ch);

    if (strlen(ch) > 1){
        printf("Rick this option is not known to the daycare ! \n");
        return failure;
    }
    else if (ch[0]==49) {
        if (getLengthList(All_Jerries_Linked_list) == 0) {
            printf("Rick we can not help you - we currently have no Jerrise in the daycare ! \n");
            return failure;
        }
        displayList(All_Jerries_Linked_list);
        return success;
    }
    else if (ch[0]==50) {
        printf("What physical characteristics ? \n");
        scanf("%s", phy);

        if (lookupInMultiValueHashTable(hash_of_physicalC, phy) == NULL) {
            printf("Rick we can not help you - we do not know any Jerry's %s ! \n", phy);
            return failure;
        }
        displayMultiValueHashElementsByKey(hash_of_physicalC, phy);
        return success;
    }
    else if (ch[0]==51) {
        for (int i = 0; i < numberOfPlanets; ++i) {
            printPlanet(G_Planet[i]);
        }
        return success;
    } else {
        printf("Rick this option is not known to the daycare ! \n");
        return failure;
    }
}

status Let_the_Jerries_play(){
    if (getLengthList(All_Jerries_Linked_list)==0){
        printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
        return success;
    }
    char ch[300];
    memset(ch,0,300);

    Jerry* j;
    printf("What activity do you want the Jerrires to partake in ? \n");
    printf("1 : Interact with fake Beth \n");
    printf("2 : Play golf \n");
    printf("3 : Adjust the picture settings on the TV \n");

    scanf("%s",ch);
    if (strlen(ch) > 1){
        printf("Rick this option is not known to the daycare ! \n");
        return failure;
    }
    else if (ch[0]==49) {
        for (int i = 0; i < getLengthList(All_Jerries_Linked_list); ++i) {
            j = getDataByIndex(All_Jerries_Linked_list, i);
            if (getHappiness(j) >= 20) {
                setHappiness(j, getHappiness(j) + 15);
            } else setHappiness(j, getHappiness(j) - 5);
        }
        printf("The activity is now over ! \n");
        displayList(All_Jerries_Linked_list);
        return success;
    }
    else if (ch[0]==50) {
        for (int i = 0; i < getLengthList(All_Jerries_Linked_list); ++i) {
            j = getDataByIndex(All_Jerries_Linked_list, i);
            if (getHappiness(j) >= 50) {
                setHappiness(j, getHappiness(j) + 10);
            } else setHappiness(j, getHappiness(j) - 10);
        }
        printf("The activity is now over ! \n");
        displayList(All_Jerries_Linked_list);
        return success;
    }
    else if (ch[0]==51) {
        for (int i = 0; i < getLengthList(All_Jerries_Linked_list); ++i) {
            j = getDataByIndex(All_Jerries_Linked_list, i);
            setHappiness(j, getHappiness(j) + 20);
        }
        printf("The activity is now over ! \n");
        displayList(All_Jerries_Linked_list);
        return success;
    } else {
        printf("Rick this option is not known to the daycare ! \n");
        return failure;
    }
}

void Exit(){
    destroyList(All_Jerries_Linked_list);
    All_Jerries_Linked_list=NULL;
    destroyMultiValueHashTable(hash_of_physicalC);
    hash_of_physicalC=NULL;
    destroyList(ALL_Physsical_Linked_list);
    ALL_Physsical_Linked_list=NULL;
    destroyHashTable(hash_of_jerrys);
    hash_of_jerrys=NULL;


    for (int i = 0; i < numberOfPlanets; ++i) {
        deletePlanet(G_Planet[i]);
        G_Planet[i]=NULL;
    }
    free(G_Planet);
    G_Planet = NULL;
    printf("The daycare is now clean and close ! \n");
}

int main(int argc, char* argv[]) {
	//INITIALIZE GLOBAL VARIABLES
	numberOfPlanets = atoi(argv[1]);
	char *configurationFile = argv[2];
	char line[300];



	//read data from file
	FILE *fp = fopen(configurationFile, "r");
	if(fp==NULL)
		return -1;

    G_Planet =(Planet**) malloc(sizeof(Planet*) * numberOfPlanets);
    if (G_Planet == NULL){
        printf("Memory Problem \n");
        return -1;
    }

	//planets
	fgets(line, 300, fp);
    fgets(line, 300, fp);


	//DECLARE VARIABLES
	char* input=NULL;
	char* pos=NULL;
	char* ptr = NULL;
    char* planet = NULL;
    char cp_line[300];

	int COUNTER_OF_PALNETS=0;
    int i;

	double X=0;
	double Z=0;
	double Y=0;

	//LOOP UNTIL WE REACH TO JERRIES
    while(strcmp(line, "Jerries") != 0){
        memset(cp_line,'\0', sizeof(cp_line));
        strcpy(cp_line,line);
        input=strtok(line, ",");
        planet=input;

        //CONVERT STRING TO DOUBLE AND INSERT TO X,Y,Z
        for(i=0; i<3; i++){
        	input = strtok(NULL, ",");

            if(i==0)
            	X = strtod(input,&ptr);
			if(i==1)
				Y = strtod(input,&ptr);
			if(i==2)
				Z = strtod(input,&ptr);
        }

        //CREATE NEW PLANET AND INSERT TO  THE GLOBAL PLANETS LIST
        G_Planet[COUNTER_OF_PALNETS] = createPlanet(planet,X,Y,Z);
        if (G_Planet[COUNTER_OF_PALNETS]==NULL){
            printf("Memory Problem \n");
            for (int k = 0; k < COUNTER_OF_PALNETS; ++k) {
                deletePlanet(G_Planet[k]);
            }
            free(G_Planet);
            return -1;
        }
        COUNTER_OF_PALNETS++;
        fgets(line, 300, fp);
        if((pos=strchr(line, '\n'))!=NULL)
            *pos='\0';
        if((pos=strchr(line, '\r'))!=NULL)
            *pos='\0';

    }
    //we reached to JERRIES

    Origin* origin_to_add=NULL;
    Planet* planet_to_add=NULL;
    Jerry* new_jerry=NULL;
    PhysicalCharacteristics* phyc_to_add=NULL;
    char* jerry_name=NULL;
    int happiness_to_add;
	int COUNTER_OF_JERRIES=0;

    char* inserted_somthing=NULL;
    char* attribute_to_add=NULL;
    char* origin_name=NULL;
    char* pch;
    double attribute_value=0;

    int counter, j;
    bool exit_loop = false;

    All_Jerries_Linked_list = createLinkedList(copy_jerry_H, (FreeFunction) deleteJerry, print_Jerry_H, equal_jerry_to_name);
    if (All_Jerries_Linked_list == NULL){
        printf("Memory Problem \n");
        for (int k = 0; k < numberOfPlanets; ++k) {
            deletePlanet(G_Planet[k]);
        }
        free(G_Planet);
        return -1;
    }
    ALL_Physsical_Linked_list = createLinkedList(copy_id_H, free_jerry_H, print_phy_H, equal_key_H);
    if (ALL_Physsical_Linked_list==NULL){
        printf("Memory Problem \n");
        for (int k = 0; k < numberOfPlanets; ++k) {
            deletePlanet(G_Planet[k]);
            G_Planet[k] = NULL;
        }
        free(G_Planet);
        return -1;
    }

    //insert data

    while(fgets(line, sizeof(line), fp) != NULL){
        while (line[0]=='\t') {
            strcpy(cp_line,line);
            attribute_to_add = strtok(cp_line, ":");
            attribute_to_add++;

            pch = strtok (line,":");
            pch = strtok (NULL, ":");
            attribute_value = strtod(pch, &ptr);

            phyc_to_add = createPhysicalCharacteristics(attribute_to_add, attribute_value);
            if (phyc_to_add==NULL){
                printf("Memory Problem \n");
                for (int k = 0; k < numberOfPlanets; ++k) {
                    deletePlanet(G_Planet[k]);
                    G_Planet[k] = NULL;
                }
                destroyList(All_Jerries_Linked_list);
                destroyList(ALL_Physsical_Linked_list);
                free(G_Planet);
                return -1;
            }


            if (searchByKeyInList(ALL_Physsical_Linked_list,phyc_to_add->name) == NULL)
                appendNode(ALL_Physsical_Linked_list, phyc_to_add->name);

            insertPhy(phyc_to_add, getDataByIndex(All_Jerries_Linked_list, COUNTER_OF_JERRIES-1));

            if(fgets(line, sizeof(line), fp)==NULL) {
                exit_loop = true;
                break;
            }
        }
        if (exit_loop==true)
            break;

        counter=0;
        inserted_somthing=strtok(line,",");

        while(inserted_somthing!=NULL){
            if(counter==0)
            	jerry_name=inserted_somthing;
            if(counter==1)
				 origin_name=inserted_somthing;
            if(counter==2){
            	for(j = 0; j < numberOfPlanets ; j++){
            		if(strcmp(G_Planet[j]->name,inserted_somthing)==0){
            			planet_to_add = G_Planet[j];
            			origin_to_add=createOrigin(origin_name, planet_to_add);
                        if (origin_to_add==NULL){
                            printf("Memory Problem \n");
                            for (int k = 0; k < numberOfPlanets; ++k) {
                                deletePlanet(G_Planet[k]);
                                G_Planet[k] = NULL;
                            }
                            destroyList(All_Jerries_Linked_list);
                            destroyList(ALL_Physsical_Linked_list);
                            free(G_Planet);
                            return -1;
                        }
            			break;
            		}
            	}
            }
            if(counter==3)
            	happiness_to_add = atoi(inserted_somthing);

            counter++;
            inserted_somthing=strtok(NULL,",");
        }

        new_jerry = createJerry(jerry_name,happiness_to_add,origin_to_add);
        if (new_jerry==NULL){
            printf("Memory Problem \n");
            for (int k = 0; k < numberOfPlanets; ++k) {
                deletePlanet(G_Planet[k]);
                G_Planet[k] = NULL;
            }
            destroyList(All_Jerries_Linked_list);
            destroyList(ALL_Physsical_Linked_list);
            free(G_Planet);
            return -1;
        }
        appendNode(All_Jerries_Linked_list, new_jerry);
        COUNTER_OF_JERRIES++;
    }
    hash_of_jerrys = createHashTable(copy_id_H, free_key_H, print_id_H, copy_jerry_H, free_jerry_H, print_Jerry_H, equal_key_H, transform_key_to_num_H, COUNTER_OF_JERRIES);

    //memory check
    if (hash_of_jerrys == NULL){
        printf("Memory Problem \n");
        for (int k = 0; k < numberOfPlanets; ++k) {
            deletePlanet(G_Planet[k]);
        }
        destroyList(All_Jerries_Linked_list);
        destroyList(ALL_Physsical_Linked_list);
        free(G_Planet);
        return -1;
    }

    hash_of_physicalC = createMultiValueHashTable(copy_id_H, free_key_H, print_phy_H, copy_jerry_H, free_jerry_H, (PrintFunction) displayValue, equal_key_H, transform_key_to_num_H, getLengthList(ALL_Physsical_Linked_list));
    //memory check
    if (hash_of_physicalC == NULL){
        printf("Memory Problem \n");
        for (int k = 0; k < numberOfPlanets; ++k) {
            deletePlanet(G_Planet[k]);
        }
        destroyList(All_Jerries_Linked_list);
        destroyHashTable(hash_of_jerrys);
        free(G_Planet);
        return -1;
    }

    Jerry* jerry=NULL;
    //insert to hash of jerries
    for (int k = 0; k < getLengthList(All_Jerries_Linked_list); ++k) {
        jerry = getDataByIndex(All_Jerries_Linked_list,k);
        addToHashTable(hash_of_jerrys, jerry->ID, jerry);
    }
    char* phy=NULL;
    KVP* kvp = NULL;
    //insert to Multi-hash pf physical
    for (int phy_i = 0; phy_i < getLengthList(ALL_Physsical_Linked_list); phy_i++) {
        phy = getDataByIndex(ALL_Physsical_Linked_list, phy_i);
        for (int jerry_index = 0; jerry_index < getLengthList(All_Jerries_Linked_list); jerry_index++) {
            jerry = getDataByIndex(All_Jerries_Linked_list, jerry_index);
            if (checkPhy(phy, jerry)==true) {
                kvp = lookupInHashTable(hash_of_jerrys, jerry->ID);
                if (kvp != NULL)
                    addToMultiValueHashTable(hash_of_physicalC, phy, kvp);
            }
        }
    }

    fclose(fp);


    char c[300];
    memset(c,0,300);
    do {
        printf("Welcome Rick, what are your Jerry's needs today ? \n");
        printf("1 : Take this Jerry away from me \n");
        printf("2 : I think I remember something about my Jerry \n");
        printf("3 : Oh wait. That can't be right \n");
        printf("4 : I guess I will take back my Jerry now \n");
        printf("5 : I can't find my Jerry. Just give me a similar one \n");
        printf("6 : I lost a bet. Give me your saddest Jerry \n");
        printf("7 : Show me what you got \n");
        printf("8 : Let the Jerries play \n");
        printf("9 : I had enough. Close this place \n");
        memset(c,0,300);
        scanf("%s",c);
        
        if (strlen(c) > 1)
            printf("Rick this option is not known to the daycare ! \n");
        else if (c[0]==49)
            take_Jerry_to_day_care();
        else if (c[0]==50)
            insert_PhysicalCharacteristics_to_jerry();
        else if (c[0]==51)
            delete_PhysicalCharacteristics_from_jerry();
        else if (c[0]==52)
            take_Jerry_from_day_care();
        else if (c[0]==53)
            give_similar_jerry_by_PhysicalCharacteristics();
        else if (c[0]==54)
            give_me_the_saddest_jerry();
        else if (c[0]==55)
            show_jerries_info();
        else if (c[0]==56)
            Let_the_Jerries_play();
        else if (c[0]==57)
            Exit();
        else
            printf("Rick this option is not known to the daycare ! \n");

    }while (c[0] != 57);

    return 0;
}
