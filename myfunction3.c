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
 * assign_sum_to_pixel - Truncates pixel's new value to match the range [0,255]
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum, int kernelScale) {

    // divide by kernel's weight
    sum.red = sum.red / kernelScale;
    sum.green = sum.green / kernelScale;
    sum.blue = sum.blue / kernelScale;

    // truncate each pixel's color values to match the range [0,255]
    current_pixel->red = (unsigned char) (min(max(sum.red, 0), 255));
    current_pixel->green = (unsigned char) (min(max(sum.green, 0), 255));
    current_pixel->blue = (unsigned char) (min(max(sum.blue, 0), 255));
    return;
}

/*
* sum_pixels_by_weight - Sums pixel values, scaled by given weight
*/
static void sum_pixels_by_weight(pixel_sum *sum, pixel p, int weight) {
    sum->red += ((int) p.red) * weight;
    sum->green += ((int) p.green) * weight;
    sum->blue += ((int) p.blue) * weight;
    // sum->num++;
    return;
}

/*
 *  Applies kernel for pixel at (i,j)
 */
static pixel applyKernel(int dim, int i, int j, pixel *src, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {

   register int ii, jj;
    register int currRow, currCol;
    register pixel_sum sum;
    register pixel current_pixel;
    register int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
    register int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
    register int min_row, min_col, max_row, max_col;
    register pixel loop_pixel;
    register int loop_count = 1;

    //initialize_pixel_sum(&sum);
    sum.green = 0;
    sum.red = 0;
    sum.blue = 0;
//***change max(i-1, 0) to i-1 and min(i+1, dim-1) to dim-1
    for(ii = i-1; ii <= i+1; ii++) {
        for(jj = j-1; jj <= j+1; jj++) {

            register int kRow, kCol;
            //***change the conditions to this equations
            kRow = ii - i + 1;
            kCol = jj - j + 1;

            // compute row index in kernel
//            if (ii < i) {
//                kRow = 0;
//            } else if (ii > i) {
//                kRow = 2;
//            } else {
//                kRow = 1;
//            }

            // compute column index in kernel
//            if (jj < j) {
//                kCol = 0;
//            } else if (jj > j) {
//                kCol = 2;
//            } else {
//                kCol = 1;
//            }

            // apply kernel on pixel at [ii,jj]
            // sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);
            //*** move the function sum_pixels_by_weight directly to here
            register pixel p=src[calcIndex(ii, jj, dim)];
            if(kernelScale == 1 ) {
                if(loop_count != 5) {
                    sum.red += -((int) p.red) ;
                    sum.green += -((int) p.green) ;
                    sum.blue += -((int) p.blue) ;
                }
                else{
                    sum.red += ((int) p.red) * 9;
                    sum.green += ((int) p.green) * 9;
                    sum.blue += ((int) p.blue) * 9;
                }

            }
            else{
                sum.red += ((int) p.red) ;
                sum.green += ((int) p.green) ;
                sum.blue += ((int) p.blue) ;
            }
            loop_count++;
        }
    }

    if (filter) {
        // find min and max coordinates
        for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) {
            for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) {
                // check if smaller than min or higher than max and update
                loop_pixel = src[calcIndex(ii, jj, dim)];
                register int check = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
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
        pixel p=src[calcIndex(min_row, min_col, dim)];
        sum.red += -((int) p.red);
        sum.green += -((int) p.green);
        sum.blue += -((int) p.blue);

        //sum_pixels_by_weight(&sum, src[calcIndex(max_row, max_col, dim)], -1);
        register pixel p1=src[calcIndex(max_row, max_col, dim)];
        sum.red += -((int) p1.red);
        sum.green += -((int) p1.green);
        sum.blue += -((int) p1.blue);

    }

    // assign kernel's resultmy to pixel at [i,j]
    //assign_sum_to_pixel(&current_pixel, sum, kernelScale);
    //*** move the function sum_pixels_by_weight directly to here
    // divide by kernel's weight
    if(kernelScale!=1) {
        sum.red = sum.red / kernelScale;
        sum.green = sum.green / kernelScale;
        sum.blue = sum.blue / kernelScale;
    }

    // truncate each pixel's color values to match the range [0,255]
    current_pixel.red = (unsigned char) (min(max(sum.red, 0), 255));
    current_pixel.green = (unsigned char) (min(max(sum.green, 0), 255));
    current_pixel.blue = (unsigned char) (min(max(sum.blue, 0), 255));

    return current_pixel;
}

/*
* Apply the kernel over each pixel.
* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than kernelSize/2 and/or
* column index smaller than kernelSize/2
*/
void smooth(int dim, pixel *src, pixel *dst, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {

    register int i, j;
    register unsigned long local_n = n;
    for (i = 1 ; i < local_n - 1; i++) { //kernelSize / 2 = 1
        for (j =  1; j < local_n - 1 ; j++) {
            dst[calcIndex(i, j, local_n)] = applyKernel(local_n, i, j, src, kernelSize, kernel, kernelScale, filter);
        }
    }
}


void doConvolution(Image *image, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {

    pixel* pixelsImg = malloc(m*n*sizeof(pixel));
    pixel* backupOrg = malloc(m*n*sizeof(pixel));

    //charsToPixels(image, pixelsImg);
    memcpy(pixelsImg, image->data, n*m*3);

    //copyPixels(pixelsImg, backupOrg);
    memcpy(backupOrg, pixelsImg, n*m*3);


    smooth(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale, filter);

    //pixelsToChars(pixelsImg, image);
    memcpy(image->data, pixelsImg, n*m*3);


    free(pixelsImg);
    free(backupOrg);
}

void doConvolution1(Image *image, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {
    pixel* pixelsImg = malloc(m*n*sizeof(pixel));
    pixel* backupOrg = malloc(m*n*sizeof(pixel));

    memcpy(pixelsImg, image->data, n*m*3);

    memcpy(backupOrg, pixelsImg, n*m*3);

    smooth(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale, filter);

    //pixelsToChars(pixelsImg, image);
    memcpy(image->data, pixelsImg, n*m*3);

    free(pixelsImg);
    free(backupOrg);
}

void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {

    /*
    * [1, 1, 1]
    * [1, 1, 1]
    * [1, 1, 1]
    */
    int blurKernel[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

    /*
    * [-1, -1, -1]
    * [-1, 9, -1]
    * [-1, -1, -1]
    */
    int sharpKernel[3][3] = {{-1,-1,-1},{-1,9,-1},{-1,-1,-1}};

    if (flag == '1') {
        // blur image
        doConvolution(image, 3, blurKernel, 9, false);

        // write result image to file
        writeBMP(image, srcImgpName, blurRsltImgName);

        // sharpen the resulting image
        doConvolution(image, 3, sharpKernel, 1, false);

        // write result image to file
        writeBMP(image, srcImgpName, sharpRsltImgName);
    } else {
        // apply extermum filtered kernel to blur image
        doConvolution(image, 3, blurKernel, 7, true);

        // write result image to file
        writeBMP(image, srcImgpName, filteredBlurRsltImgName);

        // sharpen the resulting image
        doConvolution(image, 3, sharpKernel, 1, false);

        // write result image to file
        writeBMP(image, srcImgpName, filteredSharpRsltImgName);
    }
}
