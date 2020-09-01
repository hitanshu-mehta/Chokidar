import pandas as pd 
import numpy as np 

data_path = './dataset/CICIDS2017/TrafficLabelling /'
date_col = [' Timestamp']

monday = pd.read_csv(data_path+'Monday-WorkingHours.pcap_ISCX.csv',encoding="Latin1")
df2 = pd.read_csv('dataset/CICIDS2017/MachineLearningCVE/Friday-WorkingHours-Afternoon-DDos.pcap_ISCX.csv')
not_com = np.setdiff1d(monday.columns,df2.columns)
not_com_with_dst_ip = not_com.copy()
not_com_with_dst_ip = np.delete(not_com_with_dst_ip,0)
print(not_com_with_dst_ip)

final_df = monday[0:100000].copy()

tuesday = pd.read_csv(data_path+'Tuesday-WorkingHours.pcap_ISCX.csv',encoding="Latin1")

final_df = final_df.append(tuesday[tuesday[' Label'] == 'FTP-Patator'],ignore_index = True)
final_df = final_df.append(monday[100001:150000],ignore_index = True)
final_df = final_df.append(tuesday[tuesday[' Label'] == 'SSH-Patator'],ignore_index = True)
final_df = final_df.append(monday[150001:200000],ignore_index = True)

del tuesday
print("Tuesday done")
print(final_df.shape)


wednesday = pd.read_csv(data_path+'Wednesday-workingHours.pcap_ISCX.csv',encoding="Latin1")

final_df = final_df.append(wednesday[wednesday[' Label'] == 'DoS slowloris'],ignore_index = True)
final_df = final_df.append(monday[200001:220000],ignore_index = True)
final_df = final_df.append(wednesday[wednesday[' Label'] == 'DoS Slowhttptest'],ignore_index = True)
final_df = final_df.append(monday[220001:240000],ignore_index = True)
final_df = final_df.append(wednesday[wednesday[' Label'] == 'DoS Hulk'],ignore_index = True)
final_df = final_df.append(monday[240001:260000],ignore_index = True)
final_df = final_df.append(wednesday[wednesday[' Label'] == 'DoS GoldenEye'],ignore_index = True)
final_df = final_df.append(monday[260001:280000],ignore_index = True)
final_df = final_df.append(wednesday[wednesday[' Label'] == 'Heartbleed'],ignore_index = True)
final_df = final_df.append(monday[280001:300000],ignore_index = True)

del wednesday
print("Wednesday done")
print(final_df.shape)

thur_mon = pd.read_csv(data_path+'Thursday-WorkingHours-Morning-WebAttacks.pcap_ISCX.csv',encoding="Latin1")

final_df = final_df.append(thur_mon[thur_mon[' Label'] == 'Web Attack � Brute Force'],ignore_index = True)
final_df = final_df.append(monday[300001:320000],ignore_index = True)
final_df = final_df.append(thur_mon[thur_mon[' Label'] == 'Web Attack � XSS'],ignore_index = True)
final_df = final_df.append(monday[320001:330000],ignore_index = True)
final_df = final_df.append(thur_mon[thur_mon[' Label'] == 'Web Attack � Sql Injection'],ignore_index = True)
final_df = final_df.append(monday[330001:340000],ignore_index = True)

del thur_mon

thur_afnoon = pd.read_csv(data_path + 'Thursday-WorkingHours-Afternoon-Infilteration.pcap_ISCX.csv',encoding="Latin1")

final_df = final_df.append(thur_afnoon[thur_afnoon[' Label'] == 'Infiltration'],ignore_index = True)
final_df = final_df.append(monday[340001:400000],ignore_index = True)

del thur_afnoon
print("Thursday done")
print(final_df.shape)

fri_mon = pd.read_csv(data_path + 'Friday-WorkingHours-Morning.pcap_ISCX.csv',encoding="Latin1")
final_df = final_df.append(fri_mon[fri_mon[' Label'] == 'Bot'],ignore_index = True)
final_df = final_df.append(monday[400001:420000],ignore_index = True)
del fri_mon

fri_afnoon = pd.read_csv(data_path + 'Friday-WorkingHours-Afternoon-PortScan.pcap_ISCX.csv',encoding="Latin1")
final_df = final_df.append(fri_afnoon[fri_afnoon[' Label'] == 'PortScan'],ignore_index = True)
final_df = final_df.append(monday[420001:460000],ignore_index = True)

del fri_afnoon

fri_eve = pd.read_csv(data_path + 'Friday-WorkingHours-Afternoon-DDos.pcap_ISCX.csv',encoding="Latin1")
final_df = final_df.append(fri_eve[fri_eve[' Label'] == 'DDoS'],ignore_index = True)
final_df = final_df.append(monday[460001:],ignore_index = True)


print(final_df[' Label'].value_counts())
print(final_df.shape)


final_df_with_ip = final_df.copy()
final_df.drop(columns = not_com,inplace=True)
final_df_with_ip.drop(columns = not_com_with_dst_ip,inplace=True)
print(final_df.shape)

# final_df.to_csv('dataset_reduced.csv',index = False)
final_df_with_ip.to_csv('dataset_reduced_with_ip.csv',index = False)