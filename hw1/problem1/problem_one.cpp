/*
    cpp file for problem 1 (Image Demosaicing and Histogram Equalization), this includes:
    - bilinear demosaicing function to be used on House.raw
    - histogram equalization using the transfer function based method on Toy.raw
    - histogram equalization using the cumulative probability based method on Toy.raw

*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <cmath>
#include "readwrite.h"

using namespace std;

//  declare universal variables  //
const int bilinear_height = 3;
const int bilinear_width = 3;
const int uchar_max = 0xFF;
const int uchar_min = 0x00;


//  filters for bilinear demosaicing  //
// diagonal filter to be used on blue to get red or red to get blue
const double diagonal_filter[bilinear_height][bilinear_width] = {
    {0.25, 0.00, 0.25},
    {0.00, 0.00, 0.00},
    {0.25, 0.00, 0.25}
};
// adjacent filter used on blue/red to get green
const double adjacent_filter[bilinear_height][bilinear_width] = {
    {0.00, 0.25, 0.00},
    {0.25, 0.00, 0.25},
    {0.00, 0.25, 0.00}
};
// vertical filter used on green to get either blue or red
const double vertical_filter[bilinear_height][bilinear_width] = {
    {0.00, 0.50, 0.00},
    {0.00, 0.00, 0.00},
    {0.00, 0.50, 0.00}
};
// horizontal filter used on green to get either blue or red
const double horizontal_filter[bilinear_height][bilinear_width] = {
    {0.00, 0.00, 0.00},
    {0.50, 0.00, 0.50},
    {0.00, 0.00, 0.00}
};


//  Helper Functions  //
void print_commands(){
    cout << "Commands for this program are as follows:" << endl;
    cout << "problem (A or B), input_file (path to image for problem), output_file (desired name and path for output file)" << endl;
    cout << "if you want to run problem B, you need to include the subtype after problem and before the input_file. The subtype is either transfer or bins." << endl;
}

//  Function to make sure everything becomes a valid unsigned char  //
unsigned char round_and_clip(const double value) {
	int round_val = (int)round(value);
	if (round_val > uchar_max) {
		return uchar_max;
	} 
    else if (round_val < uchar_min) {
		return uchar_min;
	} 
    else {
		return (unsigned char)round_val;
	}
}



//  Convolution function, made inline as a way to hopefully speed up the program execution time  //
template <size_t size_x1, size_t size_y1, size_t size_channels,size_t size_x2, size_t size_y2>
inline double convolution(unsigned char (&image_data)[size_x1][size_y1][size_channels], int row, int col, const int filter_height, const int filter_width, const double (&filter)[size_x2][size_y2]){
    // intialize sum and filter location
    double out = 0;
    int filter_i = 0;
    int filter_j = 0;

    // loop through the desired area of the image
    for (int i = row; i < row + filter_height; i++){
        for (int j = col; j < col + filter_width; j++){
            // multiply
            double mult = image_data[i][j][0] * filter[filter_i][filter_j];
            // accumulate
            out += mult;
            // increment filter width location
            filter_j += 1;
        }
        // increment filter height location and set width back to 0
        filter_i += 1;
        filter_j = 0;
    }
    
    // return the multiply accumulate operation
    return out;
}


// Function that takes an image as input and returns the histogram of pixel frequencies
template <size_t size_x, size_t size_y, size_t channels>
void calc_hist(unsigned char (&image_data)[size_x][size_y][channels], double hist[256]){
    // total number of pixels
    double total_pixels = size_x * size_y;

    // loop through image data and fill in histogram
    for (int i = 0; i < size_x; i++){
        for (int j = 0; j < size_y; j++){
            hist[image_data[i][j][0]] += 1;
        }
    }

    // normalize the histogram values
    for (int k = 0; k <= 255; k++){
        hist[k] /= total_pixels;
    }
}


template <size_t size_x, size_t size_y, size_t channels>
void calc_hist_bin(unsigned char (&image_data)[size_x][size_y][channels], vector< pair <int,int> > hist[256]){
    // loop through image data and fill in histogram with location of pixel with that value
    for (int i = 0; i < size_x; i++){
        for (int j = 0; j < size_y; j++){
            hist[image_data[i][j][0]].push_back(make_pair(i, j));
        }
    }
}


// Function that takes a frequency histogram as input and returns the cdf
void calc_cdf(double hist[256], double cdf[256]){
    // fill cdf with first value to build off of
    cdf[0] = hist[0];
    // loop through and fill cdf based off previous cdf value and current histogram value
    for (int i = 1; i <= 255; i++){
        cdf[i] = cdf[i-1] + hist[i];
    }
}



/*
    Function for part a of problem 1

    Part a consists of bilinear demosaicing on the house.raw image. I chose to implement this through
    convolutions with different filters depending on where we are within the image.
*/
void a(char* input_file, char * output_file){
    // pre-defining height, width, and channels for the house image
    int height = 440;
    int width = 580;
    int channels = 1;
    //  start by reading in the input file  //
    unsigned char image_data[440][580][1];
    read_in(image_data, input_file, height, width, channels);

    //  mirror the image  //
    const int mirror_height = 442;
    const int mirror_width = 582;

    unsigned char image_mirror[mirror_height][mirror_width][1];  // holder for mirrored image
    // (1) fill in the normal areas
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            image_mirror[i+1][j+1][0] = image_data[i][j][0];
        }
    }
    // (2) fill the top and bottom
    for (int i = 0; i < width; i++){
        // top
        image_mirror[0][i+1][0] = image_data[0][i][0];

        // bottom
        image_mirror[mirror_height-1][i+1][0] = image_data[height][i][0];
    }
    // (3) fill the left and rightmost columns using the mirrored image
    for (int i = 0; i < height+2; i++){
        // left
        image_mirror[i][0][0] = image_mirror[i][1][0];


        // right
        image_mirror[i][mirror_width-1][0] = image_mirror[i][mirror_width-2][0];
    }

    //  Apply filters on mirrored image using convolutions to get demosaiced image  //
    unsigned char image_demosaiced[440][580][3];  // holder for demosaiced image (now there are 3 channels instead of 1)

    // loop through the mirrored image, stopping early on height and width because we have to fit the filter
    for (int row = 0; row <= mirror_height-bilinear_height; row++){
        for (int col = 0; col <= mirror_width-bilinear_width; col++){
            // take modulus of row and col to check if even or odd
            int row_mod = row % 2;
            int col_mod = col % 2;
            // even row even col (green with blue on vertical and red on horizontal)
            if (row_mod == 0 && col_mod == 0){
                image_demosaiced[row][col][0] = round_and_clip(convolution(image_mirror, row, col, bilinear_height, bilinear_width, horizontal_filter)); // R
                image_demosaiced[row][col][1] = image_data[row][col][0]; // G
                image_demosaiced[row][col][2] = round_and_clip(convolution(image_mirror, row, col, bilinear_height, bilinear_width, vertical_filter)); // B
            }
            // even row odd col (red with blue on diagonals and green on adjacent)
            else if (row_mod == 0 && col_mod == 1){
                image_demosaiced[row][col][0] = image_data[row][col][0]; // R
                image_demosaiced[row][col][1] = round_and_clip(convolution(image_mirror, row, col, bilinear_height, bilinear_width, adjacent_filter)); // G
                image_demosaiced[row][col][2] = round_and_clip(convolution(image_mirror, row, col, bilinear_height, bilinear_width, diagonal_filter)); // B
            }
            // odd row even col (blue with red on diagonals and green on adjacent)
            else if (row_mod == 1 && col_mod == 0){
                image_demosaiced[row][col][0] = round_and_clip(convolution(image_mirror, row, col, bilinear_height, bilinear_width, diagonal_filter)); // R
                image_demosaiced[row][col][1] = round_and_clip(convolution(image_mirror, row, col, bilinear_height, bilinear_width, adjacent_filter)); // G
                image_demosaiced[row][col][2] = image_data[row][col][0]; // B
            }
            // odd row odd col (green with blue on horizontal and red on vertical)
            else{
                image_demosaiced[row][col][0] = round_and_clip(convolution(image_mirror, row, col, bilinear_height, bilinear_width, vertical_filter)); // R
                image_demosaiced[row][col][1] = image_data[row][col][0]; // G
                image_demosaiced[row][col][2] = round_and_clip(convolution(image_mirror, row, col, bilinear_height, bilinear_width, horizontal_filter)); // B
            }
        }
    }

    //  Write out Demosaiced Image  //
    write_out(image_demosaiced, output_file, 440, 580, 3);
}

/*
    Function for part b of problem 1

    Part b has us do histogram equalization in two different ways
      1. Transfer-function-based histogram equalization
      2. cumulative-probability-based histogram equalization
    
    Both are methods of making an image more visually appealing by either brightening a dark image
    or lightening a darker image.
*/
void b(char* input_file, char* output_file, char* method){
    // pre-defining height, width, and channels for the toy image
    int height = 560;
    int width = 400;
    int channels = 1;

    //  read in the input file (toy)  //
    unsigned char toy_data[560][400][1];
    read_in(toy_data, input_file, height, width, channels);

    // create holder for denoised toy image
    unsigned char toy_denoised[height][width][channels];

    // if transfer function method
    if (strcmp(method, "transfer") == 0){
        // calculate the histogram
        double toy_hist[256] = {};
        calc_hist(toy_data, toy_hist);

        // calculate cdf
        double toy_cdf[256] = {};
        calc_cdf(toy_hist, toy_cdf);

        for (int i = 0; i < 256; i++){
            cout << i << "\t" << toy_cdf[i] << endl;
        }

        // loop through the toy image and apply transfer function
        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                toy_denoised[i][j][0] = round_and_clip(255*toy_cdf[toy_data[i][j][0]]); 
            }
        }
    }
    // if bins method
    else if (strcmp(method, "bins") == 0){
        // calculate how many bins we need
        int num_pixels = height*width;
        float num_bins = 256.0;

        // pixels per bin
        int pixels_per_bin = round(num_pixels/num_bins);

        // calculate the histogram
        vector< pair <int,int> > toy_hist[256];
        calc_hist_bin(toy_data, toy_hist);

        // holders for the current bin we are on and the number of pixels in the bin
        int curr_bin_value = 0;
        int curr_bin_num_pixels = 0;

        // loop through each intensity value and iterate over each location pair
        for (int i = 0; i < 256; i++){
            for(int j = 0; j < toy_hist[i].size(); j++){
                // if we haven't reached the max bin size
                if (curr_bin_num_pixels != pixels_per_bin){
                    // remap the pixel location to the current bin value
                    toy_denoised[toy_hist[i][j].first][toy_hist[i][j].second][0] = (unsigned char)(curr_bin_value);
                    // add one to the current bins num pixels
                    curr_bin_num_pixels += 1;
                }
                // else if we have reached the max bin size
                else{
                    // iterate to the next bin value
                    curr_bin_value += 1;
                    // set the current bins num pixels to zero
                    curr_bin_num_pixels = 0;
                }
            }
        }
    }

    //  write out denoised image  //
    write_out(toy_denoised, output_file, 400, 560, 1);
}


int main(int argc, char *argv[]){
    if (argc < 4){
        cout << "Invalid command, printing out command usage..." << endl;
        print_commands();
        return 0;
    }
    else if (strcmp( argv[1], "A") == 0){
        cout << "Running Part A..." << endl;
        a(argv[2], argv[3]);
    }
    else if (strcmp( argv[1], "B") == 0){
        if (argc < 5){
            cout << "Invalid command, printing out command usage..." << endl;
            print_commands();
            cout << "HINT: You may have forgotten to include the method when running part B" << endl;
        }
        else{
            cout << "Running Part B..." << endl;
            b(argv[2], argv[3], argv[4]);
        }
    }
}