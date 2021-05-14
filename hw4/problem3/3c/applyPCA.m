%% Student Information
% Name: Armin Bazarjani
% USC ID: 4430621961
% Email: bazarjan@usc.edu
% Due Date: March 28th, 2021

function feature_pca = applyPCA(features, num_dimensions)
    % first, we transpose the features
    feat_transpose = transpose(features);
    feat_transpose = double(feat_transpose);
    % next, we do pca analysis and return the reduced dimensions
    [coeff_train, score_train, latent_train] = pca(feat_transpose);
    feature_pca = feat_transpose * coeff_train(:,1:num_dimensions);
end