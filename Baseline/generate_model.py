# This file extracts all k-mers from the reads in a given fastq file and adds them to a hash table.
# It then saves model to model.csv where each line is one element of the hash table hypervector.
# 
# python3 generate_model.py --path small_bacillus.fastq

from HDHashTable import HDHashTable
import argparse
import random

DNA_bases = ['A', 'C', 'G', 'T']

def generate_kmer(k):
  """
  Generates a k-mer of length k
  """
  kmer = ''
  for _ in range(k):
    kmer += random.choice(DNA_bases)
  return kmer

def main():
  # Get data_file, D, and k from command input
  parser = argparse.ArgumentParser()
  parser.add_argument('--path', action='store', type=str, help='path to fastq data file', required=True)
  parser.add_argument('--D', action='store', type=int, default=10000, help='number of dimensions in the encoded hypervector (default is 10000)')
  parser.add_argument('--k', action='store', type=int, default=501, help='length of a k-mer (should be odd, default is 501)')
  
  inputs = parser.parse_args()
  data_train_file = inputs.path
  D = inputs.D
  k = inputs.k

  # Initialize hash table
  # Note: k should be odd because this is what LJA requires (see jumbodbg_manual.md)
  hash_table = HDHashTable(k=k, D=D)
  hash_table.print_base_enc_dot_prods()

  # Build the hash table
  f = open(data_train_file, 'r')
  i = 0
  read = f.readline()
  while read != '':
    # Need to read the next line due to the way fastq files are structured
    # For more information, check out this Illumina page:
    #   https://knowledge.illumina.com/software/general/software-general-reference_material-list/000002211
    read = f.readline().strip() # We include a .strip() to ignore the trailing '\n'
    print(f'encoding read {i+1}')
    i += 1

    # Extract all k-mers of length k from the read and add them to our hash table
    hash_table.add_read(read)

    # Need to read in 3 more lines due to the fastq file format
    read = f.readline()
    read = f.readline()
    read = f.readline()

  f.close()

  # Save model
  f = open('model.csv', 'w')
  # First save k and D
  f.write(f'{hash_table.k}\n')
  f.write(f'{hash_table.D}\n')
  # Now the base encodings
  order_of_bases = ['A', 'C', 'G', 'T']
  for base in order_of_bases:
    for val in hash_table.encoding_scheme[base]:
      f.write(f'{val} ')
    f.write('\n')
  # Now save the hash table hypervectors
  for hash_table_hv in hash_table.hash_table_hvs:
    for entry in hash_table_hv:
      f.write(f'{str(entry)} ')
    f.write('\n')
  f.close()

if __name__ == '__main__':
  main()
