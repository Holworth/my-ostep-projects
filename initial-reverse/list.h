/**
 * @file: reverse.c
 * @summary: reverse string line in a single input file
 * @date: December 5th, 2020
 * @author: Qihan Kang
 */

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


// This struct forms a link list to 
// record the string line we read from a file
// no need to use a bidirection pointer in this reverse case
typedef struct string_line{
    char *start;
    size_t len;
    struct string_line *next;
} string_line_t;

// This struct forms a list
typedef struct linked_list{
    string_line_t *header;
    size_t node_num;
}list_t;

/**
 * This function do some essential initilization
 * for a new list 
 */
void init_list(list_t *);

/**
 * This function returns if a list is empty
 */
bool list_empty(const list_t *);

/**
 * This function do a simple insert at the beginning
 * of the list.
 * Note that inserting at the beginning is already enough for
 * this reverse case
 */
void insert_at_begin(list_t *, string_line_t *);

/**
 * This function do a traverse thing for a specific list;
 * which means it does something for every node in this list;
 * The parameter is one list and one do-function
 */
void traverse_list(const list_t *, void(*)(string_line_t *));








