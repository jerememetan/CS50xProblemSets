#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int JPEG_SIZE = 512;
int JPEG_NUMBER = 0;
int isWriting = 0;

int main(int argc, char *argv[])
{
    // Your program should accept exactly one command-line argument, the name of a forensic image
    // from which to recover JPEGs.
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    FILE *card = fopen(argv[1], "r"); // "r" = read mode
    if (card == NULL)
    {
        printf("Could not open file\n");
        return 1;
    }

    uint8_t buffer[JPEG_SIZE];
    char filename[40];
    FILE *JPEG;
    while (fread(&buffer, sizeof(uint8_t) * JPEG_SIZE, 1, card))
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            if (isWriting == 0) // if first time,   0 is false, 1 is true
            {
                sprintf(filename, "%03i.jpg", JPEG_NUMBER);
                JPEG = fopen(filename, "a");         // creates file , write file
                fwrite(&buffer, JPEG_SIZE, 1, JPEG); // start writing into the output file
                JPEG_NUMBER++;
                isWriting = 1;
            }
            else if (isWriting == 1) // if already writing,
            {
                fclose(JPEG);                               // close current file
                sprintf(filename, "%03i.jpg", JPEG_NUMBER); // create the 2nd file
                JPEG = fopen(filename, "a");
                fwrite(&buffer, JPEG_SIZE, 1, JPEG); // start writing on the 2nd file
                JPEG_NUMBER++;
            }
        }
        else if (isWriting == 1)
        {
            fwrite(&buffer, JPEG_SIZE, 1, JPEG);
        }
    }
    fclose(JPEG);
    fclose(card);
}
