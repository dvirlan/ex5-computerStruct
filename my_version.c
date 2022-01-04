
#include <stdbool.h>
#include <string.h>

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} pixel;

/*
* My algorithm: to avoid function's call I did most of the code in smooth.
* I initialize local objects to avoid repeated memory access and calculate most of my calculators
* by one move over the current 9 pixels.
* I avoided loops because we know our kernel is 3*3 so we need to calculate maximum 9 things at a time.
* Also I putted magic numbers where I can, for example: the kernel size is 9, we move over the pixels without the outside frame
* so we begin at i=j=1 and end at i=j=n-1 and etc.
*/

/*
* Apply the kernel over each pixel.
* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than kernelSize/2 and/or
* column index smaller than kernelSize/2
* if the kernelScale is 1 than we need to multiply the center pixel in 9 and remove from it all the others.
*/
void smooth(pixel *src, Image *image, int kernelScale, bool filter) {
    register int i, j;
    register pixel *p;
    register unsigned long n_loc = n;
    char *data = image->data + (3 * n_loc + 3);
    pixel p1, p2, p3, p4, p5, p6, p7, p8, p9;
    int sum_red, sum_blue, sum_green;
    int sum1, sum2, sum3, sum4, sum5, sum6, sum7, sum8, sum9;
    int total_sum, max, min, max_index, min_index;
    for (i = 1; i < n_loc - 1; ++i) {
        for (j = 1; j < n_loc - 1; ++j) {
            sum_red = 0;
            sum_green = 0;
            sum_blue = 0;
            min_index = 0;
            max_index = 0;

            //copy every pixel to a p_i pixel to avoid returned access to memory
            p = src + ((i - 1) * n_loc + j - 1);
            p1 = *p;
            p2 = *(++p);
            p3 = *(++p);
            p = src + (i * n_loc + j - 1);
            p4 = *p;
            p5 = *(++p);
            p6 = *(++p);
            p = src + ((i + 1) * n_loc + j - 1);
            p7 = *p;
            p8 = *(++p);
            p9 = *(++p);

            //no need to multiply
            if(kernelScale == 1){
                sum_red -= (int) p1.red;
                sum_green -= (int) p1.green;
                sum_blue -= (int) p1.blue;
                sum_red -= (int) p2.red;
                sum_green -= (int) p2.green;
                sum_blue -= (int) p2.blue;
                sum_red -= (int) p3.red;
                sum_green -= (int) p3.green;
                sum_blue -= (int) p3.blue;
                sum_red -= (int) p4.red;
                sum_green -= (int) p4.green;
                sum_blue -= (int) p4.blue;
                sum_red += (int) p5.red * 9;
                sum_green += (int) p5.green * 9;
                sum_blue += (int) p5.blue * 9;
                sum_red -= (int) p6.red;
                sum_green -= (int) p6.green;
                sum_blue -= (int) p6.blue;
                sum_red -= (int) p7.red;
                sum_green -= (int) p7.green;
                sum_blue -= (int) p7.blue;
                sum_red -= (int) p8.red;
                sum_green -= (int) p8.green;
                sum_blue -= (int) p8.blue;
                sum_red -= (int) p9.red;
                sum_green -= (int) p9.green;
                sum_blue -= (int) p9.blue;
            }

            else {
                //add every pixel to the total sum and check witch pixel is the biggest/smallest
                //check if the upper left is the biggest or the smallest
                sum_red += (int) p1.red;
                sum_green += (int) p1.green;
                sum_blue += (int) p1.blue;
                sum1 = sum_red + sum_green + sum_blue;
                max = sum1;
                min = sum1;
                max_index = 1;
                min_index = 1;
                total_sum = sum1;

                //check if the upper is the biggest or the smallest
                sum_red += (int) p2.red;
                sum_green += (int) p2.green;
                sum_blue += (int) p2.blue;
                sum2 = sum_red + sum_green + sum_blue - sum1;
                if (sum2 > max) {
                    max = sum2;
                    max_index = 2;
                }
                if (sum2 <= min) {
                    min = sum2;
                    min_index = 2;
                }
                total_sum += sum2;

                //check if the upper right is the biggest or the smallest
                sum_red += (int) p3.red;
                sum_green += (int) p3.green;
                sum_blue += (int) p3.blue;
                sum3 = sum_red + sum_green + sum_blue - total_sum;
                if (sum3 > max) {
                    max = sum3;
                    max_index = 3;
                }
                if (sum3 <= min) {
                    min = sum3;
                    min_index = 3;
                }
                total_sum += sum3;

                //check if the  left is the biggest or the smallest
                sum_red += (int) p4.red;
                sum_green += (int) p4.green;
                sum_blue += (int) p4.blue;
                sum4 = sum_red + sum_green + sum_blue - total_sum;
                if (sum4 > max) {
                    max = sum4;
                    max_index = 4;
                }
                if (sum4 <= min) {
                    min = sum4;
                    min_index = 4;
                }
                total_sum += sum4;

                //check if the middle is the biggest or the smallest
                sum_red += (int) p5.red;
                sum_green += (int) p5.green;
                sum_blue += (int) p5.blue;
                sum5 = sum_red + sum_green + sum_blue - total_sum;
                if (sum5 > max) {
                    max = sum5;
                    max_index = 5;
                }
                if (sum5 <= min) {
                    min = sum5;
                    min_index = 5;
                }
                total_sum += sum5;

                //check if the right is the biggest or the smallest
                sum_red += (int) p6.red;
                sum_green += (int) p6.green;
                sum_blue += (int) p6.blue;
                sum6 = sum_red + sum_green + sum_blue - total_sum;
                if (sum6 > max) {
                    max = sum6;
                    max_index = 6;
                }
                if (sum6 <= min) {
                    min = sum6;
                    min_index = 6;
                }
                total_sum += sum6;

                //check if the bottom left is the biggest or the smallest
                sum_red += (int) p7.red;
                sum_green += (int) p7.green;
                sum_blue += (int) p7.blue;
                sum7 = sum_red + sum_green + sum_blue - total_sum;
                if (sum7 > max) {
                    max = sum7;
                    max_index = 7;
                }
                if (sum7 <= min) {
                    min = sum7;
                    min_index = 7;
                }
                total_sum += sum7;

                //check if the bottom is the biggest or the smallest
                sum_red += (int) p8.red;
                sum_green += (int) p8.green;
                sum_blue += (int) p8.blue;
                sum8 = sum_red + sum_green + sum_blue - total_sum;
                if (sum8 > max) {
                    max = sum8;
                    max_index = 8;
                }
                if (sum8 <= min) {
                    min = sum8;
                    min_index = 8;
                }
                total_sum += sum8;

                //check if the bottom right is the biggest or the smallest
                sum_red += (int) p9.red;
                sum_green += (int) p9.green;
                sum_blue += (int) p9.blue;
                sum9 = sum_red + sum_green + sum_blue - total_sum;
                if (sum9 > max) {
                    max = sum9;
                    max_index = 9;
                }
                if (sum9 <= min) {
                    min = sum9;
                    min_index = 9;
                }
            }

            // If the image is blur with filter we remove the biggest and the smallest
            // image from the sum and divided by 7 instead of 9.
            // Because the min and max can't be the same pixel, we can check just 1 time
            // for every pixel if he is the smallest or the biggest.
            if (filter) {
                if (max_index == 1 || min_index == 1) {
                    sum_blue = sum_blue - (int) p1.blue;
                    sum_green = sum_green - (int) p1.green;
                    sum_red = sum_red - (int) p1.red;
                }
                if (max_index == 2 || min_index == 2) {
                    sum_blue = sum_blue - (int) p2.blue;
                    sum_green = sum_green - (int) p2.green;
                    sum_red = sum_red - (int) p2.red;
                }
                if (max_index == 3 || min_index == 3) {
                    sum_blue = sum_blue - (int) p3.blue;
                    sum_green = sum_green - (int) p3.green;
                    sum_red = sum_red - (int) p3.red;
                }
                if (max_index == 4 || min_index == 4) {
                    sum_blue = sum_blue - (int) p4.blue;
                    sum_green = sum_green - (int) p4.green;
                    sum_red = sum_red - (int) p4.red;
                }
                if (max_index == 5 || min_index == 5) {
                    sum_blue = sum_blue - (int) p5.blue;
                    sum_green = sum_green - (int) p5.green;
                    sum_red = sum_red - (int) p5.red;
                }
                if (max_index == 6 || min_index == 6) {
                    sum_blue = sum_blue - (int) p6.blue;
                    sum_green = sum_green - (int) p6.green;
                    sum_red = sum_red - (int) p6.red;
                }
                if (max_index == 7 || min_index == 7) {
                    sum_blue = sum_blue - (int) p7.blue;
                    sum_green = sum_green - (int) p7.green;
                    sum_red = sum_red - (int) p7.red;
                }
                if (max_index == 8 || min_index == 8) {
                    sum_blue = sum_blue - (int) p8.blue;
                    sum_green = sum_green - (int) p8.green;
                    sum_red = sum_red - (int) p8.red;
                }
                if (max_index == 9 || min_index == 9) {
                    sum_blue = sum_blue - (int) p9.blue;
                    sum_green = sum_green - (int) p9.green;
                    sum_red = sum_red - (int) p9.red;
                }
            }

            // assign kernel's result to pixel at [i,j]
            // divide by kernel's weight
            if (kernelScale == 9) {
                sum_red = sum_red / 9;
                sum_green = sum_green / 9;
                sum_blue = sum_blue / 9;
            }

            if (kernelScale == 7) {
                sum_red = sum_red / 7;
                sum_green = sum_green / 7;
                sum_blue = sum_blue / 7;
            }

            // truncate each pixel's color values to match the range [0,255]
            if (sum_red < 0)
                sum_red = 0;
            if (sum_red > 255)
                sum_red = 255;
            if (sum_green < 0)
                sum_green = 0;
            if (sum_green > 255)
                sum_green = 255;
            if (sum_blue < 0)
                sum_blue = 0;
            if (sum_blue > 255)
                sum_blue = 255;

            //put the pixel in the image
            *data = sum_red;
            *(data + 1) = sum_green;
            *(data + 2) = sum_blue;
            data += 3;
        }
        data += 6;
    }
}

/*
* the function a copy of the image and blur with/without filter or sharp the image in accordiance
* to the input's parameters.
*/
void doConvolution(Image *image, int kernelScale, bool filter) {

    register pixel *pixelsImg = malloc(m * n * sizeof(pixel));
    //make copy of the image into a pixel* object
    memcpy(pixelsImg, image->data, n * m * 3);
    smooth(pixelsImg, image, kernelScale, filter);
    free(pixelsImg);
}

void myfunction(Image *image, char *srcImgpName, char *blurRsltImgName, char *sharpRsltImgName,
                char *filteredBlurRsltImgName, char *filteredSharpRsltImgName, char flag) {
    char c = flag;
    if (c == '1') {
        // blur image
        doConvolution(image, 9, false);

        // write result image to file
        writeBMP(image, srcImgpName, blurRsltImgName);

        // sharpen the resulting image
        doConvolution(image, 1, false);

        // write result image to file
        writeBMP(image, srcImgpName, sharpRsltImgName);
    } else {
        // apply extermum filtered kernel to blur image
        doConvolution(image, 7, true);

        // write result image to file
        writeBMP(image, srcImgpName, filteredBlurRsltImgName);

        // sharpen the resulting image
        doConvolution(image, 1, false);

        // write result image to file
        writeBMP(image, srcImgpName, filteredSharpRsltImgName);
    }
}
