#include <cs50.h>
#include <stdio.h>
#include <ctype.h>



int main(void)
{
    char *name = get_string();

    int i = 0;
    //check if the first char is a space or not and if not just capitalize it and print
    if (name[0] != ' ')
    {
        printf("%c",toupper(name[0]));
    }

    //looping through the name
    while (true)
    {
        //check if its already the end of the name
        if (name[i] == '\0')
        {
            printf("\n");
            break;
        }

        //check is the char im looking right now is a space and aftar that is not a space or a '\0' and if yes just print it
        else if (name[i] == ' '  && name[i+1] != ' ' && name[i+1] != '\0')
        {
            printf("%c",toupper(name[i+1]));
        }

        i++;
    }
}