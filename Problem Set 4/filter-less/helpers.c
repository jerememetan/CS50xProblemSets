#include "helpers.h"
void checkRGB(double array[]);
void swap(BYTE *x, BYTE *y);
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // loop for all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // take average of R G B values
            int total = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) * 10;
            int average = (total / 3);
            if (average % 10 >= 5)
            {
                average /= 10;
                average += 1;
            }
            else
            {
                average /= 10;
            }
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
            // update pixel values
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
            // set new R G B
            double sepiaRGB[3];
            sepiaRGB[0] = (0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen +
                           0.189 * image[i][j].rgbtBlue) *
                          10;
            sepiaRGB[1] = (0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen +
                           0.168 * image[i][j].rgbtBlue) *
                          10;
            sepiaRGB[2] = (0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen +
                           0.131 * image[i][j].rgbtBlue) *
                          10;
            for (int k = 0; k < 3; k++)
            {
                if ((int) sepiaRGB[k] % 10 >= 5)
                {
                    sepiaRGB[k] /= 10;
                    sepiaRGB[k]++;
                }
                else
                {
                    sepiaRGB[k] /= 10;
                }
            }
            checkRGB(sepiaRGB);
            image[i][j].rgbtRed = sepiaRGB[0];
            image[i][j].rgbtGreen = sepiaRGB[1];
            image[i][j].rgbtBlue = sepiaRGB[2];

            // update pixel values
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, m = width / 2; j < m; j++)
        {
            swap(&image[i][j].rgbtRed, &image[i][width - j - 1].rgbtRed);
            swap(&image[i][j].rgbtGreen, &image[i][width - j - 1].rgbtGreen);
            swap(&image[i][j].rgbtBlue, &image[i][width - j - 1].rgbtBlue);
            // swap pixels around
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int x = 0; x < height; x++)
    {
        for (int y = 0; y < width; y++)
        {
            copy[x][y] = image[x][y];
            // swap pixels around
        }
    }
    // get a untouched copy of image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, m = width; j < m; j++)
        {
            int boxamount = 0;
            double boxRGB[] = {0, 0, 0};

            for (int a = -1; a < 2; a++)
            {
                for (int b = -1; b < 2; b++)
                {
                    if (i + a < 0 || i + a > height - 1) // if height becomes negative
                    {
                    }
                    else if (j + b < 0 || j + b > width - 1)
                    {
                    }
                    else
                    {
                        // insert the RGBvalues inside boxRGB
                        boxRGB[0] += copy[i + a][j + b].rgbtRed;
                        boxRGB[1] += copy[i + a][j + b].rgbtGreen;
                        boxRGB[2] += copy[i + a][j + b].rgbtBlue;
                        boxamount++;
                    }
                }
            }
            // average the value RGB compared to the box
            boxRGB[0] = boxRGB[0] / boxamount * 10;
            boxRGB[1] = boxRGB[1] / boxamount * 10;
            boxRGB[2] = boxRGB[2] / boxamount * 10;
            for (int c = 0; c < 3; c++)
            {
                if ((int) boxRGB[c] % 10 >= 5)
                {
                    boxRGB[c] /= 10;
                    boxRGB[c]++;
                }
                else
                {
                    boxRGB[c] /= 10;
                }
            }
            checkRGB(boxRGB);
            image[i][j].rgbtRed = boxRGB[0];
            image[i][j].rgbtGreen = boxRGB[1];
            image[i][j].rgbtBlue = boxRGB[2];
        }
    }
    return;
}

void checkRGB(double array[])
{
    for (int j = 0; j < 3; j++)
    {
        if (array[j] > 255)
        {
            array[j] = 255;
        }
        else if (array[j] < 0)
        {
            array[j] = 0;
        }
    }
}

void swap(BYTE *x, BYTE *y)
{
    BYTE temp = *x;
    *x = *y;
    *y = temp;
}
