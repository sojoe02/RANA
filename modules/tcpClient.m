%%  Start by opening TCP connection with RANA.
pause on
clear;clc;

portnr = 11995;
t = tcpip('localhost', portnr,'TimeOut',1);
fopen(t);

%%  Talk with the server
dataFromRana = sentToRana(t, 't1,1')
%%
tmp = "t1,";

for i = 1:101000
    if i == 1 
        tmp1 = strcat(tmp,"10");
        dataFromRana = sentToRana(t,tmp1);
    elseif i == 5000
        tmp1 = strcat(tmp,"0")
        dataFromRana = sentToRana(t,tmp1);
    elseif i == 10000
        tmp1 = strcat(tmp,"30");
        dataFromRana = sentToRana(t,tmp1);
    elseif i == 12000
        tmp1 = strcat(tmp,"0");
        dataFromRana = sentToRana(t,tmp1);
    elseif i == 15000
        tmp1 = strcat(tmp,"50");
        dataFromRana = sentToRana(t,tmp1);
    elseif i == 25000
        tmp1 = strcat(tmp,"5");
        dataFromRana = sentToRana(t,tmp1);
    elseif i == 35000
        tmp1 = strcat(tmp,"70");
        dataFromRana = sentToRana(t,tmp1);
    elseif i == 40000
        tmp1 = strcat(tmp,"10");
        dataFromRana = sentToRana(t,tmp1);
    else
        dataFromRana = sentToRana(t, 'done\n');
    end

end

%%
data = csvread('../../test.csv');
plot(data)