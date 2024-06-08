#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
int calculate_text(string text);
int get_reading_level(float index);

int main(void)
{
    string text = get_string("Text: ");
    double index = calculate_text(text); // calculate readability
    get_reading_level(index);            // print readability
}

int calculate_text(string text)
{
    int letters = 0;
    int words = 1;
    int sentences = 0;
    int length = strlen(text); // this includes spaces and puncuatations too
    double index;
    double remainder;

    for (int i = 0; i < length; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
        if (isspace(text[i]))
        {
            words++;
        }
        if ((text[i]) == '!' || (text[i]) == '.' || (text[i]) == '?')
        {
            sentences++;
        }
    }

    if (words == 1 && letters < 1)
    {
        words--;
    }
    // mutiply by 10 to convert to interger
    index = ((0.0588 * ((double) letters / (double) words) * 100) -
             (0.296 * (((double) sentences / (double) words) * 100)) - 15.8) *
            10;
    if ((int) index % 10 >= 5)
    {
        index /= 10;
        index++;
        return index;
    }
    else
    {
        index /= 10;
        return index;
    }
}
int get_reading_level(float index)
{
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int) index);
    }
    return 1;
}
