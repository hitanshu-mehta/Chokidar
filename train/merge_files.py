import pandas as pd
import glob

wd = "../"
cicids_files = "dataset/CICIDS2017/MachineLearningCVE/*.csv"

print("Reading CICIDS2017 data...")
files = glob.glob(wd+cicids_files)
df = []
f_cnt = 0
for ff in files:
    print(ff)
    print(f_cnt)
    f_cnt += 1
    df.append(pd.read_csv(ff, encoding="Latin1"))
df = pd.concat(df,ignore_index=True)

# size_mapping = {'BENIGN':0, 'FTP-Patator':1,'SSH-Patator':1,'DoS Hulk':2, 'DoS GoldenEye':2,'DoS slowloris':2,'DoS Slowhttptest':2,'Heartbleed':2,'Web Attack � Brute Force':3,'Web Attack � XSS':3,'Web Attack � Sql Injection':3,'Infiltration':4,'Bot':5,'PortScan':6,'DDoS':7}
# df[' Label'] = df[' Label'].map(size_mapping)
# pd.set_option('mode.use_inf_as_na', True) 
# df['Flow Bytes/s']=df['Flow Bytes/s'].astype('float64')
# df[' Flow Packets/s']=df[' Flow Packets/s'].astype('float64')
# df['Flow Bytes/s'].fillna(df['Flow Bytes/s'].mean(),inplace=True)
# df[' Flow Packets/s'].fillna(df[' Flow Packets/s'].mean(),inplace=True)

# df = pd.read_csv('../dataset/merged_cicids2017.csv')
# df = df.drop('Unnamed: 0',axis = 1)

df.to_csv('../dataset/merged_cicids2017_1.csv',index = False,mode ='w')