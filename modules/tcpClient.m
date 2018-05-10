%%  Start by opening TCP connection with RANA.
pause on
clear;clc;

portnr = 11999;
t = tcpip('localhost', portnr,'TimeOut',1);
fopen(t);

tmp = "t1,";
tmp1 = strcat(tmp,"0");

for i = 1:210010
    dataFromRana = sentToRana(t, tmp1);
end














