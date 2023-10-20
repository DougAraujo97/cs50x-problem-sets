import sys
from math import floor

def main():
    numlist = []
    verification = True
    cardnumber = input("Number: ")

    for digit in cardnumber:
        if digit.isdigit():
            numlist.append(int(digit))

    verification = check(numlist)

    if verification == False:
        print("INVALID")
        sys.exit(1)

    length = len(numlist)
    digit = numlist[0]
    digits = int(str(numlist[0]) + str(numlist[1]))

    if length == 15 and (digits == 34 or digits == 37):
        print("AMEX")
        sys.exit(0)

    elif length == 16 and (digits >= 51 and digits <= 55):
        print("MASTERCARD")
        sys.exit(0)

    elif (length == 13 or length == 16) and digit == 4:
        print("VISA")
        sys.exit(0)

    print("INVALID")
    sys.exit(2)


def check(numlist):
    total = 0

    for i in range(len(numlist)):
        if i % 2 == 0:
            total += numlist[-1 - i]

        elif numlist[-1 - i] * 2 >= 10:
            total += numlist[-1 - i] * 2 % 10
            total += floor(numlist[-1 - i] * 2 / 10)

        else:
            total += numlist[-1 - i] * 2

    if total % 10 == 0:
        return True

    return False

if __name__ == "__main__":
    main()