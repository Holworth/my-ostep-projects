/*
 * @Author: Qihan Kang
 * @Date: 2020-12-06 15:15:04
 * @LastEditTime: 2020-12-06 15:54:11
 * @LastEditors: Please set LastEditors
 * @Description: main.c file contains the entrance of program
 */

#include "wish.h"


int main(int argc, char *argv[])
{
    ssize_t nread = 0;
    char *read_pos = NULL;
    size_t cnt_read = 0;

    init_shell();

    while(true) 
    {
        if(argc == 1) {
            print_interface();
        }
        if((nread = getline(&read_pos, &cnt_read, stdin)) == -1)
        {
            break;
        }
        // erase the last '\n'
        size_t len = strlen(read_pos);
        read_pos[len-1] = '\0';

        bool finished = false;
        parse_input(read_pos, &finished);
        execute_cmd();
    }
}
