#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
    bool iswinner;
} candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);
int bubsort(void);
void swap(candidate *x, candidate *y);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].iswinner = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int j = 0; j < candidate_count; j++)
    {
        if (strcmp(name, candidates[j].name) == 0)
        {
            candidates[j].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    int winneramt = 1;
    for (int i = 0; i < MAX; i++) // sorts by most vote to least votw
    {

        for (int j = 0; j < i; j++)
        {
            if (candidates[i].votes > candidates[j].votes)
            {
                swap(&candidates[i], &candidates[j]);
            }
        }
    }
    candidates[0].iswinner = 1;
    for (int j = 1; j < MAX; j++)
    {
        if (candidates[0].votes == candidates[j].votes)
        {
            candidates[j].iswinner = 1;
            winneramt++;
        }
    }
    for (int k = 0; k < winneramt; k++)
    {
        printf("%s\n", candidates[k].name);
    }
    return;
}

void swap(candidate *x, candidate *y)
{
    candidate temp = *x;
    *x = *y;
    *y = temp;
}
