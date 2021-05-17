1. Verificar intervalos inválidos após realizar as operações na função leituraOperacoes
	1.1. Trocar continues por breaks para poder ir para a verificação
	1.2. Criar função auxiliar para realizar verificação de acordo com o princípio da responsabilidade
	1.3. Alterar o retorno da função leituraOperacoes para int, pois caso dê erro na verificação, 
			devemos retornar -1 e encerrar a execução atual do programa
	1.4. Realizar verificação do retorno desta função no main com o mesmo objetivo da anterior
2. Implementar função para verificar se alguma operação retorna um intervalo não unitário
	2.1. A função deve implementar uma das estratégias de Epsilon e/ou ULP para comparar operadores
			de ponto flutuante (i.e., comparação de valores)
	2.2. Caso um intervalo não seja unitário, deve ser printado novamente.
	2.3. Importante printar uma linha avisando 'Intervalos não unitários' ao entrar na função
3. Gerar script para testar o código com todos os casos de teste [DONE]
	3.1. Pegar todos os .in disponíveis em https://moodle.c3sl.ufpr.br/mod/folder/view.php?id=27968 [DONE]
	3.2. Realizar script, como os que fizemos para ML, para rodar todos os testes [DONE]
	3.3. IMPORTANTE: Os resultados não precisam estar exatos com os dos professores pois diferenças de
			implementação podem levar a algumas diferenças de valores
4. Criar arquivos .c e .h para que o arquivo principal contenha apenas o fluxo de execução do código [DONE]
	4.1. Atualizar o makefile para fazer uso destes arquivos. [DONE]



Enunciado
A Análise Intervalar pode ser usada para computar operações básicas de forma rigorosa. A ideia é representar valores reais por intervalos de comprimento mínimo que contêm esses valores, ao invés de utilizar aproximações (para o número de máquina mais próximo, por exemplo). Deste modo, operações sobre esses números resultam em intervalos que carregam os erros numéricos de forma explícita, ao mesmo tempo que garantem a existência da solução real.

Seja x um valor real qualquer. A representação intervalar de x é dada por X = [m(x), M(x)], onde m(x) é o maior número de máquina menor ou igual a x e M(x) é o menor número de máquina maior ou igual a x.

Seja X = [a,b] e Y = [c,d]. As operações básicas intervalares são dadas por:

X + Y = [a+c, b+d]

X - Y = [a-d, b-c]

X * Y = [a,b] * [c,d]  =  [min{a*c,a*d,b*c,b*d}, max{a*c,a*d,b*c,b*d}]

X / Y = [a,b] * [1/d,1/c], se 0 não pertence ao intervalo Y

Para simplificar, considere que se 0 (zero)  pertence a Y, então X / Y =[-inf,+inf], para qualquer intervalo X.

É importante notar que os limitantes dos intervalos resultantes devem ser representados na máquina de forma rigorosa. Assim, na prática:

X + Y = [ m(a+c), M(b+d) ]

X - Y = [ m(a-d), M(b-c) ]

E assim por diante...

                            

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

OBS.: Como uma operação pode resultar em [-inf, inf], casos particulares de operações aritméticas envolvendo estes operandos devem ser tratados com cuidado. Além disso, se em qualquer momento um intervalo inválido for gerado, o programa deve encerrar imediatamente, retornando -1. Exemplos de intervalos inválidos são: [2, 1], [-inf, -inf], [+inf, -inf], [+inf, +inf], [nan,nan], etc. O mesmo vale para cálculo de intervalos em que ocorram overflow ou underflow.

Exemplo (o seu programa deve funcionar apenas para o formato IEEE 754, mas aqui, para exemplificar, vamos supor uma máquina decimal com 3 dígitos na mantissa):

Entrada:

3 2
x1   3.1415
x2   1.2345
x3   -1.45
x4 = x1 + x2
x5 = x4 * x3

Saída:

X1 = [3.14, 3.15]
X2 = [1.23, 1.24]
X3 = [-1.45, -1.44]
X4 = [4.37, 4.39]
X5 = [-6.36, -6.30]
nao unitarios:
X4 = [4.37, 4.39]
X5 = [-6.36, -6.30]

 

Explicação do exemplo:

Representação intervalar dos valores x1, x2, x3 na máquina (observe que x3 já era naturalmente representável na máquina, mas a estratégia implementada sempre limita o valor de entrada por 2 números de máquina consecutivos):
X1 = [3.14, 3.15]
X2 = [1.23, 1.24]
X3 = [-1.45, -1.44]


Computando a operação x4 = x1 + x2:
X4 = X1 + X2 = [m(3.14+1.23), M(3.15+1.24)] = [4.37, 4.39]

Computando a operação x5 = x4 * x3:
X5 = X4 * X3 = [ min{m(4.37*(-1.45)), m(4.37*(-1.44)), m(4.39*(-1.45)), m(4.39*(-1.44))}, max{M(4.37*(-1.45)), M(4.37*(-1.44)), M(4.39*(-1.45)), M(4.39*(-1.44)) ]

                        = [ m(4.39*(-1.45)), M(4.37*(-1.44)) ]

                        = [ m(-6.3655), M(-6.2928) ]

                        = [ -6.36, -6.30 ]



Computando intervalos não unitários:
Neste exemplo, vamos considerar que dois números de máquina são iguais se não existe nenhum número representável entre eles. Neste caso, os limitantes dos intervalos X4 e X5 são diferentes e, portanto, X4 e X5 não são unitários.

O que deve ser entregue
Devem ser entregues os códigos-fonte do programa (em linguagem C), makefile e arquivos de teste usados pelo aluno. Todos estes itens devem ser compactados em um arquivo do tipo .tar , .tgz ou .zip .