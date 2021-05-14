/*
    Name: Armin Bazarjani
    USC ID: 4430621961
    USC Email: bazarjan@usc.edu
    Submission Date: 2-23-21
    
    cpp file for Problem 1a (Sobel Edge Detection)
*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <cmath>
#include <cstring>
#include "readwrite.h"

using namespace std;

///    Global Constants    ///
int ELEPHANT_HEIGHT = 321;
int ELEPHANT_WIDTH = 481;
int ELEPHANT_CHANNELS = 3;

int SKI_HEIGHT = 481;
int SKI_WIDTH = 321;
int SKI_CHANNELS = 3;

const int uchar_max = 0xFF;
const int uchar_min = 0x00;

const double sobel_X[3][3] = {
    {-1.00, 0.00, 1.00},
    {-2.00, 0.00, 2.00},
    {-1.00, 0.00, 1.00}
};

const double sobel_Y[3][3] = {
    {1.00, 2.00, 1.00},
    {0.00, 0.00, 0.00},
    {-1.00, -2.00, -1.00}
};

char * ski_grad_path = (char *)"/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/problem1/1a/ski_grad.raw";
char * ski_txt_path = (char *)"/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/problem1/1a/ski_sobel.txt";
char * ski_xgrad_path = (char*)"/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/problem1/1a/ski_grad_X.raw";
char * ski_ygrad_path = (char*)"/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/problem1/1a/ski_grad_Y.raw";


///    Useful Helper Functions    ///
void print_commands(){
    cout << "The usage is as follows:" << endl;
    cout << "./executable command_1 command_2 command_3" << endl;
    cout << "where:" << endl;
    cout << "command_1 is the input path to the raw file" << endl;
    cout << "command_2 is the desired output path for the edge detected image" << endl;
    cout << "command_3 is the threshold" << endl;
}

//  Function to make sure everything becomes a valid unsigned char
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

// function to convert image to grayscale
template <size_t height, size_t width, size_t size_channels1, size_t size_channels2>
void convert_grayscale(unsigned char (&image_color)[height][width][size_channels1], unsigned char (&image_gray)[height][width][size_channels2]){
    // loop through the original image and fill the gray image
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            image_gray[i][j][0] = (0.2989*image_color[i][j][0]) + (0.5870*image_color[i][j][1]) + (0.1440*image_color[i][j][2]);
        }
    }
}

// Function to zero pad the data
template<size_t size_x1, size_t size_y1, size_t size_x2, size_t size_y2, size_t size_channels>
void zero_pad(unsigned char (&image_padded)[size_x1][size_y1][size_channels], unsigned char (&image_data)[size_x2][size_y2][size_channels], int height, int width , int pad_size){
    // start by filling in the original areas
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            image_padded[i+pad_size][j+pad_size][0] = image_data[i][j][0];
        }
    }
    // top
    for (int i = 0; i < pad_size; i++){
        for (int j = 0; j < width + 2 * pad_size; j++){
            image_padded[i][j][0] = 0x00;
        }
    }
    // bottom
    for (int i = height + pad_size; i < height + 2*pad_size; i++){
        for (int j = 0; j < width + 2*pad_size; j++){
            image_padded[i][j][0] = 0x00;
        }
    }
    // left
    for (int i = 0; i < height; i++){
        for (int j = 0; j < pad_size; j++){
            image_padded[i+pad_size][j][0] = 0x00;
        }
    }
    // right
    for (int i = 0; i < height; i++){
        for (int j = width + pad_size; j < width + 2*pad_size; j++){
            image_padded[i+pad_size][j][0] = 0x00;
        }
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

//  Function to apply sobel filter to an image
template <size_t height_pad, size_t width_pad, size_t channels, size_t height, size_t width>
void sobel_filter(unsigned char (&input)[height_pad][width_pad][channels], unsigned char (&outputX)[height][width][channels], unsigned char (&outputY)[height][width][channels]){
    for (int row = 0; row <= height_pad-3; row++){
        for (int col = 0; col <= width_pad-3; col++){
            // calculate the x and y gradients respectively and take absolute value
            outputX[row][col][0] = round_and_clip(0.25*convolution(input, row, col, 3, 3, sobel_X));
            outputY[row][col][0] = round_and_clip(0.25*convolution(input, row, col, 3, 3, sobel_Y));
        }
    }
}

// Function to construct a magnitude map from the gradient outputs of the sobel filter
template <size_t height, size_t width, size_t channels>
void magnitude_map(unsigned char (&grad_X)[height][width][channels], unsigned char (&grad_Y)[height][width][channels], unsigned char (&output)[height][width][channels]){
    for (int row = 0; row < height; row++){
        for (int col = 0; col < width; col++){
            output[row][col][0] = round_and_clip(sqrt(pow(grad_X[row][col][0],2) + pow(grad_Y[row][col][0],2)));
        }
    }
}

// Function meant to threshold the magnitude map for a certain percentage (percentage is out of 255)
template <size_t height, size_t width, size_t channels>
void threshold(unsigned char (&input)[height][width][channels], unsigned char (&output)[height][width][channels], double threshold){
    unsigned char thresh_value = threshold*0xFF;
    // loop through input and threshold
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (input[i][j][0] >= thresh_value){
                output[i][j][0] = 0xFF;
            }
            else{
                output[i][j][0] = 0x00;
            }
        }
    }
}

///    MAIN FUNCTION    ///
int main(int argc, char * argv[]){
    // read in
    unsigned char ski_color[481][321][3];
    read_in(ski_color, argv[1], SKI_HEIGHT, SKI_WIDTH, 3);
    // convert to grayscale
    unsigned char ski_gray[481][321][1];
    convert_grayscale(ski_color, ski_gray);
    // zero pad
    unsigned char ski_padded[483][323][1];
    zero_pad(ski_padded, ski_gray, 481, 321,1);
    // convolve with sobel filter to get magnitudes in X and Y direction
    unsigned char ski_edge_X[481][321][1];
    unsigned char ski_edge_Y[481][321][1];
    sobel_filter(ski_padded, ski_edge_X, ski_edge_Y);
    // construct magnitude map and normalize
    unsigned char ski_grad[481][321][1];
    magnitude_map(ski_edge_X, ski_edge_Y, ski_grad);
    cout << "writing out X grad..." << endl;
    write_out(ski_edge_X, ski_xgrad_path, SKI_HEIGHT, SKI_WIDTH, SKI_CHANNELS);
    cout << "writing out Y grad..." << endl;
    write_out(ski_edge_Y, ski_ygrad_path, SKI_HEIGHT, SKI_WIDTH, SKI_CHANNELS);
    // write out the magnitude map to both raw and txt files
    cout << "writing out magnitude map to raw for ski person..." << endl;
    write_out(ski_grad, ski_grad_path, SKI_HEIGHT, SKI_WIDTH, SKI_CHANNELS);
    write_out_txt(ski_grad, ski_txt_path);
    // threshold the image
    unsigned char ski_edges[481][321][1];
    threshold(ski_grad, ski_edges, atof(argv[3]));
    // writeout
    cout << "writing out edge map for skier with a threshold of "<<atof(argv[3])<<"..."<< endl;
    write_out(ski_edges, argv[2], SKI_HEIGHT, SKI_WIDTH, 1);
}
