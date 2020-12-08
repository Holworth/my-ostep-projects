/*
 * @Author: Qihan Kang 
 * @Date: 2020-12-08 19:07:57
 * @LastEditTime: 2020-12-08 20:00:31
 * @LastEditors: Please set LastEditors
 * @Description: Header file for SVC
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct resolution {
    char *file_name;
    char *resolved_file;
}resolution;


/**
 * @description: svc_init init the whole project file, 
 *  and the svc data structures
 *  svc_init() would be only called once.
 * @param   [in] 
 *  void  : No parameters are needed
 * @return  [out]  
 *  void *: Point to the essential data structures need for next
 *          operations
 */
void *svc_init();


/**
 * @description: svc_clearup is used after all function calls
 *  Ensure to deallocate all dynamic memory
 * @param   [in] 
 *  void *helper: Pointer to essential helpful data structures
 * @return  [out]
 *  void: No return value
 */
void cleanup(void *helper);


/**
 * @description: Compute and return the hash value 
 * @param   [in]
 *  void *helper: Same as above
 *  char *file_path: File to be computed
 * @return  [out]
 *  int: Compute result
 */
int hash_file(void *helper, char *file_path);

/**
 * @description: Create a commit
 * @param   [in]
 *  void *helper: Same as above
 *  char *message: Commit message
 * @return  [out]
 *  char *: The commit id, calculated by a specific algorithm
 *          - NULL: No change to file, or commit message is NULL
 */
char *svc_commit(void *helper, char *message);


/**
 * @description: Get the commit for a specific commit id
 * @param   [in]
 *  void *helper: Same as above
 *  char *commit_id: commit id you want to search
 * @return  [out]
 *  void *: A pointer pointing to an area where the commit is stored
 *          - NULL: commit id does not exist, or commit id is NULL
 */
void *get_commit(void *helper, char *commit_id);


/**
 * @description: Search parent commit for a specific commit
 * @param   [in]
 *  void *helper: Same as above
 *  void *commit: Pointer pointing to a commit;
 *  int *n_prev : Pointer pointing to a number storing the number of 
 *                parent commit
 * @return  [out]
 *  char **: An array, each of the element represents a parent commit id
 *           Note that the value stored in n_prev needs to be modified
 *           - NULL: n_prev is NULL/commit is NULL/commit is the first commit
 *  Note:    If commit is NULL or is the first commit, *n_prev should be 0 and
 *           NULL should be returned
 */
char **get_prev_commits(void *helper, void *commit, int *n_prev);

/**
 * @description: Print commit in specific form
 * @param   [in]
 *  void *helper: Same as above
 *  char *commit_id: Same as above
 * @return  [out]
 *  void: Nothing will be returned
 * Note: If commit_id is NULL or commit_id doesn't exist, print "Invalid commit id"
 */
void print_commit(void *helper, char *commit_id);


/**
 * @description: Make a specific branch the active one
 * @param   [in]
 *  void *helper: Same as above
 *  char *branch_name: Indicates a specific branch 
 * @return  [out]
 *  int: Show status for the changing operation result
 *       - -1: branch_name is NULL or no such branch exists
 *       - -2: There are uncommitted changes, and do not make this the active change
 *       -  0: All work are done
 *  Note: Do not create a branch if it doesn't exist
 */
int svc_checkout(void *helper, char *branch_name);

/**
 * @description: Print all branches in the order they were created
 * @param   [in]
 *  void *helper: Same as above
 *  int *n_branches: Pointer indicates number of branches
 * @return  [out]
 *  char **: Return a DYNAMIC ALLOCATED array in the order where all branches are printed
 *           stores the number of branches through n_branches pointer
 *           - NULL: n_branches is NULL
 *  Note: If n_branches pointer is NULL, nothing will be printed.
 */
char **list_branches(void *helper, int *n_branches);


/**
 * @description: Add a file to the version control
 *  By using "add to version control", we mean it will be tracked
 * @param   [in]
 *  void *helper: Same as above
 *  char *file_name: Same as above
 * @return  [out]
 *  int: Return the status of operation
 *       - -1: When file_name is NULL, do not add it to version control, and
 *             return -1
 *       - -2: If the file with the same name is already being tracked in the current
 *             branch
 *       - -3: If the file does not exist
 *       otherwise return the hash value of the file
 */
int svc_add(void *helper, char *file_name);


/**
 * @description: Remove a file from version control system(not means delete it from file system)
 * @param   [in]
 *  void *helper: Same as above
 *  char *file_name: Same as above
 * @return  [out]
 *  int: Status of the operation
 *       - -1: file_name is NULL
 *       - -2: The file is not tracked in version system
 *       otherwise return the hash value of the file(last version)
 */
int svc_rm(void *helper, char *file_name);


/**
 * @description: Reset the current branch to the commit with the id given
 * @param   [in]
 *  void *helper: Same as above
 *  char *commit_id: Same as above
 * @return  [out]
 *  int: Status of the operation
 *       - -1: commit_id is NULL
 *       - -2: No commit with the given commit id
 *       -  0: Operation is done successfully
 * Note: Some commits will be dropped after calling this function
 */
int svc_reset(void *helper, char *commit_id);

/**
 * @description: Merge the branch with branch name into current branch
 * @param   [in]
 *  void *helper: Same as above
 *  void *branch_name: Same as above
 *  resolution *reoslutions: 
 *  int n_resolutions:
 * @return  [out]
 */
char *svc_merge(void *helper, char *branch_name, resolution *
    resolutions, int n_resolutions);




