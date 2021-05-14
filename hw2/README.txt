### Student Information
Name: Armin Bazarjani
Class: EE-569 Introduction to Digital Image Processing
Date: March 12th, 2021
Assignment: Homework 3


### Tech Information
I am running everything on MacOS (Catalina Version 10.15.7).
I am compiling all cpp files using g++. (g++ -dumpversion returns 12.0.0).
I am using MATLAB_R16b for the .m files.


### File Structure
homework 1
         |
         |> Problem 1
         |          |> 1a (main files: problem_1.cpp)
         |
         |> Problem 2
         |          |> 2a (main file: problem_2.m)
         |
         |> Problem 3 (main file: problem_3.cpp)
         |          |> 3a (main files: problem_3a_flower.cpp, problem_3a_jar.cpp, problem_3a_spring.cpp)
         |          |> 3b (main file: problem_3b.cpp)
         |          |> 3c (main file: problem_3c_123.cpp, problem_3c_ccl.cpp)


### How to compile
## Problem One:
# 1a
To compile Problem one type g++ problem_1a_elephant.cpp -o p1_elephant in terminal.
Or, g++ problem_1a_ski.cpp -o p1_ski

*NOTE* Both problem_1a_elephant.cpp and problem_1a_ski.cpp have global file paths inside the code that you
will have to change as well. This will output a txt file of the sobel gradient magnitude map that is later
used in problem 1d.

To run type ./p1 command_1 command_2 command_3 command_4
- command_1 is the path to the input file
- command_2 is the path to the output file
- command_3 is the desired threshold

Example_1: ./p1 ../HW2_materials/elephant.raw elephant_sobel.raw 0.18


# 1b
1b is a MATLAB file, everything should be self-contained, however you will need to change the specified paths.

elephant_path - path to elephant.jpg
ski_path - path to ski_person.jpg
elephant_edges - output path for Canny edge detection
ski_edges - output path for Canny edge detection


# 1c
1c is a MATLAB file. Again, you will have to change all of the paths to run, everything else should be self-contained.
This file will output .mat files of the SE edges to be used in problem 1d.

elephant_path - path for the elephant input image (assumes .jpg)
ski_path - path for the ski person input image (assumes .jpg)
elephant_edges - output path for SE edge map
ski_edges - output path for SE edge map
elephant_mat - otuput path for SE .mat
ski_mat - otuput path for SE .mat


# 1d
1d is yet another MATLAB file where you will again only have to change the path variables.

elephant_sobel_path - path to the elephant_sobel.txt file that was produced after running 1a
ski_sobel_path - path to the ski_sobel.txt file that was produced after running 1a
elephant_SE_path - path to the elephant_SE.mat file that was produced after running 1c
ski_SE_path - path to the ski_SE.mat file that was produced after running 1c
elephant_gt - path to the elephant_GT.mat file
ski_gt - path to the ski_person_GT.mat file



## Problem Two:
# 2a
To compile 2a type g++ problem_2a.cpp -o p2a in terminal.
To run type ./p2a command_1 command_2 command_3 command_4 command_5
- command_1 is the path to the input file
- command_2 is the path to the output file
- command_3 is the method (fixed, random, or dithering)
- command_4 is the size of the dithering matrix (has to be power of 2)
- Note that command_4 is only needed if you specify command_3 to be dithering

Example_1: ./p2a ../HW1_images/bridge.raw bridge_fixed.raw fixed
Example_2: ./p2a ../HW1_images/bridge.raw bridge_dith_8.raw dithering 8


# 2b
To compile 2b type g++ problem_2b.cpp -o p2b in terminal.
To run type ./p2b command_1 command_2 command_3
- command_1 is the path to the input file
- command_2 is the path to the output file
- command_3 is the method (fs, jjn, stucki)

Example: ./p2b ../HW1_images/bridge.raw bridge_fs.raw fs


## Problem Three:
# Part 1
To compile 3-1 type g++ problem3_part1.cpp -o p31 in terminal.
To run type ./p31 command_1 command_2 command_3 command_4
- command_1 is the path to the input file
- command_2 is the path to the output file

Example: ./p3 ../HW1_images/fish.raw fish_separable.raw


# Part 2
To compile 3-2 type g++ problem3_part2.cpp -o p32 in terminal.
To run type ./p31 command_1 command_2
- command_1 is the path to the input file
- command_2 is the path to the output file

Example: ./p3 ../HW1_images/fish.raw fish_mbvq.raw
