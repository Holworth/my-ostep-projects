/*
 * @Author: Qihan Kang
 * @Date: 2020-12-06 13:42:58
 * @LastEditTime: 2020-12-07 11:42:58
 * @LastEditors: Please set LastEditors
 * @Description: Source file for wish
 */

#include "wish.h"

// definition and declaration of global variable

char *paths[64];
size_t path_cnt;

const size_t cmd_len_limit = 64;
const size_t cmd_num_limit = 64;
const size_t arg_num_limit = 16;
const size_t arg_len_limit = 64;
const size_t path_num_limit = 64;

const char prompt[] = "wish> ";
const char error_msg[] = "An error has occurred\n";

const char arguments_sep[] = " ";
const char cmdline_sep[] = "&";

cmd_parm_t *parms[64];



// Note: the element of path array must be copied
// instead of simple assigning operation of char pointer
// The space pointer pointing to might be freed
bool wish_init()
{

    for(size_t i = 0; i < cmd_num_limit; ++i) {
        parms[i] = (cmd_parm_t *)malloc(sizeof(cmd_parm_t));
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

void wish_parse_single_cmd(const char *cmd, cmd_parm_t *target)
{
    // first set some attributes;
    target->bcmd = none;
    target->w_argc = 0;

    char *s = cmd, *token = NULL;
    bool valid_cmd = false, valid_args = true;

    // skip all space at the begining
    while(isspace(*s)) 
        ++s;
    
    // first try parse the command 
    token = strsep(&s, arguments_sep);
    if(token != NULL) {
        valid_cmd = true;
        target->args[(target->w_argc)++] = token;
    }
    
    // parse the rest arguments
    while(token) {
        token = strsep(&s, arguments_sep);
        // only when the token is valid
        // do we put it into the target args array
        if(token)  {
            target->args[(target->w_argc)++] = token;
        }
    }
    if(valid_cmd) 
    {
        if(strcmp(target->args[0], "exit") == 0) {
            target->bcmd = w_exit;
        } else if(strcmp(target->args[0], "cd") == 0) {
            target->bcmd = cd;
        } else if(strcmp(target->args[0], "path") == 0) {
            target->bcmd = path;
        }
    }

    target->valid = valid_cmd & valid_args;
}

bool wish_execute_cmd(cmd_parm_t *exec_parm)
{
    // executes_cmd should do the following work:
    // 1. check if this a built-in command
    // 2. if it is a build-in command, using built-in implementation
    //    otherwise, search path to find a binary file 

    if(exec_parm->valid == false) 
        return false;

    bool ret_e = false;
    if(exec_parm->bcmd != none) {
        ret_e = wish_execute_buildin_cmd(exec_parm);
    }else {
        //TODO non-builtin command
        ret_e = wish_execute_nonbuiltin_cmd(exec_parm);
    }
    return ret_e;
}

bool wish_execute_buildin_cmd(cmd_parm_t *exec_parm)
{
    assert(exec_parm->bcmd != none);

    switch (exec_parm->bcmd)
    {
    case w_exit:
        // TODO exit command
        wish_do_exit();
    case cd:
        // TODO cd command
        return wish_do_cd(exec_parm);
    case path:
        // TODO path command
        return wish_do_path(exec_parm);
    default:
        return false;
    }
    return true;
}

bool wish_execute_nonbuiltin_cmd(cmd_parm_t *exec_parm)
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
        // cat them together
        strcat(tmp_file_name, exec_parm->args[0]);
        // if this file exists for some specific path
        // and the current user has permission to access it
        // just executes it
        if(access(tmp_file_name, X_OK) != -1) {

            // start executes the binary file
            // create another tmp args array for executing
            char *tmp_args[exec_parm->w_argc+1];

            // the first parameter must be file name
            tmp_args[0] = tmp_file_name;
            for(size_t i = 1; i < exec_parm->w_argc; ++i) 
                tmp_args[i] = (exec_parm->args)[i];
            // WARNING: The first non-parm must be NULL, otherwise
            // execv can not recognize it
            tmp_args[exec_parm->w_argc] = NULL;

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
        // note: execv does not return if they succeed
        execv(arguments[0], arguments);
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

bool wish_do_cd(cmd_parm_t *exec_parm) {
    // which means there are more than two 
    // target directory, which is invalid
    if(exec_parm->w_argc > 2) {
        wish_print_error();
        return false;
    }

    int status = 0;
    // chidir syscall will return -1 if error occurs
    if((status = chdir((exec_parm->args)[1])) == -1){
        wish_print_error();
        return false;
    }
    return true;
}

// there is some 
bool wish_do_path(cmd_parm_t *exec_parm) {
    // if there is only one parameter
    // which means no directory parameter for path
    if(exec_parm->w_argc == 1) return false;
    // we might add multiple path
    for(size_t i = 1; i < exec_parm->w_argc; ++i){
        if(!wish_add_path((exec_parm->args)[i])) 
            return false;
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
        size_t pcmd_num = 0;
        char *s = read_pos, *token = NULL;

        token = strsep(&s, cmdline_sep);
        while(token) {
            wish_parse_single_cmd(token, parms[pcmd_num++]);
            token = strsep(&s, cmdline_sep);
        }
        // one error might occured as there might be
        // multiple process running at the same time
        for(size_t i = 0; i < pcmd_num; ++i){
            if(!(parms[i]->valid)) 
                continue;
            ret = wish_execute_cmd(parms[i]);
            flags &= ret;
        }
    }

    return flags;
}


