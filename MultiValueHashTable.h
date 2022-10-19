#ifndef JERRYBOREE_MULTIVALUEHASHTABLE_H
#define JERRYBOREE_MULTIVALUEHASHTABLE_H

#include "HashTable.h"
#include "LinkedList.h"
#include "KeyValuePair.h"

typedef struct MultiValueHashTable_T MultiValueHashTable;

MultiValueHashTable* createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey, TransformIntoNumberFunction transformIntoNumber, int hashNumber);
status destroyMultiValueHashTable(MultiValueHashTable* multiValueHashTable);
status addToMultiValueHashTable(MultiValueHashTable* multiValueHashTable, Element key, Element value);
Element lookupInMultiValueHashTable(MultiValueHashTable* multiValueHashTable, Element key);
status removeFromMultiValueHashTable(MultiValueHashTable* multiValueHashTable, Element key, Element value);
void displayMultiValueHashElementsByKey(MultiValueHashTable* multiValueHashTable, Element key);

#endif //JERRYBOREE_MULTIVALUEHASHTABLE_H
