/*
 * @Author: Qihan Kang
 * @Date: 2020-12-06 13:42:58
 * @LastEditTime: 2020-12-06 18:32:44
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
const char error_msg[] = "An error has occurred\n";


bool wish_init()
{
    // pre allocate arguments' space
    for(size_t i = 0; i < arg_num_limit; ++i) {
        args[i] = (char *)malloc(arg_len_limit);
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

bool wish_add_path(const char *new_path)
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
    // add path must use strcpy, instead of simply assigning
    // the char pointer to paths's element
    strcpy(paths[path_cnt++], new_path);
    return true;
}

bool wish_parse_input(const char *input, bool *Isfinished)
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
            //args[w_argc++] = token;
            strcpy(args[w_argc++], token);
        }
    }
    
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

bool wish_execute_cmd()
{
    // executes_cmd should do the following work:
    // 1. check if this a built-in command
    // 2. if it is a build-in command, using built-in implementation
    //    otherwise, search path to find a binary file 

    bool ret_e = false;
    if(bcmd != none) {
        ret_e = wish_execute_buildin_cmd();
    }else {
        //TODO non-builtin command
        ret_e = wish_execute_nonbuiltin_cmd();
    }
    return ret_e;
}

bool wish_execute_buildin_cmd()
{
    assert(bcmd != none);

    switch (bcmd)
    {
    case w_exit:
        // TODO exit command
        wish_do_exit();
    case cd:
        // TODO cd command
        return wish_do_cd();
    case path:
        // TODO path command
        return wish_do_path();
    default:
        return false;
    }
    return true;
}

bool wish_execute_nonbuiltin_cmd()
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
            // create another tmp args array for executing
            char *tmp_args[w_argc+1];
            for(size_t i = 0; i < w_argc; ++i) 
                tmp_args[i] = args[i];
            tmp_args[w_argc] = NULL;
            wish_execute_binary(tmp_args);
            return true;
        }
    }
    return false;
}

void wish_execute_binary(char *arguments[])
{
    int status = 0;
    if(fork() == 0) {
        execv(arguments[0], arguments);
        exit(EXIT_SUCCESS);
    }
    else {
        wait(&status);
    }
}
void wish_print_interface(){
    fprintf(stdout, "%s", prompt);
}

void wish_print_error(){
    fprintf(stderr, "%s", error_msg);
}

void wish_do_exit() {
    exit(EXIT_SUCCESS);
}

bool wish_do_cd() {
    // which means there are more than two 
    // target directory, which is invalid
    if(w_argc > 2) return false;
    int status = 0;
    // chidir syscall will return -1 if error occurs
    if((status = chdir(args[1])) == -1){
        wish_print_error();
        return false;
    }
    return true;
}

// there is some 
bool wish_do_path() {
    // if there is only one parameter
    // which means no directory parameter for path
    if(w_argc == 1) return false;
    // we might add multiple path
    for(size_t i = 1; i < w_argc; ++i){
        if(!wish_add_path(args[i])) return false;
    }
    return true;
}

bool wish_run(FILE *open_fd, bool usr_interface)
{
    bool flags = true;

    while(true)
    {
        if(usr_interface) {
            wish_print_interface();
        }

        ssize_t nread = 0;
        char *read_pos = NULL;
        size_t cnt_read = 0;

        if((nread = getline(&read_pos, &cnt_read, open_fd)) == -1)
            break;
        
        size_t len = strlen(read_pos);

        // set the last character to zero
        if(read_pos[len-1] == '\n')
            read_pos[len-1] = '\0';
        
        bool finished = false, ret = true;

        if(wish_parse_input(read_pos, &finished)) {
            ret = wish_execute_cmd();
            flags &= ret;
        }
    }
    return flags;
}


