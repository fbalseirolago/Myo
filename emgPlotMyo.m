clc;
clear;
%%Open Serial Port & print out data

%Instance of serial port object
serialPort = serial('/dev/tty.SLAB_USBtoUART','BaudRate',115200);

%Open serial port instanced
fopen(serialPort);

%Acquire description of port object
portDesc = instrfind('Port','/dev/tty.SLAB_USBtoUART');

%Plot-graph variables
nextSampleTime=0;           %Time ellapsed from previous sample
numberOfSamples=1;          %Number of samples of the data read
startSample = 0;            %Starting sample (where plot starts and continues)
endSample = 120000;         %number of samples (120 seconds)
emgData=zeros(1,endSample); %emgData array
dt = 100;                   %Time between samples in ms
tic 
while numberOfSamples< endSample
        
    if (nextSampleTime > 0.150)
        tic
        %Cleans input buffer of the serial port 
        %(prevents possible overflows)
        flushinput(serialPort);
        
        %tic
        %Reads the data from the input buffer
        dataRead = fscanf(serialPort,'%d',[1 1])
        %toc
        
        %Array which stores all the emg data
        emgData(numberOfSamples) = dataRead;
        
        %tic
        plot(emgData(1:numberOfSamples));
        
        if (numberOfSamples < 125)
          startSample = 0;
        else
          startSample = numberOfSamples-125;
        end
        
        axis([ startSample, startSample+150, 0 , 255 ]);
        
        numberOfSamples=numberOfSamples+1;
        
        drawnow
        %toc 
    end
    nextSampleTime=toc;
end

%% Close serial port

%Close serial port opened
fclose(portDesc);
delete(portDesc);