#include "LinkedList.h"

struct Linked_List_t{
    Node First;
    Node Last;
    int List_Size;
    CopyFunction cpyfun;
    FreeFunction freefun;
    PrintFunction printfun;
    EqualFunction equalfun;
};

struct Node_t{
    Element NodeElement;
    Node Next;
    Node Prev;
};

//DONE
LinkedList* createLinkedList(CopyFunction cpy, FreeFunction fr, PrintFunction prt, EqualFunction eql){
    if (cpy==NULL || fr==NULL || prt==NULL || eql==NULL)
        return NULL;

    LinkedList* list = (LinkedList*) malloc (sizeof(struct Linked_List_t));
    if (list == NULL) {
        printf("Memory Problem\n");
        return NULL;
    }

    list->First = NULL;
    list->Last = NULL;
    list->List_Size = 0;

    list->cpyfun = cpy; // --
    list->freefun = fr; // destroyKVP
    list->printfun = prt; // --
    list->equalfun = eql; // isEqualKey
    return list;
}

//DONE

status destroyList(LinkedList* list) {
    if(list == NULL)
        return success;
    if (list->First != NULL){
        Node current = list->First, next;
        while (current != NULL) {
            next = current->Next;
            
            list->freefun(current->NodeElement);
            list->List_Size--;
            free(current);
            current = NULL;
            current = next;
        }
    }
    list->Last = NULL;
    free(list);
    return success;
}

//DONE
status appendNode(LinkedList* list, Element element){
    if (list==NULL)
        return failure;
    if (list->List_Size==0){
        Node node=(Node)malloc(sizeof(struct Node_t));
        if (node==NULL){
            printf("No memory available\n");
            return failure;
        }

        list->First=node;
        list->Last=node;
        node->NodeElement=element;
        node->Next=NULL;
        node->Prev=NULL;
        list->List_Size++;
        return success;
    }
    Node node=(Node)malloc(sizeof(struct Node_t));
    if (node==NULL) {
        printf("No memory available\n");
        return failure;
    }

    node->NodeElement=element;
    list->Last->Next=node;
    node->Prev=list->Last;
    list->Last=node;
    node->Next=NULL;
    list->List_Size++;
    return success;
}

//DONE
bool deleteNode(LinkedList* list, Element element) {
    if (list == NULL)
        return true;
    if (list->List_Size == 0)
        return true;

    //search
    Node tmp=list->First;
    if (!list->equalfun(tmp->NodeElement, element)) {
        while (tmp->Next != NULL) {
            if (list->equalfun(tmp->NodeElement,element))
                break;
            if (tmp==list->Last){
                if (tmp->NodeElement == element)
                    break;
                else return false;
            }
            tmp=tmp->Next;
        }
    }

    if (list->List_Size==1){
        list->freefun(tmp->NodeElement);
        list->List_Size--;
        free(list->First);
        list->First = NULL;
        list->Last = NULL;
        return true;
    }
    if (tmp==list->Last) {
        tmp->Prev->Next=NULL;
        list->Last=list->Last->Prev;
    }
    else if (tmp==list->First){
        tmp->Next->Prev=NULL;
        list->First=tmp->Next;
    }
    else {
        tmp->Next->Prev = tmp->Prev;
        tmp->Prev->Next = tmp->Next;
    }
    list->freefun(tmp->NodeElement);
    list->List_Size--;
    free(tmp);
    return true;
}

//DONE
void printnode(LinkedList* list, Node node ){
    if (node == NULL)
        return;
    if (list == NULL)
        return;

    list->printfun(node->NodeElement);
}

//DONE
void displayList(LinkedList* list){
	if (list == NULL)
		return;

	Node node = list->First;
	while (node != NULL){
		printnode(list, node);
        node=node->Next;
	}
}

//DONE
Element getDataByIndex(LinkedList* list, int index){
    if (list == NULL)
        return NULL;
    if (index > list->List_Size)
        return NULL;

    int i;
    Node node = list->First;
    for (i = 0; i < index; ++i) {
        node = node->Next;
    }
    return node->NodeElement;
}
//DONE
int getLengthList(LinkedList* list){
    if (list==NULL)
        return 0;
    return list->List_Size;
}

Element searchByKeyInList(LinkedList* list, Element elm){
    if (list==NULL || elm==NULL)
        return NULL;

    // elm(key) = jerry_id or phycial_character.
    // node->value = (KVP)
    // isEqualKey sent to list->equalfun and compare (KVP, key)

    Node node = list->First;
    for (int i = 0; i < list->List_Size; ++i) {
        if (list->equalfun(node->NodeElement, elm)==true)
            return node->NodeElement;
        node=node->Next;
    }
    return NULL;
}
