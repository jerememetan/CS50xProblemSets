#include <cs50.h>
#include <stdio.h>
#include <string.h>
int selsort(void);
int bubsort(void);
int mergsort(int n[], int size);
void swap_int(int* a, int* b);
void merge(int left[], int right[], int n[], int ls, int rs);

#define Num 8

int numbers[] = {12, 11, 8, 5, 6, 1, 35, 15};
int sorted[Num];

int main (void)
{
    printf("1. Selection\n");
    printf("2. Bubble\n");
    printf("3. Merge\n");
    int type = get_int("Type of Sort?\n");
    if (type == 1)
    {
        printf("Selection Sort initaited!\n");
        selsort();
        for (int i = 0; i < Num; i++)
        {
            printf("%i ", sorted[i]);
        }
        printf("\n");
        return 0;
    }
    if (type == 2)
    {
        printf("Bubble Sort initaited!\n");
        bubsort();
        for (int i = 0; i < Num; i++)
        {
            printf("%i ", sorted[i]);
        }
        printf("\n");
        return 0;
    }
    if (type == 3)
    {
        printf("Merge Sort initaited!\n");
        mergsort(numbers, sizeof(numbers)/sizeof(numbers[0]));
        for (int i = 0; i < Num; i++)
        {
            printf("%i ", numbers[i]);
        }
        printf("\n");
        return 0;
    }
    else
    {
        return 1;
    }
    return 1;
}

int selsort(void)
{
    int minindex;
    for (int i = 0; i < Num; i++) // for i from 0 to n - 1
    {
        minindex = numbers[i]; // find the smallest number  between numbers [i] and number [n]
        if (i!= (Num - 1)) // if its not the last number
        {
             for (int j = i + 1; j < Num; j++)
             {

                if (minindex > numbers[j])   // compares the smallest number
                {
                    minindex = numbers[j];  // redefines the min index
                }
                if (minindex!= numbers[i])  // when the min index changes,
                {
                    swap_int(&numbers[j], &numbers[i]);  // swaps the numbers locations
                    sorted[i] = minindex; // put inside the sorted array
                }
                else
                {
                    sorted[i] = minindex;
                }
             }
        }
        else
        {
            sorted[i] = minindex;
        }

    }
    return 1;
}

int bubsort(void)
{
    for (int i = 0; i < Num; i++)                       // repeat n - 1 times, since i(0) does nothing
    {
            for (int j = 0; j < i; j++)                 // if number[i] and number [i+1] are out of order
            {
                if (numbers[i] > numbers[j])
                {
                    swap_int(&numbers[i], &numbers[j]); // swap them, else contiune
                }
            }
    }
    for (int i = 0; i < Num; i++)
    {
        sorted[i] = numbers[i];
    }
    return 0;
}

int mergsort(int n[], int size)
{
    // int numbers[] = {2, 4, 5, 12, 1, 10, 35, 15};

    if (size <= 1)
    {
        return 1;
    }
    int middle = size /2;
    // define two arrays
    int leftarray[middle];
    int rightarray[middle];
    int l = 0, r = 0;
    for (; l < size; l++) // done n / 2 times
    {
        if (l < middle)
        {
            leftarray[l] = n[l];
            //printf("left: %i\n", leftarray[l]);
        }

        else
        {
             rightarray[r] = n[middle + r];
             //printf("right: %i\n", rightarray[r]);
             r++;
        }
    }

    mergsort(leftarray, sizeof(leftarray)/sizeof(leftarray[0]));
    mergsort(rightarray, sizeof(rightarray)/sizeof(rightarray[0]));
    merge(leftarray, rightarray, n, sizeof(leftarray)/sizeof(leftarray[0]),
          sizeof(rightarray)/sizeof(rightarray[0]));
    return 0;
}

void swap_int(int* a, int* b)
{
    int aux = *a;
    *a = *b;
    *b = aux;
}

void merge(int left[], int right[], int n[], int ls, int rs)
{
    int leftsize = ls;   // do it x amount of times, based on the size of array of left []
    if (leftsize == 0)
    {
        leftsize = 1;
    }
    int rightsize = rs;
    if (rightsize == 0)  // do it x amount of times, based on the size of array of right []
    {
        rightsize = 1;
    }
    int i = 0; int l = 0; int r = 0; // track counter, only "going forward"

    while (l < leftsize && r < rightsize)
    {
        if (left[l] > right[r])   // initiate value  accordingly
        {
            n[i] = right[r];
            i++;
            r++;
        }
        else
        {
            n[i] = left[l];
            i++;
            l++;
        }

    }
    while(l < leftsize)   //if reaches a certain end (either left of right), put remaining
    {                     //values in
        n[i] = left[l];
        i++;
        l++;
    }

     while(r < rightsize)
    {
        n[i] = right[r];
        i++;
        r++;
    }
    return;
}

