clc;
clear;

%%Input from user
str ='';

while  (isempty(str))
    prompt = 'Enter the name of the file where data is stored: ';
    str = input(prompt,'s');
end

fileName = strcat(str,'.mat');

%%Open Serial Port & print out data

%Instance of serial port object
serialPort = serial('/dev/tty.SLAB_USBtoUART','BaudRate',115200);

%Open serial port instanced
fopen(serialPort);

%Acquire description of port object
portDesc = instrfind('Port','/dev/tty.SLAB_USBtoUART');

%Plot-graph variables
numberOfSensors=8;
nextSampleTime=0;                         %Time ellapsed from previous sample
numberOfSamples=1;                        %Number of samples of the data read
%endSample = 120000;                       %number of samples (120 seconds)
%emgData=zeros(numberOfSensors,endSample); %emgData array

%Configure dialogue box to exit program
ButtonHandle = uicontrol('Style', 'PushButton', ...
                         'String', 'Stop loop', ...
                         'Callback', 'delete(gcbf)');
                                      
tic 
while (ishandle(ButtonHandle))
        
    if (nextSampleTime > 0.150)
        tic
        %Cleans input buffer of the serial port 
        %(prevents possible overflows)
        flushinput(serialPort);
        
        %Reads the data from the input buffer
        dataRead = fscanf(serialPort, "%d");
        
        [rowDataRead,colDataRead] = size(dataRead);
        
        %If any sensor data lost => place a 0
        if (rowDataRead < 8)
            dataRead = lastDataRead;
            disp 'data lost'
        end
        
        %Array which stores all the emg data
        emgData(:,numberOfSamples) = dataRead;
        
        state=net(dataRead);
        
        if state<0.5
            disp 'Open'
        else
            disp 'Closed'
        end
        
        numberOfSamples=numberOfSamples+1;
        
        drawnow
        
        lastDataRead = dataRead;
    end
    nextSampleTime=toc;
end

%% Close serial port

%Close serial port opened
fclose(portDesc);
delete(portDesc);

disp 'Program Terminated...'

prompt = 'Do you want to save the data? Y/N [Y]: ';
saveResults = input(prompt,'s');
if isempty(saveResults)
    saveResults = 'Y';
end

if strcmp('Y', saveResults)
    save(fileName,'emgData')
end