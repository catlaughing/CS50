#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long long number;
    do
    {
        printf("Number: ");
        number = get_long_long();
    }
    while (number < 0);

    int count_num = 0;

    for (long long dig = number; dig > 0; dig /= 10)
    {
        count_num++;
    }

    if (count_num != 13 && count_num != 15 && count_num != 16)
    {
        printf("INVALID\n");
    }

    int total1 = 0;
    long long x =  (number/10);
    for (int i = 0; i <= count_num / 2; i++)
    {
        long long n = (x % 10) * 2;
        if (n >= 10)
        {
            n = (n % 10) + 1;
        }

        total1 = total1 + n;
        x /= 100;
    }

    int total2 = 0;
    long long y = number;
    for (int j = 0; j <= count_num / 2 ; j++)
    {
        long long m = (y % 10);
        if (m >= 10)
        {
            m = (m % 10) + 1;
        }

        total2 = total2 + m;
        y /= 100;
    }

    int total3 = total1+total2;

    if (total3 % 10 == 0)
    {
        if (count_num == 15)
        {
            printf("AMEX\n");
        }
        else if(count_num == 13)
        {
            printf("VISA\n");
        }
        else if(count_num == 16)
        {
            if(number > 5100000000000000)
            {
                printf("MASTERCARD\n");
            }
            else
            {
                printf("VISA\n");
            }
        }
        else
        {
            printf("INVALID/n");
        }
    }
}