function Safety_Compliance_Detector(varargin)  %#codegen
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%Model_Init_CODE    
% 
convnet = squeezenet;

sema = 0; %weights the current reading with previous reading, so only outputs if two frames in a row give same value
    %discourages outliers from affecting the text put on screen

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%Load Model
load NB_Feature_Layer_PTEN.mat
load NB_Classifier2.mat

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%END_Model_Init_CODE

    width = uint32(1280);
    height = uint32(720);
    
    if (nargin ~= 1 && coder.target('MATLAB'))
        error("Invalid parameters. Usage: face_detection_s32v234_camera_main('192.168.1.0')");
    end
    
    if nargin == 1
        ipAddress = varargin{:};
        s32Obj = nxpvt.s32v234(ipAddress);
        % Cleanup after Ctrl+C
        cleanupObj = onCleanup(@() destroyObj(s32Obj));
    end
    
    if coder.target('MATLAB')
        input = nxpvt.cameraboard(s32Obj, 1, 'Resolution', '720x1280');
    else
        input = nxpvt.videoinput('sony', 1, width, height, true, false);
    end
    
     while true

        if coder.target('MATLAB')
            frame = input.snapshot();
        else
            frame = input.getsnapshot();
        end
     
         I = frame.data;

         I = imresize(I, [227 227]);
           
         testFeatures = activations(convnet, I, featureLayer ,'OutputAs','rows');
         predictedLabels = predict(classifier, testFeatures)

         nxpvt.imshow(frame);
         
         if(predictedLabels == 'no_hard_hats')
   
           if(sema == -1)  
                str = 'NO HARD HATS';
           else
              sema = -1;
              str = '';
           end

         else
    if(sema == 1)
    str = 'HARD HATS';
    else
       sema = 1;
       str = '';
    end
end

text(70, 70, str , 'FontSize', 30, 'Color', 'red' )
        
     end
end

function destroyObj(s32Obj)
    % Send Ctrl + C to the board
    s32Obj.disconnect();
end





