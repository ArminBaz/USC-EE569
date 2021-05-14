%% Student Information
% Name: Armin Bazarjani
% USC ID: 4430621961
% Email: bazarjan@usc.edu
% Due Date: March 28th, 2021


%% Clear Everything
clear all
close all
clc


%% Image paths and dimensions
img_height = 420;
img_width = 640;
dog_1 = "/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework4/HW4_material/Dog_1.png";
dog_2 = "/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework4/HW4_material/Dog_2.png";
dog_3 = "/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework4/HW4_material/Dog_3.png";
cat = "/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework4/HW4_material/Cat.png";


%% Read in the images
dog_1_data = imread(dog_1);
dog_2_data = imread(dog_2);
dog_3_data = imread(dog_3);
cat_data = imread(cat);


%% Extract SIFT Features for the images
% dog3
dog3_I = single(rgb2gray(dog_3_data));
[f_dog3,d_dog3] = vl_sift(dog3_I);

% dog1
dog1_I = single(rgb2gray(dog_1_data));
[f_dog1,d_dog1] = vl_sift(dog1_I);

% dog2
dog2_I = single(rgb2gray(dog_2_data));
[f_dog2,d_dog2] = vl_sift(dog2_I);

% cat
cat_I = single(rgb2gray(cat_data));
[f_cat,d_cat] = vl_sift(cat_I);


%% Find the largest score in SIFT features for dog3 and return it's index
high_dog3 = findHighestIndex(f_dog3);
disp("The highest score of : "+f_dog3(3,high_dog3)+" is associated with index: "+high_dog3);


%% Find the Associated feature in dog1
% initialize min dist and index variables
min_dist = 10000000000000.0;
min_index = 0;
% load dog3 descriptor
desc_dog3 = double(d_dog3(:,high_dog3));

% loop through dog 1
for i = 1:size(d_dog1,2)
    % get the descriptor for dog 1
    desc_dog1 = double(d_dog1(:,i));
    % calculate distance
    dist = sum((desc_dog3-desc_dog1).^2);
    
    % check distance
    if dist < min_dist
        min_index = i;
        min_dist = dist;
    end
end

disp("The minimum index is: "+min_index);


%% Plot the two keypoints along with the SIFT frame and feature descriptors
figure(1);
subplot(1,2,1);
imshow(dog_3);
h1_dog3 = vl_plotframe(f_dog3(:,high_dog3));
set(h1_dog3,'color','y','linewidth',2) ;
h3_dog3 = vl_plotsiftdescriptor(d_dog3(:,high_dog3),f_dog3(:,high_dog3)) ;
set(h3_dog3,'color','g') ;

subplot(1,2,2);
imshow(dog_1);
h1_dog1 = vl_plotframe(f_dog1(:,min_index));
set(h1_dog1,'color','y','linewidth',2);
h3_dog1 = vl_plotsiftdescriptor(d_dog1(:,min_index),f_dog1(:,min_index)) ;
set(h3_dog1,'color','g') ;


%% Plot the matches for dog1 and dog3
[matches score] = vl_ubcmatch(d_dog3,d_dog1,1.5); %threshold originally is 1.5
im = dog_3_data;
im2 = dog_1_data;
% check for sizes and take longest width and longest height into
% account
if (size(im,1) > size(im2,1))
    longestWidth = size(im,1);       
else
    longestWidth = size(im2,1);
end

if (size(im,2) > size(im2,2))
    longestHeight = size(im,2);
else
    longestHeight = size(im2,2);
end


% create new matrices with longest width and longest height
newim = uint8(zeros(longestWidth, longestHeight, 3)); 
newim2 = uint8(zeros(longestWidth, longestHeight, 3));

% transfer both images to the new matrices respectively.
newim(1:size(im,1), 1:size(im,2), 1:3) = im;
newim2(1:size(im2,1), 1:size(im2,2), 1:3) = im2;

% with the same proportion and dimension, we can now show both
% images. Parts that are not used in the matrices will be black.
figure(2); imshow([newim newim2]);

xa = f_dog3(1,matches(1,:)) ;
xb = f_dog1(1,matches(2,:)) + size(dog_3_data,2) ;
ya = f_dog3(2,matches(1,:)) ;
yb = f_dog1(2,matches(2,:)) ;

hold on ;
h = line([xa ; xb], [ya ; yb]) ;
set(h,'linewidth', 1, 'color', 'b') ;

vl_plotframe(f_dog3(:,matches(1,:))) ;
f_dog1(1,:) = f_dog1(1,:) + size(dog_3_data,2) ;
vl_plotframe(f_dog1(:,matches(2,:))) ;
axis image off ;
hold off;


%% Plot the matches for dog3 and dog2
[matches score] = vl_ubcmatch(d_dog3,d_dog2,1.5); %threshold originally is 1.5
im = dog_3_data;
im2 = dog_2_data;
% check for sizes and take longest width and longest height into
% account
if (size(im,1) > size(im2,1))
    longestWidth = size(im,1);       
else
    longestWidth = size(im2,1);
end

if (size(im,2) > size(im2,2))
    longestHeight = size(im,2);
else
    longestHeight = size(im2,2);
end


% create new matrices with longest width and longest height
newim = uint8(zeros(longestWidth, longestHeight, 3)); 
newim2 = uint8(zeros(longestWidth, longestHeight, 3));

% transfer both images to the new matrices respectively.
newim(1:size(im,1), 1:size(im,2), 1:3) = im;
newim2(1:size(im2,1), 1:size(im2,2), 1:3) = im2;

% with the same proportion and dimension, we can now show both
% images. Parts that are not used in the matrices will be black.
figure(3); imshow([newim newim2]);

xa = f_dog3(1,matches(1,:)) ;
xb = f_dog2(1,matches(2,:)) + size(dog_3_data,2) ;
ya = f_dog3(2,matches(1,:)) ;
yb = f_dog2(2,matches(2,:)) ;

hold on ;
h = line([xa ; xb], [ya ; yb]) ;
set(h,'linewidth', 1, 'color', 'b') ;

vl_plotframe(f_dog3(:,matches(1,:))) ;
f_dog2(1,:) = f_dog2(1,:) + size(dog_3_data,2) ;
vl_plotframe(f_dog2(:,matches(2,:))) ;
axis image off ;
hold off;


%% Plot the matches for dog3 and cat
[matches score] = vl_ubcmatch(d_dog3,d_cat,1.5); %threshold originally is 1.5
im = dog_3_data;
im2 = cat_data;
% check for sizes and take longest width and longest height into
% account
if (size(im,1) > size(im2,1))
    longestWidth = size(im,1);       
else
    longestWidth = size(im2,1);
end

if (size(im,2) > size(im2,2))
    longestHeight = size(im,2);
else
    longestHeight = size(im2,2);
end


% create new matrices with longest width and longest height
newim = uint8(zeros(longestWidth, longestHeight, 3)); 
newim2 = uint8(zeros(longestWidth, longestHeight, 3));

% transfer both images to the new matrices respectively.
newim(1:size(im,1), 1:size(im,2), 1:3) = im;
newim2(1:size(im2,1), 1:size(im2,2), 1:3) = im2;

% with the same proportion and dimension, we can now show both
% images. Parts that are not used in the matrices will be black.
figure(4); imshow([newim newim2]);

xa = f_dog3(1,matches(1,:)) ;
xb = f_cat(1,matches(2,:)) + size(dog_3_data,2) ;
ya = f_dog3(2,matches(1,:)) ;
yb = f_cat(2,matches(2,:)) ;

hold on ;
h = line([xa ; xb], [ya ; yb]) ;
set(h,'linewidth', 1, 'color', 'b') ;

vl_plotframe(f_dog3(:,matches(1,:))) ;
f_cat(1,:) = f_cat(1,:) + size(dog_3_data,2) ;
vl_plotframe(f_cat(:,matches(2,:))) ;
axis image off ;
hold off;


%% Plot the matches for dog1 and cat
[matches score] = vl_ubcmatch(d_cat,d_dog1,1.5); %threshold originally is 1.5
im = cat_data;
im2 = dog_1_data;
% check for sizes and take longest width and longest height into
% account
if (size(im,1) > size(im2,1))
    longestWidth = size(im,1);       
else
    longestWidth = size(im2,1);
end

if (size(im,2) > size(im2,2))
    longestHeight = size(im,2);
else
    longestHeight = size(im2,2);
end


% create new matrices with longest width and longest height
newim = uint8(zeros(longestWidth, longestHeight, 3)); 
newim2 = uint8(zeros(longestWidth, longestHeight, 3));

% transfer both images to the new matrices respectively.
newim(1:size(im,1), 1:size(im,2), 1:3) = im;
newim2(1:size(im2,1), 1:size(im2,2), 1:3) = im2;

% with the same proportion and dimension, we can now show both
% images. Parts that are not used in the matrices will be black.
figure(5); imshow([newim newim2]);

xa = f_cat(1,matches(1,:)) ;
xb = f_dog1(1,matches(2,:)) + size(cat_data,2) ;
ya = f_cat(2,matches(1,:)) ;
yb = f_dog1(2,matches(2,:)) ;

hold on ;
h = line([xa ; xb], [ya ; yb]) ;
set(h,'linewidth', 1, 'color', 'b') ;

vl_plotframe(f_cat(:,matches(1,:))) ;
f_dog1(1,:) = f_dog1(1,:) + size(cat_data,2) ;
vl_plotframe(f_dog1(:,matches(2,:))) ;
axis image off ;
hold off;












