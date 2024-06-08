#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void swap(pair *a, pair *b);
bool isloop(int winner, int loser);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks)) // if return false, else contiune with the code
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks); // LOOP FOR EACH VOTER!!

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[]) // returns only true/false
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            for (int j = 0; j < rank; j++)
            {
                if (ranks[j] == ranks[rank])
                {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] += 1;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int counter = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (preferences[i][j] == preferences[j][i])
            {
                // bruh its equal, ignore
            }
            else if (preferences[i][j] > preferences[j][i])
            {
                pair_count++;
                pairs[counter].winner = i;
                pairs[counter].loser = j;
                counter++;
            }
            else
            {
                pair_count++;
                pairs[counter].winner = j;
                pairs[counter].loser = i;
                counter++;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (preferences[pairs[i].winner][pairs[i].loser] >
                preferences[pairs[j].winner][pairs[j].loser])
            {
                swap(&pairs[i], &pairs[j]);
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        // before we lock a pair, check if the loser is pointing to anyone and creating a loop
        if (!isloop(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int candidatepoint[candidate_count];
    int min = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        candidatepoint[i] = 0;
    }
    // look at locked[j], least amount of true in the columm wins
    for (int j = 0; j < candidate_count; j++)
    {
        for (int k = 0; k < candidate_count; k++)

            if (locked[j][k] == true)
            {
                candidatepoint[k]++;
            }
    }

    for (int x = 0; x < candidate_count; x++) // find min
    {
        if (min == 0 && x == 0)
        {
            min = candidatepoint[x];
        }
        else if (candidatepoint[x] < min)
        {
            min = candidatepoint[x];
        }
    }

    for (int y = 0; y < candidate_count; y++) // print all that are at min
    {
        if (candidatepoint[y] == min)
        {
            printf("%s\n", candidates[y]);
        }
    }

    return;
}

void swap(pair *a, pair *b)
{
    pair aux = *a;
    *a = *b;
    *b = aux;
}

bool isloop(int winner, int loser)
{
    if (winner == loser)
    {
        printf("loop detected\n");
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {

        if (locked[loser][i] ==
            true) // if loser row has "True" and winner is being pointed at, follow loser breadcrumb
        {
            if (isloop(winner, i))
            {
                return true;
            }
        }
    }
    return false;
}
