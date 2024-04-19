
typedef int hvtype;

#define COPY_HV_DEF(ELEMTY) \
extern "C" void cu_rt_copy_hv_##ELEMTY(void* hv1, size_t row1, void* dst);

COPY_HV_DEF(int);
COPY_HV_DEF(float);
COPY_HV_DEF(double);





template<int K, int D>
void encode_kmer(
        __hypervector__<D, hvtype>* encoded_hv_ptr, size_t encoded_size, 
        __hypervector__<D, hvtype>* base_ptr, size_t base_size, 
        __hypermatrix__<4,D, hvtype>* encoding_scheme_ptr, size_t encoded_scheme_size,
        __hypervector__<D, hvtype>* shifted_hv_ptr, size_t shifted_size, 
        int index
        ){

#ifndef NODFG
    void* section = __hetero_section_begin();

    void* task1 = __hetero_task_begin(
            3,  base_ptr, base_size, encoding_scheme_ptr, encoded_scheme_size, index,
        /* Output Buffers: 1*/ 1, base_ptr, base_size,
        "produce_dot_prod_inner_task_1"
    );



#endif
    {
#if 1
        auto base_hv = __hetero_hdc_get_matrix_row<4,D,hvtype>(*encoding_scheme_ptr, 4,D,index);
       *base_ptr  = __hetero_hdc_wrap_shift<D, hvtype>(base_hv, index);
#endif
    }

#ifndef NODFG
    __hetero_task_end(task1); 
    void* task2 = __hetero_task_begin(
            5, encoded_hv_ptr, encoded_size, base_ptr, base_size, encoding_scheme_ptr, encoded_scheme_size, shifted_hv_ptr, shifted_size, index,
        /* Output Buffers: 1*/ 1, encoded_hv_ptr, encoded_size,
        "produce_dot_prod_inner_task_2"
    );

#endif

#if 1
    {
    __hypervector__<D, hvtype> product = __hetero_hdc_mul<D, hvtype>(*base_ptr, *encoded_hv_ptr); 
    *encoded_hv_ptr = product;

    }
#endif
#ifndef NODFG
    __hetero_task_end(task2); 
    __hetero_section_end(section);
#endif
}



template <typename T>
T one(size_t loop_index_var) {
    return 1;
}

template<int K, int D>
void   __attribute__ ((noinline)) encode_kmer_wrapper(
        int* kmer, size_t kmer_size,
        __hypervector__<D, hvtype>* encoded_hv_ptr, size_t encoded_size, 
        __hypervector__<D, hvtype>* base_ptr, size_t base_size, 
        __hypervector__<D, hvtype>* shifted_hv_ptr, size_t shifted_size, 
        __hypermatrix__<4,D, hvtype>* encoding_scheme_ptr, size_t encoded_scheme_size
        ){


        //std::cout << "encode wrapper_start: "<<  "\n";
        




    for(int i = 0; i < K; i++){

#ifndef NODFG
        void* EncodeDAG = __hetero_launch(
        (void*) encode_kmer<K,D>, 5, encoded_hv_ptr, encoded_size, base_ptr, base_size, encoding_scheme_ptr, encoded_scheme_size, shifted_hv_ptr, shifted_size, kmer[i], 1, encoded_hv_ptr, encoded_size);

        __hetero_wait(EncodeDAG);
#else
        encode_kmer<K,D>(encoded_hv_ptr, encoded_size, base_ptr, base_size, encoding_scheme_ptr, encoded_scheme_size, shifted_hv_ptr, shifted_size, kmer[i]);

#endif

    }

        // std::cout << "encode wrapper_end: "<<  "\n";


}



template<int K, int D, int N>
void produce_dot_prod(
        __hypermatrix__<N,D, hvtype>* hash_table, size_t hash_table_size,
        __hypervector__<D, hvtype>* encoded_hv_ptr, size_t encoded_size,
        __hypervector__<N, hvtype>* output_ptr, size_t output_size,
        int* argmax, size_t argmax_size
        ){

#ifndef NODFG
    void* section = __hetero_section_begin();

    void* task = __hetero_task_begin(
         3, hash_table, hash_table_size, encoded_hv_ptr, encoded_size, output_ptr, output_size,
        /* Output Buffers: 1*/ 1, output_ptr, output_size,
        "produce_dot_prod_inner_task"
    );

#endif

    *output_ptr = __hetero_hdc_matmul<N, D, hvtype>(*encoded_hv_ptr, *hash_table); 

#ifndef NODFG
    __hetero_task_end(task); 

    void* task2 = __hetero_task_begin(
         2, output_ptr, output_size, argmax, argmax_size,
         2, output_ptr, output_size, argmax, argmax_size,
        "produce_dot_prod_argmax_task"
    );

#endif

    *argmax = __hetero_hdc_arg_max<N, hvtype>(*output_ptr);

#ifndef NODFG

    __hetero_task_end(task2); 
    __hetero_section_end(section);
#endif

}




template<int K, int D, int N>
bool __attribute__ ((noinline))  query(int* kmer, size_t kmer_size,
        __hypermatrix__<4,D, hvtype>* encoding_scheme_ptr, size_t encoded_scheme_size,
        __hypermatrix__<N,D, hvtype>* hash_table, size_t hash_table_size,
        __hypervector__<D, hvtype>* base_ptr, size_t base_size, 
        __hypervector__<D, hvtype>* shifted_hv_ptr, size_t shifted_size,
        __hypervector__<D, hvtype>* encoded_hv_handle, size_t encoded_size ,
        __hypervector__<N, hvtype>* output_handle, size_t output_size 
        ){






     encode_kmer_wrapper<K,D>(kmer, kmer_size,encoded_hv_handle, encoded_size, base_ptr, base_size, shifted_hv_ptr, shifted_size, encoding_scheme_ptr, encoded_scheme_size);


    //return false;
    int arg_max[1];
    size_t arg_max_size = sizeof(int);


#ifndef NODFG
        void* ProduceDAG = __hetero_launch(
        (void*) produce_dot_prod<K,D, N>, 4, 
        hash_table, hash_table_size,
        encoded_hv_handle, encoded_size,
        output_handle, output_size,
        arg_max, arg_max_size, 
        2, 
        output_handle, output_size,
        arg_max, arg_max_size);

        __hetero_wait(ProduceDAG);
#else
    produce_dot_prod<K,D, N>( 
        hash_table, hash_table_size,
        encoded_hv_handle, encoded_size,
        output_handle, output_size,
        arg_max, arg_max_size) ;
#endif
    
    //std::cout <<"arg max: "<< arg_max <<"\n";

#ifdef NODFG
    hvtype* output_copy = (hvtype*) malloc(sizeof(hvtype) * N);
    cu_rt_copy_hv_int(output_handle, N, output_copy);
    bool condition = output_copy[arg_max[0]] > (0.8 * D);
    free(output_copy);
    return condition;

#else
    hvtype* output_base = (hvtype*) output_handle;
    return output_base[arg_max[0]] > 0.8 * D;//output_base[arg_max] > (0.8 * D);
#endif





}
