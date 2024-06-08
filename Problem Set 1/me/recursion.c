#include <cs50.h>
#include <stdio.h>
#include <string.h>
int getfirst(int n[], int size);
int numbers[] = {2, 4, 5, 12, 1, 10};

int main (void)
{

    getfirst(numbers, sizeof(numbers)/ sizeof(numbers[0]));
}

int getfirst(int n[], int size)
{
    int middle = size / 2;
    if (size <= 1)
    {
        return 1;
    }

    int newarray[middle];
    for (int i = 0; i < middle; i++)
    {
        newarray[i] = numbers[i];
        printf("Value: %i ", newarray[i]);
    }
    getfirst(newarray, sizeof(newarray)/ sizeof(newarray[0]));
    return 0;
}
