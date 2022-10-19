#ifndef LinkedList_H_
#define LinkedList_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Defs.h"


typedef struct Node_t* Node;
typedef struct Linked_List_t LinkedList;

LinkedList* createLinkedList(CopyFunction cpy, FreeFunction fr, PrintFunction prt, EqualFunction eql);
status destroyList(LinkedList* list);
status appendNode(LinkedList* list, Element value);
bool deleteNode(LinkedList* list, Element value);
void displayList(LinkedList* list);
Element getDataByIndex(LinkedList* list, int index);
int getLengthList(LinkedList* list);
Element searchByKeyInList(LinkedList* list,Element elm );
void printnode(LinkedList* list, Node node);







#endif /*LinkedList_H_*/
