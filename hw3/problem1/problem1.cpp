/*
    cpp file for problem 1 (Geometric Image Modification), this includes:
        - Warp function to make image into a disk-shaped image
        - Inverse function to make disk-shaped image back into square image
*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <tuple>
#include <string.h>
#include <cmath>
#include "readwrite.h"

using namespace std;

///    Universal Variables    ///
int bytes_per_pixel = 3;
int image_length = 329;
int image_width = 329;


///    Helper Functions    ///
// function to print out valid commands
void print_commands(){
    cout << "The usage is as follows:" << endl;
    cout << "./executable command_1 command_2 command_3" << endl;
    cout << "where:" << endl;
    cout << "command_1 is the input path to the raw file" << endl;
    cout << "command_2 is the desired output path for the disk-shaped image" << endl;
    cout << "command_3 is the desired output path for reversed image" << endl;
}

template <size_t height, size_t width, size_t channels, size_t height_new, size_t width_new>
void forward_map(unsigned char(&input)[height][width][channels], unsigned char(&output)[height_new][width_new][channels]){
    // get the difference between the two circles radii
    int r = 464/2;
    float diff = (464.0/329.0);
    int offset = (329/2)-1;
    int offset_new = (464.0/2.0)-1.0;
    bool visited[464][464];
    // loop through the intermediate and map it to a larger circle
    for(int i = 0; i < 329; i++){
        for(int j = 0; j < 329; j++){
            for(int k = 0; k < 3; k++){
                // get the x and y coordinates and normalize from -1 to 1
                float x = (i-(offset));
                float y = (j-(offset));

                // normalize
                x /= (float)(offset);
                y /= (float)(offset);

                // use the normalized x and y values to solve for u and v
                float u = x * sqrt(1-((y*y) / 2));
                float v = y * sqrt(1-((x*x) / 2));

                // unnormalize u and v
                u = (u * offset) + offset;
                v = (v * offset) + offset;

                // use the small u and v values to find the larger circle u and v valye
                float u_new = u * diff;
                float v_new = v * diff;
                //cout << "u old: " << u << " u new: " << u_new << " v old: " << v << " v new: " << v_new << endl;

                // get pixel locations from u and v
                int i_new = (int) (u_new);
                int j_new = (int) (v_new);
                visited[i_new][j_new] = true;
                output[i_new][j_new][k] = input[i][j][k];
            }
        }
    }
    // first step
    for (int i = 0; i < 464; i++){
        for (int j = 0; j < 464; j++){
            for (int k = 0; k < 3; k++){
                // get the x and y coordinates and normalize from -1 to 1
                float x = (i-(offset_new));
                float y = (j-(offset_new));
                if (x*x + y*y > r*r){
                    continue;
                }
                if (visited[i][j] != true){
                    output[i][j][k] = output[i+1][j][k];
                }
            }
        }
    }
    // second step
    for (int i = 0; i < 464; i++){
        for (int j = 0; j < 464; j++){
            for (int k = 0; k < 3; k++){
                // get the x and y coordinates and normalize from -1 to 1
                float x = (i-(233));
                float y = (j-(233));
                if (x*x + y*y <= 233*233){
                    if (output[i][j][k] == 0){
                        output[i][j][k] = (output[i][j+1][k]+output[i][j-1][k])/2;
                    }
                }
            }
        }
    }
}

template <size_t height, size_t width, size_t channels>
void reverse_map(unsigned char(&input)[height][width][channels], unsigned char(&output)[height][width][channels]){
    int offset = (329/2)-1;
    for(int i = 0; i < 329; i++){
        for(int j = 0; j < 329; j++){
            for (int k = 0; k < 3; k++){
                // get the x and y values according to the offset
                float x = i-offset;
                float y = j-offset;
                // normalize the x and y values
                x /= (offset);
                y /= (offset);
                // use the normalized x and y values to solve for u and v
                float u = x * sqrt(1-((y*y) / 2));
                float v = y * sqrt(1-((x*x) / 2));
                // use the u and v values and map backwards to x and y
                float x_new = (1.0/2.0)*(sqrt(2 + (u*u) - (v*v) + (2*sqrt(2)*u)) - sqrt(2 + (u*u) - (v*v) - (2*sqrt(2)*u)));
                float y_new = (1.0/2.0)*(sqrt(2 - (u*u) + (v*v) + (2*sqrt(2)*v)) - sqrt(2 - (u*u) + (v*v) - (2*sqrt(2)*v)));
                // unnormalize the i and j values
                unsigned int i_new = round((x_new * offset) + offset);
                unsigned int j_new = round((y_new * offset) + offset);
                // write out the values
                output[i_new][j_new][k] = input[i][j][k];
            }
        }
    }
}

///    Main Function    ///
int main(int argc, char *argv[]){
	if (argc < 4) {
		cout << "Incorrect parameter usage, printing out valid commands..." << endl;
		print_commands();
		return 0;
	}

    // create holder for and read in original image data
    unsigned char image_data[329][329][3];
    read_in(image_data, argv[1], 329, 329, 3);

    // forward mapping
    unsigned char forward_mapped[464][464][3];
    cout << "performing forward mapping..." << endl;
    forward_map(image_data, forward_mapped);
    write_out(forward_mapped, argv[2], 464, 464, 3);
    // reverse mapping
    unsigned char reverse_mapped[329][329][3];
    cout << "performing reverse mapping..." << endl;
    reverse_map(image_data, reverse_mapped);
    write_out(reverse_mapped, argv[3], 329, 329, 3);
}