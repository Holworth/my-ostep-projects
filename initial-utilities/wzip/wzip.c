/***********************************************************
 * @file: wzip.c
 * 
 * @summary: This file provide a simple compression tool,
 *           called wzip, which use run-length encoding 
 *           method
 * 
 * @date: December 1st, 2020
 * 
 * @author: Qihan Kang
 * 
 ***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

// a int-char pair which occupies 5 bytes per pair
static const int bytes_per_pair  = 5;
// a constraints on the number of chars per execution of fread
static const int readFileSizePerBatch  = 1024;
static const int writePairPerBatch     = 512;
static const int writeFileSizePerBatch = writePairPerBatch * bytes_per_pair;

/**
 * @function: wzip
 * @parameter: target file name
 * @summary: using RLE method to encode a file contents,
 *           the target file is given by standard output
 * WARNNING: Note that when parsing multiple files in command line,
 *           wzip function must be able to compress them into a 
 *          !!!single file !!!
 */
bool wzip(const char *file_name, bool last_file)
{
    FILE *fp = fopen(file_name, "r");
    if(fp == NULL) {
        // fprintf allows us to choose the file stream
        // we want. here using stdout is nothing different
        // from printf
        fprintf(stdout, "wzip: cannot open file\n");
        return false;
    }

    char read_buf[readFileSizePerBatch];
    char write_buf[writeFileSizePerBatch+1];

    size_t nread = 0;
    static char last_tail = 0;
    static int last_tail_num = 0;

    size_t cur_pair_idx = 0;

    while((nread = fread((void *)read_buf, sizeof(char), readFileSizePerBatch, fp))){
        size_t cur_idx = 0;
        // read the first a few elements that is the same as the last one
        while(cur_idx < nread && read_buf[cur_idx] == last_tail)
        {
            ++last_tail_num;
            ++cur_idx;
        }
        // which means we read the last tail elements 
        // even until the last
        if(cur_idx == nread) { continue; }
        //else we have found the last position which has the value of last tail
        else if(last_tail_num > 0)
        {
            // write the write buffer
            char *nextIntWritePos = write_buf + cur_pair_idx * bytes_per_pair;
            char *nextCharWritePos = nextIntWritePos + sizeof(int);
            memcpy((void *)nextIntWritePos, (void *)(&last_tail_num), sizeof(int));
            *nextCharWritePos = last_tail;
            ++cur_pair_idx;

            // the write buffer is full, write it to the file
            // and reset the pair index
            if(cur_pair_idx == writePairPerBatch)
            {
                fwrite(write_buf, sizeof(char), writeFileSizePerBatch, stdout);
                cur_pair_idx = 0;
            }
        }

        // now let's check current buf
        int write_int = 0;
        while(cur_idx != nread)
        {
            char standard = read_buf[cur_idx];
            write_int = 0;
            // write until we find the next different element 
            // or  we reached the end of the read buf
            while(cur_idx < nread && read_buf[cur_idx] == standard)
            {
                ++write_int;
                ++cur_idx;
            }

            if(cur_idx < nread) 
            {
                // write to the write buffer
                char *nextIntWritePos = write_buf + cur_pair_idx * bytes_per_pair;
                char *nextCharWritePos = nextIntWritePos + sizeof(int);
                memcpy((void *)nextIntWritePos, (void *)(&write_int), sizeof(int));
                *nextCharWritePos = standard;
                ++cur_pair_idx;

                if(cur_pair_idx == writePairPerBatch)
                {
                    fwrite(write_buf, sizeof(char), writeFileSizePerBatch, stdout);
                    cur_pair_idx = 0;
                }
            }
            else 
            {
                last_tail = standard;
                last_tail_num = write_int;
            }
        }
    }
    if(ferror(stdout)) {
        printf("wzip: read file failed\n");
        return false;
    } 
    // else means we have read all contents of file successfully
    else {
        if(last_tail_num > 0 && last_file) {
            char *nextIntWritePos = write_buf + cur_pair_idx * bytes_per_pair;
            char *nextCharWritePos = nextIntWritePos + sizeof(int);
            memcpy((void *)nextIntWritePos, (void *)(&last_tail_num), sizeof(int));
            *nextCharWritePos = last_tail;
            ++cur_pair_idx;
            fwrite(write_buf, sizeof(char), cur_pair_idx * bytes_per_pair, stdout);
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
    if(argc == 1) 
    {
        printf("wzip: file1 [file2 ...]\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 1; i < argc; ++i)
    {
        bool ret = wzip(argv[i], i == (argc-1));
        if(ret == false) exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}



