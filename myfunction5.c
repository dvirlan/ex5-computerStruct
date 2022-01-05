#include <stdbool.h>
#include <string.h>
#define min(a,b)  ((a) < (b) ? (a) : (b))
#define max(a,b)  ((a) > (b) ? (a) : (b))
#define calcIndex(i,j,n) (((i)*(n))+(j))

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} pixel;

typedef struct {
    int red;
    int green;
    int blue;
    // int num;
} pixel_sum;


/*
 *  Applies kernel for pixel at (i,j)
 */

void smooth(int dim, pixel *src, pixel *dst, int kernelSize, int kernelScale, bool filter) {

    register int i, j;
    register unsigned long local_n = n;
    for (i = 1 ; i < local_n - 1; i++) { //kernelSize / 2 = 1
        for (j = 1; j < local_n - 1; j++) {
                register int ii, jj;
                register pixel *p;
                register int currRow, currCol;
                register pixel_sum sum;
                register unsigned long local_n = n;
                register pixel current_pixel;
                register int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
                register int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
                register int min_row, min_col, max_row, max_col;
                register pixel loop_pixel;
                register int loop_count = 1;
                register pixel p1, p2, p3, p4, p5, p6, p7, p8, p9;

                //initialize_pixel_sum(&sum);
                sum.green = 0;
                sum.red = 0;
                sum.blue = 0;

                p = src + ((i - 1) * local_n + j - 1);
                p1 = *p;
                p2 = *(++p);
                p3 = *(++p);
                p = src + (i * local_n + j - 1);
                p4 = *p;
                p5 = *(++p);
                p6 = *(++p);
                p = src + ((i + 1) * local_n + j - 1);
                p7 = *p;
                p8 = *(++p);
                p9 = *(++p);

                if (kernelScale != 1) {
                    sum.red += ((int) p1.red);
                    sum.green += ((int) p1.green);
                    sum.blue += ((int) p1.blue);

                    sum.red += ((int) p2.red);
                    sum.green += ((int) p2.green);
                    sum.blue += ((int) p2.blue);

                    sum.red += ((int) p3.red);
                    sum.green += ((int) p3.green);
                    sum.blue += ((int) p3.blue);

                    sum.red += ((int) p4.red);
                    sum.green += ((int) p4.green);
                    sum.blue += ((int) p4.blue);

                    sum.red += ((int) p5.red);
                    sum.green += ((int) p5.green);
                    sum.blue += ((int) p5.blue);

                    sum.red += ((int) p6.red);
                    sum.green += ((int) p6.green);
                    sum.blue += ((int) p6.blue);

                    sum.red += ((int) p7.red);
                    sum.green += ((int) p7.green);
                    sum.blue += ((int) p7.blue);

                    sum.red += ((int) p8.red);
                    sum.green += ((int) p8.green);
                    sum.blue += ((int) p8.blue);

                    sum.red += ((int) p9.red);
                    sum.green += ((int) p9.green);
                    sum.blue += ((int) p9.blue);
                } else {
                    sum.red += -((int) p1.red);
                    sum.green += -((int) p1.green);
                    sum.blue += -((int) p1.blue);

                    sum.red += -((int) p2.red);
                    sum.green += -((int) p2.green);
                    sum.blue += -((int) p2.blue);

                    sum.red += -((int) p3.red);
                    sum.green += -((int) p3.green);
                    sum.blue += -((int) p3.blue);

                    sum.red += -((int) p4.red);
                    sum.green += -((int) p4.green);
                    sum.blue += -((int) p4.blue);

                    sum.red += ((int) p5.red) * 9;
                    sum.green += ((int) p5.green) * 9;
                    sum.blue += ((int) p5.blue) * 9;

                    sum.red += -((int) p6.red);
                    sum.green += -((int) p6.green);
                    sum.blue += -((int) p6.blue);

                    sum.red += -((int) p7.red);
                    sum.green += -((int) p7.green);
                    sum.blue += -((int) p7.blue);

                    sum.red += -((int) p8.red);
                    sum.green += -((int) p8.green);
                    sum.blue += -((int) p8.blue);

                    sum.red += -((int) p9.red);
                    sum.green += -((int) p9.green);
                    sum.blue += -((int) p9.blue);
                }

                if (filter) {
                    // find min and max coordinates
                    for (ii = i - 1; ii <= i + 1; ii++) {
                        for (jj = j - 1; jj <= j + 1; jj++) {
                            // check if smaller than min or higher than max and update
                            loop_pixel = src[calcIndex(ii, jj, dim)];
                            register int check = (((int) loop_pixel.red) + ((int) loop_pixel.green) +
                                                  ((int) loop_pixel.blue));
                            if (check <= min_intensity) {
                                min_intensity = check;
                                min_row = ii;
                                min_col = jj;
                            }
                            if (check > max_intensity) {
                                max_intensity = check;
                                max_row = ii;
                                max_col = jj;
                            }
                        }
                    }
                    // filter out min and max
                    // sum_pixels_by_weight(&sum, src[calcIndex(min_row, min_col, dim)], -1);
                    //*** move the function sum_pixels_by_weight directly to here
                    register pixel p = src[calcIndex(min_row, min_col, dim)];
                    sum.red += -((int) p.red);
                    sum.green += -((int) p.green);
                    sum.blue += -((int) p.blue);

                    //sum_pixels_by_weight(&sum, src[calcIndex(max_row, max_col, dim)], -1);
                    register pixel p1 = src[calcIndex(max_row, max_col, dim)];
                    sum.red += -((int) p1.red);
                    sum.green += -((int) p1.green);
                    sum.blue += -((int) p1.blue);

                }

                // assign kernel's resultmy to pixel at [i,j]
                //assign_sum_to_pixel(&current_pixel, sum, kernelScale);
                //*** move the function sum_pixels_by_weight directly to here
                // divide by kernel's weight
                if (kernelScale != 1) {
                    sum.red = sum.red / kernelScale;
                    sum.green = sum.green / kernelScale;
                    sum.blue = sum.blue / kernelScale;
                }

                // truncate each pixel's color values to match the range [0,255]
                current_pixel.red = (unsigned char) (min(max(sum.red, 0), 255));
                current_pixel.green = (unsigned char) (min(max(sum.green, 0), 255));
                current_pixel.blue = (unsigned char) (min(max(sum.blue, 0), 255));

            dst[calcIndex(i, j, local_n)] = current_pixel;
        }
    }
}


void doConvolution(Image *image, int kernelSize,  int kernelScale, bool filter) {

    register pixel* pixelsImg = malloc(m*n*sizeof(pixel));
    register pixel* backupOrg = malloc(m*n*sizeof(pixel));
    register int num = n*m*3;

    //charsToPixels(image, pixelsImg);
    memcpy(pixelsImg, image->data, num);

    //copyPixels(pixelsImg, backupOrg);
    memcpy(backupOrg, pixelsImg, num);


    smooth(m, backupOrg, pixelsImg, kernelSize,  kernelScale, filter);

    //pixelsToChars(pixelsImg, image);
    memcpy(image->data, pixelsImg, num);


    free(pixelsImg);
    free(backupOrg);
}


void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {

    if (flag == '1') {
        // blur image
        doConvolution(image, 3,  9, false);

        // write result image to file
        writeBMP(image, srcImgpName, blurRsltImgName);

        // sharpen the resulting image
        doConvolution(image, 3,  1, false);

        // write result image to file
        writeBMP(image, srcImgpName, sharpRsltImgName);
    } else {
        // apply extermum filtered kernel to blur image
        doConvolution(image, 3,  7, true);

        // write result image to file
        writeBMP(image, srcImgpName, filteredBlurRsltImgName);

        // sharpen the resulting image
        doConvolution(image, 3, 1, false);

        // write result image to file
        writeBMP(image, srcImgpName, filteredSharpRsltImgName);
    }
}
