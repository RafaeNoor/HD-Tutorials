# Most of this is copied verbatim from ../Attempt2-Testing/HDHashTable
# The difference is that we have a different hypervector for every 400 k-mers rather than a single
# hypervector for the entire hash table. This allows us to maintain a low dimension of D=10000
# without compromising accuracy.

from math import ceil, floor
import numpy as np
from numpy import dot
from numpy.linalg import norm

class HDHashTable:
  def __init__(self, k: int, D: int):
    """
    Constructor that creates an array of D-dimensional arrays to store the contents of the hash table
    Creates an encoding scheme for each of the 4 bases: A, C, G, and T

    Parameters:
      k (int): The length k of a k-mer. All k-mers in the hash table should have the same length
      D (int): The dimension of a hypervector
    """
    # We don't want the dimension to be too small. Otherwise, there's a higher chance that encoded
    # hypervectors will be too similar to each other. Ideally, they should be nearly orthogonal
    if D < 10 * k:
      raise ValueError("Please choose a larger D")

    self.k = k
    self.D = D
    self.hash_table_hvs = [[1] * self.D]
    self.kmers_in_last_hv = 0

    # Encoding scheme: each encoding will have half its values be -1 and the other half be 1
    self.encoding_scheme = {}
    for base in ['A', 'C', 'G', 'T']:
      encoding = [-1] * floor(D/2) + [1] * ceil(D/2)
      np.random.shuffle(encoding)
      self.encoding_scheme[base] = encoding

  def add_hv(self, read_hv: list):
    """
    Adds the HV representation of a read to the list of hash table HVs.

    Parameters:
      read_hv (list): The hypervector representation of a read whose k-mers are in the hash table
    """
    self.hash_table_hvs.append(read_hv)

  def set_A_encoding(self, A_encoding: list):
    """
    Sets the encoding scheme for base 'A'.

    Parameters:
      A_encoding (list): The encoding scheme for 'A'
    """
    self.encoding_scheme['A'] = A_encoding

  def set_C_encoding(self, C_encoding: list):
    """
    Sets the encoding scheme for base 'C'.

    Parameters:
      C_encoding (list): The encoding scheme for 'C'
    """
    self.encoding_scheme['C'] = C_encoding

  def set_G_encoding(self, G_encoding: list):
    """
    Sets the encoding scheme for base 'G'.

    Parameters:
      G_encoding (list): The encoding scheme for 'G'
    """
    self.encoding_scheme['G'] = G_encoding

  def set_T_encoding(self, T_encoding: list):
    """
    Sets the encoding scheme for base 'T'.

    Parameters:
      T_encoding (list): The encoding scheme for 'T'
    """
    self.encoding_scheme['T'] = T_encoding

  def encode(self, kmer: str):
    """
    Encodes a k-mer into a D-dimensional hypervector
    Follows the algorithm described in GenieHD

    Parameters:
      kmer (str): The kmer we want to encode

    Returns:
      D-dimensional hypervector representing the kmer
    """
    if len(kmer) != self.k:
      raise ValueError(f'k-mer must have length {self.k}')
    enc_hv = [1] * self.D
    for i in range(self.k):
      base_enc = self.encoding_scheme[kmer[i]]
      base_enc = np.roll(base_enc, i)
      enc_hv = [a * b for a, b in zip(enc_hv, base_enc)]
    return enc_hv

  def add_enc_kmer(self, enc_kmer: list):
    """
    Adds an encoded k-mer to the hash table.

    Parameters:
      enc_kmer: hypervector representation of the k-mer being added
    """
    if self.kmers_in_last_hv >= 400:
      self.hash_table_hvs.append(enc_kmer)
      self.kmers_in_last_hv = 1
    else:
      self.hash_table_hvs[-1] = [sum(x) for x in zip(self.hash_table_hvs[-1], enc_kmer)]
      self.kmers_in_last_hv += 1

  def add_read(self, read: str):
    """
    Adds all the kemrs from a given read to our hash table. Since there's no way of knowing if kmers
    already exist in the hash table, we will add them again if there are duplicates.

    Parameters:
      read (str): The read whose k-mers we are adding to the hash table
    """
    if len(read) < self.k:
      raise ValueError(f'read must have length >= {self.k}')
    kmer = read[:self.k]
    first_base_in_kmer = kmer[0]
    kmer_enc = self.encode(kmer)
    self.add_enc_kmer(kmer_enc)

    # Use sliding window technique from GenieHD
    for i in range(self.k, len(read)):
      first_base_enc = self.encoding_scheme[first_base_in_kmer]
      kmer_enc = [a * b for a, b in zip(kmer_enc, first_base_enc)]
      kmer_enc = np.roll(kmer_enc, -1)
      last_base_enc = self.encoding_scheme[read[i]]
      last_base_enc = np.roll(last_base_enc, self.k - 1)
      kmer_enc = [a * b for a, b in zip(kmer_enc, last_base_enc)]
      self.add_enc_kmer(kmer_enc)
      first_base_in_kmer = read[i - self.k + 1]

  def query(self, kmer: str):
    """
    Returns whether or not the given k-mer is in our hash table
    If the dot product of the encoded k-mer with any of the HVs is greater than the
    threshold of 0.8 * D, return True. Otherwise, return false.

    Parameters:
      kmer (str): The k-mer we are querying

    Returns:
      Whether or not the k-mer exists in the hash table
    """
    enc_hv = self.encode(kmer)
    largest_dot_prod = 0
    for read_hv in self.hash_table_hvs:
      dot_prod = dot(read_hv, enc_hv)
      if dot_prod > largest_dot_prod:
        largest_dot_prod = dot_prod
      if dot_prod > 0.8 * self.D:
        # Early Search Termination (EST)
        # print(f'dot product for {kmer} is {dot_prod}')
        return True
    print(f'largest dot product for {kmer} is {largest_dot_prod}')
    return False

  def max_dot_prod(self, kmer: str):
    """
    Returns the largest dot product of the encoded k-mer with any of the hash table HVs

    Parameters:
      kmer (str): The k-mer we are querying

    Returns:
      The largest dot product of the query
    """
    enc_hv = self.encode(kmer)
    largest_dot_prod = 0
    for read_hv in self.hash_table_hvs:
      dot_prod = dot(read_hv, enc_hv)
      if dot_prod > largest_dot_prod:
        largest_dot_prod = dot_prod
    return largest_dot_prod

  def max_cos_sim(self, kmer: str):
    """
    Returns the largest cosine similarity of the encoded k-mer with any of the hash table HVs

    Parameters:
      kmer (str): The k-mer we are querying

    Returns:
      The largest cosine similarity of the query
    """
    enc_hv = self.encode(kmer)
    largest_cos_sim = -1
    for read_hv in self.hash_table_hvs:
      cos_sim = dot(read_hv, enc_hv) / (norm(read_hv) * 100) # norm(enc_hv) = 1
      if cos_sim > largest_cos_sim:
        largest_cos_sim = cos_sim
    return largest_cos_sim

  def print_base_enc_dot_prods(self):
    """
    Prints the dot product between all pairs of base encodings.
    """
    print("Print dot product between base encodings to ensure they're dissimilar enough")
    print(f"A and C: {dot(self.encoding_scheme['A'], self.encoding_scheme['C'])}")
    print(f"A and G: {dot(self.encoding_scheme['A'], self.encoding_scheme['G'])}")
    print(f"A and T: {dot(self.encoding_scheme['A'], self.encoding_scheme['T'])}")
    print(f"C and G: {dot(self.encoding_scheme['C'], self.encoding_scheme['G'])}")
    print(f"C and T: {dot(self.encoding_scheme['C'], self.encoding_scheme['T'])}")
    print(f"G and T: {dot(self.encoding_scheme['G'], self.encoding_scheme['T'])}")
    print()
