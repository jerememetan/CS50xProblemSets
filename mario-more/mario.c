#include <cs50.h>
#include <stdio.h>
int get_size(void);
void print_bricks(int s);
void print_blank();
void print_block();

int main(void)
{
    // check int value
    int s = get_size();

    // print bricks
    print_bricks(s);
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

void print_bricks(int s)
{
    int gapsize = 2; // defining the gaps to use for loops later

    for (int i = 1; i <= s; i++) // caps the height to S
    {
        for (int j = 0; j < s - i; j++) // printing blanks
        {
            print_blank();
        }

        for (int k = 0; k < i && k < s; k++)
        {
            print_block();
        }
        for (int l = 0; l < gapsize; l++)
        {
            print_blank();
        }
        for (int m = 0; m < i && m < s; m++)
        {
            print_block();
        }
        printf("\n");
    }
}

void print_blank()
{
    printf(" ");
}

void print_block()
{
    printf("#");
}
