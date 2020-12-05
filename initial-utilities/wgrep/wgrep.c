/************************************************************
 * @file: wgrep.c                                           
 *                                              
 * @summary: This file contains a simple implementation of
 *           a linux grep conmmand.
 * 
 * @date: December 1st, 2020
 * 
 * @author: Qihan Kang
 ************************************************************/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

static const int pattern_limit   = 512;
static const int stdin_limit     = 1024;
static const int file_name_limit = 64;

// pattern is the term that user expects to find
// in file or stdin

/**
 * @function: wgrep
 * @summary:  open a file and output each line which contains
 *            a specific term
 * @paremeter: const char pointer to the file name and target term
 * @return: return a bool type. 
 *          true: If successfully finish all the things 
 *          false: not successfully, eg: failed to open file; failed to read
 */
bool wgrep(const char *, const char *);

int main(int argc, char *argv[])
{
    // which means there is no pattern to search
    if(argc == 1){
        printf("wgrep: searchterm [file ...]\n");
        exit(EXIT_FAILURE);
    }
    else {
        // which means we have a pattern but the target for searching 
        // is in std input
        char pattern[pattern_limit];
        char stdin_[stdin_limit];
        strcpy(pattern, argv[1]);
        if(argc == 2) {
            // fgets is a function that receive a "line" of characters 
            // from standard input.
            // note that fgets can not only read from some specific file, but
            // also standard input
            // a typical way to use standard input is: cat XXX | ./wgrep
            char *fgets_ret = NULL;
            while((fgets_ret = fgets(stdin_, stdin_limit, stdin))){
                char *s = strstr(stdin_, pattern);
                if(s != NULL) {
                    printf("%s", stdin_);
                }
            }
            if(ferror(stdin)) {
                printf("Failed to read from stdin\n");
                exit(EXIT_FAILURE);
            }
        }
        // which means we need to search each file one by one
        else {
            char file_name[file_name_limit];
            for(int i = 2; i < argc; ++i)   {
                strcpy(file_name, argv[i]);
                bool ret = wgrep(file_name, pattern);
                if(!ret) exit(EXIT_FAILURE);
            }
        }
    }
    exit(EXIT_SUCCESS);
}

bool wgrep(const char *file_name, const char *target)
{
    FILE *fp = fopen(file_name, "r");
    if(fp == NULL) {
        printf("wgrep: cannot open file\n");
        return false;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;
    // getline is a function that reads a line from file stream
    // WARNNING: getline receives char ** and int * as parameters
    // to modify the final result of reading
    // Note: getline is not a part of standard libc. Use _GNU_SOURCE
    // macro at the beginning of the file
    while((nread = getline(&line, &len, fp)) != -1) {
        char *s = strstr(line, target);
        if(s != NULL) {
            printf("%s", line);
        }
    }
    if(ferror(fp) != 0) {
        printf("wgrep: read file failed\n");
        return false;
    }

    return true;
}

