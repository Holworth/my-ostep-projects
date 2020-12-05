/***********************************************************
 * @file: wcat.c
 * 
 * @summary: This file contains a simple implementation of
 * a Linux command: cat
 * 
 * @date: December 1st, 2020
 * 
 * @author: Qihan Kang
 ***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>


static const int file_name_limit = 128; 
static const int output_limit    = 128;

/**
 * @summary:   output file in ASIIC
 * @parameter: a const char * which specifies the file name
 */ 
bool wcat(const char *file_name);

int main(int argc, char *argv[])
{
    if(argc == 1)
        exit(EXIT_SUCCESS);

    char file_name[file_name_limit];
    bool flag = true;

    for(int i = 1; i < argc; ++i)
    {
        strcpy(file_name, argv[i]);
        bool ret = wcat(file_name);
        flag &= ret;
        if(!flag) exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

/**
 * @summary:   output file in ASIIC
 * @parameter: a const char * which specifies the file name
 */ 

bool wcat(const char *file_name)
{
    // fopen open a file and return a non-zero FILE pointer if
    // it successfully open the file, otherwise it returns zero
    FILE *fp = fopen(file_name, "r");
    if(fp == NULL) {
        printf("wcat: cannot open file\n");
        return false;
    }
    char output[output_limit];
    char *ret = NULL;

    // fgets is a function reading string from given file 
    // @return: It return NULL if it reaches the end of the file or an error
    // occured
    // @parameter: the second parameter constraints the max number of character
    // that can be read once. fgets will return if the limit-1 characters
    // have been read or it reaches a newline
    while((ret = fgets(output, output_limit, fp)) != NULL){
        printf("%s", output);
    }
    fclose(fp);
    if(ferror(fp) != 0) {
        printf("read file failed\n");
        return false;
    }
    return true;
}