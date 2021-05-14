%% Student Information
% Name: Armin Bazarjani
% USC ID: 4430621961
% Email: bazarjan@usc.edu
% Due Date: March 28th, 2021

function acc = SupportVectorMachine(X_train, X_test, y_train, y_test)
    % construct holders for the solvers and the kernel
    solvers = {'ISDA', 'SMO'};
    kernels = {'linear', 'rbf', 'polynomial'};
    
    % loop through the kernels
    for i = 1:size(solvers,2)
        for j = 1:size(kernels,2)
            solver = char(solvers(i));
            kernel = char(kernels(j));
            % construct template for the SVM
            t = templateSVM('Standardize',true,'KernelFunction',kernel,'Solver',solver);
            
            % fit the svm
            SVM = fitcecoc(X_train, y_train, 'Learners', t, 'FitPosterior',true);
            
            % Make a prediction
            pred = predict(SVM, X_test);
            
            % Calculate the accuracy
            num_true = 0;
            for k = 1:size(pred,1)
                if pred(k) == y_test(k)
                    num_true = num_true + 1;
                end
            end
            acc = num_true/size(pred,1);
            disp("The accuracy of an SVM with a "+kernel+" kernel with a "+solver+" solver is : "+acc);
        end
    end
end
