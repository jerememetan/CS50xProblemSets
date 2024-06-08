#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
bool checkdigits(string s);
int converttoint(string s);
string prompt_text(void);
string encipher_text(string s, int k);

int main(int argc, string argv[])
{
    string plaintext;
    string ciphertext;
    int key;
    // Make sure program was run with just one command-line argument
    {
        if (argc != 2)
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
        else
        {
            if (checkdigits(argv[1]) == 1)
            {
                return 1;
            }
        }
    }
    key = converttoint(argv[1]);
    plaintext = prompt_text();
    ciphertext = encipher_text(plaintext, key);
    printf("ciphertext: %s\n", ciphertext);
}

bool checkdigits(string s)
{
    int length = strlen(s);
    for (int i = 0; i < length; i++)
    {
        if (isdigit(s[i]) == 0)
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }
    return 0;
}

int converttoint(string s)
{
    int value;
    value = atoi(s);
    return value;
}

string prompt_text(void)
{
    string plaintext = get_string("plaintext:  ");
    return plaintext;
}

string encipher_text(string s, int k)
{
    string ciphertext = s;
    int UpperConvert = 65;
    int LowerConvert = 97;
    for (int i = 0, length = strlen(s); i < length; i++)
    {
        if (isupper(s[i]))
        {
            s[i] -= UpperConvert;
            (ciphertext[i]) = (s[i] + k) % 26;
            ciphertext[i] += UpperConvert;
        }
        else if (islower(s[i]))
        {
            s[i] -= LowerConvert;
            (ciphertext[i]) = (s[i] + k) % 26;
            ciphertext[i] += LowerConvert;
        }
    }
    return ciphertext;
}
