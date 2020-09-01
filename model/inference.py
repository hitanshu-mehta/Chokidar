import pickle
import pandas as pd
import numpy as np 
from fetch_data import read_mongo


def check_for_attacks(predictions,type):
  attack_idxs = []
  for i,prediction in enumerate(predictions):
    if prediction:
      attack_idxs.append(i)
  return attack_idxs

def load_model_and_infer(BATCH_SIZE = 64,INPUT_SIZE = 78):

    trained_model_path = './model/trained_models'

    input_df = read_mongo("idsdb","flows")

    # # TO TEST
    # input_df = pd.read_csv('../dataset_with_ip.csv')
    # input_df = input_df.drop([' Label',' Destination IP'],axis = 1)

    # if(input_df.shape[0] < 1000):
    #   return None
    
    # input_df['Flow Bytes/s']=input_df['Flow Bytes/s'].astype('float64')
    # input_df[' Flow Packets/s']=input_df['Flow Packets/s'].astype('float64')
    # input_df.replace(np.inf,np.nan,inplace=True)
    # input_df['Flow Bytes/s'].fillna(input_df['Flow Bytes/s'].mean(),inplace=True)
    # input_df[' Flow Packets/s'].fillna(input_df['Flow Packets/s'].mean(),inplace=True)

    # input_df.replace([np.inf, -np.inf], np.nan).dropna(axis=1,inplace=True)

    
    # NaN_values=input_df.isnull().sum() 
    # for s,i in enumerate(NaN_values):
    #   print(i,input_df.columns[s])

    dos_detector = pickle.load(open(trained_model_path+'/dos_rf.sav','rb'))
    ddos_detector = pickle.load(open(trained_model_path+'/ddos_rf.sav','rb'))
    portscan_detector = pickle.load(open(trained_model_path+'/portscan_rf.sav','rb'))
    abnormal_detector = pickle.load(open(trained_model_path + '/ab_rf.sav','rb'))

    atk_dst_port = []
    atk_type = []
    
    if(input_df.size):
      dos_predict = dos_detector.predict(input_df)
      ddos_predict = ddos_detector.predict(input_df)
      portscan_predict = portscan_detector.predict(input_df)
      abnormal_predict = abnormal_detector.predict(input_df)
    else:
      return [atk_dst_port,atk_type]

    # check for attacks


    attack_idxs_dos = check_for_attacks(dos_predict,'dos')
    for idx in attack_idxs_dos:
      dst_port = input_df.iloc[idx]['Destination Port']
      atk_dst_port.append(int(dst_port))
      atk_type.append(0)

    attack_idxs_ddos = check_for_attacks(ddos_predict,'ddos')
    for idx in attack_idxs_ddos:
      dst_port = input_df.iloc[idx]['Destination Port']
      atk_dst_port.append(int(dst_port))
      atk_type.append(1)

    attack_idxs_portscan = check_for_attacks(portscan_predict,'portscan')
    for idx in attack_idxs_portscan:
      dst_port = input_df.iloc[idx]['Destination Port']
      atk_dst_port.append(int(dst_port))
      atk_type.append(2)

    attack_idxs_ab = check_for_attacks(abnormal_predict,'unknown')
    for idx in attack_idxs_ab:
      dst_port = input_df.iloc[idx]['Destination Port']
      atk_dst_port.append(int(dst_port))
      atk_type.append(3)

    # for p,t in zip(atk_dst_port,atk_type):
    #   print(p,t)
    
    return (atk_dst_port,atk_type)


    

    






      