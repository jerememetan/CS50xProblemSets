#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
} candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name); // put values inside the voter table
void tabulate(void);                         // put from voter table into candidates table
bool print_winner(void);                     // determine whether there is a winner
int find_min(void);                          // find min value to eliminate
bool is_tie(int min);                        // check whether everyone is tied
void eliminate(int min);                     // eliminate last place

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++) // repeat for amount of voters
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++) // for each voter, repeat for all candidates
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name)) // return false, if return true, it will update accordingly I =
                                   // voter ID, J = Rank ID, name = Candidate
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break; // break means finish
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    for (int i = 0; i < candidate_count; i++) //
    {
        if (strcmp(name, candidates[i].name) == 0) // check valid name
        {
            preferences[voter][rank] = i;  // int of candidate is initaited
            for (int k = 0; k < rank; k++) // check repeats
            {
                if (preferences[voter][k] == preferences[voter][rank])
                {
                    return false;
                }
            }
            return true; // end here once checks are done
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (candidates[preferences[i][j]].eliminated == true)
            {
                // is candidate eliminated?
            }
            else if (candidates[preferences[i][j]].eliminated == false)
            {
                // if not, add vote count to candidate
                candidates[preferences[i][j]].votes++;
                // ends the loop, go to the next voter
                j += candidate_count;
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    int votestowin = voter_count / 2 + 1;
    for (int i = 0, j = candidate_count; i < j; i++)
    {
        if (candidates[i].votes >= votestowin)
        {
            // as long as voter count hits more than 50%, win
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int min = 0;
    for (int i = 0, j = candidate_count; i < j; i++)
    {
        if (candidates[i].eliminated)
        {
            // is it elimiated?
        }
        else if (min == 0)
        {
            // is min value the original value?
            min = candidates[i].votes;
        }
        else if (candidates[i].votes < min)
        {
            // if not. compare to previous min value
            min = candidates[i].votes;
        }
    }
    return min;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    for (int i = 0, j = candidate_count; i < j; i++)
    {
        // as long as 1 candidate more than min, false
        if (candidates[i].votes > min)
        {
            return false;
        }
    }
    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    for (int i = 0, j = candidate_count; i < j; i++)
    {
        // Setting eliminated value as true
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
    return;
}
