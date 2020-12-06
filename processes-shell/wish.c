/*
 * @Author: Qihan Kang
 * @Date: 2020-12-06 13:42:58
 * @LastEditTime: 2020-12-06 16:11:36
 * @LastEditors: Please set LastEditors
 * @Description: Source file for wish
 */

#include "wish.h"

// definition and declaration of global variable

char *cmd, *args[16];
char *paths[64];
size_t w_argc, path_cnt;
enum builtin_cmd bcmd;

const int cmd_len_limit = 64;
const int arg_num_limit = 16;
const int arg_len_limit = 64;
const int path_num_limit = 64;

const char prompt[] = "wish> ";


bool init_shell()
{
    // pre allocate arguments' space
    for(size_t i = 0; i < arg_num_limit; ++i)
    {
        args[i] = NULL;
    }

    // set default path
    path_cnt = 0;
    paths[0] = (char *)malloc(strlen("/bin")+1);
    if(paths[0] == NULL) {
        fprintf(stderr, "[wish]: malloc failed\n");
        return false;
    }
    strcpy(paths[path_cnt++], "/bin");
    return true;
}

bool add_path(const char *new_path)
{
    // check array boundary is neccessary
    if(path_cnt >= path_num_limit) {
        fprintf(stderr, "[wish]: No space for new path\n");
        return false;
    }
    paths[path_cnt] = (char *)malloc(strlen(new_path)+1);
    if(paths[path_cnt] == NULL) {
        fprintf(stderr, "[wish]: malloc failed\n");
        return false;
    }
    strcpy(paths[path_cnt++], new_path);
    return true;
}

bool parse_input(const char *input, bool *Isfinished)
{

    // we set Isfinished to ture as we first only
    // consider one command condition
    *Isfinished = true;
    
    // first let built-in command flags to be none
    bcmd = none;
    w_argc = 0;

    char *s = input, *token = NULL;
    bool valid_cmd = false, valid_args = true;
    // jump all space at the beginning
    while(isspace(*s)) 
        ++s;

    token = strsep(&s, " ");
    if(token != NULL) {
        valid_cmd = true;
        cmd = token;
        args[w_argc++] = cmd;
    }else return false;

    // parse all possible arguments
    while(token) {
        token = strsep(&s, " ");
        // only when the token is valid will we
        // put it in the args array
        if(token) {
            args[w_argc++] = token;
        }
    }
    
    // set all args after this args to zero
    for(size_t idx = w_argc; idx < arg_num_limit; ++idx)
        args[idx] = NULL;

    // now see if these commands are built-in one
    if(strcmp(cmd, "exit") == 0) {
        bcmd = w_exit;
    } else if(strcmp(cmd, "cd") == 0) {
        bcmd = cd;
    } else if(strcmp(cmd, "path") == 0) {
        bcmd = path;
    }

    return (valid_cmd && valid_args);
}

bool execute_cmd()
{
    // executes_cmd should do the following work:
    // 1. check if this a built-in command
    // 2. if it is a build-in command, using built-in implementation
    //    otherwise, search path to find a binary file 

    bool ret_e = false;
    if(bcmd != none) {
        ret_e = execute_buildin_cmd();
    }else {
        //TODO non-builtin command
        ret_e = execute_nonbuiltin_cmd();
    }
    return ret_e;
}

bool execute_buildin_cmd()
{
    assert(bcmd != none);

    if(bcmd == w_exit){
        return true;
    } else if(bcmd == cd) {
        //TODO cd command
    } else if(bcmd == path) {
        //TODO path command
    }
    switch (bcmd)
    {
    case w_exit:
        exit(EXIT_SUCCESS);
    case cd:
        // TODO cd command
    case path:
        // TODO path command
    default:
        return true;
    }
    return true;
}

bool execute_nonbuiltin_cmd()
{
    // first we need to search which path 
    // has the binary file
    char tmp_file_name[128];
    for(size_t i = 0; i < path_cnt; ++i)
    {
        strcpy(tmp_file_name, paths[i]);
        size_t len = strlen(tmp_file_name);
        // if the end of the path has no directory
        // flag "/"
        if(tmp_file_name[len-1] != '/') {
            strcat(tmp_file_name, "/");
        }
        strcat(tmp_file_name, cmd);
        // if this file exists for some specific path
        // and the current user has permission to access it
        // just executes it
        if(access(tmp_file_name, X_OK) != -1) {
            // start executes the binary file
            strcpy(args[0], tmp_file_name);
            execute_binary(args);
            return true;
        }
    }
    return false;
}

void execute_binary(char *arguments[])
{
    int status = 0;
    if(fork() == 0) {
        execv(args[0], args);
        exit(EXIT_SUCCESS);
    }
    else {
        wait(&status);
    }
}

void print_interface()
{
    printf("%s", prompt);
}



