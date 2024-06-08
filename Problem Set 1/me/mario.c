#include <cs50.h>
#include <stdio.h>

int get_size(void);
void print_grid(int size);

int main(void)
{
    int s = get_size();

    print_grid(s);
}

int get_size(void)
{
    int s;
    do
    {
        s = get_int("Size: ");
    }
    while (s < 1);
    return s;
}

void print_grid(int s)
{
    int counter = s;
    int startsize = 1;

    for (int j = 0; j < counter; j++)
    {
        for (int k = 0; k < s - startsize; k++) // prints the blanks
        {
            printf(" ");
        }
        for (int i = 0; i < startsize && i < s; i++) // prints the #s
        {
            printf("#");
        }
        startsize++;
        printf("\n");
    }
}
