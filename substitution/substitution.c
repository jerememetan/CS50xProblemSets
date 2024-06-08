#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
bool checkvalidinput(int argc, string s);
bool checkletters(string s);
bool checkrepetition(string s);
string prompt_text(void);
string encipher_text(string txt, string key);

int main(int argc, string argv[])
{
    if (checkvalidinput(argc, argv[1]) == 0)
    {
        string plaintext = prompt_text();
        string ciphertext = encipher_text(plaintext, argv[1]);
        printf("ciphertext: %s\n", ciphertext);
    }
    else
    {
        return 1;
    }
}

bool checkvalidinput(int argc, string s)
{
    string k;
    if (argc == 2) // check command line argument
    {
        if (checkletters(s) == 0)
        {
            k = s;
            if (checkrepetition(s) == 1)
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }
    else
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    return 0;
}

bool checkletters(string s)
{
    int length = strlen(s);
    if (length != 26) // should have 26 digits
    {
        printf("Key must contains 26 characters.\n");
        return 1;
    }
    else
    {
        for (int i = 0; i < 26; i++)
        {
            if (isalpha(s[i]) == 0) // should not have numbers
            {
                printf("Key must only contain alphabetic characters.\n");
                return 1;
            }
        }
    }
    return 0;
}

bool checkrepetition(string s)
{
    char key[25];
    for (int i = 0; i < 26; i++) // lower caps everything
    {
        if (isupper(s[i]))
        {
            s[i] = tolower(s[i]);
        }
        key[i] = s[i];

        if (i > 0) // repeated characters check
        {
            for (int j = 0; j < i; j++)
            {
                if (key[i] == key[j])
                {
                    printf("Key must not contain repeated characters.\n");
                    return 1;
                }
            }
        }
    }
    printf("%s\n", key);
    return 0;
}

string prompt_text(void)
{
    string plaintext = get_string("plaintext:  ");
    return plaintext;
}

string encipher_text(string txt, string key)
{
    string ciphertext = txt;
    int UpperConvert = 65;
    int LowerConvert = 97;
    // NOTE: key is currently in all small letters
    for (int i = 0, length = strlen(txt); i < length; i++)
    {
        if (isupper(txt[i]))
        {
            int value = txt[i] - UpperConvert;
            ciphertext[i] = key[value];
            ciphertext[i] = toupper(ciphertext[i]);
        }
        else if (islower(txt[i]))
        {
            int value = txt[i] - LowerConvert;
            ciphertext[i] = key[value];
        }
    }
    return ciphertext;
}
