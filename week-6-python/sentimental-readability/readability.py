# import methods
from cs50 import get_string
import sys


# TODO


def main():

    # get user input
    text = get_string("Text: ")

    # count letters, words, sentences, and calculate the grade level
    lettercount = count_letters(text)
    wordcount = count_words(text)
    sentencecount = count_sentences(text)
    gradelevel = calculate_grade_level(lettercount, wordcount, sentencecount)

    # print the grade level
    if gradelevel < 1:
        print("Before Grade 1")

    elif gradelevel >= 16:
        print("Grade 16+")

    else:
        print(f"Grade {gradelevel}")

    # exit
    sys.exit(0)


def count_letters(text):

    # iterate through the text and update the counter with every letter
    lettercount = 0

    for character in text:
        if character.isalpha():
            lettercount += 1

    return lettercount


def count_words(text):

    # split the text into a list of words and the wordcount will be the length of the list
    wordlist = text.split()
    wordcount = len(wordlist)

    return wordcount


def count_sentences(text):

    # update the sentence counter whenever we encounter a punctuation, exclamation, or question mark
    sentencecount = 0

    for character in text:
        if character == '.' or character == '!' or character == '?':

            sentencecount += 1

    return sentencecount


def calculate_grade_level(lettercount, wordcount, sentencecount):

    # define l and s variables and place them into the formula to determine grade level
    l = lettercount / wordcount * 100
    s = sentencecount / wordcount * 100

    gradelevel = round((0.0588 * l) - (0.296 * s) - 15.8)

    return gradelevel


if __name__ == "__main__":
    main()