/*
This file has the basic commands that will be used to parse commands and run them
*/

#ifndef __COMMAND_HELPER_H__
#define __COMMAND_HELPER_H_

//A few assumptions regarding size. I will hardcode these.
#define MAX_COMMAND_LENGTH 10
#define MAX_NO_OF_COMMANDS 20
#define MAX_COMMAND_LINE_LENGTH (MAX_COMMAND_LENGTH * MAX_NO_OF_COMMANDS)

enum special_command
{
    e_INVALID = 0,
    e_EXIT_COMMAND,
    e_PREVIOUS_COMMAND,
    e_MAX
};

/*
This function will take a line of text consiting of multiple words and separate them and store in 
the array command_list. It will also check if & is passed at the end of line and set the variable
b_background_execution if yes.
*/
int separate_arguments(char* command_line, 
            char command_list[][MAX_COMMAND_LENGTH], 
            int *b_background_execution);


/*
This function will check if the passed command is a special command that has to be handled
by the shell
*/
enum special_command check_special_command(char commands[][MAX_COMMAND_LENGTH]);

/*
This function will take a list of arguments. The first character after last argument will be 0
This fucntion will search for all the pipes in the passed inout and store these indices in pipe_indices,
array
*/
int find_pipes(char command_list[][MAX_COMMAND_LENGTH], 
            int *pipe_indices);

/*
We save the commands to history before executing them
*/
int copy_to_history(char previous_commands[][MAX_COMMAND_LENGTH], 
                    char commands[][MAX_COMMAND_LENGTH]);

/*
Execute the commands. We pass the indices of all the pipes. Create pipes accordingly
and use them for communication between processes.
We also pass a flag to indicate if parent should wait for the child to complete
*/
void execute_commands(char commands[][MAX_COMMAND_LENGTH], 
                int *pipe_indices, 
                int b_background_execution);


#endif