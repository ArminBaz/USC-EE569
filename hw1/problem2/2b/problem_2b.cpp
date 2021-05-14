/*
    Name: Armin Bazarjani
    USC ID: 4430621961
    USC Email: bazarjan@usc.edu
    Submission Date: 2-7-21

    cpp file for problem 2b (Image Denoising), this includes:
     - Bilateral filter
*/
 
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "readwrite.h"

using namespace std;

//  Universal Variables  //
const int uchar_max = 0xFF;
const int uchar_min = 0x00;
const int fruit_width = 500;
const int fruit_height = 400;
const int fruit_channels = 1;


//  Define Helper Functions  //
void print_commands(){
    cout << "Commands for this program are as follows:" << endl;
    cout << "input_file_path.raw, output_file_path.raw, neighborhood_size, sigma_s, sigma_c" << endl;
    cout << "where neighborhood_size is an odd int" << endl;
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

// Function to zero pad the data
template<size_t size_x, size_t size_y, size_t size_channels>
void zero_pad(unsigned char **fruit_padded, unsigned char (&fruit_data)[size_x][size_y][size_channels], int pad_size){
    // start by filling in the original areas
    for (int i = 0; i < fruit_height; i++){
        for (int j = 0; j < fruit_width; j++){
            fruit_padded[i+pad_size][j+pad_size] = fruit_data[i][j][0];
        }
    }
    // top
    for (int i = 0; i < pad_size; i++){
        for (int j = 0; j < fruit_width + 2 * pad_size; j++){
            fruit_padded[i][j] = 0x00;
        }
    }
    // bottom
    for (int i = fruit_height + pad_size; i < fruit_height + 2*pad_size; i++){
        for (int j = 0; j < fruit_width + 2*pad_size; j++){
            fruit_padded[i][j] = 0x00;
        }
    }
    // left
    for (int i = 0; i < fruit_height; i++){
        for (int j = 0; j < pad_size; j++){
            fruit_padded[i+pad_size][j] = 0x00;
        }
    }
    // right
    for (int i = 0; i < fruit_height; i++){
        for (int j = fruit_width + pad_size; j < fruit_width + 2*pad_size; j++){
            fruit_padded[i+pad_size][j] = 0x00;
        }
    }
}


// distance function to assist with bilateral filter weighting
inline double distance(int i, int j, int k, int l){
    return double(pow(i-k,2) + pow(j-l,2));
}

// norm squared function to assist with bilateral filter weighting
//template <size_t size_x, size_t size_y>
//inline double l2_squared(unsigned char (&image_data)[size_x][size_y], int i, int j, int k, int l){

inline double l2_squared(unsigned char **image_data, int i, int j, int k, int l){
    // because we square the l2 norm, we dont have to include the sqrt operation
    double difference = double(image_data[i][j] - image_data[k][l]);
    return double(pow(difference,2));
}


//  Weight function to be used in bilateral filtering
inline double bilateral_weight(unsigned char **image_data, int i, int j, int k, int l, double sigma_s, double sigma_c){
    double spatial = distance(i, j, k, l) / (2*pow(sigma_c,2));
    double range = l2_squared(image_data, i, j, k, l) / (2*pow(sigma_s,2));
    return double(exp(-spatial-range));
}


// applies the bilateral filter at a given point and returns the weighted output
inline double bilateral(unsigned char **fruit_padded, int x, int y, int radius, int filter_size, double sigma_s, double sigma_c){
    // get the true i and j positions
    int i = x + radius;
    int j = y + radius;

    // intialize w and sum
    double w_I = 0.0;
    double w_sum = 0.0;

    // loop through all the neighbors
    for (int k = x; k < x+filter_size; k++){
        for (int l = y; l < y+filter_size; l++){
            // make sure we're not at our i,j point
            if ((k != i) && (l != j)){
                double weight = bilateral_weight(fruit_padded, i, j, k, l, sigma_s, sigma_c);

                w_I += (weight * fruit_padded[k][l]);
                w_sum += weight;
            }
        }
    }

    return (w_I / w_sum);
}


/*
    Function for part b of problem 2

    Part b has us make a bilateral low pass filter as a means of preserving edges when we filter an image.

    The function takes as input:
     - the inout image file path
     - the output image file path
     - value of sigma_s
     - value of sigma_c
     - neighborhood radius
*/
void b(char * input_file, char * output_file, double sigma_s, double sigma_c, int radius){
    //  Read in Data  //
    unsigned char fruit_data[400][500][1];
    read_in(fruit_data, input_file, fruit_height, fruit_width, fruit_channels);

    // find out how much we have to pad
    int added_pad = 2*radius;
    int filter_size = 2*radius+1;
    int pad_height = fruit_height + added_pad;
    int pad_width = fruit_width + added_pad;

    // allocate memory for the padded array
    unsigned char **fruit_padded = new unsigned char*[pad_height];
    for(int i = 0; i < pad_height; ++i) {
        fruit_padded[i] = new unsigned char[pad_width];
    }

    // pass the padding array into the padding function
    zero_pad(fruit_padded, fruit_data, radius);

    // intialize denoised image
    unsigned char fruit_denoised[400][500][1];

    // loop through the padded array and apply the bilateral filter at each point
    for (int row = 0; row <= pad_height-filter_size; row++){
        for (int col = 0; col <= pad_width-filter_size; col++){
            fruit_denoised[row][col][0] = round_and_clip(bilateral(fruit_padded, row, col, radius, filter_size, sigma_s, sigma_c));
        }
    }

    // deallocate memory for padded array
    for(int i = 0; i < pad_height; ++i) {
        delete [] fruit_padded[i];
    }
    delete [] fruit_padded;

    ///    (5) : Write out the denoised image    ///
    write_out(fruit_denoised, output_file, fruit_height, fruit_width, fruit_channels);
}

/*
    argv[1] - input file path
    argv[2] - output file path
    argv[3] - neighbor radius
    argv[4] - sigma_s
    argv[5] - sigma_c
*/
int main(int argc, char * argv[]){
    if (argc < 5){
        cout << "Invalid command, printing out command usage..." << endl;
        cout << endl;
        print_commands();
        return 0;
    }
    int radius = atoi(argv[3]);
    double sigma_s = atof(argv[4]);
    double sigma_c = atof(argv[5]);
    cout << "bilateral filter with sigma_s: " << sigma_s << " sigma_c: " << sigma_c << " and radius: " << radius << endl;
    b(argv[1], argv[2], sigma_s, sigma_c, radius);
}