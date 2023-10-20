#include <cs50.h>
#include <stdio.h>

bool check(long number); // abstracted the Luhn's algorithm part of verification
int main(void)

{

    // declaring variables
    long card_number;
    int length = 0, digit = 0, double_digits = 0;
    bool verification;

    // prompting the user for the card number
    card_number = get_long("Number: ");

    // verifying number validity
    verification = check(card_number);
    if (verification == false)
    {

        // this ends the program if the Luhn's Algorithm verification is false
        return 0;
    }
    while (card_number > 0)
    {
        if (card_number < 10)
        {
            digit = card_number;
        }
        if (card_number > 10 && card_number < 100)
        {
            double_digits = card_number;
        }

        // takes digits away one at a time to operate both the counter, and the first digit identifier
        card_number /= 10;

        // counts how long the card number is
        length++;
    }

    // final verification process, determining what brand  of card it is
    if (length == 15 && (double_digits == 34 || double_digits == 37))
    {
        printf("AMEX\n");
        return 0;
    }
    else if (length == 16 && (double_digits == 51 ||
                              double_digits == 52 ||
                              double_digits == 53 ||
                              double_digits == 54 ||
                              double_digits == 55))
    {
        printf("MASTERCARD\n");
        return 0;
    }
    else if ((length == 13 || length == 16) && digit == 4)
    {
        printf("VISA\n");
        return 0;
    }
    else
    {
        printf("INVALID\n");
        return 0;
    }
}

bool check(long number)
{

    // sum is the total number which has to end in a 0. holder is a placeholder that gets added to the sum
    int sum = 0, holder = 0;
    bool x = true;
    while (number > 0)
    {

        // the card number always starts as true
        if (x == true)
        {

            // the last digit is separated and added to the total
            sum += number % 10;
            number /= 10;

            // this starts the next loop so every other number can be multiplied by 2
            x = false;
        }
        else
        {
            holder = number % 10;

            // multiplying the previously second-to-last number by 2
            holder *= 2;
            if (holder >= 10)
            {

                // this is so we can separate and add the individual digits in a number over 10
                sum += holder % 10;
                sum += holder / 10;
            }
            else
            {

                // this is where the sum and the holder are added together
                sum += holder;
            }

            // resetting the loop until all numbers have been separated and added together accordingly
            x = true;
            number /= 10;
        }
    }

    // the card is valid if the remainder of the sum / 10 is 0!
    if (sum % 10 == 0)
    {
        return true;
    }
    else
    {
        printf("INVALID\n");
        return false;
    }
}