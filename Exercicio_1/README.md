Enunciado
Neste exercício você está convidado a implementar um experimento para comparar as iterações dos métodos Newton-Raphson e Secante na resolução de equações não lineares.

Faça um programa em C que leia da entrada padrão os seguintes dados:

uma string contendo a descrição de uma função f(x);
uma aproximação inicial x0 (double);
uma precisão mínima epsilon (double);
o número máximo de iterações max_iter (int).
Para processar a função f(x) e sua derivada, você deve utilizar a biblioteca libmatheval (veja instruções de instalação e compilação no final deste enunciado).

Seu programa deve aplicar ambos os métodos para encontrar o zero da função f(x) com precisão epsilon e gerar na saída padrão uma tabela (em formato .csv) com as seguintes colunas:

iteração: número da iteração i;
newton_x: aproximação obtida pelo método de Newton-Raphson na iteração i;
newton_crit: valor do critério de parada do método de Newton-Raphson calculado na iteração i;
secante_x: aproximação obtida pelo método da Secante na iteração i;
secante_crit: valor do critério de parada do método da Secante calculado na iteração i;
EA: erro absoluto da aproximação obtida pelo método da Secante na iteração i em relação à aproximação obtida pelo método de Newton-Raphson na mesma iteração;
ER: erro relativo da aproximação obtida pelo método da Secante na iteração i em relação à aproximação obtida pelo método de Newton-Raphson na mesma iteração;
ULPs: quantidade de ULPs da aproximação obtida pelo método da Secante na iteração i em relação à aproximação obtida pelo método de Newton-Raphson na mesma iteração (Obs: lembre-se que neste exercício os valores são do tipo double, ou seja, 64 bits);
Considerando o que foi estudado na disciplina, proponha um critério de parada para os métodos. Além do critério proposto, considere também como critério de parada o máximo de max_iter iterações. Quando o critério de parada proposto for atingido por um dos métodos, o valores das colunas deste método devem ser repetidos para todas as iterações restantes. Quando ambos os métodos atingirem o critério de parada, o programa deve terminar, sem imprimir as demais iterações.

 

No método da Secante, considere x1 sendo a aproximação obtida pelo método de Newton-Raphon na primeira iteração.

 

Para a impressão de doubles, utilize a flag "%1.16e".

 

O programa deve ser implementado de forma que possa ser compilado com o comando

$ make

E executado com o comando

$ ./ep01 < sample.in > saida.csv

 

Exemplo de Execução
Para fins de simplificação, aqui foi usado como critério de parada apenas o valor absoluto de f(x), isto é, |f(x)| < epsilon. Um critério mais elaborado deve ser considerado na implementação final.

Entrada:

x^2-2

1

1e-09

20

Saída:

0, 1.0000000000000000e+00, 1.0000000000000000e+00, 1.0000000000000000e+00, 1.0000000000000000e+00, 0.0000000000000000e+00, 0.0000000000000000e+00, 0

1, 1.5000000000000000e+00, 2.5000000000000000e-01, 1.5000000000000000e+00, 2.5000000000000000e-01, 0.0000000000000000e+00, 0.0000000000000000e+00, 0

2, 1.4166666666666667e+00, 6.9444444444446418e-03, 1.3999999999999999e+00, 4.0000000000000258e-02, 1.6666666666666829e-02, 1.1764705882353056e-02, 1145324612

3, 1.4142156862745099e+00, 6.0073048828712672e-06, 1.4137931034482758e+00, 1.1890606420930094e-03, 4.2258282623408583e-04, 2.9881073328164124e-04, 473346632

4, 1.4142135623746899e+00, 4.5106141044470860e-12, 1.4142156862745099e+00, 6.0073048828712672e-06, -2.1238998200168169e-06, 1.5018239652930854e-06, 975259845

5, 1.4142135623746899e+00, 4.5106141044470860e-12, 1.4142135620573204e+00, 8.9314555751229818e-10, 3.1736946404237187e-10, 2.2441410016564823e-10, 1429304

 

Libmatheval
 

A biblioteca para processamento de funções pode ser acessada por este link. Exemplos de uso estão disponíveis aqui.

Para a instação, execute:
$ sudo apt-get update

$ sudo apt-get install libmatheval-dev

Para a compilação, por exemplo:
$ gcc ep01.c -lm -I/usr/include -L/usr/lib -lmatheval

 

Entrega
 

Este exercício deve ser elaborado em equipes de até 2 alunos. Apenas um dos alunos da equipe deve efetuar a entrega dos arquivos. Devem ser entregues os códigos-fonte do programa (em linguagem C), makefile, arquivos de teste usados pela equipe e um arquivo LEAIME.txt contendo as seguintes informações:

Nomes, GRR e login DInf dos alunos da equipe;
Justificativa do critério de parada implementado;
Demais informações relevantes sobre o código e execução.
Todos estes itens devem ser compactados em um arquivo do tipo .tar , .tgz ou .zip.