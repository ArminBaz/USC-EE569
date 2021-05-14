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


// function to allocate memory for a 3d array
template <typename T>
T*** mem_alloc_3d(T ***arr, int height, int width, int channels){
    arr = new T **[height]();
	for (int i = 0;i < height; i++){
		arr[i] = new T *[width]();
		for (int j = 0; j < width; j++){
			arr[i][j] = new T[channels]();
			for (int k = 0; k < channels; k++){
				arr[i][j][k] = 0x00;
			}
		}
	}
	return arr;
}


// function to allocate memory for a 2d array
template <typename T>
T** mem_alloc_2d(T **arr, int height, int width){
    arr = new T*[height]();
    for(int i = 0; i < height; i++) {
        arr[i] = new T[width]();
    }
        return arr;
}


// function to deallocate memory for a 3d array
template <typename T>
void mem_dealloc_3d(T ***arr, int height, int width){
    for(int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j){
            delete[] arr[i][j];
        }
        delete [] arr[i];
    }
    delete [] arr;
}


// function to deallocate memory for a 2d array
template <typename T>
void mem_dealloc_2d(T **arr, int height){
    for(int i = 0; i < height; ++i) {
    delete [] arr[i];
    }
    delete [] arr;
}

void fill_3d(unsigned char ***output, unsigned char *input, int height, int width, int channels){
    for (int row = 0; row < height; row++){
        for (int col = 0; col < width; col++){
            for (int channel = 0; channel < channels; channel++){
                output[row][col][channel] = input[row + height*(col+width*channel)];
            }
        }
    }
}

void revert_1d(unsigned char *output, unsigned char ***input, int height, int width, int channels){
    for (int row = 0; row < height; row++){
        for (int col = 0; col < width; col++){
            for (int channel = 0; channel < channels; channel++){
                output[row + height*(col+width*channel)] = input[row][col][0];
            }
        }
    }
}

void revert_1d_gray(unsigned char *output, unsigned char ***input, int height, int width){
    for (int row = 0; row < height; row++){
        for (int col = 0; col < width; col++){
            output[row + col*height] = input[row][col][0];
        }
    }
}