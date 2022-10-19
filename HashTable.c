#include "HashTable.h"
#include "LinkedList.h"
#include "KeyValuePair.h"

struct hashTable_s{
    int size;
    LinkedList** linkedLists;

    CopyFunction copyKey;
    FreeFunction freeKey;
    PrintFunction printKey;
    CopyFunction copyValue;
    FreeFunction freeValue;
    PrintFunction printValue;
    EqualFunction equalKey;
    TransformIntoNumberFunction transformIntoNumber;
};

hashTable createHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey, TransformIntoNumberFunction transformIntoNumber, int hashNumber){
    if(copyKey==NULL ||freeKey==NULL ||printKey==NULL ||copyValue==NULL ||freeValue==NULL ||printValue==NULL ||equalKey==NULL ||transformIntoNumber==NULL || hashNumber < 1)
        return NULL;

    hashTable hash = (hashTable) malloc(sizeof(struct hashTable_s));
    if (hash==NULL) {
        printf("Memory Problem\n");
        return NULL;
    }

    hash->linkedLists = (LinkedList**) malloc(hashNumber*sizeof(LinkedList*));
    if (hash->linkedLists==NULL){
        printf("Memory Problem\n");
        free(hash);
        return NULL;
    }

    for (int i = 0; i < hashNumber; ++i) {
        hash->linkedLists[i] = NULL;
    }

    hash->size=hashNumber;
    hash->copyKey=copyKey; // multi = copy_phy_name_H //
    hash->freeKey=freeKey;  // multi  = free_phy_key_H //
    hash->printKey=printKey; // multi = print_phy_H //
    hash->copyValue=copyValue; // multi = copy jerry //
    hash->freeValue=freeValue; // multi = destroyList //
    hash->printValue=printValue;  // multi = displayList //
    hash->equalKey=equalKey; // multi = equal_phy_names_H //
    hash->transformIntoNumber=transformIntoNumber;

    return hash;
}
//DONE
status destroyHashTable(hashTable hashTable1){
    if (hashTable1==NULL)
        return failure;

    for (int i = 0; i < hashTable1->size; ++i) {
        destroyList(hashTable1->linkedLists[i]);
        hashTable1->linkedLists[i]=NULL;
    }
    free(hashTable1->linkedLists);
    hashTable1->linkedLists = NULL;
    free(hashTable1);
    hashTable1=NULL;
    return success;
}

status addToHashTable(hashTable h, Element key, Element value){
    if (h==NULL)
        return failure;
    if (key==NULL||value==NULL)
        return failure;

    int x = h->transformIntoNumber(key);
    int place = x % h->size;

    KVP* kvp = createKeyValuePair(key,value,h->copyKey,h->freeKey,h->printKey,h->equalKey,h->copyValue,h->freeValue,h->printValue);
                                //phy_name, jerries_list, copy_phy_name_H,free_phy_key_H,print_phy_H,equal_phy_names_H, --copy--, destroyList, displayList

    // node->value = KVP.
    // KEY - jerry_id or phycial_character
    if (h->linkedLists[place] == NULL) {
        h->linkedLists[place] = createLinkedList(h->copyKey, (FreeFunction) destroyKeyValuePair, (PrintFunction) displayValue, (EqualFunction) isEqualKey);
        if (h->linkedLists[place] == NULL)
            return failure;
    }
    return appendNode(h->linkedLists[place], kvp);
}

Element lookupInHashTable(hashTable hashTable1, Element key){
    if (hashTable1==NULL || key==NULL)
        return NULL;

    int x = hashTable1->transformIntoNumber(key);
    int place = x % hashTable1->size;

    //search the key value pair by key. return KVP
    Element element = searchByKeyInList(hashTable1->linkedLists[place], key);
    if (element==NULL)
        return NULL;
    return element;
}

status removeFromHashTable(hashTable hashTable1, Element key){
    if (hashTable1==NULL || key==NULL)
        return failure;

    int x = hashTable1->transformIntoNumber(key);
    int place = x % hashTable1->size;

    //search the key value pair by key. delete KVP
    Element element = searchByKeyInList(hashTable1->linkedLists[place], key); // element = kvp
    if (element != NULL){
        if (deleteNode(hashTable1->linkedLists[place], key)==true) //the list automatic deleted if empty
            return success;
    }
    return failure;
}

status displayHashElements(hashTable hashTable1){
    if (hashTable1==NULL)
        return failure;

    for (int i = 0; i < hashTable1->size; ++i) {
        displayList(hashTable1->linkedLists[i]);
    }
    return success;
}
