#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCKSIZE 512

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{

    // check for proper usage
    if (argc != 2)
    {
        printf("usage: ./recover <filename>\n");
        return 1;
    }

    // creating filename variable
    char *filename = argv[1];

    // creating a buffer that holds 512 bytes
    BYTE buffer[BLOCKSIZE];

    // open file and check if it points to NULL
    FILE *input = fopen(filename, "r");

    FILE *output = NULL;

    if (input == NULL)
    {
        printf("Could not open %s.\n", filename);
        return 1;
    }

    // jpegcounter helps to iterate the filenames and to check for the first jpeg
    int jpegcounter = -1;

    // buffer holds the next filename
    char outfilebuffer[8];

    // iterate through the file
    while (fread(buffer, 1, BLOCKSIZE, input) == BLOCKSIZE)
    {
        // check for jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // update counter and filename buffer
            jpegcounter++;
            sprintf(outfilebuffer, "%03i.jpg", jpegcounter);

            // check if jpeg is the first one
            if (jpegcounter == 0)
            {
                // create output file with the name stored in the buffer and write to it
                output = fopen(outfilebuffer, "w");
                fwrite(buffer, 1, BLOCKSIZE, output);
            }

            // if this jpeg isn't the first one...
            else if (jpegcounter > 0)
            {
                // close previous jpeg and create a new output file and write to it
                fclose(output);
                output = fopen(outfilebuffer, "w");
                fwrite(buffer, 1, BLOCKSIZE, output);
            }

        }

        // if the block isn't the start of a jpeg, and no jpeg has been found yet, move onto the next block
        else if (output != NULL)
        {
            // if a jpeg has already been found, however, continue writing to the new output file
            fwrite(buffer, 1, BLOCKSIZE, output);
        }
    }

    // close files
    fclose(input);
    fclose(output);

    return 0;

}