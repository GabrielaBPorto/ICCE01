import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import os
import seaborn as sns
expected_sizes = [10, 32]
#, 50, 64, 100, 128, 200, 250, 256, 300, 400, 512, 600, 1000, 1024, 2000, 2048, 3000, 4096]
#diretorio='/nobackup/ibm/gbp16/nobackup'
diretorio='./results'

# %%
def getGroupTable(group: str, size: int, version: str, columns: list = None) -> pd.DataFrame:
    # Executa likwid no terminal
    if(version == 'opt'):
        cmd = f"likwid-perfctr -C 3 -g { group } -o { diretorio }/{ size }.{ group }.Opt.csv -f -m ./principalOpt  { diretorio }/sistemas_{ size }.res"    
        file_name = f"{diretorio}/{ size }.{ group}.Opt.csv"
    
    else:
        cmd = f"likwid-perfctr -C 3 -g { group } -o { diretorio }/{ size }.{ group }.csv -f -m ./principal  { diretorio }/sistemas_{ size }.res"
        file_name = f"{diretorio}/{ size }.{ group}.csv"
    
    
    print (cmd)
    os.system(cmd)

    # Carrega arquivo de resultado
    # file_name = f"{diretorio}/{ size }.{ group}.csv"
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

cmdPerformance = "echo \"performance\" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor"
os.system(cmdPerformance)

cmdRM = f"rm -rf {diretorio}"
os.system(cmdRM)
cmdMkDir = f"mkdir {diretorio}"
os.system(cmdMkDir)

cmdMake = f"make avx"
print (cmdMake)
os.system(cmdMake)

# %%
markers = ['StartNewton', 'AlocVariavel', 'LeVariavel', 'EscreveParciais', 'MetodoNewton', 'MatrizJacobiana', 'Max', 'Pivoteamento', 'EliminacaoGauss', 'ResultadoJacobiana', 'LiberaMemoria', 'ResultadoNewton']
testColumns = ['L2 miss ratio', 'Runtime (RDTSC) [s]','call count']+['L3 bandwidth [MBytes/s]']+['DP [MFLOP/s]', 'AVX DP [MFLOP/s]', 'AVG TIME']
dfFinal = pd.DataFrame(columns = testColumns,dtype=np.float64)
for size in expected_sizes:
    cmdEntrada = f"./broyden.sh {size} > {diretorio}/sistemas_{size}.res"
    print (cmdEntrada)
    os.system(cmdEntrada)
    for version in ['normal','opt']:
        L2CACHE_table = getGroupTable('L2CACHE', size, version, ['L2 miss ratio', 'Runtime (RDTSC) [s]','call count'])
        L3_table = getGroupTable('L3', size, version, ['L3 bandwidth [MBytes/s]'])
        FLOPS_DP_table = getGroupTable('FLOPS_DP', size, version, ['DP [MFLOP/s]', 'AVX DP [MFLOP/s]'])
        dfr = pd.concat([L2CACHE_table,L3_table,FLOPS_DP_table], axis=1)
        dfr["AVG TIME"] = dfr['Runtime (RDTSC) [s]']/dfr['call count']

        dfFinal=dfFinal.append(dfr)

print('Iniciando criação das imagens')
dfFinal[['region', 'n_size']]= dfFinal.index.tolist()
dfFinal.to_csv(f"{diretorio}/tabelaFinal.csv")
for marker in markers:
    dfMarker = dfFinal.loc[dfFinal['region'].str.contains(marker)]
    for col in testColumns:
        if(col == "call count" or col == 'Runtime (RDTSC) [s]'):
            continue
        dfMarker.loc[:,col].apply(lambda x :float(x))
        try:
            path = os.path.join(f'{diretorio}/',col.replace('/','p').replace(' ','_')+f'_{ marker }'+'.png')
            plt.figure(figsize=(14, 7))
            g = sns.lineplot(data =dfMarker, x='n_size', y =col, hue='region')
            if (col == "AVG TIME"):
                g.set_yscale('log')
            g.figure.savefig(path)            
            plt.close()
        except:
            print(path + " deu ruim")
            pass


cmdPowerSave = "echo \"powersave\" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor"
os.system(cmdPowerSave)

cmdMake = f"make clean; make purge faxina"
print (cmdMake)
os.system(cmdMake)