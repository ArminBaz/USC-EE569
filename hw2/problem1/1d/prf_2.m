%% Student Information
% Name: Armin Bazarjani
% Student ID: 4430621961
% Email: bazarjan@usc.edu
% Submission Date: 2-23-21
% 
% This is a function that is meant for problem1-d part 2. The function is
% similar to the one for problem1-d part 1 except it takes a thresh_numb as
% input that dictates the number of thresholds to test on the image
%
% Returns a vector of thresholds and the F score associated with each
% threshold
function [F, T] = prf_2(gt_file, edge, thresh_num)
    g = load(gt_file);
    g = g.groundTruth;
    num_gt = size(g, 2);
    P = zeros(num_gt, thresh_num);
    R = zeros(num_gt, thresh_num);
    for i = 1 : num_gt
       groundTruth{1} = g{i};
       gt_holder = ['gt_temp_', num2str(i), '.mat'];
       save(gt_holder, 'groundTruth');
       [thrs, cntR, sumR, cntP, sumP, ~] = edgesEvalImg(edge, gt_holder, {'thrs', thresh_num}); 
       delete(gt_holder);
       P(i,:) = cntP ./ sumP;
       R(i,:) = cntR ./ sumR;
       T = thrs;
    end
    % Construct mean and variance vectors
    meanP = zeros(thresh_num, 1);
    meanR = zeros(thresh_num, 1);
    F = zeros(thresh_num, 1);
    for i = 1 : thresh_num
        meanP(i) = mean(P(:,i));
        meanR(i) = mean(R(:,i));
        F(i) = 2 * (meanP(i) * meanR(i)) / (meanP(i)+meanR(i));
    end
end