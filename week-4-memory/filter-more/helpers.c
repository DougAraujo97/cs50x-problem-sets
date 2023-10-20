#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Initializing an average color value variable to 0
    int average_color_value = 0;

    // Iterate through the image
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {

            // Get the average color value rounded to the nearest integer
            average_color_value = round((image[row][col].rgbtRed + image[row][col].rgbtGreen + image[row][col].rgbtBlue) / 3.0);

            // Set RGB values to the new average value
            image[row][col].rgbtRed = average_color_value;
            image[row][col].rgbtGreen = average_color_value;
            image[row][col].rgbtBlue = average_color_value;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{

    // Iterating through the image
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width / 2; col++)
        {

            // Placing the current pixel into a buffer and swapping it with the last pixel
            RGBTRIPLE buffer = image[row][col];
            image[row][col] = image[row][width - (col + 1)];
            image[row][width - (col + 1)] = buffer;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    // Declaring variables for original and updated values
    RGBTRIPLE tempimage[height][width], ogvalues[height][width];

    // Iterating through the original image
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {

            // Storing original values into my ogvalues variable
            ogvalues[row][col] = image[row][col];
        }
    }

    // Iterate through the image
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {

            // initializing variables for total rgb values and a counter by which i'll divide to get the average
            float totalr, totalg, totalb;
            int counter;

            totalr = totalg = totalb = counter = 0;

            // Iterate through a 3x3 grid around every pixel
            for (int grid_row = 0; grid_row < 3; grid_row++)
            {
                for (int grid_col = 0; grid_col < 3; grid_col++)
                {

                    // Check if pixel is outside image borders
                    if (grid_row + row - 1 < 0 || grid_row + row - 1 > height - 1)
                    {
                        continue;
                    }

                    if (grid_col + col - 1 < 0 || grid_col + col - 1 > width - 1)
                    {
                        continue;
                    }

                    // Add the total rgb values for every pixel within the 3x3 grid and iterate the counter
                    totalr += ogvalues[grid_row + row - 1][grid_col + col - 1].rgbtRed;
                    totalg += ogvalues[grid_row + row - 1][grid_col + col - 1].rgbtGreen;
                    totalb += ogvalues[grid_row + row - 1][grid_col + col - 1].rgbtBlue;
                    counter++;
                    // The counter will have a value of either 4, 6 or 9 depending on the pixel's placement
                }
            }

            // Store average color values in the temp image
            tempimage[row][col].rgbtRed = round(totalr / counter);
            tempimage[row][col].rgbtGreen = round(totalg / counter);
            tempimage[row][col].rgbtBlue = round(totalb / counter);
            // the "total" variables are floats so the round function should return an accurate value
        }
    }

    // Iterate through the image
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {

            // Update original image with new color values
            image[row][col] = tempimage[row][col];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{

    // Declaring a variable to store original values and a variable to store updated values
    RGBTRIPLE ogvalues[height][width], tempimage[height][width];

    // Iterate through the image
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {

            // Pass every pixel's original value to the ogvalues variable
            ogvalues[row][col] = image[row][col];
        }
    }

    // Initializing gx and gy matrix
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Iterate through the image
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {

            // Initializing a 3x3 grid to apply the gx and gy matrix to
            int gridy[3] = {row - 1, row, row + 1};
            int gridx[3] = {col - 1, col, col + 1};

            // Initialize gx and gy variables for each color and set them to 0
            int gxred, gxgreen, gxblue, gyred, gygreen, gyblue;

            gxred = gxgreen = gxblue = gyred = gygreen = gyblue = 0;

            // Iterate through a 3x3 grid around the pixel
            for (int grid_row = 0; grid_row < 3; grid_row++)
            {
                for (int grid_col = 0; grid_col < 3; grid_col++)
                {

                    // Check if pixel is outside the original image
                    if (row + grid_row - 1 < 0 || row + grid_row - 1 > height - 1)
                    {
                        continue;
                    }

                    if (col + grid_col - 1 < 0 || col + grid_col - 1 > width - 1)
                    {
                        continue;
                    }

                    // Declaring a pixel variable
                    RGBTRIPLE pixel = ogvalues[gridy[grid_row]][gridx[grid_col]];

                    // Compute gx and gy for individual pixel's rgb values
                    gxred += pixel.rgbtRed * gx[grid_row][grid_col];
                    gyred += pixel.rgbtRed * gy[grid_row][grid_col];

                    gxgreen += pixel.rgbtGreen * gx[grid_row][grid_col];
                    gygreen += pixel.rgbtGreen * gy[grid_row][grid_col];

                    gxblue += pixel.rgbtBlue * gx[grid_row][grid_col];
                    gyblue += pixel.rgbtBlue * gy[grid_row][grid_col];

                }
            }

            // Compute final color values and cap at 255
            int finalred = round(sqrt(gxred * gxred + gyred * gyred));
            tempimage[row][col].rgbtRed = finalred > 255 ? 255 : finalred;

            int finalgreen = round(sqrt(gxgreen * gxgreen + gygreen * gygreen));
            tempimage[row][col].rgbtGreen = finalgreen > 255 ? 255 : finalgreen;

            int finalblue = round(sqrt(gxblue * gxblue + gyblue * gyblue));
            tempimage[row][col].rgbtBlue = finalblue > 255 ? 255 : finalblue;
        }
    }

    // Pass new color values from tempimage to original image
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            image[row][col] = tempimage[row][col];
        }
    }

    return;
}
