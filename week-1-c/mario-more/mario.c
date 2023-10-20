#include <cs50.h>
#include <stdio.h>

int main(void)

{

    // Declaring my variables
    int w, x, y, z, a;

    // Prompting user for height between 1 and 8, inclusive
    do
    {
        w = get_int("Height: ");
    } while (w < 1 || w > 8);

    // This loop prints a new row
    for (x = 0; x < w; x++)
    {

        // This loop prints the first set of spaces
        for (y = 0; y < w - x - 1; y++)
        {
            printf(" ");
        }

        // This loop prints the first set of hashes
        for (z = 0; z <= x; z++)
        {
            printf("#");
        }

        // This prints the 2 spaces that separate the 2 pyramids
        printf("  ");

        // This prints the rightmost pyramid
        for (a = 0; a <= x; a++)
        {
            printf("#");
        }
        printf("\n");
    }
}
