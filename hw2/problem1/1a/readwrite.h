/*
    Name: Armin Bazarjani
    USC ID: 4430621961
    USC Email: bazarjan@usc.edu
    Submission Date: 2-23-21
    
    Header file to assist me with reading in and writing out images, all other functions will be defined
    inside of the corresponding cpp files
*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
using namespace std;

// function that reads in image into an array of size [height][width][channels]
void read_in(void* Imagedata, char* input_name, int height, int width, int channels){
    FILE *file;
    // read in image
	file = fopen(input_name, "rb");
	fread(Imagedata, sizeof(unsigned char), height*width*channels, file);
	fclose(file);
}


// function that writes out an input array into a .raw file
void write_out(void * Imagedata, char* output_name, int height, int width, int channels){
    FILE *file;

    // write out image
	file = fopen(output_name, "wb");
	fwrite(Imagedata, sizeof(unsigned char), height*width*channels, file);
	fclose(file);
}

// function to write out to txt file
template <size_t height, size_t width, size_t channels>
void write_out_txt(unsigned char (&image_data)[height][width][channels], char * filename){
    // open ofstream operator
    ofstream ofs(filename);
    // loop through image data and write out
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            // if we are at the last value we don't want a space
            if ((i == height-1) && (j == width-1)){
                ofs << (int)(image_data[i][j][0]);
            }
            else{  
                ofs << (int)(image_data[i][j][0]);
                ofs << " ";
            }
        }
        ofs << endl;
    }

    // close ofstream operator
    ofs.close();
}
