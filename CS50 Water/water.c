#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int minute;
    do
    {
        printf("Minute: ");
        minute = get_int();
    }
    while (minute < 0);
    int bottle = minute * 12;
    printf("Bottle: %i\n",bottle);
}