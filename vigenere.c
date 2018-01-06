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
    int count_c = 0;
    while (true)
    {
        if (argv[1][count_c] == '\0')
        {
            count_c++;
            break;
        }

        else if (isalpha(argv[1][count_c]) == false)
        {
            return 1;
            break;
        }

        count_c++;
    }

    //prompt for text
    printf("plaintext: ");
    char *plain = get_string();

    int i = 0;
    int k = 0;
    printf("ciphertext: ");
    while (plain[i] != '\0')
    {
        //check is it a aplhabetical?
        if (isalpha(plain[i]))
        {
            //check is it a upper or lowercase?
            if (isupper(argv[1][k % (count_c - 1)]))
            {
                int num_key = argv[1][k % (count_c - 1)];
                int num_plain = plain[i];
                int num_print = num_plain + (num_key - 65) % 26;
                //check if the number is exceed the ascii for 'Z' then we restart from 'A'
                if (num_print > 90)
                {
                    num_print -= 26;
                }
                printf("%c",num_print);
            }
            else
            {
                int num_key = argv[1][k % (count_c - 1)];
                int num_plain = plain[i];
                int num_print = num_plain + (num_key - 97) % 26;

                //check if the number is exceed the ascii for 'z' then we restart from 'a'
                if (num_print > 122)
                {
                    num_print -= 26;
                }

                //check if letter is uppercase and the number is exceed the ascii for 'Z'
                else if(isupper(plain[i]) && num_print > 90)
                {
                    num_print -= 26;
                }
                printf("%c",num_print);
            }

            k++;
        }
        else
        {
            //print anything beside the alphabetical characters
            printf("%c",plain[i]);
        }
        i++;
    }

    printf("\n");
}





