#include "linked_list.h"
#include "utils.h"

List *
init() {
    List *list = malloc(sizeof(*list));

    list->head = NULL;

    return list;
}

int listSize(List *list)
{
    int size = 0;

    File *current = list->head;
    while(current != NULL) {
        size += sizeof(current->path);
        size += sizeof((current->mtime));
        current = current->next;
    }
    return size;
}

void
serializeList(List *list, char *buffer) {
    int cur;

    File *current = list->head;
    while(current != NULL) {
        memcpy(&buffer[cur], &current->path, sizeof(current->path));
        cur += sizeof(current->path); /* move seeker ahead by a byte */
        memcpy(&buffer[cur], &current->mtime, sizeof(time_t));
        cur += sizeof(time_t);
        current = current->next;
    }
}

List *
deserializeList(char file[PATH_MAX]) {
    int size;
    int done = 0;
    char path[PATH_MAX];
    time_t timet;
    struct stat s;
    FILE *fd_rd;

    if(stat(file, &s) < 0)
        return NULL;

    size = fileSize(file);
    fd_rd = fopen(file, "rb");

    List *list = init();
    while(done < size) {
        fread(path, PATH_MAX, 1, fd_rd);
        fread(&timet, sizeof(time_t), 1, fd_rd);
        done += PATH_MAX;
        done += sizeof(time_t);
        insert(list, path, timet);
    }
    fclose(fd_rd);

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

    //printf("\n------LIST OF FILES------\n");
    while (current != NULL)
    {
        char buff[20];
        struct tm * timeinfo;
        timeinfo = localtime(&current->mtime);
        strftime(buff, sizeof(buff), "%b %d %H:%M", timeinfo);

        printf("%-40s %s\n", current->path, buff);
        current = current->next;
    }
    printf("-------------------- END OF LIST --------------------\n\n");
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

int
countEntries(List *list)
{
    int entries =0;

    if (list == NULL)
    {
        exit(EXIT_FAILURE);
    }

    File *current = list->head;

    while (current != NULL)
    {
        entries++;
        current = current->next;
    }

    return entries;
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
