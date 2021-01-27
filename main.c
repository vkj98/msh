#include <stdio.h>
#include "command_helper.h"

int main()
{
    char command_line[MAX_COMMAND_LINE_LENGTH];
    char command_array[MAX_NO_OF_COMMANDS][MAX_COMMAND_LENGTH];
    char previous_command_array[MAX_NO_OF_COMMANDS][MAX_COMMAND_LENGTH];

    memset(command_line, MAX_COMMAND_LINE_LENGTH, 0);
    memset(command_array, MAX_COMMAND_LINE_LENGTH, 0);
    memset(previous_command_array, MAX_COMMAND_LINE_LENGTH, 0);


    int b_in_loop = 1;
    int b_background_execution = 0;
    int word_counter = 0;

    int pipe_indices[MAX_NO_OF_COMMANDS] = {};

    enum special_command command = e_INVALID;

    while(b_in_loop)
    {
        printf("msh>");
        fflush(stdout);
        fgets(command_line, MAX_COMMAND_LINE_LENGTH, stdin);
        separate_arguments(command_line, command_array, &b_background_execution);
        if(command_array[0][0] == 0)
        {
            continue;
        }
        int word_counter = 0;
        // while(command_array[word_counter][0] != 0)
        // {
        //     printf("%d : %s ", word_counter, command_array[word_counter]);
        //     word_counter++;
        // }printf("\n");
        command = check_special_command(command_array);
        if (command != e_INVALID)
        {
            //This is a special command
            switch (command)
            {
            case e_EXIT_COMMAND:
                b_in_loop = 0;
                break;
            case e_PREVIOUS_COMMAND:
                //Execute the previous commands
                //printf("Execute the previous command\n");
                if(previous_command_array[0][0] == 0)
                {
                    printf("No previous command available\n");
                    break;
                }
                word_counter = 0;
                //while (previous_command_array[word_counter][0] != 0)
                //{
                //    printf("%s ", previous_command_array[word_counter]);
                //    word_counter++;
                //}
                //printf("\n");
                find_pipes(previous_command_array, pipe_indices);
                //printf("After finding pipes\n");
                int pipe_index = 0;
                //while (pipe_indices[pipe_index] != 0)
                //{
                //    printf("%d ", pipe_indices[pipe_index++]);
                //}
                //printf("After printing pipes of previous\n");
                execute_commands(previous_command_array, pipe_indices, b_background_execution);
                break;
            }
        }
        else
        {
            //Children have to be spawned for the commands
            copy_to_history(previous_command_array, command_array);
            find_pipes(command_array, pipe_indices);
            //printf("After finding pipes\n");
            int pipe_index = 0;
            // while (pipe_indices[pipe_index] != 0)
            // {
            //     printf("%d ", pipe_indices[pipe_index++]);
            // }
            //printf("After printing pipes\n");
            execute_commands(command_array, pipe_indices, b_background_execution);
        }
    }


}