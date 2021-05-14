%% Student Information
% Name: Armin Bazarjani
% USC ID: 4430621961
% Email: bazarjan@usc.edu
% Due Date: March 28th, 2021


%% Clear Everything
clear all; close all; clc;


%% Load in all the matrices that were saved from the script for 1(a)
% Load saved data from .mat files
feat_train_full = load('../features_train.mat');
feat_train_pca = load('../features_train_pca.mat');
feat_test_full = load('../features_test.mat');
feat_test_pca = load('../features_test_pca.mat');
labels_train = load('../labels_train.mat');
labels_test = load('../labels_test.mat');

% Load the matrices from the structs
feat_test_full = feat_test_full.features_test;
feat_train_full = feat_train_full.features_train;
feat_train_pca = feat_train_pca.features_train_pca;
feat_test_pca = feat_test_pca.features_test_pca;
labels_test = labels_test.true_labels;
labels_train = labels_train.labels_train;


%% Unsupervised Learning
% for unsupervised learning, we can combine the train and test datasets
merged_full = [feat_train_full; feat_test_full];
merged_pca = [feat_train_pca; feat_test_pca];
merged_labels = [labels_train, labels_test];

% Call KMeans function to perform Kmeans clustering
acc_KMeans_full = KMeans(merged_full, merged_labels);
disp("Accuracy of KMeans on 25D Feature Vector: " + acc_KMeans_full);

acc_KMeans_pca = KMeans(merged_pca, merged_labels);
disp("Accuracy of KMeans on 3D Feature Vector: " + acc_KMeans_pca);


%% Supervised Learning
% Get the accuracy of Random Forest algorithm
RandomForest(feat_train_pca, feat_test_pca, labels_train, labels_test);

% Get the accuracy of Support Vector Machine algorithm
SupportVectorMachine(feat_train_pca, feat_test_pca, labels_train, labels_test);