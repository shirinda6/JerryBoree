#ifndef JERRYBOREE_KEYVALUEPAIR_H
#define JERRYBOREE_KEYVALUEPAIR_H

#include "Defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct KeyValuePair KVP;

KVP* createKeyValuePair(Element key, Element value, CopyFunction cpykey, FreeFunction frkey, PrintFunction prtkey, EqualFunction eqlkey, CopyFunction cpyval, FreeFunction frval, PrintFunction prtval);
status destroyKeyValuePair(KVP* kv);
void displayValue(KVP* kv);
void displayKey(KVP* kv);
Element getValue(KVP* kv);
Element getKey(KVP* kv);
bool isEqualKey(KVP* kv, Element key);


#endif //JERRYBOREE_KEYVALUEPAIR_H
