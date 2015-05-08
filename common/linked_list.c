#include "linked_list.h"

List *
init() {
    List *list = malloc(sizeof(*list));

    list->head = NULL;

    return list;
}

void
insert(List *list, char path[NAME_MAX+1], time_t mtime)
{
    File *new = malloc(sizeof(*new));

    strncpy(new->path, path, NAME_MAX+1);
    new->mtime = mtime;
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
        char buff[20];
        struct tm * timeinfo;
        timeinfo = localtime(&current->mtime);
        strftime(buff, sizeof(buff), "%b %d %H:%M", timeinfo);

        printf("->%s ------ %s\n", current->path, buff);
        current = current->next;
    }
    printf("\n-------END OF LIST-------\n\n");
}

File *
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
            return current;

        current = current->next;
    }
    return NULL;
}

/* Generate a diff  between l1 and l2 based on modification time of files */
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
        File *f = searchList(l2, current->path);
        if(f == NULL) {
            insert(diff, current->path, current->mtime);
        }
        else if(f->mtime < current->mtime) {
            insert(diff, current->path, current->mtime);
        }

        current = current->next;
    }

    return diff;
}
