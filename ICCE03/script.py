import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import os
import seaborn as sns
expected_sizes = [10, 32]
## expected_sizes = [64, 100, 128, 500]
#expected_sizes = [5,10]

diretorio='./results'

# %%
def getGroupTable(group: str, size: int, columns: list = None) -> pd.DataFrame:
    # Executa likwid no terminal

    cmd = f"likwid-perfctr -C 3 -g { group} -o { diretorio}/{ size }.{ group}.csv -f -m ./gaussJacobi-likwid { diretorio}/sistema_{ size }.res"
    print (cmd)
    os.system(cmd)

    # Carrega arquivo de resultado
    file_name = f"{diretorio}/{ size }.{ group}.csv"
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

cmdMake = f"make"
print (cmdMake)
os.system(cmdMake)

# %%
testColumns = ['L2 miss ratio', 'Runtime (RDTSC) [s]','call count']+['L3 bandwidth [MBytes/s]']+['DP [MFLOP/s]', 'AVX DP [MFLOP/s]']
dfFinal = pd.DataFrame(columns = testColumns,dtype=np.float64)
for size in expected_sizes:
    cmdEntrada = f"./geraSL {size} > results/sistema_{size}.res"
    print (cmdEntrada)
    os.system(cmdEntrada)
    L2CACHE_table = getGroupTable('L2CACHE', size, ['L2 miss ratio', 'Runtime (RDTSC) [s]','call count'])
    L3_table = getGroupTable('L3', size, ['L3 bandwidth [MBytes/s]'])
    FLOPS_DP_table = getGroupTable('FLOPS_DP', size, ['DP [MFLOP/s]', 'AVX DP [MFLOP/s]'])
    dfr = pd.concat([L2CACHE_table,L3_table,FLOPS_DP_table], axis=1)
    dfr["AVG TIME"] = dfr['Runtime (RDTSC) [s]']/dfr['call count']

    dfFinal=dfFinal.append(dfr)

dfFinal[['region', 'n_size']]= dfFinal.index.tolist()
dfFinal.to_csv("./results/tabelaFinal.csv")
for size in expected_sizes:
    dfSize = dfFinal.loc[dfFinal['n_size'] == size]
    print(dfSize)
    for col in testColumns:
        if(col == "call count"):
            continue
        dfSize.loc[:,col]=dfSize.loc[:,col].apply(lambda x :float(x))
        try:
            path = os.path.join('./results/',col.replace('/','p').replace(' ','_')+'_'+str(size)+'.png')
            plt.figure(figsize=(14, 7))
            g = sns.barplot(data =dfSize, x='region', y =col, hue='region')
            g.legend_.remove()
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