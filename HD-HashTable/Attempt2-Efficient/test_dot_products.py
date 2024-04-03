# This file calculates and writes the dot product of test queries to dot_prods.csv.
# 
# python3 test_dot_products.py --modelPath model.csv --testDataPath data_test.csv

import argparse

from numpy import dot
from HDHashTable import HDHashTable

parser = argparse.ArgumentParser()
parser.add_argument('--modelPath', action='store', type=str, help='path to model csv file', required=True)
parser.add_argument('--testDataPath', action='store', type=str, help='path to test data csv file', required=True)

inputs = parser.parse_args()
model_path = inputs.modelPath
test_data_path = inputs.testDataPath

# Build the model
f = open(model_path, 'r')
k = int(f.readline().strip()) # Need .strip() to exclude the trailing '\n'
D = int(f.readline().strip())
hash_table = HDHashTable(k=k, D=D)
hash_table.set_A_encoding([int(x) for x in f.readline().strip().split(' ')])
hash_table.set_C_encoding([int(x) for x in f.readline().strip().split(' ')])
hash_table.set_G_encoding([int(x) for x in f.readline().strip().split(' ')])
hash_table.set_T_encoding([int(x) for x in f.readline().strip().split(' ')])
hash_table.set_hash_table_hv([int(x) for x in f.readline().strip().split(' ')])
f.close()
hash_table.print_base_enc_dot_prods()

# Writing dot products to dot_prods.csv
f_dot_prods = open('dot_prods.csv', 'w')
f_test_data = open(test_data_path, 'r')
data = f_test_data.readline()
while data != '':
  data = data.split(',')
  kmer = data[0]
  encoded_kmer = hash_table.encode(kmer)
  dot_prod = dot(hash_table.hash_table_hv, encoded_kmer)
  f_dot_prods.write(f'{dot_prod},{data[1]}')
  data = f_test_data.readline()
f_dot_prods.close()
f_test_data.close()