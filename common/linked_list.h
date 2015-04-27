#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include <stdio.h>

typedef struct File File;
struct File {
    char *path;
    File *next;
};

typedef struct List List;
struct List
{
     File *head;
     File *curr;
};

List *
init();

void
insert(List *list, char *path);

void
destroy(List *list);

void
printList(List *list);

#endif
