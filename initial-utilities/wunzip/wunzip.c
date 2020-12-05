/***********************************************************
 * @file: unwzip.c
 * 
 * @summary: This file provide a simple uncompression tool,
 *           called wunzip, which use run-length encoding 
 *           method
 * 
 * @date: December 1st, 2020
 * 
 * @author: Qihan Kang
 * 
 ***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

static const int bytesPerPair  =   5;
static const int readPairBatch = 512;
static const int readBufLimit  = readPairBatch * bytesPerPair;
static const int writeBufLimit = 4096;


bool wunzip(const char *file_name)
{
    FILE *fp = fopen(file_name, "r");
    if(fp == NULL){
        fprintf(stderr, "wunzip: cannot open file\n");
        return false;
    }

    char read_buf [readBufLimit];
    char write_buf[writeBufLimit];

    size_t nread = 0;
    static size_t write_idx = 0;
    size_t write_left = 0;


    while((nread = fread((void*)read_buf, 1, readBufLimit, fp)))
    {
        int pair_num = nread / 5;
        int pair_int;
        char pair_ch;
        for(int i = 0; i < pair_num; ++i)
        {
            char *pair_start = read_buf + i * bytesPerPair;
            char *pair_char_start = pair_start + sizeof(int);
            memcpy((void *)(&pair_int), (void *)(pair_start), sizeof(int));
            pair_ch = *pair_char_start;

            write_left = pair_int;
            while(write_left) {
                while(write_left && write_idx < writeBufLimit) {
                    write_buf[write_idx++] = pair_ch;
                    --write_left;
                }
                if(write_idx == writeBufLimit)
                {
                    fwrite((void *)write_buf, sizeof(char), write_idx, stdout);
                    write_idx = 0;
                }
            }
        }
    }

    if(ferror(stdout)) {
        fprintf(stderr, "wunzip: failed to read file\n");
        return false;
    } else{
        fwrite((void *)write_buf, sizeof(char), write_idx, stdout);
    }
    return true;
}

int main(int argc, char *argv[])
{
    if(argc == 1) 
    {
        fprintf(stdout, "wunzip: file1 [file2 ...]\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        for(int i = 1; i < argc; ++i)
        {
            bool ret = wunzip(argv[i]);
            if(!ret) exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}



