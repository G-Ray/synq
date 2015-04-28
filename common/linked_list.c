#include "linked_list.h"

List *
init() {
    List *list = malloc(sizeof(*list));

    list->head = NULL;

    return list;
}

void
insert(List *list, char path[NAME_MAX+1])
{
    File *new = malloc(sizeof(*new));

    strncpy(new->path, path, NAME_MAX+1);
    new->next = NULL;

    if(list->head == NULL) {
        //insert to the HEAD position
        list->head = list->curr = new;
        return;
    }

    // insert to the end
    list->curr->next = new;
    list->curr = new;
}

void destroy(List *list) {

    File *tmp;
    while (list->head != NULL)
    {
        tmp = list->head;
        list->head = list->head->next;
        free(tmp);
    }
}

void
printList(List *list)
{
    if (list == NULL)
    {
        exit(EXIT_FAILURE);
    }

    File *current = list->head;

    printf("\n------LIST OF FILES------\n");
    while (current != NULL)
    {
        printf("->%s\n", current->path);
        current = current->next;
    }
    printf("\n-------END OF LIST-------\n\n");
}

int
searchList(List *list, char * path)
{
    if (list == NULL)
    {
        exit(EXIT_FAILURE);
    }

    File *current = list->head;

    while (current != NULL)
    {
        if(strcmp(current->path, path) == 0)
            return 1;
        current = current->next;
    }
    return 0;
}

List *
compareLists(List *l1, List *l2) {
    List *diff = init();

    if (l1 == NULL)
    {
        exit(EXIT_FAILURE);
    }

    File *current = l1->head;

    while (current != NULL)
    {
        if(searchList(l2, current->path) == 0) {
            insert(diff, current->path);
        }

        current = current->next;
    }

    return diff;
}
