 clc; clear all; close all;

%image-net database of hard-hats 1365
%  url = 'http://www.image-net.org/api/text/imagenet.synset.geturls?wnid=n03492922';
%  html = webread(url);
% 
% %image-net database of not-hard-hats 1432
%   url = 'http://image-net.org/api/text/imagenet.synset.geturls?wnid=n07942152';
%  html2 = webread(url);
% C = strsplit(html2,'\n');       %splits html into lines 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%START of main

flag = 1;
trained = 0;
featureLayer = 0;
classifier = 0;

while(flag)

    
    fprintf("\nMenu...\n");
    fprintf("         train(tr): to download train data\n");
    fprintf("          test(te): to download test data\n");
    fprintf("          info (i): info about folders\n");
    fprintf("          make (m): makes new directory in test & train folders (not complete: does nothing)\n");
    fprintf("  make convnet (c): calls CNN maker and returns stats (trash model)\n");
    fprintf(" remove images(rm): removes .pngs from selected folders (not complete: deletes all .pngs)\n");
    fprintf('display images (d): displays .png files\n');
    fprintf('get raw images (g): gets binary rgb images to be sent to (c) (only displays & returns first image)\n');
    fprintf("          quit (q): to quit\n");

    skip = input('Enter: ','s');
    fprintf("\n");

switch skip

    case 'tr'

    [x,y] = ui(); % gets user input for train data

    [C,x] = load_data(x); %loads in app. data

    load_images(C,x,y) % loads in train images as .png files

    case 'te'

    [z,v,b] = uit(); % gets user input for test data

    [C,z] = load_data_t(z); %loads in app. data for test

    load_images_t(C,z,v,b) % loads in test images as .png files

    case 'q'
        
        flag = 0; %ends program
        
    case 'i' 
        
        fprintf("test_data/ folder directory:\n");
        dir('test_data/')
        fprintf("train_data/ folder directory:\n");
        dir('train_data/')
          fprintf("train_data_Dman/ folder directory:\n");
        dir('train_data_Dman/')
        
        
        
        d = dir('test_data/hard_hats_Dman');
        fprintf("test_data/hard_hats_Dman .png files: %d\n", length(d) - 2);
        
        d = dir('test_data/no_hard_hats');
        fprintf("test_data/no_hard_hats .png files: %d\n", length(d) - 2);
        
        d = dir('train_data/hard_hats');
        fprintf("train_data/hard_hats .png files: %d\n", length(d) - 2);
        
        d = dir('train_data/no_hard_hats');
        fprintf("train_data/no_hard_hats .png files: %d\n", length(d) - 2);
        
        d = dir('train_data_Dman/hard_hats_Dman');
        fprintf("train_data_Dman/hard_hats_Dman .png files: %d\n", length(d) - 2);
        
        
    case 'm'
         fprintf("Nothing happened\n");
        
    case 'c'
        
       num = input('Number of images to train on: ');
       
       if(exist ('rgb_in'))
           [featureLayer, classifier, trained] = HH_CNN(num, rgb_in, trained, featureLayer, classifier);
       else
           fprintf("ERROR: call (g) first\n");    
       end
       
    case 'rm'
        
        delete('test_data/hard_hats/*.png')
        delete('test_data/no_hard_hats/*.png')
        delete('train_data/hard_hats/*.png')
        delete('train_data/no_hard_hats/*.png')
    
    case 'd'
        
        folder = input('Choose your folder:','s');
        display_img(folder);
        
        
    case 'g'
        
        [z,v,b] = uit(); % gets user input for test data

        [C,z] = load_data_t(z); %loads in app. data for test
    
       rgb_in = get_raw(C,z,v,b);
        
    otherwise
        
    fprintf("\n\nError invalid input, try again...");
end

end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%END of main


function [x,y] = ui() %User Interface
x = input('Train: load hard_hats (Y) or no_hard_hats (N)? ','s');

if(x == 'Y' || x == 'y')
    x = 'Y';
else
    x = 'N';

end

    display('Image Max is ~1365 for HH and 1432 for NHH ');
    y = input('Number of images? ');
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [C,x] = load_data(x)

if (0 == exist('train_data', 'dir')) %Makes train directory
 mkdir('train_data')
end 


if (0 == exist('test_data', 'dir')) %Makes test directory
 mkdir('test_data')
end 


if( strcmp(x,'Y') )

load html;                     %loads saved html
C = strsplit(html,'\n');       %splits html into lines 
x = 'train_data/hard_hats/';

if (0 == exist('train_data/hard_hats', 'dir')) %Makes hard_hat folder
 mkdir('train_data/hard_hats')
end 

else

if (0 == exist('train_data/no_hard_hats', 'dir')) %Makes no_hard_hat folder
 mkdir('train_data/no_hard_hats')

end
 
 load html2;                    %loads saved html2
 C = strsplit(html2,'\n');       %splits html into lines 
 x = 'train_data/no_hard_hats/';
end

end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function void = load_images(C,x,y) 

i = 0;

for k = 1 : y %length(C)
    
% Specify the filename of images (URL).
filename = C{1,k};

if(contains(filename,'.html'))
        continue;
end
        
try
    webread(filename); 
    
catch
    
    continue;
end

% Read it in to a variable in my m-file.
try
 rgbImage = imread(filename,'jpg');
 catch
    
    continue;
end

 %Display the image.
 %imshow(rgbImage);
 
 rgbImage = imresize(rgbImage, [227 227]);
 
% Save the image to a file on my local hard drive.
 i = i + 1;
  imwrite(rgbImage,strcat(x,strcat(num2str(i),'.png')),'png');

end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [z,v,b] = uit() %User Interface fot test

z = input('Test: load hard_hats (Y) or no_hard_hats (N)? ','s');

if(z == 'Y' || z == 'y')
    z = 'Y';
else
     z = 'N';

end

    display('Image Max is ~1365 for HH and 1432 for NHH ');
    v = input('Number of images? ');
    b = input('Starting Index? ');
    
    v = v + b;
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [C,z] = load_data_t(z)

if (0 == exist('train_data', 'dir')) %Makes train directory
 mkdir('train_data')
end 


if (0 == exist('test_data', 'dir')) %Makes test directory
 mkdir('test_data')
end 


if( strcmp(z,'Y') )

load html;                     %loads saved html
C = strsplit(html,'\n');       %splits html into lines 
z = 'test_data/hard_hats/';

if (0 == exist('test_data/hard_hats', 'dir')) %Makes hard_hat folder
 mkdir('test_data/hard_hats')
end 

else

if (0 == exist('test_data/no_hard_hats', 'dir')) %Makes no_hard_hat folder
 mkdir('test_data/no_hard_hats')

end
 
 load html2;                    %loads saved html2
 C = strsplit(html2,'\n');       %splits html into lines 
 z = 'test_data/no_hard_hats/';
end

end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function void = load_images_t(C,z,v,b) 

i = b;

for k = b : v %length(C)
    
% Specify the filename of images (URL).
filename = C{1,k};

if(contains(filename,'.html'))
        continue;
end
        
try
    webread(filename); 
    
catch
    
    continue;
end

% Read it in to a variable in my m-file.
try
 rgbImage = imread(filename,'jpg');
 catch
    
    continue;
end

 %Display the image.
 %imshow(rgbImage);
 rgbImage = imresize(rgbImage, [227 227]);
 
% Save the image to a file on my local hard drive.
 i = i + 1;
  imwrite(rgbImage,strcat(z,strcat(num2str(i),'.png')),'png');

end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function void = display_img(folder)

 d = dir(folder);
 len =  length(d) - 2;
 
 for i = 1 : len
     
     imshow(strcat(folder,d(i+2).name));
     
 end

end
 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function rgbImage = get_raw(C,z,v,b) 

i = 0;

for k = b : v %length(C)
    
% Specify the filename of images (URL).
filename = C{1,k};

if(contains(filename,'.html'))
        continue;
end
        
try
    webread(filename); 
    
catch
    
    continue;
end

% Read it in to a variable in my m-file.
try
 rgbImage = imread(filename,'jpg');
 catch
    
    continue;
end

 %Display the image.
 imshow(rgbImage);
 
 i = i + 1;
 
 rgbImage = imresize(rgbImage, [227 227]);
 break;
 
 %imwrite(rgbImage,strcat(z,strcat(num2str(i),'.png')),'png');

end
end




