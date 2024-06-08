#include <cs50.h>
#include <stdio.h>
int get_change(void);
int calc(int change);
void printcoinstotal(int coins_total);

int main(void)
{
    int change = get_change();     // gets input from user
    int coins_total = calc(change); // gets value of total number of coins
    printcoinstotal(coins_total);   // prints coins needed
}

int get_change(void)
{
    int change;
    do
    {
        change = get_int("Change owed: ");
    }
    while (change < 0);
    return change;
}

int calc(int change)
{
    int quarter = 25;
    int dime = 10;
    int nickel = 5;
    int penny = 1;
    int coinstotal = 0;

    do
    {
        if (change >= quarter)
        {
            change = change - quarter;
            coinstotal++;
        }
        else if (change >= dime)
        {
            change = change - dime;
            coinstotal++;
        }
        else if (change >= nickel)
        {
            change = change - nickel;
            coinstotal++;
        }
        else
        {
            change = change - penny;
            coinstotal++;
        }
    }
    while (change > 0);
    return coinstotal;
}

void printcoinstotal(int coins_total)
{
    printf("%i\n", coins_total);
}
