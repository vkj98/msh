#include "command_helper.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

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
            //printf("Pipe at index %d \n", word_counter);
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
    memcpy(previous_commands, commands, MAX_COMMAND_LINE_LENGTH);
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
    printf("Execute command\n");
    int index_count = 0;

    int no_of_commands = 0;
    int counter = 0;
    int pids[MAX_NO_OF_COMMANDS] = {0};
    int pid_index = 0;
    for(counter=0; counter<=MAX_NO_OF_COMMANDS; counter++)
    {
        if(commands[counter][0] == 0)
        {
            no_of_commands = counter;
            break;
        }
    }
    int pipefd[MAX_NO_OF_COMMANDS][2] = {0};
    while(pipe_indices[index_count] != 0)
    {
        //Create a pipe
        //Store it an array of pipe fds
        //printf("Creating pipe set %d\n", index_count);
        pipe(pipefd[index_count]);
        index_count++;
    }
    for(int i=0; i <= index_count; i++)
    {
        //Executing command group i
        //printf("Executing command group %d \n", i);
        //For command group i, we have to check if ith pipe is available
        //If it is available, we have to check if i-1th pipe is available
        //If both are available, the command group is from pipe_indices[i-1] + 1
        //to pipe_indices[i]-1. We have to execute this.
        //If pipe[i-1] is available, we will get stdin from there
        //If pipe[i] is available, we will put stdout there
        if(((i-1) >= 0 ) && (pipe_indices[i-1] != 0))
        {
            //There is a left command that will give us input
            if(pipe_indices[i] != 0)
            {
                //There is also a right command
                pids[pid_index++] = spawn_child(commands, pipe_indices[i-1] + 1, 
                    pipe_indices[i] - 1, 
                    pipefd[i-1], 
                    pipefd[i]);
            }
            else
            {
                //There is nothing to the right
                pids[pid_index++] = spawn_child(commands, pipe_indices[i-1] + 1,
                    no_of_commands-1,
                    pipefd[i-1],
                    NULL);
            }
            
        }
        else
        {
            //Nothing for input. Check if there is something for output
            if(pipe_indices[i] != 0)
            {
                //There is comething to the right
                pids[pid_index++] = spawn_child(commands, 0,
                    pipe_indices[i]-1,
                    NULL,
                    pipefd[i]);
            }
            else
            {
                //Nothing for output as well
                pids[pid_index++] = spawn_child(commands, 0, no_of_commands-1, NULL, NULL);
            }
            
        }
        
    }
    if(b_background_execution)
    {
        //Do not wait for execution to complete.
    }
    else
    {
        for(int i=0; i<pid_index; i++)
        {
            wait(NULL);
        }
    }
    
    
    return;
}

int spawn_child(char commands[][MAX_COMMAND_LENGTH], 
                int left_index, 
                int right_index,
                int *input_fid,
                int *output_fid)
{
    printf("spawning a child with li %d and ri %d: \n", left_index, right_index);
    int input_from_file = 0;
    int output_to_file = 0;
    char output_file_name[MAX_COMMAND_LENGTH];
    char input_file_name[MAX_COMMAND_LENGTH];
    int input_index = 0;
    int output_index = 0;

    for(int i=left_index; i<=right_index; i++)
    {
         printf("%s ", commands[i]);
    }
    printf("\n");
    int counter = 0;
    //while(commands[counter][0] != 0)
    for(counter=left_index; counter<=right_index;)
    {
        if(strcmp(commands[counter], ">") == 0)
        {
            output_to_file = 1;
            strcpy(output_file_name, commands[counter+1]);
            input_index = counter;
        } 
        else if(strcmp(commands[counter], "<") == 0)
        {
            input_from_file = 1;
            strcpy(input_file_name, commands[counter+1]);
            output_index = counter;
        }
        counter++;
    }
    if(input_index > 0 && output_index == 0)
    {
        right_index = input_index - 1;
    }
    else if(output_index > 0 && input_index == 0)
    {
        right_index = output_index - 1;
    }
    else if(input_index > 0 && output_index > 0)
    {
        right_index = (input_index > output_index ? output_index : input_index) - 1;
    }

    pid_t pid = fork();
    if(pid == 0)
    {
        //child process
        //If redirect I/O to file then do this first
        printf("Inside child\n");
        int in_fd = 0;
        int out_fd = 0;
        if(input_from_file)
        {
            //Open the file and set it for input
            in_fd = open(input_file_name, O_RDONLY, 0);
            dup2(in_fd, STDIN_FILENO);
        } 
        else if(input_fid != NULL)
        {
            //close the write end and set the read end as stdin
            close(input_fid[WRITE_END_OF_PIPE]);
            dup2(input_fid[READ_END_OF_PIPE], STDIN_FILENO);
        }

        if(output_to_file)
        {
            //Open the file and set it for output
            out_fd = open(output_file_name, O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU);
            dup2(out_fd, STDOUT_FILENO);
        }
        else if(output_fid != NULL)
        {
            //Close the read end and set the write end for output
            close(output_fid[READ_END_OF_PIPE]);
            dup2(output_fid[WRITE_END_OF_PIPE], STDOUT_FILENO);
        }
        char* arguments[MAX_NO_OF_COMMANDS];
        int i = 0;
        fprintf(stderr, "Li and Ri are %d and %d\n", left_index, right_index);
        for(i=left_index; i<=right_index; i++)
        {
            arguments[i-left_index] = commands[i];
            fprintf(stderr, "VK : %s ", commands[i]);
        }
        fprintf(stderr, "Null at location %d\n", i);
        arguments[i-left_index] = (char*)NULL;
        fprintf(stderr, "Calling execvp\n");
        execvp(commands[left_index], arguments);
        //execvp will be called here
    }
    else
    {
        //Parent process
        //Parent has no need of pipes so close it
        if(input_fid != NULL)
        {
            close(input_fid[READ_END_OF_PIPE]);
            //The child will use the read end and parent can close
        }
        if(output_fid != NULL)
        {
            close(output_fid[WRITE_END_OF_PIPE]);
        }
        return pid;
    }
}
