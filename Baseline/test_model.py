# This file calculates and prints the accuracy of a given HDC model on a given test dataset.
#
# python3 test_model.py --modelPath model.csv --testDataPath data_test.csv

import argparse
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
hypervector = f.readline().strip()
while len(hypervector) > 0:
  hash_table.add_hv([int(x) for x in hypervector.split(' ')])
  hypervector = f.readline().strip()
f.close()
hash_table.print_base_enc_dot_prods()

# Testing
true_pos = 0
true_neg = 0
total_pos = 0
total_neg = 0

f = open(test_data_path, 'r')
data = f.readline()
while data != '':
  data = data.split(',')
  kmer = data[0]
  in_hash_table = data[1].startswith('T')
  query_result = hash_table.query(kmer)
  #print("QUERY RESULT:", query_result)
  if (in_hash_table):
    total_pos += 1
    if (query_result):
      true_pos += 1
  if (not in_hash_table):
    total_neg += 1
    if (not query_result):
      true_neg += 1
  data = f.readline()

num_kmers = total_pos + total_neg
f.close()

# Print results
print(f'{num_kmers}')
print(f'\nCorrectly identified {true_pos} / {total_pos} {k}-mers that were in the hash table')
print(f'Accuracy: {true_pos / total_pos * 100:.2f}%\n')
print(f'Correctly identified {true_neg} / {total_neg} {k}-mers that were not in the hash table')
print(f'Accuracy: {true_neg / total_neg * 100:.2f}%\n')
print(f'Total Accuracy: {(true_pos + true_neg) / (total_pos + total_neg) * 100:.2f}%')
