from cs50 import get_int


# TODO


def main():

    # repeat get_int as long as the user doesn't input a number between 1 and 8
    while True:
        height = get_int("Height: ")

        if height >= 1 and height <= 8:
            break

        else:
            print("Height must be an integer between 1 and 8!")

    # draw the pyramid
    draw(height)


def draw(height):

    # iterate through each row and print the spaces and hashes required
    for row in range(height):
        print(' ' * (height - row - 1) + '#' * (row + 1) + '  ' + '#' * (row + 1))


if __name__ == "__main__":
    main()
