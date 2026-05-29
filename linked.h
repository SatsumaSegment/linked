#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node {
    void *data;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct {
    Node *head;
    Node *tail;
    int size;
    void (*destroy)(void *data);
} List;

List* initList(void (*destroyFunction)(void *data));
Node* createNode(void *data);
void prepend(List *list, void *newData);
void append(List *list, void *newData);
void insertAt(List *list, int index, void *newData);
void deleteHead(List *list);
void deleteTail(List *list);
void deleteAt(List *list, int index);
Node* getHead(List *list);
Node* getTail(List *list);
Node* find(List *list, void *data, int (*compareFunction)(const void*, const void*));
int getSize(List *list);
void reverseList(List *list);
void concatenateList(List *list, List *addList);
void printList(const List *list, void (*printFunction)(const void*));
void printListBackward(const List *list, void (*printFunction)(const void*));
void freeList(List *list);

#endif