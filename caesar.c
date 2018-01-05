#include <cs50.h>
#include <stdio.h>
#include <ctype.h>


int main(int argc, char *argv[])
{
    //check if the argument is 2
    if (argc != 2)
    {
        printf("Usage: ./vigenere k");
        return 1;
    }

    //check if there non alphabetical characters
    //prompt for text
    printf("plaintext: ");
    char *plain = get_string();

    int i = 0;
    int key = atoi(argv[1]);
    printf("ciphertext: ");
    while (plain[i] != '\0')
    {
        int num_plain = plain[i];
        int num_print = num_plain + key % 26;
        //check if t
        if (isupper(plain[i]) && num_print > 90)
        {
            num_print -= 26;
        }

        else if (num_print > 122)
        {
            num_print -= 26;
        }
        printf("%c",num_print);
        i++;
    }
    printf("\n");
}