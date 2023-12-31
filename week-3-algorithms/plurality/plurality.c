#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
} candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

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
    // check if the vote matches any candidates, ignoring case
    for (int i = 0; i < candidate_count; i++)
    {

        // update candidate's vote count
        if (strcasecmp(name, candidates[i].name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }

    // returning false if vote is invalid
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{

    // declaring my variable that I'll compare to vote counts
    int winning_votes = 0;

    for (int i = 0; i < candidate_count; i++)
    {

        // updating winning vote count to highest number of votes
        if (candidates[i].votes > winning_votes)
        {
            winning_votes = candidates[i].votes;
        }
    }

    for (int j = 0; j < candidate_count; j++)
    {

        // printing the names of the candidates with the highest vote count
        if (candidates[j].votes == winning_votes)
        {
            printf("%s\n", candidates[j].name);
        }
    }
    return;
}