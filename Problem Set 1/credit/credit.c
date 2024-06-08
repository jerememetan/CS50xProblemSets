#include <cs50.h>
#include <stdio.h>
long get_number(void);
int checksum(long n);
int checklength(long n);
int printinvalid(void);
int printamex(void);
int printvisa(void);
int printmastercard(void);

int main(void)
{
    // prompt for input
    long n = get_number();
    // calc checksum
    checksum(n);
    if (checksum(n) == 0)
    {
        printinvalid();
    }
    else
    {
        checklength(n); // check length and starting digits
    }
}

long get_number(void)
{
    long number;
    do
    {
        number = get_long("Number: ");
    }
    while (number < 0);
    return number;
}

int checksum(long n)
{
    int jump1 = 10;
    int jump2 = 100;
    int value = 0;
    int evenvalues = 0;
    int oddvalues = 0;
    // print every other digits, starting from the 2nd value of the back
    for (long i = jump2, divider = 10; i <= n * jump1; i *= jump2, divider *= jump2)
    {
        long numeven = ((n % i) / divider) * 2;
        if (numeven >= jump1)
        {
            for (int j = 10, div = 1; j <= numeven * jump1; j *= jump1, div *= jump1)
            {
                value = (numeven % j) / div;
                evenvalues += value;
            }
        }
        else
        {
            evenvalues += numeven;
        }
    }
    for (long i = jump1, divider = 1; i <= n * jump1; i *= jump2, divider *= jump2)
    {
        long numodd = ((n % i) / divider);
        oddvalues += numodd;
    }
    int totalvalues = evenvalues + oddvalues;
    int output = totalvalues % 10;
    if (output == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int checklength(long n)
{
    int numberlength = 0;
    long jump1 = 10;
    int visalength = 13;
    int mastercardlength = 16;
    int americanexplength = 15;
    for (long i = jump1, divider = 10; i <= n * 10; i *= 10, divider *= 10)
    {
        long numvalue = (n % i) / divider;
        numberlength++;
    }
    if (numberlength == visalength || numberlength == mastercardlength ||
        numberlength == americanexplength)
    {
        long numvalue;
        if (numberlength == visalength)
        {
            // prints first digit out for a 13 digit input
            for (long j = 1, remainder = 10, divider = 1; j < visalength + 1;
                 j++, remainder *= 10, divider *= 10)
            {
                numvalue = (n % remainder) / divider;
            }
            if (numvalue == 4)
            {
                printvisa();
            }
            else
            {
                printinvalid();
            }
        }
        else if (numberlength == americanexplength)
        {
            for (long j = 1, remainder = 100, divider = 1; j < americanexplength;
                 j++, remainder *= 10, divider *= 10)
            {
                numvalue = (n % remainder) / divider;
            }
            printf("%.2li\n", numvalue);
            if (numvalue == 34 || numvalue == 37)
            {
                printamex();
            }
            else
            {
                printinvalid();
            }
        }
        else if (numberlength == mastercardlength)
        {
            for (long j = 1, remainder = 100, divider = 1; j < mastercardlength;
                 j++, remainder *= 10, divider *= 10)
            {
                numvalue = (n % remainder) / divider;
            }
            if (numvalue >= 50 && numvalue <= 55)
            {
                printmastercard();
            }
            else if (numvalue >= 40 && numvalue <= 49)
            {
                printvisa();
            }
            else
            {
                printinvalid();
            }
        }
    }
    else
    {
        printinvalid();
    }
    return 0;
}

int printinvalid(void)
{
    printf("INVALID\n");
    return 0;
}

int printamex(void)
{
    printf("AMEX\n");
    return 1;
}

int printvisa(void)
{
    printf("VISA\n");
    return 1;
}

int printmastercard(void)
{
    printf("MASTERCARD\n");
    return 1;
}
