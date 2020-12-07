/*
 * @Author: Qihan Kang
 * @Date: 2020-12-06 15:15:04
 * @LastEditTime: 2020-12-07 14:41:48
 * @LastEditors: Please set LastEditors
 * @Description: main.c file contains the entrance of program
 */

#include "wish.h"


int main(int argc, char *argv[]) {
    ssize_t nread = 0;
    char *read_pos = NULL;
    size_t cnt_read = 0;

    wish_init();

    FILE *open_fd = NULL;
    bool usr_interface = false;
    
    if(argc == 1) {
        open_fd = stdin;
        usr_interface = true;
        wish_run(open_fd, usr_interface);
    } 
    else {
        for(size_t i = 1; i < argc; ++i) {
            open_fd = fopen(argv[i], "r");
            if(open_fd == NULL) {
                wish_print_error();
                exit(EXIT_FAILURE);
            }
            if(!wish_run(open_fd, usr_interface)){
                fclose(open_fd);
                exit(EXIT_FAILURE);
            }
            fclose(open_fd);
        }
    }
    exit(EXIT_SUCCESS);
}


