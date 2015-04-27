#include "linked_list.h"

List *
init() {
    List *list = malloc(sizeof(*list));
    File *file = malloc(sizeof(*file));

    file->path = NULL;
    file->next = NULL;

    list->head = list->curr = file;

    return list;
}

void
insert(List *list, char *path)
{
    File *new = malloc(sizeof(*new));

    new->path = path;
    new->next = NULL;

    if(list->head->path == NULL) {
        //insert to the HEAD position
        list->head->path = path;
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
        printf("%s -> ", current->path);
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

void
compareLists(List *list) {
    if (list == NULL)
    {
        exit(EXIT_FAILURE);
    }

    File *current = list->head;

    printf("\n------LIST OF FILES------\n");
    while (current != NULL)
    {
        printf("%s -> ", current->path);
        current = current->next;
    }
    printf("\n-------END OF LIST-------\n\n");
}
