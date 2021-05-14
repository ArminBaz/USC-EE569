/*
    Name: Armin Bazarjani
    USC ID: 4430621961
    USC Email: bazarjan@usc.edu
    Submission Date: 2-23-21
    
    cpp file for Problem 3-part 2 (MBVQ-Based Error Diffusion)

    *NOTE* you must compile with -std=c++11
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

///    Global Parameters    ///
int fish_height = 426;
int fish_width = 640;
int fish_channels = 3;
const double fixed_threshold = 128.0;

// define normal and mirrored floyd-steinberg matrices
const double fs_normal[3][3] = {
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 7.0},
    {3.0, 5.0, 1.0}
};

const double fs_mirrored[3][3] = {
    {0.0, 0.0, 0.0},
    {7.0, 0.0, 0.0},
    {1.0, 5.0, 3.0}
};

// enum for mbvq quadrants
enum class quadruple {
	KRGB,
	CMYW,
	MYGC,
	RGMY,
	RGBM,
	CMGB
};

// struct for returning vector of vertex
struct vertex{
    vector<double> red{255.0, 0.0, 0.0};
    vector<double> green{0.0, 255.0, 0.0};
    vector<double> blue{0.0, 0.0, 255.0};
    vector<double> yellow{255.0, 255.0, 0.0};
    vector<double> white{255.0, 255.0, 255.0};
    vector<double> cyan{0.0, 255.0, 255.0};
    vector<double> magenta{255.0, 0.0, 255.0};
    vector<double> black{0.0, 0.0, 0.0};
};

///    Useful Helper Functions    ///
// Function to print out usage
void print_commands(){
    cout << "The usage is as follows:" << endl;
    cout << "./executable command_1 command_2" << endl;
    cout << "where:" << endl;
    cout << "command_1 is the input path to the raw file" << endl;
    cout << "command_2 is the desired output path for the half-toned image" << endl;
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
template<size_t size_x, size_t size_y, size_t size_channels, size_t height, size_t width>
void zero_pad(double (&output)[height][width][size_channels], unsigned char (&input)[size_x][size_y][size_channels], int pad_size){
    // start by filling in the original areas
    for (int i = 0; i < fish_height; i++){
        for (int j = 0; j < fish_width; j++){
            for (int k = 0; k < size_channels; k++){
                output[i+pad_size][j+pad_size][k] = (double) input[i][j][k];
            }
        }
    }
    // top
    for (int i = 0; i < pad_size; i++){
        for (int j = 0; j < fish_width + 2 * pad_size; j++){
            for (int k = 0; k < size_channels; k++){
                output[i][j][k] = 0.0;
            }
        }
    }
    // bottom
    for (int i = fish_height + pad_size; i < fish_height + 2*pad_size; i++){
        for (int j = 0; j < fish_width + 2*pad_size; j++){
            for (int k = 0; k < size_channels; k++){
                output[i][j][k] = 0.0;
            }
        }
    }
    // left
    for (int i = 0; i < fish_height; i++){
        for (int j = 0; j < pad_size; j++){
            for (int k = 0; k < size_channels; k++){
                output[i+pad_size][j][k] = 0.0;
            }
        }
    }
    // right
    for (int i = 0; i < fish_height; i++){
        for (int j = fish_width + pad_size; j < fish_width + 2*pad_size; j++){
            for (int k =0; k < size_channels; k++){
                output[i+pad_size][j][k] = 0.0;
            }
        }
    }
}

// Function to assist with operation with matrix within error diffusion
template<size_t height_pad, size_t width_pad, size_t channels, size_t matrix_size>
void diffuse(double (&padded_image)[height_pad][width_pad][channels], const double (&matrix)[matrix_size][matrix_size],int row, int col, int offset, bool mirror, double divisor, int channel, double vertex_val){
    // calculate error
    double error = (double) padded_image[row+offset][col+offset][channel]-vertex_val;
    // update value
    padded_image[row+offset][col+offset][channel] = vertex_val;
    // distribute the error
    if (mirror == false){
        int matrix_i = 0;
        int matrix_j = 0;
        for (int i = row; i < row+matrix_size; i++){
            for (int j = col; j < col+matrix_size; j++){
                padded_image[i][j][channel] += (divisor*matrix[matrix_i][matrix_j]*error);
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
                padded_image[i][j][channel] += (divisor*matrix[matrix_i][matrix_j]*error);
                matrix_j -= 1;
            }
            matrix_i += 1;
            matrix_j = matrix_size-1;
        }
    }
}

// Function to find mbvq quadrant : returns a string that tells us the quadrant
quadruple mbvq_quad(double R, double G, double B){
    if((R+G) > 255){
        if((G+B) > 255){
            if((R+G+B) > 510){
                return quadruple::CMYW;
            }
            else{
                return quadruple::MYGC;
            }
        }
        else{
            return quadruple::RGMY;
        }
    }
    else{
        if ((G+B) <= 255){
            if((R+G+B) <= 255){
                return quadruple::KRGB;
            }
            else{
                return quadruple::RGBM;
            }
        }
        else{
            return quadruple::CMGB;
        }
    }
}

// Function to find the nearest vertex : returns a vector of size (3) corresponding to RGB values
vector<double> mbvq_vertex(quadruple mbvq, double R, double G, double B){
    vertex v;
    switch(mbvq){
        case quadruple::CMYW:
            if (B < fixed_threshold){
                if (B < R){
                    if (B <= G){
                        return v.yellow;
                    }
                }
            }
            if (G < fixed_threshold){
                if (G <= B){
                    if (G <= R){
                        return v.magenta;
                    }
                }
            }
            if (R < fixed_threshold){
                if (R <= B){
                    if (R <= G){
                        return v.cyan;
                    }
                }
            }
            return v.white;

        case quadruple::MYGC:
            if (G >= B){
                if (R >= B){
                    if (R >= fixed_threshold){
                        return v.yellow;
                    }
                    else{
                        return v.green;
                    }
                }
            }
            if (G >= R){
                if (B >= R){
                    if (B >= fixed_threshold){
                        return v.cyan;
                    }
                    else{
                        return v.green;
                    }
                }
            }
            return v.magenta;

        case quadruple::RGMY:
            if (B > fixed_threshold){
                if (R > fixed_threshold){
                    if (B >= G){
                        return v.magenta;
                    }
                    else{
                        return v.yellow;
                    }
                }
                else{
                    if (G > (B+R)){
                        return v.green;
                    }
                    else{
                        return v.magenta;
                    }
                }
            }
            else{
                if (R >= fixed_threshold){
                    if (G >= fixed_threshold){
                        return v.yellow;
                    }
                    else{
                        return v.red;
                    }
                }
                else{
                    if (R >= G){
                        return v.red;
                    }
                    else{
                        return v.green;
                    }
                }
            }
        
        case quadruple::KRGB:
            if (B > fixed_threshold){
                if (B >= R){
                    if (B >= G){
                        return v.blue;
                    }
                }
            }
            if (G > fixed_threshold){
                if (G >= B){
                    if (G >= R){
                        return v.green;
                    }
                }
            }
            if (R > fixed_threshold){
                if (R >= B){
                    if (R >= G){
                        return v.red;
                    }
                }
            }
            return v.black;
        
        case quadruple::RGBM:
            if (R > G){
                if (R >= B){
                    if (B < fixed_threshold){
                        return v.red;
                    }
                    else{
                        return v.magenta;
                    }
                }
            }
            if (B > G){
                if (B >= R){
                    if (R < fixed_threshold){
                        return v.blue;
                    }
                    else{
                        return v.magenta;
                    }
                }
            }
            return v.green;

        case quadruple::CMGB:
            if (B > fixed_threshold){
                if (R > fixed_threshold){
                    if (G >= R){
                        return v.cyan;
                    }
                    else{
                        return v.magenta;
                    }
                }
                else{
                    if (G > fixed_threshold){
                        return v.cyan;
                    }
                    else{
                        return v.blue;
                    }
                }
            }
            else{
                if (R > fixed_threshold){
                    if ((R-G+B) >= fixed_threshold){
                        return v.magenta;
                    }
                    else{
                        return v.green;
                    }
                }
                else{
                    if (G >= B){
                        return v.green;
                    }
                    else{
                        return v.blue;
                    }
                }
            }
    }
}


// Function to perform mbvq error diffusion
template<size_t height, size_t width, size_t channels, size_t height_pad, size_t width_pad, size_t matrix_size>
void mbvq_error_diff(double (&padded_image)[height_pad][width_pad][channels], unsigned char (&output)[height][width][channels], const double (&mat_normal)[matrix_size][matrix_size], const double (&mat_mirror)[matrix_size][matrix_size], int pad_size, double divisor, char * output_path){
    // serpentine loop through original image
    for(int i = 0; i <= height_pad-matrix_size; i++){
        // if we are on an even row go from left to right
        if (i%2 == 0){
            for (int j = 0; j <= width_pad-matrix_size; j++){
                // define R, G, B values in original image location (for code readability)
                double R = padded_image[i+pad_size][j+pad_size][0];
                double G = padded_image[i+pad_size][j+pad_size][1];
                double B = padded_image[i+pad_size][j+pad_size][2];
                // find the mbvq quadrant
                quadruple quad = mbvq_quad(R, G, B);
                // find the vertex from mbvq quadrant
                vector<double> vert = mbvq_vertex(quad, R, G, B);
                // define vertex values (for code readability)
                double v_r = vert[0];
                double v_g = vert[1];
                double v_b = vert[2];
                // diffuse with normal matrix on each channel
                diffuse(padded_image, mat_normal, i, j, pad_size, false, divisor, 0, v_r);
                diffuse(padded_image, mat_normal, i, j, pad_size, false, divisor, 1, v_g);
                diffuse(padded_image, mat_normal, i, j, pad_size, false, divisor, 2, v_b);
            }   
        }
        // else we are on an odd row and we go from right to left
        else{
            for (int j = width_pad; j >= matrix_size-1; j--){
                // define R, G, B values in original image location (for code readability)
                double R = padded_image[i+pad_size][j+pad_size][0];
                double G = padded_image[i+pad_size][j+pad_size][1];
                double B = padded_image[i+pad_size][j+pad_size][2];
                // find the mbvq quadrant
                quadruple quad = mbvq_quad(R, G, B);
                // find the vertex from mbvq quadrant
                vector<double> vert = mbvq_vertex(quad, R, G, B);
                // define vertex values (for code readability)
                double v_r = vert[0];
                double v_g = vert[1];
                double v_b = vert[2];
                // diffuse with mirrored matrix
                diffuse(padded_image, mat_mirror, i, j, pad_size, true, divisor, 0, v_r);
                diffuse(padded_image, mat_mirror, i, j, pad_size, true, divisor, 1, v_g);
                diffuse(padded_image, mat_mirror, i, j, pad_size, true, divisor, 2, v_b);
            }
        }
    }

    // copy the padded image to the output
    for (int row = 0; row < height; row++){
        for (int col = 0; col < width; col++){
            for (int channel = 0; channel < channels; channel++){
                output[row][col][channel] = round_and_clip(padded_image[row+pad_size][col+pad_size][channel]);
            }
        }
    }

    // write out the new image
    write_out(output, output_path, fish_height, fish_width, fish_channels);
}

///    Main Function    ///
int main(int argc, char * argv[]){
    if (argc < 2){
        cout << "incorrect parameter usage, printing out commands..." << endl;
        print_commands();
        return 0;
    }
    cout << "Running MBVQ-Based Error Diffusion..." << endl;
    // read in fish image
    unsigned char fish_data[426][640][3];
    read_in(fish_data, argv[1], fish_height, fish_width, fish_channels);

    // pad image
    double fish_padded[428][642][3];
    zero_pad(fish_padded, fish_data, 1);

    // divisor
    double divisor = (1.0/16.0);

    // construct output holder
    unsigned char fish_output[426][640][3];

    // call mbvq error diffusion function
    mbvq_error_diff(fish_padded, fish_output, fs_normal, fs_mirrored, 1, divisor, argv[2]);
}