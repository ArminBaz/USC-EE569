% Name: Armin Bazarjani
% USC ID: 4430621961
% Email: bazarjan@usc.edu
% Due Date: March 28th, 2021

function KNN_label = KNN(train, test_vector)
    % Split train set
    blanket_train = train(1:9,:);
    brick_train = train(10:18,:);
    grass_train = train(19:27,:);
    rice_train = train(28:36,:);
    
    % get the mahalanobis distance of each sample in test w.r.t each
    % texture
    mahal_blanket = mahal(test_vector, blanket_train);
    mahal_brick = mahal(test_vector, brick_train);
    mahal_grass = mahal(test_vector, grass_train);
    mahal_rice = mahal(test_vector, rice_train);
    
    % Output Label
    % 1 : blanket
    % 2 : brick
    % 3 : grass
    % 4 : rice
    if (mahal_blanket < mahal_brick) && (mahal_blanket < mahal_grass) && (mahal_blanket < mahal_rice)
        KNN_label = 1;
    elseif (mahal_brick < mahal_blanket) && (mahal_brick < mahal_grass) && (mahal_brick < mahal_rice)
        KNN_label = 2;
    elseif (mahal_grass < mahal_blanket) && (mahal_grass < mahal_brick) && (mahal_grass < mahal_rice)
        KNN_label = 3;
    elseif (mahal_rice < mahal_blanket) && (mahal_rice < mahal_brick) && (mahal_rice < mahal_grass)
        KNN_label = 4;
    end
end