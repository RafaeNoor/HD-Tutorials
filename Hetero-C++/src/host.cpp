#define HPVM 1
#include <fstream>
#include <iostream>

#define Dhv 10000
#define Khv 501
#define HASH_ROWS (67 - 4)
#define N_TEST 4 


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
    std::vector<hvtype> hash_table_entries(num_entries);

    std::string hash_file_name = "./dataset/model.csv";
    std::fstream myfile(hash_file_name, std::ios_base::in);

    int a;
    int counter = 0;
    while (myfile >> a)
    {
        hash_table_entries[counter] = a;
        counter++;
    }

    myfile.close();

    // First four rows correspond to A,C,G,T encoding

    hvtype* base_ptr = hash_table_entries.data();
    // Read vectors from file
    hvtype* A_vec = base_ptr;
    hvtype* C_vec = base_ptr + Dhv;
    hvtype* G_vec = base_ptr + (2 * Dhv);
    hvtype* T_vec = base_ptr + (3 * Dhv);

    for(int i =0; i < 10; i++){
        std::cout << "Hash table entry: "<< hash_table_entries[i] << "\n";
    }

	__hypervector__<Dhv, hvtype> A_encoding = __hetero_hdc_create_hypervector<Dhv, hvtype>(1, (void*) copy<hvtype>, A_vec);	
	__hypervector__<Dhv, hvtype> C_encoding = __hetero_hdc_create_hypervector<Dhv, hvtype>(1, (void*) copy<hvtype>, C_vec);	

	__hypervector__<Dhv, hvtype> G_encoding = __hetero_hdc_create_hypervector<Dhv, hvtype>(1, (void*) copy<hvtype>, G_vec);	

	__hypervector__<Dhv, hvtype> T_encoding = __hetero_hdc_create_hypervector<Dhv, hvtype>(1, (void*) copy<hvtype>, T_vec);	

    __hypermatrix__<Khv, Dhv, hvtype> encoding_scheme = __hetero_hdc_hypermatrix<Khv, Dhv, hvtype>();

    // Initialize encoding scheme
	__hetero_hdc_set_matrix_row<Khv, Dhv, hvtype>(encoding_scheme, A_encoding, 0); 
	__hetero_hdc_set_matrix_row<Khv, Dhv, hvtype>(encoding_scheme, C_encoding, 1); 
	__hetero_hdc_set_matrix_row<Khv, Dhv, hvtype>(encoding_scheme, G_encoding, 2); 
	__hetero_hdc_set_matrix_row<Khv, Dhv, hvtype>(encoding_scheme, T_encoding, 3); 


    auto encoding_scheme_handle = __hetero_hdc_get_handle(encoding_scheme);
    size_t encoding_scheme_size = sizeof(hvtype) * Dhv * Khv;


    __hypermatrix__<HASH_ROWS, Dhv, hvtype> hash_table = __hetero_hdc_create_hypermatrix<HASH_ROWS, Dhv, hvtype>(1, (void*) copy<hvtype>, hash_table_entries.data());


    
    auto hash_table_handle = __hetero_hdc_get_handle(hash_table);
    size_t hash_table_size = sizeof(hvtype) * Dhv * HASH_ROWS;

    std::string test_file_name = "./dataset/data_test_small.csv";
    std::fstream testFile(test_file_name, std::ios_base::in);
    
    std::vector<hvtype> kmer(Khv);

    int true_pos = 0;
    int true_neg = 0;
    int total_pos = 0;
    int total_neg = 0;

    for(int i = 0; i < N_TEST; i++){
        std::string entry;
        testFile >> entry;
        std::cout <<" Entry: "<<entry<<"\n";

        int total_length = entry.length();
        std::string included_str = entry.substr(total_length-1, 1);
        std::string sequence = entry.substr(0,total_length-2);

        assert(sequence.length() == Khv);



        std::cout << "Sequence: "<< sequence << "\n";
        std::cout << "Included Str: "<< included_str << "\n";

        for(int j =0; j < sequence.length(); j++){
            char token = sequence[j];

            hvtype value;
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

        bool prediction = query<Khv, Dhv, HASH_ROWS>(kmer.data(), sizeof(hvtype) * Khv,
                encoding_scheme_handle, encoding_scheme_size,
                hash_table_handle, hash_table_size
                );
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


    float accuracy = (true_pos + true_neg) / (total_pos + total_neg);
    std::cout << "Accuracy: "<<accuracy <<"\n";
    


#ifndef NODFG
	__hpvm__cleanup();
#endif	

    return 0;
}
