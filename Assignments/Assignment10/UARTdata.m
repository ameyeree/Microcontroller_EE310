% If any connections are open, close them
close all;
clear all;

%open serial port
s = serialport("COM3",9600,"Timeout",5);
%fopen(s);

index = 1;
while(1)
    data(index) = read(s,1,"int8");
    plot(data);
    title('UART Data Graph');
    xlabel('Reading Number');
    ylabel('Number Read');
    pause(1);
    index = index + 1;
end


