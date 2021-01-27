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
    }


}