import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import os
import seaborn as sns
expected_sizes = [10, 32]
# , 50, 64, 100, 128, 200, 250, 256, 300, 400, 512, 600, 1000, 1024, 2000, 2048, 3000, 4096]

#diretorio='/nobackup/ibm/gbp16/nobackup'
diretorio='./results'

def getGroupTable(group: str, columns: list = None) -> pd.DataFrame:

    cmd = f"likwid-perfctr -C 3 -g { group } -o { diretorio }/{ group }.csv -f -m ./trab1 -o { diretorio }/resultado_{ group }.res < { diretorio }/sistemas.res"
    print (cmd)
    os.system(cmd)

    # Carrega arquivo de resultado
    file_name = f"{diretorio}/{ group }.csv"
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
    print(tables)

    # Gera dataframe e seleciona colunas desejadas
    # df = pd.DataFrame(tables).T
    # if columns:
        # df = df[columns]

    # df["n_size"] = size
    # df = df.set_index([df.index, "n_size"])
    #  return df

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
testColumns = ['L2 miss ratio', 'Runtime (RDTSC) [s]','call count']+['L3 bandwidth [MBytes/s]']+['DP MFLOP/s', 'AVX DP MFLOP/s']
dfFinal = pd.DataFrame(columns = testColumns,dtype=np.float64)
# for size in expected_sizes:
cmdEntrada = f"./broyden.sh > {diretorio}/sistemas.res"
print (cmdEntrada)
os.system(cmdEntrada)
L2CACHE_table = getGroupTable('L2CACHE',  ['L2 miss ratio', 'Runtime (RDTSC) [s]','call count'])
L3_table = getGroupTable('L3',  ['L3 bandwidth [MBytes/s]'])
FLOPS_DP_table = getGroupTable('FLOPS_DP',  ['DP MFLOP/s', 'AVX DP MFLOP/s'])
# dfr = pd.concat([L2CACHE_table,L3_table,FLOPS_DP_table], axis=1)
# dfr["AVG TIME"] = dfr['Runtime (RDTSC) [s]']/dfr['call count']

#dfFinal=dfFinal.append(dfr)

# dfFinal[['region', 'n_size']]= dfFinal.index.tolist()
# dfFinal.to_csv(f"{diretorio}/tabelaFinal.csv")
# for size in expected_sizes:
#     dfSize = dfFinal.loc[dfFinal['n_size'] == size]
#     for col in testColumns:
#         if(col == "call count"):
#             continue
#         dfSize.loc[:,col].apply(lambda x :float(x))
#         try:
#             path = os.path.join(f'{diretorio}/',col.replace('/','p').replace(' ','_')+'_'+str(size)+'.png')
#             plt.figure(figsize=(14, 7))
#             g = sns.barplot(data =dfSize, x='region', y =col, hue='region')
#             g.legend_.remove()
#             if (col == "Runtime (RDTSC) [s]"):
#                 g.set_yscale('log')
#             g.figure.savefig(path)            
#             plt.close()
#         except:
#             print(path + " deu ruim")
#             pass


cmdPowerSave = "echo \"powersave\" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor"
os.system(cmdPowerSave)

cmdMake = f"make clean; make purge faxina"
print (cmdMake)
os.system(cmdMake)