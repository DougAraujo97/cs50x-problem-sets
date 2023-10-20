#include <cs50.h>
#include <stdio.h>
#include <strings.h>

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
bool cycles(int winner, int loser);
void lock_pairs(void);
void print_winner(void);

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

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // iterate through every candidate in search of
    // a match between a candidate and the user's input, regardless of case!
    for (int i = 0; i < candidate_count; i++)
    {

        // strcmp works, but I don't trust users to always get the case right
        if (strcasecmp(name, candidates[i]) == 0)
        {

            // place the user's votes in order of preference into ranks
            ranks[rank] = i;
            return true;
        }
    }

    // return false if the vote is invalid
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{

    // nested "for" loops to place values into the preferences "grid."
    // visualizing the preferences as a grid REALLY helped.
    // I see the variable "i" like a row and "j" like a column.
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // increment the value at that position in the grid by 1
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // we'll be iterating through the preferences grid
    // by using nested "for" loops again
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {

            // comparing 2 "cells" in the grid to determine which
            // of the 2 is the winner.
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }

            // no need to do the reverse "if pref[j][i] > pref[i][j]
            // because the result of that comparison will have already been determined!
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {

        // a bit long winded, but the reasoning behind this is just to
        // determine the difference between how many people wanted one candidate
        // over the other and vice versa.
        int strength_one = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];

        // initializing a variable "max_strength" to help me make the swap later
        int max_strength = i;

        for (int j = i + 1; j < pair_count; j++)
        {

            // determining the strength of victory for the second pair of candidates
            int strength_two = preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner];

            // replacing the max strength if the second pair's strength > the first pair's.
            if (strength_two > strength_one)
            {
                max_strength = j;
                strength_one = strength_two;
            }

            // if the second pair had a greater S.O.V. i use a placeholder to hold
            // the first pair's value, swap the second into the first, and the placeholder's
            // value into the second pair!
            if (max_strength == j)
            {
                pair placeholder = pairs[i];
                pairs[i] = pairs[max_strength];
                pairs[max_strength] = placeholder;
            }
        }
    }
    return;
}

bool cycles(int winner, int loser)
{
    // if at any point, a candidate with an arrow pointed at them
    // points an arrow at a candidate before them, we have a loop or a cycle.
    if (locked[loser][winner] == true)
    {
        return true;
    }

    // the variable "i" is just out of habit, it could be anything
    for (int i = 0; i < candidate_count; i++)
    {
        // if a candidate with an arrow pointed at them points back to any candidate
        // before them at position "i" as "i" is incremented,

        // AND "i" ALSO cycles back to any candidate stored in "winner"
        // as "i" increments, we have a cycle.
        if (locked[loser][i] == true && cycles(winner, i))
        {
            return true;
        }
    }

    // having passed both these tests, there must not be a cycle
    // and we can lock the pair in by returning false.
    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // iterate through every pair
    for (int i = 0; i < pair_count; i++)
    {

        // check for cycles
        if (!cycles(pairs[i].winner, pairs[i].loser))
        {
            // lock them in if there is no cycle
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // looking at the grid of locked pairs, the winning
    // candidate's column is all false, as i noticed in the walkthrough.

    // set variable column
    int column;

    // iterate through columns
    for (column = 0; column < candidate_count; column++)
    {
        // this variable will be set to 0 if there is a single "true"
        // in a given candidate's column.
        int verify_false_column = 1;

        // iterate through the rows
        for (int row = 0; row < candidate_count; row++)
        {

            // set verifier to "true" which will ultimately remove
            // this candidate from contention
            if (locked[row][column] == true)
            {
                verify_false_column = 0;
            }
        }

        // if there was no "true" cell in a given candidate's column, the variable
        // will still be "1" and that candidate is the winner.
        if (verify_false_column == 1)
        {
            break;
        }
    }

    // the winner is the candidate with no "true" in their column
    string winner = candidates[column];

    // print the winning candidate's name
    printf("%s\n", winner);
    return;
}