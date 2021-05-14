%% Student Information
% Name: Armin Bazarjani
% Student ID: 4430621961
% Email: bazarjan@usc.edu
% Submission Date: 2-23-21
% 
% This is a function to assist with problem1-d part 1.
% 
% Returns mean precision and recall, an F score calculated with the mean
% precision and recall, and a vector containing the precision and recall
% for each ground truth image.
function [meanP, meanR, F, P, R] = prf_1(gt_file, edge)
    g = load(gt_file);
    g = g.groundTruth;
    num_gt = size(g, 2);
    P = zeros(num_gt, 1);
    R = zeros(num_gt, 1);
    for i = 1 : num_gt
       groundTruth{1} = g{i};
       gt_holder = ['gt_temp_', num2str(i), '.mat'];
       save(gt_holder, 'groundTruth');
       [~, cntR, sumR, cntP, sumP, ~] = edgesEvalImg(edge, gt_holder, {'thrs', 1}); 
       delete(gt_holder);
       P(i) = cntP / sumP;
       R(i) = cntR / sumR;
    end
    meanP = mean(P);
    meanR = mean(R);   
    F = 2 * (meanP * meanR) / (meanP + meanR);
end