#include <stdio.h>
#include "command_helper.h"

int main()
{
    char command_line[MAX_COMMAND_LINE_LENGTH];
    char command_array[MAX_NO_OF_COMMANDS][MAX_COMMAND_LENGTH];
    char previous_command_array[MAX_NO_OF_COMMANDS][MAX_COMMAND_LENGTH];

    int b_in_loop = 1;

    while(b_in_loop)
    {
        printf("msh>");
        fflush(stdin);
        getchar();
    }


}