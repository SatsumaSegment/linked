#include <stdio.h>
#include <stdlib.h>
#include "linked.h"

List* initList(void (*destroyFunction)(void *data)) {
    List *list = malloc(sizeof(List));
    list->head = NULL;
    list->tail = NULL;
    list->destroy = destroyFunction;
    return list;
}

Node* createNode(void *data) {
    Node *newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Error: Out of memory!\n");
        exit(1);
    }

    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

void prepend(List *list, void *newData) {
    Node *newNode = createNode(newData);

    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
        list->size++;
        return;
    }

    newNode->next = list->head;
    list->head->prev = newNode;
    list->head = newNode;
    list->size++;
}


void append(List *list, void *newData) {
    Node *newNode = createNode(newData);

    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
        list->size++;
        return;
    }

    newNode->prev = list->tail;
    list->tail->next = newNode;
    list->tail = newNode;
    list->size++;
}


void insertAt(List *list, int index, void *newData) {
    if (index == 0) {
        prepend(list, newData);
        return;
    }

    int i = 0;
    Node *current = list->head;
    Node *prevNode = NULL;

    while (i != index) {
        if (current == NULL) {
            fprintf(stderr, "Error: Index out of range\n");
            exit(1);
        }
        prevNode = current;
        current = current->next;
        i++;
    }

    if (current == NULL) {
        append(list, newData);
        return;
    }

    Node *newNode = createNode(newData);

    newNode->next = current;
    newNode->prev = current->prev;
    current->prev = newNode;
    prevNode->next = newNode;
    list->size++;
}


void deleteHead(List *list) {
    if (list->head == NULL) {
        return;
    }
    Node *head = list->head;
    if (list->destroy != NULL) {
        list->destroy(head->data);
    }
    list->head = head->next;

    if (list->head == NULL) {
        list->tail = NULL;
    } else {
        list->head->prev = NULL;
    }

    free(head);
    list->size--;
}


void deleteTail(List *list) {
    if (list->tail == NULL) {
        return;
    }
    Node *tail = list->tail;
    if (list->destroy != NULL) {
        list->destroy(tail->data);
    }
    list->tail = tail->prev;

    if (list->tail == NULL) {
        list->head = NULL;
    } else {
        list->tail->next = NULL;
    }
    
    free(tail);
    list->size--;
}


void deleteAt(List *list, int index) {
    if (index < 0 || index > list->size - 1 || list->head == NULL) {
        fprintf(stderr, "Error: Index out of range.\n");
        return;
    }

    if (index == 0) {
        deleteHead(list);
        return;
    }

    if (index == list->size - 1) {
        deleteTail(list);
        return;
    }

    if (index > list->size / 2) {
        Node *current = list->tail;
        int i = list->size - 1;
        while (i != index) {
            current = current->prev;
            i--;
        }
        current->prev->next = current->next;
        current->next->prev = current->prev;
        if (list->destroy != NULL) {
            list->destroy(current->data);
        }
        free(current);
        list->size--;
        return;
    }
    int i = 0;
    Node *current = list->head;
    while (i != index) {
        current = current->next;
        i++;
    }
    current->prev->next = current->next;
    current->next->prev = current->prev;
    if (list->destroy != NULL) {
        list->destroy(current->data);
    }
    free(current);
    list->size--;
}


Node* getHead(List *list) {
    return list->head;
}


Node* getTail(List *list) {
    return list->tail;
}


Node* find(List *list, void *data, int (*compareFunction)(const void*, const void*)) {
    Node *current = list->head;
    while (current != NULL) {
        if (compareFunction(data, current->data) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}


int getSize(List *list) {
    return list->size;
}


void reverseList(List *list) {
    if (list == NULL || list->head == NULL || list->head->next == NULL) {
            return;
    }
    
    Node *current = list->head;
    Node *temp = NULL;

    while(current != NULL) {
        temp = current->prev;
        current->prev = current->next;
        current->next = temp;

        current = current->prev;
    }

    list->tail = list->head;
    if (temp != NULL) {
        list->head = temp->prev;
    }
}


void concatenateList(List *list, List *addList) {
    if (addList == NULL || addList->head == NULL) {
        return;
    }

    if (list->head == NULL) {
        list->head = addList->head;
        list->tail = addList->tail;
        list->size = addList->size;
        return;
    }

    list->tail->next = addList->head;
    addList->head->prev = list->tail;
    list->tail = addList->tail;

    list->size += addList->size;

    addList->head = NULL;
    addList->tail = NULL;
    addList->size = 0;
}


void printList(const List *list, void (*printFunction)(const void*)) {
    if (list == NULL || list->head == NULL) {
        printf("NULL\n");
        return;
    }
    Node *current = list->head;

    while (current != NULL) {
        printFunction(current->data);
        current = current->next;
    }
}


void printListBackward(const List *list, void (*printFunction)(const void*)) {
    if (list == NULL || list->head == NULL) {
        printf("NULL\n");
        return;
    }
    Node *current = list->tail;

    while (current != NULL) {
        printFunction(current->data);
        current = current->prev;
    }
}


void freeList(List *list) {
    Node *current = list->head;

    while (current != NULL) {
        Node *nextNode = current->next;
        
        if (list->destroy != NULL && current->data != NULL) {
            list->destroy(current->data); 
        }

        free(current);
        current = nextNode;
    }
    
    free(list);
}