/*
 * @Author: Qihan Kang
 * @Date: 2020-12-06 13:23:14
 * @LastEditTime: 2020-12-07 14:32:05
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
#include <stdint.h>

// Essential for using getline()
#define _GNU_SOURCE

// enumeration for built-in command
enum builtin_cmd 
{
    none, w_exit, cd, path
};


// usr interface
extern const char prompt[];

// error message
extern const char error_msg[]; 

// some limits on char array len; 
extern const size_t cmd_len_limit;
extern const size_t cmd_num_limit;
extern const size_t arg_num_limit;
extern const size_t arg_len_limit;
extern const size_t path_num_limit;


extern char *paths[64];
extern size_t path_cnt;


/* a struct for single command line */
typedef struct cmd_parm
{
    char *args[16];
    enum builtin_cmd bcmd;
    size_t w_argc;
    bool valid;
}cmd_parm_t;

extern cmd_parm_t *parms[64];

/**
 * @description: This function do neccessary init for shell
 * @param { void }
 * @return { bool } If initialization succeed, returns true
 * otherwise returns false
 * @summay: what init_shell do includes: set the basic path
 * to /bin.
 */
bool wish_init();

/**
 * @description: add one path to the path structure
 * @param {const char *}, which shows the new path to insert
 * @return {bool} returns true if add successful, otherwise returns
 * false
 */
bool wish_add_path(const char *new_path);

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
bool wish_parse_input(const char *, bool *);

/**
 * @description: This function do a simple command line parse
 * @param { const char * }: where the start address of the input
 *        { cmd_parm_t *}: where the parse result should be put;
 * @return { void }
 * Note: This function will also check whether the arguments of this command is valid
 * The validation of the command contains:
 *  1. Arguments number      2.Arguments type     3. Redirection
 */
void wish_parse_single_cmd(const char *, cmd_parm_t *);
/**
 * @description: This function executes the command
 * @param { void } Neccessary arguments are cmd array & args array, which are global;
 * @return { bool } If it executes successfully, return true, otherwise return false
 */
bool wish_execute_cmd(cmd_parm_t *);


/**
 * @description: This function executes build-in command
 * @param { void } The bcmd variable is a global one
 * @return { bool } Same as above function
 */
bool wish_execute_buildin_cmd(cmd_parm_t *);

/**
 * @description: This function search path directory and find the executable file
 * @param { void }
 * @return { bool }
 */
bool wish_execute_nonbuiltin_cmd(cmd_parm_t *);

/**
 * @description: This function execute binary file with args
 * @param { char *[] } which is the array of parameter, also includes the command name
 * @return { void }
 */
void wish_execute_binary(const char*, char *[]);

/**
 * Help output the usr interface
 * and output error message
 */
void wish_print_interface();
void wish_print_error();

// following are some simple function
// for built-in execute
void wish_do_exit();

bool wish_do_cd(cmd_parm_t *);

bool wish_do_path(cmd_parm_t *);

/**
 * @description: This function runs shell from a given input file stream
 * @param { FILE *} where we can get the input
 * @return { bool }  If there is any error in this run turn
 */
bool wish_run(FILE *, bool);









