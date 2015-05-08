#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <time.h>

typedef struct File File;
struct File {
    char path[NAME_MAX + 1];
    time_t mtime;
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
insert(List *list, char *path, time_t mtime);

int
countEntries(List *list);

void
destroy(List *list);

void
printList(List *list);

File *
searchList(List *list, char * path);

List *
compareLists(List *l1, List *l2);

#endif
