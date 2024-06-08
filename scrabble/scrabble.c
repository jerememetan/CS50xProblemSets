#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
int get_score(string word);
void annouce_winner(int score1, int score2);
const int POINTS[] = {1, 3, 3, 2,  1, 4, 2, 4, 1, 8, 5, 1, 3,
                      1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int main(void)
{
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");
    // determine score
    int score1 = get_score(word1);
    int score2 = get_score(word2);
    // annouce winner
    annouce_winner(score1, score2);
}

int get_score(string word)
{
    int score = 0;
    int length = strlen(word);
    for (int i = 0; i < length; i++)
    {
        if (islower(word[i]))
        {
            word[i] = toupper(word[i]);
        }
        score += POINTS[word[i] - 'A'];
    }
    return score;
}

void annouce_winner(int score1, int score2)
{
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}
