/*
 * @Author: Qihan Kang
 * @Date: 2020-12-06 13:23:14
 * @LastEditTime: 2020-12-06 16:03:04
 * @LastEditors: Please set LastEditors
 * @Description: The main file of project wish(Wisconshing Shell)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>

// Essential for using getline()
#define _GNU_SOURCE

// enumeration for built-in command
enum builtin_cmd 
{
    none, w_exit, cd, path
};


// usr interface
extern const char prompt[];

// some limits on char array len; 
extern const int cmd_len_limit;
extern const int arg_num_limit;
extern const int arg_len_limit;
extern const int path_num_limit;

/* char cmd[] is the char array used for storing parsed command
 * 
 * char *arg[] used for storing args
 * 
 * char *path[] used for storing path
 * 
 * w_argc is the number of arguments
 * we use w_argc instead of argc, which may contradicts argc
 * main function
 * 
 * size_t path_cnt records how many path are recorded here
 */
extern char *cmd, *args[16];
extern char *paths[64];
extern size_t w_argc, path_cnt;

// bcmd will show if this command is a built-in one
extern enum builtin_cmd bcmd;

/**
 * @description: This function do neccessary init for shell
 * @param { void }
 * @return { bool } If initialization succeed, returns true
 * otherwise returns false
 * @summay: what init_shell do includes: set the basic path
 * to /bin.
 */
bool init_shell();

/**
 * @description: add one path to the path structure
 * @param {const char *}, which shows the new path to insert
 * @return {bool} returns true if add successful, otherwise returns
 * false
 */
bool add_path(const char *new_path);

/**
 * @description: This function parse the wish input and set the 
 * cmd & args arrays
 * @param {const char *} the position of the input
 *        {bool *} shows whether parse has been finished
 * @return { bool } if the parse succeeds, return true, otherwise, return false
 * Note: parse succeeds means the cmd array and args array are usable. Sometimes there
 * might be some invalid arguments, in such case, we return false to warn the shell do not
 * executes the command
 */
bool parse_input(const char *, bool *);


/**
 * @description: This function executes the command
 * @param { void } Neccessary arguments are cmd array & args array, which are global;
 * @return { bool } If it executes successfully, return true, otherwise return false
 */
bool execute_cmd();


/**
 * @description: This function executes build-in command
 * @param { void } The bcmd variable is a global one
 * @return { bool } Same as above function
 */
bool execute_buildin_cmd();

bool execute_nonbuiltin_cmd();

void execute_binary(char *[]);

void print_interface();









