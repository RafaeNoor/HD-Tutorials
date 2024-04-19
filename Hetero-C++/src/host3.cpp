#define HPVM 1
#include <fstream>
#include <iostream>

#define Dhv 10000
#define Khv 501
#define HASH_ROWS (67 - 4)
#define N_TEST 400


#ifdef HPVM
#include <heterocc.h>
#include <hpvm_hdc.h>
#include "DFG.hpp"
#endif
#include <vector>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <random>
#include <cstring>



template <typename T>
T copy(T* data, size_t loop_index_var) {
    return data[loop_index_var];
}

int main(){

#ifndef NODFG
	__hpvm__init();
#endif

    int num_entries = HASH_ROWS * Dhv;
    // Populate hash_table
    hvtype* hash_table_entries = new hvtype[num_entries];

    std::string hash_file_name = "./dataset/model.csv";
    std::ifstream myfile(hash_file_name);

    int a;
    int counter = 0;
    while (myfile >> a)
    {
        hash_table_entries[counter] = a;
        counter++;
    }

    myfile.close();

    // First four rows correspond to A,C,G,T encoding

    hvtype* base_ptr = hash_table_entries;
    // Read vectors from file
    hvtype* A_vec = base_ptr;
    hvtype* C_vec = base_ptr + Dhv;
    hvtype* G_vec = base_ptr + (2 * Dhv);
    hvtype* T_vec = base_ptr + (3 * Dhv);


	__hypervector__<Dhv, hvtype> A_encoding = __hetero_hdc_create_hypervector<Dhv, hvtype>(1, (void*) copy<hvtype>, A_vec);	
	__hypervector__<Dhv, hvtype> C_encoding = __hetero_hdc_create_hypervector<Dhv, hvtype>(1, (void*) copy<hvtype>, C_vec);	

	__hypervector__<Dhv, hvtype> G_encoding = __hetero_hdc_create_hypervector<Dhv, hvtype>(1, (void*) copy<hvtype>, G_vec);	

	__hypervector__<Dhv, hvtype> T_encoding = __hetero_hdc_create_hypervector<Dhv, hvtype>(1, (void*) copy<hvtype>, T_vec);	

    __hypermatrix__<4, Dhv, hvtype> encoding_scheme = __hetero_hdc_hypermatrix<4, Dhv, hvtype>();

    // Initialize encoding scheme
	__hetero_hdc_set_matrix_row<4, Dhv, hvtype>(encoding_scheme, A_encoding, 0); 
	__hetero_hdc_set_matrix_row<4, Dhv, hvtype>(encoding_scheme, C_encoding, 1); 
	__hetero_hdc_set_matrix_row<4, Dhv, hvtype>(encoding_scheme, G_encoding, 2); 
	__hetero_hdc_set_matrix_row<4, Dhv, hvtype>(encoding_scheme, T_encoding, 3); 

    std::cout << "Created encoding scheme!" << std::endl;

    auto encoding_scheme_handle = __hetero_hdc_get_handle(encoding_scheme);
    size_t encoding_scheme_size = sizeof(hvtype) * Dhv * 4;


    __hypermatrix__<HASH_ROWS, Dhv, hvtype> hash_table = __hetero_hdc_create_hypermatrix<HASH_ROWS, Dhv, hvtype>(1, (void*) copy<hvtype>, (hvtype*) hash_table_entries);


    
    auto hash_table_handle = __hetero_hdc_get_handle(hash_table);
    size_t hash_table_size = sizeof(hvtype) * Dhv * HASH_ROWS;

    std::cout << "Created Table!" << std::endl;

    std::string test_file_name = "./dataset/data_test.csv";
    std::ifstream testFile(test_file_name);
    
    //std::vector<hvtype> kmer(Khv);
    int kmer[Khv];

    int true_pos = 0;
    int true_neg = 0;
    int total_pos = 0;
    int total_neg = 0;

    __hypervector__<Dhv, hvtype> base_hv = __hetero_hdc_hypervector<Dhv, hvtype>();	
    __hypervector__<Dhv, hvtype> shifted_hv = __hetero_hdc_hypervector<Dhv, hvtype>();	

    auto base_ptr_handle = __hetero_hdc_get_handle(base_hv) ;
    auto shifted_hv_ptr = __hetero_hdc_get_handle(shifted_hv) ;
    
    size_t base_size = sizeof(hvtype) * Dhv;
    size_t shifted_size = sizeof(hvtype) * Dhv;


    __hypervector__<Dhv, hvtype> encoded_hv = __hetero_hdc_create_hypervector<Dhv, hvtype>(0, (void*) one<hvtype>);	
    auto encoded_hv_handle = __hetero_hdc_get_handle(encoded_hv);



	__hypervector__<HASH_ROWS, hvtype> output_hv = __hetero_hdc_hypervector<HASH_ROWS, hvtype>();
    auto output_handle = __hetero_hdc_get_handle(output_hv);
    size_t output_size = sizeof(hvtype) * HASH_ROWS;

    size_t encoded_size = sizeof(hvtype) * Dhv;
    for(int i = 0; i < N_TEST; i++){
        std::cout << "Test iteration "<< i << std::endl;
        std::string entry;
        testFile >> entry;

        int total_length = entry.length();
        std::string included_str = entry.substr(total_length-1, 1);
        std::string sequence = entry.substr(0,total_length-2);

        assert(sequence.length() == Khv);

        for(int j =0; j < sequence.length(); j++){
            char token = sequence[j];

            int value;
            switch(token){
                case 'A':
                    value = 0;
                    break;
                case 'C':
                    value = 1;
                    break;
                case 'G':
                    value = 2;
                    break;
                case 'T':
                    value = 3;
                    break;
                default:
                    assert(false && "Unreachable");
            };

            kmer[j] = value;

        }

        bool label = included_str == "T";


        for(int i =0; i < Khv; i++){
            //std::cout << "i: "<< i << "\n";
            //std::cout << "kmer[i]: "<< kmer[i] << "\n";

            /*
            void* EncodeDAG = __hetero_launch(
                    (void*) encode_kmer_part_1<Khv,Dhv>, 
                    5, 
                    encoded_hv_handle, encoded_size, 
                    base_ptr_handle, base_size, 
                    encoding_scheme_handle, encoding_scheme_size, 
                    shifted_hv_ptr, shifted_size, 
                    kmer[i], 
                    1, 
                    base_ptr_handle, base_size);

            __hetero_wait(EncodeDAG);
            */

            void* Encode2DAG = __hetero_launch(
                    (void*) encode_kmer_part_2<Khv,Dhv>, 
                    5, 
                    encoded_hv_handle, encoded_size, 
                    base_ptr_handle, base_size, 
                    encoding_scheme_handle, encoding_scheme_size, 
                    shifted_hv_ptr, shifted_size, 
                    kmer[i], 
                    1, 
                    encoded_hv_handle, encoded_size);

            __hetero_wait(Encode2DAG);

        }
        bool prediction = kmer[i] == 0;

        if(label){
            total_pos += 1;
            if(prediction){
                true_pos += 1;
            }

        } else {
            total_neg += 1;
            if(!prediction){
                true_neg += 1;
            }
        }




    }
    testFile.close();

    std::cout << "Correctly identified " << true_pos << " / "<< total_pos <<" "<< Khv<<"-mers that were in the hash table\n";
    std::cout << "Accuracy: "<< ((100.0 *true_pos) / total_pos) << "%\n";

    std::cout << "Correctly identified " << true_neg << " / " << total_neg  <<" "<< Khv<<"-mers that were not in the hash table\n";
    std::cout << "Accuracy: "<< ((100.0 *true_neg) / total_neg) << "%\n";

    float total_accuracy  = 100.0 * (true_pos + true_neg) / (total_pos + total_neg);
    std::cout <<"Total Accuracy: "<< total_accuracy << "%\n";

    


#ifndef NODFG
	__hpvm__cleanup();
#endif	

    return 0;
}
