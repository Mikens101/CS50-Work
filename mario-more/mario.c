#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height; // Define data type

    // Accept <height> if between 1 and 8...
    do
    {
        height = get_int("Height of pyramid: "); // Ask height
    }
    while (height < 1 || height > 8);

    // Build height
    for (int i = 0; i < height; i++)
    {
        // Build length L
        for (int j = 0; j < height; j++)
        {
            if (i + j < height - 1)
            {
                printf(" ");
            }
            else
            {
                printf("#");
            }
        }
        printf("  ");

        // Build length R
        for (int j = 0; j < height; j++)
        {
            if (j - i <= 0)
            {
                printf("#");
            }
        }

        printf("\n");
    }

}