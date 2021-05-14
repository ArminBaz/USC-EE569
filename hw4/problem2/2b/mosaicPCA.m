% Name: Armin Bazarjani
% USC ID: 4430621961
% Email: bazarjan@usc.edu
% Due Date: March 28th, 2021

function mosaicPCA = mosaicPCA(mosaicData, lawHolder, windowSize, height, width)
    % Call lawFilters function on mosaicData
    mosaicEnergy = lawFilters(mosaicData, lawHolder, windowSize, height, width);
    
    % Normalize using mean and std deviation
    for i = 1:height
        for j = 1:width
            mean_e = mean(mosaicEnergy(i,j,:));
            std_e = std(mosaicEnergy(i,j,:));
            for k = 1:24
                mosaicEnergy(i,j,k) = (mosaicEnergy(i,j,k)-mean_e)/std_e;
            end
        end
    end
    
    % Reshape into 24-D feature vector
    mosaicReshape = reshape(mosaicEnergy,[height*width 24]);
    
    % Apply PCA on the reshaped vector
    [coeff_train, score_train, latent_train] = pca(mosaicReshape);
    mosaicPCA = mosaicReshape * coeff_train(:,1:5);
end