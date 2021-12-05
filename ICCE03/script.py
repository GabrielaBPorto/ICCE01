import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import os
import seaborn as sns
expected_sizes = [64, 100, 128, 500, 516,1000, 1024, 2000, 2048, 3000, 4096, 5000]
## expected_sizes = [64, 100, 128, 500]
#expected_sizes = [5,10]

diretorio='./results'

# %%
def getGroupTable(group: str, size: int, columns: list = None) -> pd.DataFrame:
    # Executa likwid no terminal
	./gerarSL $n >>  $diretorio/sistema_$n.in
    cmd = likwid-perfctr -C 3 -g $group -f -m ./gaussJacobi-likwid $diretorio/sistema_$n.in > $diretorio/group.csv
    print (cmd)
    os.system(cmd)

    # Carrega arquivo de resultado
    file_name = f'./Resultados/result{group}_{size}.csv'
    result = open(file_name, 'r')

    # Filtra arquivo de resultado
    tables = {}
    currentTable = 'trash'
    tables[currentTable] = {}
    for line in result.readlines():
        # print(line)
        lineSplit = line.split(',')
        if lineSplit[0] == "TABLE":
            currentTable = lineSplit[1].split()[1]
            if currentTable not in tables:
                tables[currentTable] = {}
        elif lineSplit[0] == "STRUCT":
            currentTable = 'trash'
        else:
            try:
                tables[currentTable][lineSplit[0]] = float(lineSplit[1])
            except:
                tables[currentTable][lineSplit[0]] = lineSplit[1]
    # Remove lixo do lido
    tables.pop('trash')

    # Gera dataframe e seleciona colunas desejadas
    df = pd.DataFrame(tables).T
    if columns:
        df = df[columns]

    df["n_size"] = size
    df = df.set_index([df.index, "n_size"])
    return df


# %%
tabelados = {}
resultados = {}
for size in expected_sizes:
    cmdCurva = f"./gera_entrada {size} > ./Resultados/gerado{size}.in; ./curvas <./Resultados/gerado{size}.in > ./Resultados/gerado{size}.out"
    os.system(cmdCurva)
    tabelados[size]=pd.read_csv(f'./Resultados/gerado{size}.in',skiprows=1, sep=' ', header = None).iloc[:,:-1]
    resultados[size]= pd.read_csv(f'./Resultados/gerado{size}.out', sep=' ', header = None).iloc[:,:-1]
    cmdLikwid= f"./gera_entrada {size} > ./Resultados/gerado{size}.in; ./curvas <./Resultados/gerado{size}.in > ./Resultados/gerado{size}.out"


# %%
testColumns = ['L2 miss ratio', 'Runtime (RDTSC) [s]','call count']+['L3 bandwidth [MBytes/s]']+['DP MFLOP/s', 'AVX DP MFLOP/s']
dfFinal = pd.DataFrame(columns = testColumns,dtype=np.float64)
for size in expected_sizes:
    L2CACHE_table = getGroupTable('L2CACHE', size, ['L2 miss ratio', 'Runtime (RDTSC) [s]','call count'])
    L3_table = getGroupTable('L3', size, ['L3 bandwidth [MBytes/s]'])
    FLOPS_DP_table = getGroupTable('FLOPS_DP', size, ['DP MFLOP/s', 'AVX DP MFLOP/s'])
    dfr = pd.concat([L2CACHE_table,L3_table,FLOPS_DP_table], axis=1)
    dfr["AVG TIME"] = dfr['Runtime (RDTSC) [s]']/dfr['call count']

    dfFinal=dfFinal.append(dfr)

dfFinal[['region', 'n_size']]= dfFinal.index.tolist()
dfFinal.to_csv("./Resultados/tabelaFinal.csv")
for col in testColumns:
    dfFinal.loc[:,col]=dfFinal.loc[:,col].apply(lambda x :float(x))
    try:
        path = os.path.join('./Resultados/',col.replace('/','p').replace(' ','_')+'.png')
        sns.lineplot(data =dfFinal, x='n_size', y =col, hue='region').figure.savefig(path)
        plt.close()
    except:
        pass


# Para cada n esperado
for item in expected_sizes:
    m = len(tabelados[item])-1
    xTab = tabelados[item].loc[0]
    # Para cada função com valor tabelado
    for iM in range(m):
        yTab = tabelados[item].loc[iM+1]
        intervalX=max(xTab)- min(xTab)
        # Margem de 5% no valor do plot do eixo x
        padding=intervalX*0.05
        xs = np.linspace(min(xTab), max(xTab)+padding, 1000)
        df = pd.DataFrame(columns=["interpolador", "ajuste"], index=xs)
        interpol = resultados[item].loc[iM*2]
        ajuste = resultados[item].loc[iM*2+1]
        df['interpolador'] = polynomial_coefficients(xs, interpol)
        df['ajuste'] = polynomial_coefficients(xs, ajuste)
        xsG = np.linspace(-5, 5, 10)
        df.plot(style= ['-','-.'])
	        
        plt.scatter(xTab,yTab, color = 'black',  label='Pontos Tabelados',marker = 'x' )
        plt.legend(loc='best')
        plt.xscale("log")
        plt.title(f"Valores tabelados = {item}, Funçao Tabelada {iM}")
        path = os.path.join('./Resultados/',f'n{item}f[{iM}].png')
        plt.savefig(path)
        plt.close()