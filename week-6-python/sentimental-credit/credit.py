from cs50 import get_int
import sys
from math import floor


def main():

    # declare variables and get user input
    length = 0
    digit = 0
    double_digits = 0
    verification = True

    card_number = get_int("Number: ")

    # check if the card number is valid and exit if not
    verification = check(card_number)

    if verification == False:
        sys.exit(1)

    # keep track of card length and first 2 digits
    while card_number > 0:

        if card_number < 10:
            digit = card_number

        if card_number > 10 and card_number < 100:
            double_digits = card_number

        card_number = floor(card_number / 10)
        length += 1

    # check for card type based on length and digits
    if length == 15 and (double_digits == 34 or double_digits == 37):
        print(f"AMEX\n")
        sys.exit(0)

    elif length == 16 and (double_digits == 51 or
                           double_digits == 52 or
                           double_digits == 53 or
                           double_digits == 54 or
                           double_digits == 55):
        print(f"MASTERCARD\n")
        sys.exit(0)

    elif (length == 13 or length == 16) and digit == 4:
        print(f"VISA\n")
        sys.exit(0)

    else:
        print("INVALID")
        sys.exit(2)


def check(number):

    # declare variables
    sum = 0
    holder = 0
    x = True

    # iterate through the entire card number sequence
    while number > 0:

        # add every other last digit to the sum
        if x == True:

            sum += number % 10
            number = floor(number / 10)
            x = False

        else:

            # multiply every other last digit by 2 and add those digits to the sum
            holder = number % 10
            holder *= 2

            # adds individual digits to the sum if the product of holder * 2 is > 10
            if holder >= 10:

                sum += holder % 10
                sum += floor(holder / 10)

            else:

                sum += holder

            # resets x to True and resets the loop
            x = True
            number = floor(number / 10)

    # check if the sum is divisible by 10 and return false if not
    if sum % 10 == 0:
        return True

    else:
        print("INVALID")
        return False


if __name__ == "__main__":
    main()