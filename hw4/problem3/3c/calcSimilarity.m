%% Student Information
% Name: Armin Bazarjani
% USC ID: 4430621961
% Email: bazarjan@usc.edu
% Due Date: March 28th, 2021

function similarity = calcSimilarity(hist1, hist2)
    % initialize numerator and denominator
    numerator = 0;
    denominator = 0;
    
    % calculate the numberator and denominator in same for loop
    for i = 1:size(hist1,2)
        numerator = numerator + min(hist1(i),hist2(i));
        denominator = denominator + max(hist1(i),hist2(i));
    end
    
    similarity = double(numerator/denominator);
end