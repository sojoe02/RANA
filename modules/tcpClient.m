%%  Start by opening TCP connection with RANA.
pause on
clear;clc;

portnr = 11999;
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
clc;clf;clear;
file_start = '../../test_';
file_end = '.csv';

i = 2;
while true
    filepath = strcat(file_start, num2str(i), file_end);
    if exist(filepath,'file') == 0
        i = i -1;
        break;
    end    
    
    i = i +1;
end

for j = 2:i
    
    filepath = strcat(file_start, num2str(j), file_end);
    data = csvread(filepath);
    
    subplot(3,3,j)
    plot(data)
    ylim([-20 130])
    grid on


end
ddata = [];
for i = 2:size(data,1)
    ddata = [ddata; data(i)-data(i-1)];
end

%hold on 
%plot(ddata)
%hold off
grid on















