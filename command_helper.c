#include "command_helper.h"

/*
This function will take a line of text consiting of multiple words and separate them and store in 
the array command_list. It will also check if & is passed at the end of line and set the variable
b_background_execution if yes.
*/
int separate_arguments(char* command_line, 
            char command_list[][MAX_COMMAND_LENGTH], 
            int *b_background_execution)
{
    int displacement = 0;
    int ret = 0;
    int position = 0;

    //Keep on reading until we are getting words
    while (ret = sscanf(command_line + displacement, "%s", command_list[position++]) > 0)
    {
        //printf("Read %s with return value %d", command_list[position-1], ret);
        displacement += strlen(command_list[position - 1]) + 1;
        //printf("word is of length %lu\n", strlen(command_list[position-1]));
    }
    position--;
    //we read words and stored then in postions from 0 to position-1. At position we will
    //put a word that has 0 as the first character. This is our terminator
    //printf("Zeroing position %d\n", position);
    command_list[position][0] = 0;
    if(strcmp(command_list[position-1], "&") == 0)
    {
        *b_background_execution = 1;
        command_list[position-1][0] = 0;
        //printf("Zeroing position %d\n", position-1);
    }
    return 0;
}


/*
This function will check if the passed command is a special command that has to be handled
by the shell
*/
enum special_command check_special_command(char commands[][MAX_COMMAND_LENGTH])
{
    enum special_command ret_val = e_INVALID;
    if(strcmp(commands[0], "exit") == 0)
    {
        ret_val = e_EXIT_COMMAND;
    }
    else if(strcmp(commands[0], "!!") == 0)
    {
        ret_val = e_PREVIOUS_COMMAND;
    }
    return ret_val;
}

/*
This function will take a list of arguments. The first character after last argument will be 0
This fucntion will search for all the pipes in the passed inout and store these indices in pipe_indices,
array
*/
int find_pipes(char command_list[][MAX_COMMAND_LENGTH], 
            int *pipe_indices)
{
    int word_counter = 0;
    int index_counter = 0;
    while(command_list[word_counter][0] != 0)
    {
        if(strcmp(command_list[word_counter], "|") == 0)
        {
            pipe_indices[index_counter++] = word_counter;
            printf("Pipe at index %d \n", word_counter);
        }
        word_counter++;
    }
    pipe_indices[index_counter] = 0;
    return 0;
}

/*
We save the commands to history before executing them
*/
int copy_to_history(char previous_commands[][MAX_COMMAND_LENGTH], 
                    char commands[][MAX_COMMAND_LENGTH])
{
    return 0;
}

/*
Execute the commands. We pass the indices of all the pipes. Create pipes accordingly
and use them for communication between processes.
We also pass a flag to indicate if parent should wait for the child to complete
*/
void execute_commands(char commands[][MAX_COMMAND_LENGTH], 
                int *pipe_indices, 
                int b_background_execution)
{
    return;
}
