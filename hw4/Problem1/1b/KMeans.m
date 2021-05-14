%% Student Information
% Name: Armin Bazarjani
% USC ID: 4430621961
% Email: bazarjan@usc.edu
% Due Date: March 28th, 2021

function acc = KMeans(input_data, labels)
    MaxIters = 100000;
    predictions = kmeans(input_data, 4, 'Distance', 'cityblock', 'MaxIter', MaxIters);
    
    label_counter = zeros(4,4);
    for i = 1:size(predictions)
        label_counter(predictions(i), labels(i)) = label_counter(predictions(i), labels(i)) + 1;
    end

    label_kmean = [];
    for i = 1:4
        [M,I] = max(label_counter(i,:)); 
        label_kmean = [label_kmean, I];
    end

    num_correct = 0;
    for i = 1:48
       if label_kmean(predictions(i)) == labels(i)
           num_correct = num_correct + 1;
       end
    end
    acc = num_correct/48;
end