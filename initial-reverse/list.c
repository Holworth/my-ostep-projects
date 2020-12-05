#include "list.h"

void init_list(list_t *list)
{
    // set list node num to 0
    list->node_num = 0;
    list->header = (string_line_t *)malloc(sizeof(string_line_t));
    if(list->header == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }
    // we only set the next pointer of header
    // as we didn't care about the other two fields
    list->header->next = NULL;
}

bool list_empty(const list_t *list)
{
    // another way to specify whether a list
    // is empty is to check the list header's next pointer
    // however, it's troublesome here
    return list->node_num == 0;
}

void insert_at_begin(list_t *list, string_line_t *node)
{
    string_line_t *tmp = list->header->next;
    list->header->next = node;
    node->next = tmp;
    // don't forget the increament
    list->node_num += 1;
}

void traverse_list(const list_t *list, void(*func)(string_line_t *))
{
    string_line_t *first = list->header->next;
    for(; first != NULL; first=first->next)
    {
        func(first);
    }
}