#include "MultiValueHashTable.h"

struct MultiValueHashTable_T{
    hashTable hash;

    EqualFunction equalKey;
    CopyFunction copyValue;
    FreeFunction freeValue;
    PrintFunction printValue;
};

MultiValueHashTable* createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey, TransformIntoNumberFunction transformIntoNumber, int hashNumber){
    if(copyKey==NULL ||freeKey==NULL ||printKey==NULL ||copyValue==NULL ||freeValue==NULL ||printValue==NULL ||equalKey==NULL ||transformIntoNumber==NULL)
        return NULL;

    MultiValueHashTable* multiValueHashTable = (MultiValueHashTable*) malloc(sizeof(struct MultiValueHashTable_T));
    if (multiValueHashTable == NULL){
        printf("Memory Problem\n");
        return NULL;
    }

    multiValueHashTable->hash = createHashTable(copyKey, freeKey, printKey, copyValue, (FreeFunction) destroyList, (PrintFunction) displayList, equalKey, transformIntoNumber, hashNumber);
    if (multiValueHashTable->hash==NULL) {
        free(multiValueHashTable);
        return NULL;
    }

    multiValueHashTable->equalKey=equalKey;
    multiValueHashTable->copyValue=copyValue;
    multiValueHashTable->freeValue=freeValue;
    multiValueHashTable->printValue=printValue;
    return multiValueHashTable;
}

status addToMultiValueHashTable(MultiValueHashTable* m, Element key, Element value) {
    if (m == NULL || key == NULL || value == NULL)
        return failure;

    // key = physical name
    // value = kvp

    Element element = lookupInMultiValueHashTable(m, key);
    if (element != NULL)
        return appendNode(getValue(element), value);

    LinkedList* list_of_kvp = createLinkedList(m->copyValue,m->freeValue, m->printValue, (EqualFunction) isEqualKey);
    if (list_of_kvp == NULL)
        return failure;

    if (appendNode(list_of_kvp, value)==success)
        return addToHashTable(m->hash, key, list_of_kvp);
    return failure;
}

Element lookupInMultiValueHashTable(MultiValueHashTable* multiValueHashTable, Element key){
    if (multiValueHashTable==NULL || key==NULL )
        return NULL;

    return lookupInHashTable(multiValueHashTable->hash, key);
}

status removeFromMultiValueHashTable(MultiValueHashTable* multiValueHashTable, Element key, Element value){
    if (multiValueHashTable==NULL || key==NULL || value==NULL)
        return failure;

    // key = physical name
    // value = jerry name
    Element element = getValue(lookupInMultiValueHashTable(multiValueHashTable, key)); //element = kvp's linked list  - the value of kvp of phy

    int list_lenght = getLengthList(element);
    if (element != NULL){
        if (deleteNode(element, value)==true) { //the list automatic deleted if empty
            list_lenght--;
            if (list_lenght == 0)
                return removeFromHashTable(multiValueHashTable->hash, key); //kvp auto-delete if empty from values
            return success;
        }
    }
    return failure;
}

void displayMultiValueHashElementsByKey(MultiValueHashTable* multiValueHashTable, Element key){
    if (multiValueHashTable==NULL || key==NULL )
        return;
    Element element = lookupInMultiValueHashTable(multiValueHashTable, key);
    if (element == NULL)
        return;
    displayKey(element);
    displayValue(element);
}

status destroyMultiValueHashTable(MultiValueHashTable* multiValueHashTable){
    if (multiValueHashTable==NULL)
        return failure;

    if (destroyHashTable(multiValueHashTable->hash)==success){
        multiValueHashTable->hash=NULL;
        free(multiValueHashTable);
        multiValueHashTable=NULL;
        return success;
    }
    return failure;
}
