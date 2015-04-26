#include "linked_list.h"

List *
init(char *path) {
    List *list = malloc(sizeof(*list));
    File *file = malloc(sizeof(*file));

    file->path = path;
    file->next = NULL;

    list->head = list->curr = file;

    return list;
}

void
insert(List *list, char *path)
{
    File *new = malloc(sizeof(*new));
    if(list == NULL)
        list = init(path);

    new->path = path;
    new->next = NULL;

    // insert to the end
    list->curr->next = new;
    list->curr = new;
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
