/*
    Name: Armin Bazarjani
    USC ID: 4430621961
    USC Email: bazarjan@usc.edu
    Submission Date: 2-23-21
    
    cpp file for Problem 2b (Error Diffusion)
    
    There are three implementations we were tasked with:
      1. Floyd-Steinberg
      2. Jarvis, Judice, and Ninke
      3. Stucki
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

///    Declare Global Variables    ///
const double fixed_threshold = 127.0;  // threshold for comparing, chan change if you desire

int bridge_height = 501;
int bridge_width = 332;
int bridge_channels = 1;

// define normal and mirrored floyd-steinberg matrices
const double fs_normal[3][3] = {
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 7.0},
    {3.0, 5.0, 1.0}
};

unsigned char test[5][5][1] = {
    {{128}, {128}, {128}, {128}, {128}},
    {{128}, {128}, {128}, {128}, {128}},
    {{128}, {128}, {128}, {128}, {128}},
    {{128}, {128}, {128}, {128}, {128}},
    {{128}, {128}, {128}, {128}, {128}}
};

const double fs_mirrored[3][3] = {
    {0.0, 0.0, 0.0},
    {7.0, 0.0, 0.0},
    {1.0, 5.0, 3.0}
};

// define normal and mirrored JJN matrices
const double jjn_normal[5][5] = {
    {0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 7.0, 5.0},
    {3.0, 5.0, 7.0, 5.0, 3.0},
    {1.0, 3.0, 5.0, 3.0, 1.0}
};

const double jjn_mirrored[5][5] = {
    {0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0},
    {5.0, 7.0, 0.0, 0.0, 0.0},
    {3.0, 5.0, 7.0, 5.0, 3.0},
    {1.0, 3.0, 5.0, 3.0, 1.0}
};

// define normal and mirrored stucki matrices
const double stucki_normal[5][5] = {
    {0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 8.0, 4.0},
    {2.0, 4.0, 8.0, 4.0, 2.0},
    {1.0, 2.0, 4.0, 2.0, 1.0}
};

const double stucki_mirrored[5][5] = {
    {0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0},
    {4.0, 8.0, 0.0, 0.0, 0.0},
    {2.0, 4.0, 8.0, 4.0, 2.0},
    {1.0, 2.0, 4.0, 2.0, 1.0}
};

///    Useful Helper Functions    ///
void print_commands(){
    cout << "The usage is as follows:" << endl;
    cout << "./executable command_1 command_2 command_3" << endl;
    cout << "where:" << endl;
    cout << "command_1 is the input path to the raw file" << endl;
    cout << "command_2 is the desired output path for the half-toned image" << endl;
    cout << "command_3 is the method (fs, jjn, or stucki)" << endl;
}

//  Function to make sure everything becomes a valid unsigned char
unsigned char round_and_clip(double value) {
	int round_val = (int)round(value);
	if (round_val > 0xFF) {
		return 0xFF;
	} 
    else if (round_val < 0x00) {
		return 0x00;
	} 
    else {
		return (unsigned char)round_val;
	}
}

// Function to zero pad the data
template<size_t in_height, size_t in_width, size_t size_channels, size_t height, size_t width>
void zero_pad(double (&output)[height][width][size_channels], unsigned char (&input)[in_height][in_width][size_channels], int pad_size){
    // start by filling in the original areas
    for (int i = 0; i < in_height; i++){
        for (int j = 0; j < in_width; j++){
            output[i+pad_size][j+pad_size][0] = (double) input[i][j][0];
            double test = (double) input[i][j][0];
        }
    }
    // top
    for (int i = 0; i < pad_size; i++){
        for (int j = 0; j < in_width + 2 * pad_size; j++){
            output[i][j][0] = 0.0;
        }
    }
    // bottom
    for (int i = in_height + pad_size; i < in_height + 2*pad_size; i++){
        for (int j = 0; j < in_width + 2*pad_size; j++){
            output[i][j][0] = 0.0;
        }
    }
    // left
    for (int i = 0; i < in_height; i++){
        for (int j = 0; j < pad_size; j++){
            output[i+pad_size][j][0] = 0.0;
        }
    }
    // right
    for (int i = 0; i < in_height; i++){
        for (int j = in_width + pad_size; j < in_width + 2*pad_size; j++){
            output[i+pad_size][j][0] = 0.0;
        }
    }
}

// Function to assist with operation with matrix within error diffusion
template<size_t height_pad, size_t width_pad, size_t channels, size_t matrix_size>
void diffuse(double (&padded_image)[height_pad][width_pad][channels], const double (&matrix)[matrix_size][matrix_size],int row, int col, int offset, bool mirror, double divisor){
    // calculate the offset to know how far we have to move within padded image
    double error = 0.0;
    // calculate error and update the current value
    if (mirror == false){
        if (padded_image[row+offset][col+offset][0] >= fixed_threshold){
            error = (double) padded_image[row+offset][col+offset][0]-255.0;
            padded_image[row+offset][col+offset][0] = 255.0;
        }
        else{
            error = (double) padded_image[row+offset][col+offset][0]-0.0;
            padded_image[row+offset][col+offset][0] = 0.0;
        }
    }
    else{
        if (padded_image[row-offset][col-offset][0] >= fixed_threshold){
            error = (double) padded_image[row-offset][col-offset][0]-255.0;
            padded_image[row-offset][col-offset][0] = 255.0;        
        }
        else{
            error = (double) padded_image[row-offset][col-offset][0]-0.0;
            padded_image[row-offset][col-offset][0] = 0.0;
        }
    }
    // check if we are mirroring or not
    if (mirror == false){
        int matrix_i = 0;
        int matrix_j = 0;
        // if not do everything normally
        for (int i = row; i < row+matrix_size; i++){
            for (int j = col; j < col+matrix_size; j++){
                padded_image[i][j][0] += (divisor*matrix[matrix_i][matrix_j]*error);
                matrix_j += 1;
            }
            matrix_i += 1;
            matrix_j = 0;
        }
    }
    else{
        int matrix_i = 0;
        int matrix_j = matrix_size-1;
        for (int i = row; i < row+matrix_size; i++){
            for (int j = col; j > col-matrix_size; j--){
                padded_image[i][j][0] += (divisor*matrix[matrix_i][matrix_j]*error);
                matrix_j -= 1;
            }
            matrix_i += 1;
            matrix_j = matrix_size-1;
        }
    }
}

// Function to perform error diffusion
template<size_t height, size_t width, size_t channels, size_t height_pad, size_t width_pad, size_t matrix_size>
void error_diffuse(double (&padded_image)[height_pad][width_pad][channels], unsigned char (&output)[height][width][channels], const double (&mat_normal)[matrix_size][matrix_size], const double (&mat_mirror)[matrix_size][matrix_size], int pad_size, double divisor, char * output_path){
    // serpentine loop through original image
    for(int i = 0; i <= height_pad-matrix_size; i++){
        // if we are on an even row go from left to right
        if (i%2 == 0){
            for (int j = 0; j <= width_pad-matrix_size; j++){
                // diffuse with normal matrix
                diffuse(padded_image, mat_normal, i, j, pad_size, false, divisor); 
            }
        }
        // else we are on an odd row and we go from right to left
        else{
            for (int j = width_pad-1; j >= matrix_size-1; j--){
                // diffuse with mirrored matrix
                diffuse(padded_image, mat_mirror, i, j, pad_size, true, divisor);
            }
        }
    }

    // copy the padded image to the output
    // for (int row = 0; row < height; row++){
    //     for (int col = 0; col < width; col++){
    //         cout << padded_image[row+pad_size][col+pad_size][0] << " ";
    //         output[row][col][0] = padded_image[row+pad_size][col+pad_size][0];
    //     }
    //     cout << endl;
    // }
    for (int row = 0; row < height_pad; row++){
        for (int col = 0; col < width_pad; col++){
            cout << padded_image[row][col][0] << " ";
            //output[row][col][0] = padded_image[row+pad_size][col+pad_size][0];
        }
        cout << endl;
    }

    // write out the new image
    //write_out(output, output_path, bridge_height, bridge_width, bridge_channels);
}

///    Main Program    ///
int main(int argc, char * argv[]){
    if (argc < 3){
        cout << "incorrect parameter usage, printing out commands..." << endl;
        print_commands();
        return 0;
    }

    // read in the Bridge image
    unsigned char bridge_data[501][332][1];
    read_in(bridge_data, argv[1], bridge_height, bridge_width, bridge_channels);

    // construct output bridge
    unsigned char bridge_output[501][332][1];

    unsigned char test_output[5][5][1];

    if (strcmp(argv[3],"fs")==0){
        cout << "performing Floyd-Steinberg error diffusion..." << endl;
        // perform zero padding
        //double bridge_padded[503][334][1];
        //int pad_size = 1;
        //zero_pad(bridge_padded, bridge_data, pad_size);

        // divisor
        double divisor = (1.0/16.0);

        double test_padded[7][7][1];
        zero_pad(test_padded, test, 1);
        error_diffuse(test_padded, test_output, fs_normal, fs_mirrored, 1, divisor, argv[2]);

        // call error diffusion function
        //error_diffuse(bridge_padded, bridge_output, fs_normal, fs_mirrored, pad_size, divisor, argv[2]);
    }
    else if (strcmp(argv[3],"jjn")==0){
        cout << "performing Jarvis, Judice, and Ninke error diffusion..." << endl;
        // perform zero padding
        double bridge_padded[505][336][1];
        int pad_size = 2;
        zero_pad(bridge_padded, bridge_data, pad_size);

        // divisor
        double divisor = (1.0/48.0);

        // call error diffusion function
        error_diffuse(bridge_padded, bridge_output, jjn_normal, jjn_mirrored, pad_size, divisor, argv[2]);
    }
    else if (strcmp(argv[3],"stucki")==0){
        cout << "performing Stucki error diffusion..." << endl;
        // perform zero padding
        double bridge_padded[505][336][1];
        int pad_size = 2;
        zero_pad(bridge_padded, bridge_data, pad_size);

        // divisor
        double divisor = (1.0/42.0);

        // call error diffusion function
        error_diffuse(bridge_padded, bridge_output, stucki_normal, stucki_mirrored, pad_size, divisor, argv[2]);
    }
    else{
        cout << "Incorrect paramtere usage, printing out commands..." << endl;
        print_commands();
        return 0;
    }
}