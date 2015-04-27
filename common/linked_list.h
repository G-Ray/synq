#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

typedef struct File File;
struct File {
    char path[NAME_MAX+1];
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

int
searchList(List *list, char * path);

List *
compareLists(List *l1, List *l2);

#endif
