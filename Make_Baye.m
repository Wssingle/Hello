  clc; clear all; close all;
 
  
 %Load SqueezeNet and show its layers 
 convnet = squeezenet
%  convnet.Layers 
% %  plot(convnet) 
%  
%  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Test and Train
% 
% rootFolder = 'train_data_Dman'; % We use D-man training set, (Purified by Damon M.)
% categories = {'no_hard_hats', 'hard_hats_Dman'};
% imds = imageDatastore(fullfile(rootFolder, categories), 'LabelSource', 'foldernames');
% imds.ReadFcn = @readFunctionTrain;
% 
% % Change the number 50 to as many training images as you would like to use
% % how does increasing the number of images change the 
% % accuracy of the classifier?
%  [trainingSet, ~] = splitEachLabel(imds, 569, 'randomize'); %569 Dman 483 Dman
% 
%  %featureLayer = 'pool10';
%  featureLayer = 'ClassificationLayer_predictions';
%  
% %trainingFeatures = activations(convnet, trainingSet, featureLayer); %This
% %output form is not used
% 
% trainingFeatures = activations(convnet, trainingSet, featureLayer ,'OutputAs','rows');
% 
% classifier = fitcnb(trainingFeatures, trainingSet.Labels); %Train N. Bayes
% 
% rootFolder = 'test_data';
% testSet = imageDatastore(fullfile(rootFolder, categories), 'LabelSource', 'foldernames');
% testSet.ReadFcn = @readFunctionTrain;
% 
% testFeatures = activations(convnet, testSet, featureLayer ,'OutputAs','rows');
% %testFeatures = activations(convnet, testSet, featureLayer);
% predictedLabels = predict(classifier, testFeatures);
% 
% confMat = confusionmat(testSet.Labels, predictedLabels);
% confMat = confMat./sum(confMat,2);
% accuracy = mean(diag(confMat));
% 
% fprintf('Accuracy: %f\n', accuracy);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% End of Test
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% and Train

%Load Pre-built classifier
load NB_Feature_Layer_CLP.mat
load NB_Classifier.mat
% featureLayer = NB_Feature_Layer.mat
% classifier = NB_Classifier.mat

% Read the image to classify. Images are manually called from my PC (Not efficient but it works) 
Im= imread('C:\Users\will_\Pictures\Edge_Safety_Glasses.png'); % place a path to a .png file
%here

imshow(Im)   
I = Im;
I = imresize(I, [227 227]); 

% Classify the image using SqueezeNet, shows vanilla SN classes 
 label = classify(convnet, I); 
 
testFeatures = activations(convnet, I, featureLayer ,'OutputAs','rows');
predictedLabels = predict(classifier, testFeatures)

% Show the image and classification result 
figure 
imshow(I) 
text(10, 20, char(label), 'Color', 'white' )


if(predictedLabels == 'no_hard_hats')
   str = 'NO HARD HATS';

else
    str = 'HARD HAT';
end

text(5, 5, str , 'Color', 'red' )
