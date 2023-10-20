#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Prompting the user for their name
    string name = get_string("What's your name? ");

    // Printing "hello, <user's reply to prompt>"
    printf("hello, %s\n", name);
}