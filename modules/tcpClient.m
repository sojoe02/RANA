%%  Start by opening TCP connection with RANA.
pause on
clear;clc;

portnr = 11995;
t = tcpip('localhost', portnr);
fopen(t);

%%  Talk with the server
dataFromRana = sentToRana(t, 't1,0')
dataFromRana = sentToRana(t, 'done\n')
dataFromRana = sentToRana(t, 't1,1')
dataFromRana = sentToRana(t, 'done\n')
dataFromRana = sentToRana(t, 't1,0')
dataFromRana = sentToRana(t, 'done\n')
%%
for i = 1:1110
    i
    %tmp = strcat(stringToRana,num2str(i))
    %dataFromRana = sentToRana(t, tmp)
    dataFromRana = sentToRana(t, 'done\n')
    %pause(0.001)
end

%%
data = csvread('../../test.csv');
plot(data)