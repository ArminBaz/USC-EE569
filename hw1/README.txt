### Student Information
Name: Armin Bazarjani
Class: EE-569 Introduction to Digital Image Processing
Date: February 6th, 2021
Assignment: Homework 1


### Tech Information
I am running everything on MacOS (Catalina Version 10.15.7).
I am compiling all cpp files using g++. (g++ -dumpversion returns 12.0.0).
I am using MATLAB_R2020b for the .m files.


### File Structure
homework 1
         |
         |> Problem 1 (main file: problem_one.cpp)
         |
         |> Problem 2
         |          |> 2a (main file: problem_2a.cpp)
         |          |> 2b (main file: problem_2b.cpp)
         |          |> 2c (main file: problem_2c.cpp)
         |          |> 2d (main file: problem_2d.cpp)
         |
         |> Problem 3 (main file: problem_3.cpp)


### How to compile
## Problem One:
To compile Problem one type g++ problem_one.cpp -o p1 in terminal.
To run type ./p1 command_1 command_2 command_3 command_4
- command_1 is either A or B depending on which sub problem you want to run
- command_2 is the path to the input file
- command_3 is the path to the output file
- command_4 is only required if command_1 is B. This command is either transfer or bins.

Example_1: ./p1 A ../HW_images/House.raw House_demosaiced.raw
Example_2: ./p1 B ../HW1_images/Toy.raw toy_histogram.raw transfer


## Problem Two:
# 2a
To compile 2a type g++ problem_2a.cpp -o p2a in terminal.
To run type ./p2a command_1 command_2 command_3 command_4 command_5
- command_1 is the path to the input file
- command_2 is the path to the output file
- command_3 is the filter type (uniform or gaussian)
- command_4 is the filter size (odd int greater than 1)
- command_5 is only required if command_2 is gaussian, it determines the sigma value of the gaussian filter

Example_1: ./p2a ../HW1_images/Fruits_gray_noisy.raw Fruits_uniform_3.raw uniform 3
Example_2: ./p2a ../HW1_images/Fruits_gray_noisy.raw Fruits_gauss_5_0.6.raw gaussian 5 0.6


# 2b
To compile 2b type g++ problem_2b.cpp -o p2b in terminal.
To run type ./p2b command_1 command_2 command_3 command_4 command_5
- command_1 is the path to the input file
- command_2 is the path to the output file
- command_3 is the neighborhood size (at least 1)
- command_4 is the value of sigma_s (can be float)
- command_5 is the value of sigma_c (can be float)

Example: ./p2b ../HW1_images/Fruits_gray_noisy.raw fruits_gray_bilateral.raw 3 20 10


# 2c
2c is a MATLAB file, everything should be self contained. However, if you would like to run it please read the code
and see where I commented to change the desired values. There are two path values that you must change in order
to properly read in and write out an input and output image. And if you would like to modify the NLM parameters, you will
have to do so within the MATLAB script itself.

# 2d
To compile 2d type g++ problem_2d.cpp -o p2d in terminal.
To run type ./p2d command_1 command_2 command_3
- command_1 is the path to the input file
- command_2 is the path to the output file
- command_3 is the method to implement (histogram or clean)
    - histogram is for calculating the noise histograms
    - clean is for cleaning the image using the cascaded deniosing methods
- command_4 filter_size (odd greater than 1) (only required if command_3 is clean)
- command_5 sigma (only required if command_3 is clean)

Example: ./p2d ../HW1_images/Fruits.raw Fruits_cascaded.raw clean 3 0.6


## Problem Three:
To compile 3 type g++ problem_3.cpp -o p3 in terminal.
To run type ./p2d command_1 command_2 command_3 command_4
- command_1 is the path to the input file
- command_2 is the path to the output file
- command_3 is the number of colors (either 64 or 512)
- command_4 is the filter_size (odd greater than 1)

Example: ./p3 ../HW1_images/Fruits.raw fruits_oil_painted.raw 3