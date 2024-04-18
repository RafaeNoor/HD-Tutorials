# This file generates numPos k-mers in the given fastq data and numNeg k-mers not in the given data.
# 
# python3 generate_test_data.py --path small_bacillus.fastq

import random
import numpy as np
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--path', action='store', type=str, help='path to fastq data file', required=True)
parser.add_argument('--k', action='store', type=int, default=501, help='length of a k-mer (should be odd, default is 501)')
parser.add_argument('--numPos', action='store', type=int, default=200, help='number of k-mers in the generated test data that are in the hash table (default 200)')
parser.add_argument('--numNeg', action='store', type=int, default=200, help='number of k-mers in the generated test data that are not in the hash table (default 200)')

inputs = parser.parse_args()
data_file = inputs.path
k = inputs.k
num_pos = inputs.numPos
num_neg = inputs.numNeg

DNA_bases = ['A', 'C', 'G', 'T']

def extract_kmers(data_file, k):
  """
  Extracts all k-mers of length k from a fastq data file and appends them to a list named kmers

  Parameters:
    data_file (str): The name of the fastq file
    k (str): k-mer length

  Returns:
    Number of kmers
    List containing all k-mers of length k from the given file
  """
  f = open(data_file, 'r')
  kmers = []
  num_kmers = 0
  
  read = f.readline()
  while read != '':
    # Need to read the next line due to the way fastq files are structured
    # For more information, check out this Illumina page:
    #   https://knowledge.illumina.com/software/general/software-general-reference_material-list/000002211
    read = f.readline().strip() # We include a .strip() to ignore the trailing '\n'
    
    # Extract all k-mers of length k from the read and add them to our hash table
    for i in range(len(read) - k + 1):
      kmer = read[i:i+k]
      kmers.append(kmer)
      num_kmers += 1
    
    # Need to read in 3 more lines due to the fastq file format
    read = f.readline()
    read = f.readline()
    read = f.readline()

  f.close()
  return num_kmers, kmers

def generate_kmer(k):
  """
  Generates a k-mer of length k
  """
  kmer = ''
  for _ in range(k):
    kmer += random.choice(DNA_bases)
  return kmer

def main():
  num_kmers, kmers = extract_kmers(data_file, k)

  f = open("data_test.csv", "w")

  # Randomly sample num_pos k-mers in our hash table to be in our testing data
  in_test_set = [True] * num_pos + [False] * (num_kmers - num_pos)
  np.random.shuffle(in_test_set)
  for i in range(num_kmers):
    if in_test_set[i]:
      f.write(f'{kmers[i]},T\n')

  # Randomly generate num_neg k-mers not in our hash table to be in our testing data
  total_neg = 0
  while total_neg < num_neg:
    kmer = generate_kmer(k)
    if kmer not in kmers:
      f.write(f'{kmer},F\n')
      total_neg += 1

  f.close()
    
if __name__=="__main__":
  main()
