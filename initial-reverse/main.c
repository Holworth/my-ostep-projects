/**
 * @file: main.c
 * @summary: This file contains the
 *           main work logic 
 * @date: December 5th, 2020
 * @author: Qihan Kang
 */

#include "list.h"
#include <sys/stat.h>
#include <string.h>

list_t *list;

/**
 * This function detects whether the two file is the same
 */
int same_file(int fd1, int fd2);

int main(int argc, char *argv[])
{
    // as specified in the tutor material, output 
    // an error message if there are too many arguments
    // USE fprintf and stderr INSTEAD OF printf and stdout
    if(argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(EXIT_FAILURE);
    }

    // when the input and output file are the same
    // just exit(1)

    list = (list_t *)malloc(sizeof(list_t));
    init_list(list);

    FILE *open_fd = NULL, *write_fd = NULL;

    // there is no input & output file parameter
    // just use the stdin and stdout
    // set the input src and output des first
    if(argc == 1){
        open_fd = stdin, write_fd = stdout;
    } else if(argc >= 2) {
        open_fd = fopen(argv[1], "r");
        if(open_fd == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(EXIT_FAILURE);
        }
        // 
        if(argc == 2)
            write_fd = stdout;
        // else argc = 3
        // write must be written to file
        else {
            // first check if the input and output file is the same
            // if the file we want to open can't open, means it's the same as input file
            FILE *tmp_open = fopen(argv[2], "r");
            /* 
            if(tmp_open == NULL) {
            // the file dosen't exist, we don't care about that if it doesn't exist

                fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
                exit(EXIT_FAILURE);
            }
            */
            if(open_fd && tmp_open) {
                int fd1 = fileno(open_fd), fd2 = fileno(tmp_open);
                if(same_file(fd1, fd2) == 1) {
                    fprintf(stderr, "reverse: input and output file must differ\n");
                    exit(EXIT_FAILURE);
                }
                fclose(tmp_open);
            }
            write_fd = fopen(argv[2], "w");
            if(write_fd == NULL) {
                fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
                exit(EXIT_FAILURE);
            }
        }
    }

    // get all possible input
    char *tmp_start = NULL;
    size_t tmp_len = 0;
    ssize_t ret_len = 0;

    while((ret_len = getline(&tmp_start, &tmp_len, open_fd)) != -1)
    {
        string_line_t *new_node = (string_line_t *)malloc(sizeof(string_line_t));
        if(new_node == NULL) {
            fprintf(stderr, "failed to malloc\n");
            exit(EXIT_FAILURE);
        }
        new_node->len = ret_len;
        new_node->start = (char *)malloc(ret_len + 2);
        strcpy(new_node->start, tmp_start);

        insert_at_begin(list, new_node);
    }

    if(ferror(open_fd) != 0) {
        fprintf(stderr, "file read failed\n");
        exit(EXIT_FAILURE);
    }
    // remember to close input file
    // if there is any
    if(argc >= 2) {
        fclose(open_fd);
    }

    // output these string in reverse order
    // the question is: do we have "\n" at the string ?
    string_line_t *first = list->header->next;
    for(; first != NULL; first=first->next)
    {
        fwrite((void *)(first->start), first->len, 1, write_fd);
    }
    // remember to close output file
    if(argc == 3) {
        fclose(write_fd);
    }

    exit(EXIT_SUCCESS);
}

int same_file(int fd1, int fd2)
{
    struct stat stat1, stat2;
    if(fstat(fd1, &stat1) < 0) return -1;
    if(fstat(fd2, &stat2) < 0) return -1;
    return (stat1.st_dev == stat2.st_dev) && (stat1.st_ino == stat2.st_ino);
}

