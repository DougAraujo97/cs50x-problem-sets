#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string input_text);
int count_words(string input_text);
int count_sentences(string input_text);
int calculate_grade_level(int total_letters, int total_words, int total_sentences);

int main(void)
{
    // Prompting for text
    string input_text = get_string("Text: ");

    // Calculating letters, words, and sentences
    int total_letters = count_letters(input_text);
    int total_words = count_words(input_text);
    int total_sentences = count_sentences(input_text);

    // calculating grade level
    int grade_level = calculate_grade_level(total_letters, total_words, total_sentences);

    // printing grade level
    if (grade_level < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade_level >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade_level);
    }
    return 0;
}

int count_letters(string input_text)
{
    int total_letters = 0;

    // If the character is alphabetic, it's a letter
    for (int i = 0; i < strlen(input_text); i++)
    {
        if (isalpha(input_text[i]))
        {
            total_letters++;
        }
    }
    return total_letters;
}

int count_words(string input_text)
{
    int total_words = 1;

    // There are as many words as there are spaces + 1 for the last word which has no space after it
    for (int i = 0; i < strlen(input_text); i++)
    {
        if (isspace(input_text[i]))
        {
            total_words++;
        }
    }
    return total_words;
}

int count_sentences(string input_text)
{
    int total_sentences = 0;

    // If it ends with a period, exclamation point, or question mark, it's a sentence
    for (int i = 0; i < strlen(input_text); i++)
    {
        char current_character = input_text[i];

        // I felt the switch statement would be best for this
        switch (current_character)
        {
        case '.':
        case '!':
        case '?':
            total_sentences++;
            break;
        }
    }
    return total_sentences;
}

int calculate_grade_level(int total_letters, int total_words, int total_sentences)
{

    // converting l and s to floats and then rounding to the nearest integer and returning it
    float l = ((float)total_letters / total_words) * 100;
    float s = ((float)total_sentences / total_words) * 100;

    // the formula is copied and pasted from the assignment sheet, I even used the same variables L and S
    int grade_level = round((0.0588 * l) - (0.296 * s) - 15.8);
    return grade_level;
}