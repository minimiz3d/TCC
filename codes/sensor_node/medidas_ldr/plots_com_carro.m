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
[maxy,idx]=max(y3);

plot(x,y1,'o',x,y2,'o',x,y3,'o',x(idx),y3(idx),'pg')
title('Medidas com carro estacionado sobre a vaga')
xlabel('Amostra')
ylabel('Leitura do sensor')
leg = legend('Com sol','Ao lado de sombra','Bem nublado',sprintf('Valor maximo = %0.3f',maxy))
set(leg,'location', 'south')
axis([0 200 450 700])
