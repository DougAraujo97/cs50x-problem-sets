import csv
import sys


def main():

    # TODO: Check for command-line usage

    # Exit if the user doesn't input enough command-line arguments
    if len(sys.argv) != 3:
        print("USAGE: dna.py .csv file .txt file")
        sys.exit(1)

    # Initialize a list of sequences and a dictionary for the  sequence's corresponding longest match
    sequence_list = []
    sequence_dict = {}

    # TODO: Read database file into a variable

    # Open the database file
    database = open(sys.argv[1], "r")

    reader = csv.DictReader(database)

    # Input as many sequences into the list as there are fieldnames minus the "name" column
    sequence_list = reader.fieldnames[1:]

    # TODO: Read DNA sequence file into a variable

    # Open the sequence file
    sequence_file = open(sys.argv[2], "r").read()

    # TODO: Find longest match of each STR in DNA sequence

    # Populate the dictionary with the longest match for every sequence in the list
    for sequence in sequence_list:
        sequence_dict[sequence] = longest_match(sequence_file, sequence)

    # TODO: Check database for matching profiles

    # Iterate through the sequence list for every row in the database file
    for row in reader:
        matches = 0
        name = row["name"]

        for sequence in sequence_list:

            # Update the "matches" variable every time there's a match between the database and the dictionary
            if int(row[sequence]) == sequence_dict[sequence]:
                matches += 1

        # If the sequence matches a name in the database, there will be as many matches as there are sequences in the sequence list
        if matches == len(sequence_list):

            # Print the person's name, close the database file and exit the program
            print(name)
            database.close()
            sys.exit(0)

    # If there are no matches, print "No match", close the database file and exit the program
    print("No match")
    database.close()
    sys.exit(2)


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
