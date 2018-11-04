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
numberOfSensors=8;
nextSampleTime=0;                         %Time ellapsed from previous sample
numberOfSamples=1;                        %Number of samples of the data read
startSample = 0;                          %Starting sample (where plot starts and continues)
endSample = 120000;                       %number of samples (120 seconds)
emgData=zeros(numberOfSensors,endSample); %emgData array
dt = 100;                                 %Time between samples in ms
tic 
while numberOfSamples< endSample
        
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
            dataRead = zeros(1,numberOfSensors)';
        end
        
        %Array which stores all the emg data
        emgData(:,numberOfSamples) = dataRead;
        
        %Create axis for plots
        if (numberOfSamples < 125)
          startSample = 0;
        else
          startSample = numberOfSamples-125;
        end
        
        %First Plot Emg Sensor 1
        subplot(4,2,1)
        plot(emgData(1,1:numberOfSamples));
        title('Emg Sensor 1 Data');
        axis([ startSample, startSample+150, -50 , 50 ]);
        
        %Second Plot Emg Sensor 2
        subplot(4,2,2)
        plot(emgData(2,1:numberOfSamples));
        title('Emg Sensor 2 Data');
        axis([ startSample, startSample+150, -50 , 50 ]);
        
        %Third Plot Emg Sensor 3
        subplot(4,2,3)       
        plot(emgData(3,1:numberOfSamples));
        title('Emg Sensor 3 Data');
        axis([ startSample, startSample+150, -50 , 50 ]);
        
        %Fourth Plot Emg Sensor 4
        subplot(4,2,4)      
        plot(emgData(4,1:numberOfSamples));
        title('Emg Sensor 4 Data');
        axis([ startSample, startSample+150, -50 , 50 ]);
        
        %Fifth Plot Emg Sensor 5
        subplot(4,2,5)      
        plot(emgData(5,1:numberOfSamples));
        title('Emg Sensor 5 Data');
        axis([ startSample, startSample+150, -50 , 50 ]);
        
        %Sixth Plot Emg Sensor 6
        subplot(4,2,6)      
        plot(emgData(6,1:numberOfSamples));
        title('Emg Sensor 6 Data');
        axis([ startSample, startSample+150, -50 , 50 ]);
        
        %Seventh Plot Emg Sensor 7
        subplot(4,2,7)       
        plot(emgData(7,1:numberOfSamples));
        title('Emg Sensor 7 Data');
        axis([ startSample, startSample+150, -50 , 50 ]);
        
        %Eigth Plot Emg Sensor 8
        subplot(4,2,8)     
        plot(emgData(8,1:numberOfSamples));
        title('Emg Sensor 8 Data');
        axis([ startSample, startSample+150, -50 , 50 ]);
        
        numberOfSamples=numberOfSamples+1;
        
        drawnow
    end
    nextSampleTime=toc;
end

%% Close serial port

%Close serial port opened
fclose(portDesc);
delete(portDesc);