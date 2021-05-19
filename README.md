Trabalho de ICC

O objetivo é para cada valor e operação realizadas criar um intervalo em que o valor esteja contido dentro.

Tarefa
Faça um programa que leia uma sequência de operações sobre valores reais e retorna os intervalos resultantes. Seu programa também deve informar se alguma operação resultou em um intervalo [a,b] não unitário (a < b). Para essa comparação (e outras que se façam necessárias), devem ser utilizadas estratégias de comparação de ponto flutuante adequadas, conforme visto em aula (epsilon absoluto, epsilon relativo e/ou ULP).

Os valores de entrada do programa devem ser lidos da entrada padrão (stdin) e devem ter o seguinte formato:

m  n
x1    3.1415
x2    0.1
x3    999.999
...
xm   3.4567
x<m+1> = x<i> op x<j>       onde 1 <= i, j <= m    e  op  pode ser +, -, * ou /
...
x<m+n> = x<i> op x<j>       onde 1 <= i, j <= m+n-1   e  op  pode ser +, -, * ou /

Onde m é o número de valores reais conhecidos, definidos nas m primeiras linhas do arquivo, e n é o número de operações que devem ser computadas. Cada operação resulta em um novo valor e os operandos são valores conhecidos ou que foram obtidos anteriormente.

O seu programa deve apresentar na saida padrão (stdout) os intervalos correspondentes de todas as variáveis x1, x2, ..., x<m+n> e uma lista dos intervalos não unitários das variáveis geradas pelas operações (x<m+1> , x<m+2> , ..., x<m+n>).