text_file = fopen('7.txt');
t = textscan(text_file, '%f%f', 'Delimiter',',');
fclose(text_file);
x=0:1:300;
y1=t{2};

text_file = fopen('8.txt');
t = textscan(text_file, '%f%f', 'Delimiter',',');
fclose(text_file);
y2=t{2};

plot(x,y1,'o',x,y2,'o')
title('Medidas com carros nas vagas - 8:40 am')
xlabel('Amostra')
ylabel('Leitura do sensor')
leg = legend('Carro 1','Carro 2')
set(leg,'location', 'south')
axis([0 300 400 800])
