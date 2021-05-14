%% Student Information
% Name: Armin Bazarjani
% USC ID: 4430621961
% Email: bazarjan@usc.edu
% Due Date: March 28th, 2021

function hist = calcHist(kmeans_pred, num_bins)
    % create an empty histogram
    hist = zeros(1,num_bins);
    
    % loop through the kmeans prediction and fill the histogram
    for i = 1:size(kmeans_pred,1)
        hist(kmeans_pred(i)) = hist(kmeans_pred(i)) + 1;
    end
end