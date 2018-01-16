#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int Height;
    do
    {
        printf("Height: ");
        Height = get_int();
    }
    while (Height < 0 || Height > 23);

    //making the pyramide
    for (int i = 0; i < Height; i++)
    {
        //pring left-hand side
        for (int k = 0; k < Height - (i + 1); k++)
        {
            printf(" ");
        }
        for (int j = 0; j < i + 1; j++)
        {
            printf("#");
        }

        //print gap
        printf("  ");

        //print right-hand side
        for (int l = 0; l < i + 1; l++)
        {
            printf("#");
        }
        printf("\n");
    }
}