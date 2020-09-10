import pymongo
from pymongo import MongoClient
import pandas


def _connect_mongo(host, port, db,username = None, password = None):
    """ A util for making a connection to mongo """

    if username and password:
        mongo_uri = 'mongodb://%s:%s@%s:%s/%s' % (username, password, host, port, db)
        conn = MongoClient(mongo_uri)
    else:
        conn = MongoClient(host, port)
    return conn[db]


def read_mongo(_db, collection, query={}, host='127.0.0.1', port=27017, username=None, password=None, no_id=False):
    """ Read from Mongo and Store into DataFrame """

    
    # Connect to MongoDB
    db = _connect_mongo(host=host, port=port, username=username, password=password, db=_db)

    
    # Make a query to the specific DB and Collection
    cursor = db[collection].find(query)
    
    # Expand the cursor and construct the DataFrame
    df =  pandas.DataFrame.from_dict(list(cursor))
    # print(df)
    # Delete the _id
    if no_id:
        del df['_id']
    
    return (df.iloc[:,5:])
