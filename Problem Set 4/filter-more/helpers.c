#include "helpers.h"
#include <math.h>
#include <stdlib.h>
void swap(RGBTRIPLE *a, RGBTRIPLE *b);
void checkRGB(double array[]);
void createcopy(int height, int width, RGBTRIPLE image[height][width],
                RGBTRIPLE copy[height][width]);
RGBTRIPLE convertarray(double array[], RGBTRIPLE newimage);
double calculateGxarray(int h, int w, int height, int width, double GRGB[],
                        RGBTRIPLE copy[height][width]);
double calculateGyarray(int h, int w, int height, int width, double GRGB[],
                        RGBTRIPLE copy[height][width]);
int RGBarray = 3;
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avg;
            avg = ((image[i][j].rgbtRed + image[i][j].rgbtBlue + image[i][j].rgbtGreen) * 10) / 3;
            if (avg % 10 >= 5)
            {
                avg /= 10;
                avg++;
            }
            else
            {
                avg /= 10;
            }
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, k = width / 2; j < k; j++)
        {
            swap(&image[i][j], &image[i][width - j - 1]);
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    createcopy(height, width, image, copy);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
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
                        boxRGB[0] += copy[i + a][j + b].rgbtRed;
                        boxRGB[1] += copy[i + a][j + b].rgbtGreen;
                        boxRGB[2] += copy[i + a][j + b].rgbtBlue;
                        boxamount++;
                    }
                }
            }
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
            image[i][j] = convertarray(boxRGB, image[i][j]);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    createcopy(height, width, image, copy);
    for (int i = 0; i < height; i++) // i = 2
    {
        for (int j = 0; j < width; j++) // j = 2
        {
            double *GxRGB = malloc(RGBarray * sizeof(double)); // initalisation
            double *GyRGB = malloc(RGBarray * sizeof(double));
            double *newRGB = malloc(RGBarray * sizeof(double));
            for (int x = 0; x < 3; x++) // setting arrays to 0
            {
                GxRGB[x] = 0;
                GyRGB[x] = 0;
                newRGB[x] = 0;
            }
            *GxRGB = calculateGxarray(i, j, height, width, GxRGB, copy); // calculate for Gx
            *GyRGB = calculateGyarray(i, j, height, width, GyRGB, copy); // calculate for Gy
            for (int a = 0; a < RGBarray; a++)
            {
                newRGB[a] = sqrt((GxRGB[a] * GxRGB[a]) + (GyRGB[a] * GyRGB[a])) * 10;
                if ((int) newRGB[a] % 10 >= 5)
                {
                    newRGB[a] /= 10;
                    newRGB[a]++;
                }
                else
                {
                    newRGB[a] /= 10;
                }
            }
            free(GyRGB);
            free(GxRGB);
            checkRGB(newRGB);
            image[i][j] = convertarray(newRGB, copy[i][j]);
            free(newRGB);

            // to ge the new updated value, square the red value of in Gx and Gy, then sqrt it
            // repeat for all R G and B
        }
    }
    return;
}

void swap(RGBTRIPLE *a, RGBTRIPLE *b)
{
    RGBTRIPLE temp = *a;
    *a = *b;
    *b = temp;
}

void createcopy(int height, int width, RGBTRIPLE image[height][width],
                RGBTRIPLE copy[height][width])
{
    for (int x = 0; x < height; x++)
    {
        for (int y = 0; y < width; y++)
        {
            copy[x][y] = image[x][y];
        }
    }
    return;
}

RGBTRIPLE convertarray(double array[], RGBTRIPLE newimage)
{
    newimage.rgbtRed = array[0];
    newimage.rgbtGreen = array[1];
    newimage.rgbtBlue = array[2];
    return newimage;
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

double calculateGxarray(int h, int w, int height, int width, double GRGB[],
                        RGBTRIPLE copy[height][width])
{
    for (int a = -1; a < 2; a++) // do 3 times,
    {
        for (int b = -1; b < 2; b += 2) // do 2 times,
        {
            if (h + a < 0 || h + a > height - 1) // checking for edge cases
            {
                // add 0 into the array
            }
            else if (w + b < 0 || w + b > width - 1)
            {
                // add 0 into the array
            }
            else if (a == 0)
            {
                GRGB[0] += (b * copy[h + a][w + b].rgbtRed) * 2;
                GRGB[1] += (b * copy[h + a][w + b].rgbtGreen) * 2;
                GRGB[2] += (b * copy[h + a][w + b].rgbtBlue) * 2;
            }
            else
            {
                GRGB[0] += (b * copy[h + a][w + b].rgbtRed);
                GRGB[1] += (b * copy[h + a][w + b].rgbtGreen);
                GRGB[2] += (b * copy[h + a][w + b].rgbtBlue);
            }
        }
    }
    return *GRGB;
}

double calculateGyarray(int h, int w, int height, int width, double GRGB[],
                        RGBTRIPLE copy[height][width])
{
    for (int a = -1; a < 2; a++) // do 3 times,
    {
        for (int b = -1; b < 2; b += 2) // do 2 times,
        {
            if (w + a < 0 || w + a > width - 1) // checking for edge cases
            {
                // add 0 into the array
            }
            else if (h + b < 0 || h + b > height - 1)
            {
                // add 0 into the array
            }
            else if (a == 0)
            {
                GRGB[0] += (b * copy[h + b][w + a].rgbtRed) * 2;
                GRGB[1] += (b * copy[h + b][w + a].rgbtGreen) * 2;
                GRGB[2] += (b * copy[h + b][w + a].rgbtBlue) * 2;
            }
            else
            {
                GRGB[0] += (b * copy[h + b][w + a].rgbtRed);
                GRGB[1] += (b * copy[h + b][w + a].rgbtGreen);
                GRGB[2] += (b * copy[h + b][w + a].rgbtBlue);
            }
        }
    }
    return *GRGB;
}
