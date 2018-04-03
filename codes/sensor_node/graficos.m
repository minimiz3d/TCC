text_file = fopen('carro_ao_lado_de_somra.txt');
t = textscan(text_file, '%f%f', 'Delimiter',',');
fclose(text_file);
x=t{1};
y=t{2};
figure(1)
plot(x, y, 'o')
grid
title('Carro ao lado de sombra')
xlabel('Amostra')
ylabel('Leitura do sensor')

text_file = fopen('carro_nubladao.txt');
t = textscan(text_file, '%f%f', 'Delimiter',',');
fclose(text_file);
x=t{1};
y=t{2};
figure(2)
plot(x, y, 'o', 'color', 'm')
grid
title('Carro com sombra')
xlabel('Amostra')
ylabel('Leitura do sensor')

text_file = fopen('teste1_oficial_com_carro.txt');
t = textscan(text_file, '%f%f', 'Delimiter',',');
fclose(text_file);
x=t{1};
y=t{2};
figure(3)
plot(x, y, 'o', 'color', 'g')
grid
title('Carro normal')
xlabel('Amostra')
ylabel('Leitura do sensor')
