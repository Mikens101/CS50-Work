#include <math.h>
#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float average = (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.00;
            int avgr = round(average);

            image[i][j].rgbtRed = avgr;
            image[i][j].rgbtGreen = avgr;
            image[i][j].rgbtBlue = avgr;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //formula from instructions
            int sepiaRed = round((.393 * image[i][j].rgbtRed) + (.769 * image[i][j].rgbtGreen) + (.189 * image[i][j].rgbtBlue));
            int sepiaGreen = round((.349 * image[i][j].rgbtRed) + (.686 * image[i][j].rgbtGreen) + (.168 * image[i][j].rgbtBlue));
            int sepiaBlue = round((.272 * image[i][j].rgbtRed) + (.534 * image[i][j].rgbtGreen) + (.131 * image[i][j].rgbtBlue));

            // If sepia-- is over 255, return 255, else return sepia-- (ternary)
            //image[i][j].rgbtRed = (sepiaRed > 255) ? 255 : sepiaRed;
            //image[i][j].rgbtGreen = (sepiaGreen > 255) ? 255 : sepiaGreen;
            //image[i][j].rgbtBlue = (sepiaBlue > 255) ? 255 : sepiaBlue;
            //                              condition (if) true : false

            if (sepiaRed > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = sepiaRed;
            }

            if (sepiaGreen > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = sepiaGreen;
            }

            if (sepiaBlue > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = sepiaBlue;
            }
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            //make tmp values
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;

            //copy values shifted over (same row, different opposite column)
            //work toward the middle, one side then the other
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;

            //restoring overwritten side
            image[i][width - j - 1].rgbtRed = red;
            image[i][width - j - 1].rgbtGreen = green;
            image[i][width - j - 1].rgbtBlue = blue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //make a tmp copy of the image
    RGBTRIPLE tmp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //set vars to 0
            int sumRed = 0;
            int sumGreen = 0;
            int sumBlue = 0;
            float counter = 0;

            //for 3 rows (forming the 9x9 around the selected tile)
            for (int k = -1; k < 2; k++)
            {
                //for 3 columns (forming the 9x9 around the selected tile)
                for (int h = -1; h < 2; h++)
                {
                    //k & h = -1 to offset the values from the selected tile
                    //if they don't fit conditions, it means we're tryign to take a...
                    //...value from outside the frame of the image
                    if (i + k < 0 || i + k > height - 1 || j + h < 0 || j + h > width - 1)
                    {
                        continue;
                    }

                    //adding the values to prep for avg
                    sumRed += image[i + k][j + h].rgbtRed;
                    sumGreen += image[i + k][j + h].rgbtGreen;
                    sumBlue += image[i + k][j + h].rgbtBlue;

                    //how many times we will need to divide
                    counter++;
                }
            }

            tmp[i][j].rgbtRed = round(sumRed / counter);
            tmp[i][j].rgbtGreen = round(sumGreen / counter);
            tmp[i][j].rgbtBlue = round(sumBlue / counter);
        }
    }

    //replacing original image values with those stored in tmp
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = tmp[i][j].rgbtRed;
            image[i][j].rgbtGreen = tmp[i][j].rgbtGreen;
            image[i][j].rgbtBlue = tmp[i][j].rgbtBlue;
        }
    }
    return;
}
