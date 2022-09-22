data_RSSI = readtable('H:\Meu Drive\UTFPR\2021.1\TCC1\Codigos\Save_Data_experimento_v1\primeiro_cenario_400.csv');

distance = [.3 .5 1 2 4 6 9];

RSSI_mean = zeros(1,length(distance));
for i = 1:length(distance)
    RSSI_mean(i) = mean(data_RSSI{:,i},'omitnan');
end

figure(1);
scatter(distance,RSSI_mean);

x = 10*log10(distance);
y = RSSI_mean;

f = fitlm(x,y);

n_Fitting = f.Coefficients{2,1};
b_Fitting = f.Coefficients{1,1};
PL_FITTING = b_Fitting + 10*n_Fitting*log10(distance); 