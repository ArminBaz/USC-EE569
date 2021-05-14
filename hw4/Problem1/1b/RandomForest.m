%% Student Information
% Name: Armin Bazarjani
% USC ID: 4430621961
% Email: bazarjan@usc.edu
% Due Date: March 28th, 2021

function RandomForest(X_train, X_test, y_train, y_test)
    % construct a holder for different numbers of decision trees
    num_trees = [10, 20, 50, 100, 200, 500, 1000];
    
    % loop through the different numbers of decision trees
    for i = 1:size(num_trees,2)
        % construct the tree and make a prediction
        RF = TreeBagger(num_trees(i), X_train, y_train, 'MinLeafSize', 3);
        pred = RF.predict(X_test);
        pred = str2double(pred);
        
        % calculate how many values match
        num_true = 0;
        for j = 1:size(pred,1)
            if pred(j) == y_test(j)
                num_true = num_true + 1;
            end
        end
        
        % get the accuracy
        acc = num_true/size(pred,1);
        disp("The accuracy for using " + num_trees(i) + " decision trees is: " + acc);
    end
end