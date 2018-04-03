text_file = fopen('1.txt');
t = textscan(text_file, '%f%f', 'Delimiter',',');
fclose(text_file);
x=0:1:200;
y1=t{2};

text_file = fopen('2.txt');
t = textscan(text_file, '%f%f', 'Delimiter',',');
fclose(text_file);
y2=t{2};

text_file = fopen('3.txt');
t = textscan(text_file, '%f%f', 'Delimiter',',');
fclose(text_file);
y3=t{2};

plot(x,y1,'o',x,y2,'o',x,y3,'o')
title('Medidas com carro estacionado sobre a vaga')
xlabel('Amostra')
ylabel('Leitura do sensor')
legend('Com sol','Ao lado de sombra','Bem nublado')
axis([0 200 500 700])
