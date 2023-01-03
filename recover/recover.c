#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check num of command args
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open file
    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL)
    {
        printf("File not valid");
        return 1;
    }

    // Important vars
    unsigned char buffer[512];
    int counter = 0;    // Track num of images created
    FILE *output_file = NULL;   // Pointer for recovered imgs
    char *filename = malloc(8 * sizeof(char));  // memory allocation

    // Write new files
    while (fread(buffer, sizeof(char), 512, input_file))
    {
        // Check if start of new file
        if ((buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff) && ((buffer[3] & 0xf0) == 0xe0))
        {
            if (counter > 0)
            {
                fclose(output_file);
            }

            // Create file name
            sprintf(filename, "%03i.jpg", counter);

            // Open file
            output_file = fopen(filename, "w");

            // Track # of images
            counter++;
        }

        // Check if file has not been used
        if (output_file != NULL)
        {
            fwrite(buffer, sizeof(char), 512, output_file);
        }
    }

    // Close all files
    fclose(input_file);
    fclose(output_file);
    free(filename);

    return 0;
}