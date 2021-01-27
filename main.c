#include <stdio.h>
#include "command_helper.h"

int main()
{
    char command_line[MAX_COMMAND_LINE_LENGTH];
    char command_array[MAX_NO_OF_COMMANDS][MAX_COMMAND_LENGTH];
    char previous_command_array[MAX_NO_OF_COMMANDS][MAX_COMMAND_LENGTH];

    int b_in_loop = 1;
    int b_background_execution = 0;
    int word_counter = 0;

    int pipe_indices[MAX_NO_OF_COMMANDS];

    enum special_command command = e_INVALID;

    while(b_in_loop)
    {
        printf("msh>");
        fgets(command_line, MAX_COMMAND_LINE_LENGTH, stdin);
        separate_arguments(command_line, command_array, &b_background_execution);
        int word_counter = 0;
        while(command_array[word_counter][0] != 0)
        {
            printf("%d : %s ", word_counter, command_array[word_counter]);
            word_counter++;
        }printf("\n");
        command = check_special_command(command_array);
        find_pipes(command_array, pipe_indices);
        printf("After finding pipes\n");
        int pipe_index = 0;
        while(pipe_indices[pipe_index] != 0)
        {
            printf("%d ", pipe_indices[pipe_index++]);
        }
        printf("After printing pipes\n");
        if(command != e_INVALID)
        {
            //This is a special command
            switch(command)
            {
                case e_EXIT_COMMAND:
                    b_in_loop = 0;
                    break;
                case e_PREVIOUS_COMMAND:
                    //Execute the previous commands
                    printf("Execute the previous command");
                    break;
            }
        }
        else
        {
            //Children have to be spawned for the commands
        }
        
    }


}