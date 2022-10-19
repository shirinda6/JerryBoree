#include "KeyValuePair.h"

struct KeyValuePair{
    Element key;
    Element value;
    CopyFunction cpykey;
    FreeFunction freekey;
    PrintFunction printkey;
    EqualFunction equalkey;
    CopyFunction cpyval;
    FreeFunction freeval;
    PrintFunction printval;
};


KVP* createKeyValuePair(Element key, Element value, CopyFunction cpykey, FreeFunction frkey, PrintFunction prtkey, EqualFunction eqlkey, CopyFunction cpyval, FreeFunction frval, PrintFunction prtval){
    if (key == NULL)
        return NULL;
    if (value == NULL)
        return NULL;
    if (cpyval==NULL || frval==NULL || prtval==NULL || prtkey==NULL || frkey==NULL || eqlkey==NULL || cpykey==NULL)
        return NULL;

    KVP* kvp= (KVP*) malloc(sizeof(struct KeyValuePair));
    if (kvp==NULL) {
        printf("Memory Problem\n");
        return NULL;
    }

    kvp->cpykey=cpykey; // copy_id_H // multi - copy_phy_name_H
    kvp->freekey=frkey; // free_key_H // multi - free_phy_key_H
    kvp->printkey=prtkey; // print_id_H // multi - print_phy_H
    kvp->equalkey=eqlkey; // equal_key_H // multi - equal_phy_names_H

    kvp->cpyval=cpyval; // copy_jerry_H // multi - copy_jerry_H
    kvp->freeval=frval; // free_jerry_H // multi - destroyList
    kvp->printval=prtval; // print_Jerry_H // multi - displayList

    kvp->key = kvp->cpykey(key);
    kvp->value =kvp->cpyval(value);

    return kvp;
}
status destroyKeyValuePair(KVP* kv){
    if (kv==NULL)
        return success;

    kv->freeval(kv->value);
    kv->value = NULL;
    kv->freekey(kv->key);
    kv->key=NULL;
    free(kv);
    kv = NULL;
    return success;
}
void displayValue(KVP* kv){
    if (kv==NULL)
        return;

    kv->printval(kv->value);
}
void displayKey(KVP* kv){
    if (kv==NULL)
        return;

    kv->printkey(kv->key);
}

Element getValue(KVP* kv){
    if (kv==NULL)
        return NULL;

    return kv->value;
}
Element getKey(KVP* kv){
    if (kv==NULL)
        return NULL;

    return kv->key;
}

bool isEqualKey(KVP* kv, Element key){
    if (kv==NULL)
        return false;

    if (kv->equalkey(key,kv->key))
        return true;
    return false;
}
