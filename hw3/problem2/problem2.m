clear all
close all
clc

%% Student Information
% Name: Armin Bazarjani
% Student ID: 4430621961
% Email: bazarjan@usc.edu
% Submission Date: 3-11-21


%% Description
% This is a file for problem 2 of homework 2. 
% In this problem, we are tasked stitching three different images together
% to construct a panoramic photo


%% Define the paths
% Paths to all the .png files for the three images to stitch together
left_path = "/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework3/HW3_material/left.png";
right_path = "/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework3/HW3_material/right_updated.png";
middle_path = "/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework3/HW3_material/middle.png";


%% (0) Read in the images
% Read in the images using the pre-defined paths
left = imread(left_path);
middle = imread(middle_path);
right = imread(right_path);

left_gray = im2gray(left);
middle_gray = im2gray(middle);
right_gray = im2gray(right);


%% (1.1) Find matching point pairs using SIFT
% find the SURF points of each
points_l = detectSURFFeatures(left_gray);
points_m = detectSURFFeatures(middle_gray);
points_r = detectSURFFeatures(right_gray);

% extract the features from each image
[features_left, points_left] = extractFeatures(left_gray, points_l);
[features_middle, points_middle] = extractFeatures(middle_gray, points_m);
[features_right, points_right] = extractFeatures(right_gray, points_r);

% Find correspondences between left and middle
indexPairs_LM = matchFeatures(features_middle, features_left, 'Unique', true);
indexPairs_RM = matchFeatures(features_middle, features_right);

% Matched points on left image
matchedPoints_L = points_left(indexPairs_LM(:,2), :);
% Matched points on right image
matchedPoints_R = points_right(indexPairs_RM(:,2), :);
% Matched points on middle image to the left image
matchedPoints_LM = points_middle(indexPairs_LM(:,1), :);
% Matched points on the middle image to the right image
matchedPoints_RM = points_middle(indexPairs_RM(:,1), :);


%% (1.2) Check matched plots on left
figure(1); showMatchedFeatures(middle_gray,left_gray,matchedPoints_LM,matchedPoints_L);
legend('matched points middle','matched points left');

figure(2); showMatchedFeatures(middle_gray,right_gray,matchedPoints_RM,matchedPoints_R);
legend('matched points middle','matched points right');

%% (1.3) Check the matched SURF points by plotting them on the left and middle images
figure(1); imshow(left_gray);
axis on;
hold on;
plot(matchedPoints_L([21,16,2,4]));

figure(2); imshow(middle_gray);
axis on;
hold on;
plot(matchedPoints_LM([21,16,2,4]));


%% (1.4) Check the matched SURF points by plotting them on right and middle images
figure(1); imshow(right_gray);
axis on;
hold on;
plot(matchedPoints_R([21,6,12,32]));

figure(2); imshow(middle_gray);
axis on;
hold on;
plot(matchedPoints_RM([21,6,12,32]));



%% (2) Create the canvas
% Original Shit
%canvas = zeros(1255, 1500);
canvas = zeros(1255, 1500, 3);

%% (3) Populate the canvas with left, middle, and right
%%% Original Shit
% left
% left top and bottom offset = {1255-(height left)}/2 = 386;
canvas(287:769, 1:322, 1) = left(:,:,1);
canvas(287:769, 1:322, 2) = left(:,:,2);
canvas(287:769, 1:322, 3) = left(:,:,3);
% middle
canvas(285:771, 587:911, 1) = middle(:,:,1);
canvas(285:771, 587:911, 2) = middle(:,:,2);
canvas(285:771, 587:911, 3) = middle(:,:,3);
% right
canvas(384:872, 1176:1500, 1) = right(:,:,1);
canvas(384:872, 1176:1500, 2) = right(:,:,2);
canvas(384:872, 1176:1500, 3) = right(:,:,2);
 
% display canvas to make sure everything is working properly
canvas_disp = uint8(canvas);
imshow(canvas_disp);


%% (4) Select 4 matching points for each
% left
left_x = matchedPoints_L.Location([21,16,2,4],1);
left_y = matchedPoints_L.Location([21,16,2,4],2);

% middle-left
middleLeft_x =  matchedPoints_LM.Location([21,16,2,4],1);
middleLeft_y =  matchedPoints_LM.Location([21,16,2,4],2);

% middle-right
middleRight_x =  matchedPoints_RM.Location([21,6,12,32],1);
middleRight_y =  matchedPoints_RM.Location([21,6,12,32],2);

% right
right_x = matchedPoints_R.Location([21,6,12,32],1);
right_y = matchedPoints_R.Location([21,6,12,32],2);


%% (5) Transform the matching points to be on the same scale as our canvas
%%% Original Shit
% left
left_x = left_x; % remains unchanged
left_y = left_y + 286; % add the offset at the bottom of canvas
 
% middle left
middleLeft_x = middleLeft_x + 586; % add width of left + offset
middleLeft_y = middleLeft_y + 284; % add offset at bottom
 
% middle right
middleRight_x = middleRight_x + 586;
middleRight_y = middleRight_y + 284;
 
% right 
right_x = right_x + 1175; % add width of left and middle + 2*offest
right_y = right_y + 283; % add offset at bottom


%% (6.1) Use the left and middle-left 4 points to construct homography matrix for left image
% Construct the variables and equations
syms h11_l h12_l h13_l h21_l h22_l h23_l h31_l h32_l
eqn1 = h11_l*left_x(1) + h12_l*left_y(1) + h13_l == middleLeft_x(1)*(h31_l*left_x(1) + h32_l*left_y(1) + 1);
eqn2 = h21_l*left_x(1) + h22_l*left_y(1) + h23_l == middleLeft_y(1)*(h31_l*left_x(1) + h32_l*left_y(1) + 1);
eqn3 = h11_l*left_x(2) + h12_l*left_y(2) + h13_l == middleLeft_x(2)*(h31_l*left_x(2) + h32_l*left_y(2) + 1);
eqn4 = h21_l*left_x(2) + h22_l*left_y(2) + h23_l == middleLeft_y(2)*(h31_l*left_x(2) + h32_l*left_y(2) + 1);
eqn5 = h11_l*left_x(3) + h12_l*left_y(3) + h13_l == middleLeft_x(3)*(h31_l*left_x(3) + h32_l*left_y(3) + 1);
eqn6 = h21_l*left_x(3) + h22_l*left_y(3) + h23_l == middleLeft_y(3)*(h31_l*left_x(3) + h32_l*left_y(3) + 1);
eqn7 = h11_l*left_x(4) + h12_l*left_y(4) + h13_l == middleLeft_x(4)*(h31_l*left_x(4) + h32_l*left_y(4) + 1);
eqn8 = h21_l*left_x(4) + h22_l*left_y(4) + h23_l == middleLeft_y(4)*(h31_l*left_x(4) + h32_l*left_y(4) + 1);

% put the variables and equations into a matrix
[A,B] = equationsToMatrix([eqn1, eqn2, eqn3, eqn4, eqn5, eqn6, eqn7, eqn8], [h11_l, h12_l, h13_l, h21_l, h22_l, h23_l, h31_l, h32_l]);

% linsolve the matrix
X = linsolve(A,B);

%% (6.2) fill the variables and construct the matrix
h11_l = double(X(1));
h12_l = double(X(2));
h13_l = double(X(3));
h21_l = double(X(4));
h22_l = double(X(5));
h23_l = double(X(6));
h31_l = double(X(7));
h32_l = double(X(8));
h33_l = double(1);

left_homog = [h11_l h12_l h13_l; h21_l h22_l h23_l; h31_l h32_l h33_l];


%% (7.1) Use the left and middle-left 4 points to construct homography matrix for left image
% Construct the variables and equations
syms h11_r h12_r h13_r h21_r h22_r h23_r h31_r h32_r
eqn1 = h11_r*right_x(1) + h12_r*right_y(1) + h13_r == middleRight_x(1)*(h31_r*right_x(1) + h32_r*right_y(1) + 1);
eqn2 = h21_r*right_x(1) + h22_r*right_y(1) + h23_r == middleRight_y(1)*(h31_r*right_x(1) + h32_r*right_y(1) + 1);
eqn3 = h11_r*right_x(2) + h12_r*right_y(2) + h13_r == middleRight_x(2)*(h31_r*right_x(2) + h32_r*right_y(2) + 1);
eqn4 = h21_r*right_x(2) + h22_r*right_y(2) + h23_r == middleRight_y(2)*(h31_r*right_x(2) + h32_r*right_y(2) + 1);
eqn5 = h11_r*right_x(3) + h12_r*right_y(3) + h13_r == middleRight_x(3)*(h31_r*right_x(3) + h32_r*right_y(3) + 1);
eqn6 = h21_r*right_x(3) + h22_r*right_y(3) + h23_r == middleRight_y(3)*(h31_r*right_x(3) + h32_r*right_y(3) + 1);
eqn7 = h11_r*right_x(4) + h12_r*right_y(4) + h13_r == middleRight_x(4)*(h31_r*right_x(4) + h32_r*right_y(4) + 1);
eqn8 = h21_r*right_x(4) + h22_r*right_y(4) + h23_r == middleRight_y(4)*(h31_r*right_x(4) + h32_r*right_y(4) + 1);

% put the variables and equations into a matrix
[A,B] = equationsToMatrix([eqn1, eqn2, eqn3, eqn4, eqn5, eqn6, eqn7, eqn8], [h11_r, h12_r, h13_r, h21_r, h22_r, h23_r, h31_r, h32_r]);

% linsolve the matrix
X = linsolve(A,B);

%% (7.2) fill the variables and construct the matrix
h11_r = double(X(1));
h12_r = double(X(2));
h13_r = double(X(3));
h21_r = double(X(4));
h22_r = double(X(5));
h23_r = double(X(6));
h31_r = double(X(7));
h32_r = double(X(8));
h33_r = double(1);

right_homog = [h11_r h12_r h13_r; h21_r h22_r h23_r; h31_r h32_r h33_r];


%% Test Canvas
%%% Old Shit
canvas_plz = zeros(1055, 1500, 3);
canvas_plz(285:771, 587:911, 1) = middle(:,:,1);
canvas_plz(285:771, 587:911, 2) = middle(:,:,2);
canvas_plz(285:771, 587:911, 3) = middle(:,:,3);
canvas_plz = uint8(canvas_plz);
imshow(canvas_plz);


%% Multiply the left image with the homography matrix
% loop through the left image
for i = 1:483
    for j = 1:322
        for k = 1:3
            disp(i +","+j);
            % convert the coordinates to cartesian coordinates of our canvas
            x = j-0.5;
            y = (i+286)-0.5;

            % get the intermediate values
            x_prime = x*left_homog(1,1) + y*left_homog(1,2) + left_homog(1,3);
            y_prime = x*left_homog(2,1) + y*left_homog(2,2) + left_homog(2,3);
            w_prime = x*left_homog(3,1) + y*left_homog(3,2) + left_homog(3,3);

            % calculate the new cartesian values
            x_new = x_prime/w_prime;
            y_new = y_prime/w_prime;

            % get the new canvas location
            i_canvas = round(y_new-0.5);
            j_canvas = round(x_new+0.5);

            % First, check if we are in the original region
            if ((285<=i_canvas)&&(i_canvas<=771)) && ((587<=j_canvas)&&(j_canvas<=911))
                % if in original region, output normal
                canvas_plz(i_canvas, j_canvas, k) = canvas_plz(i_canvas, j_canvas, k);
            else
                canvas_plz(i_canvas, j_canvas, k) = left(i,j,k);
            end
        end
    end
end

%% Intermediate step to inspect results
imshow(canvas_plz);

%% Multiply the right image with the homograpy matrix
% loop through the right image
for i = 1:489
    for j = 1:325
        for k = 1:3
            disp(i +","+j);
            % convert the coordinates to cartesian coordinates of our canvas
            x = (j+1175)-0.5;
            y = (i+283)-0.5;

            % get the intermediate values
            x_prime = x*right_homog(1,1) + y*right_homog(1,2) + right_homog(1,3);
            y_prime = x*right_homog(2,1) + y*right_homog(2,2) + right_homog(2,3);
            w_prime = x*right_homog(3,1) + y*right_homog(3,2) + right_homog(3,3);

            % calculate the new cartesian values
            x_new = x_prime/w_prime;
            y_new = y_prime/w_prime;

            % get the new canvas location
            i_canvas = round(y_new-0.5);
            j_canvas = round(x_new+0.5);
            % First, check if we are in the original region
            if ((285<=i_canvas)&&(i_canvas<=771)) && ((587<=j_canvas)&&(j_canvas<=911))
                % if in original region, output normal
                canvas_plz(i_canvas, j_canvas, k) = canvas_plz(i_canvas, j_canvas, k);
                continue;
            else
                canvas_plz(i_canvas, j_canvas, k) = right(i,j,k);
            end
        end
    end
end


%% plz work
figure(2); imshow(canvas_plz);
canvas_ = canvas_plz;

%% loop through the image and take the average on black pixel locations
% loop through the canvas
for i = 2:1054
    for j = 2:1499
        for k = 1:3
            % if we are on a black pixel
            if canvas_plz(i,j, k) == 0
                % check neighbors
                % top left
                if canvas_plz(i-1, j-1, k) ~= 0
                    canvas_(i,j,k) = canvas_plz(i-1, j-1, k);
                % top
                elseif canvas_plz(i-1, j, k) ~= 0
                    canvas_(i,j,k) = canvas_plz(i-1, j, k);
                % top right
                elseif canvas_plz(i-1, j+1, k) ~= 0
                    canvas_(i,j,k) = canvas_plz(i-1, j+1, k);
                % left
                elseif canvas_plz(i, j-1, k) ~= 0
                    canvas_(i,j,k) = canvas_plz(i, j-1, k);
                % right
                elseif canvas_plz(i, j+1, k) ~= 0
                    canvas_(i,j,k) = canvas_plz(i, j+1, k);
                % bottom left
                elseif canvas_plz(i+1, j-1, k) ~= 0
                    canvas_(i,j,k) = canvas_plz(i+1, j-1, k);
                % bottom right
                elseif canvas_plz(i+1, j+1, k) ~= 0
                    canvas_(i,j, k) = canvas_plz(i+1, j+1, k);
                % bottom
                elseif canvas_plz(i+1, j, k) ~= 0
                    canvas_(i,j,k) = canvas_plz(i+1, j, k);
                end
            end
        end
    end
end



%% show that shit
imshow(canvas_);
canvas_final = canvas_;


%% Fill Black values one more time
for i = 2:1054
    for j = 2:1499
        for k = 1:3
            % if we are on a black pixel
            if canvas_(i,j,k) == 0
                % check neighbors
                % top left
                if canvas_(i-1, j-1, k) ~= 0
                    canvas_final(i,j,k) = canvas_(i-1, j-1, k);
                % top
                elseif canvas_(i-1, j, k) ~= 0
                    canvas_final(i,j,k) = canvas_(i-1, j, k);
                % top right
                elseif canvas_(i-1, j+1, k) ~= 0
                    canvas_final(i,j,k) = canvas_(i-1, j+1, k);
                % left
                elseif canvas_(i, j-1, k) ~= 0
                    canvas_final(i,j,k) = canvas_(i, j-1, k);
                % right
                elseif canvas_(i, j+1, k) ~= 0
                    canvas_final(i,j,k) = canvas_(i, j+1, k);
                % bottom left
                elseif canvas_(i+1, j-1, k) ~= 0
                    canvas_final(i,j,k) = canvas_(i+1, j-1, k);
                % bottom right
                elseif canvas_(i+1, j+1, k) ~= 0
                    canvas_final(i,j,k) = canvas_(i+1, j+1, k);
                % bottom
                elseif canvas_(i+1, j, k) ~= 0
                    canvas_final(i,j,k) = canvas_(i+1, j, k);
                end
            end
        end
    end
end

%% Inspect final results
imshow(canvas_final);

