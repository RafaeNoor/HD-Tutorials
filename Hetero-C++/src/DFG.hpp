
typedef int hvtype;


template<int K, int D>
void encode_kmer(
        __hypervector__<D, hvtype>* encoded_hv_ptr, size_t encoded_size, 
        __hypervector__<D, hvtype>* base_ptr, size_t base_size, 
        __hypermatrix__<K,D, hvtype>* encoding_scheme_ptr, size_t encoded_scheme_size,
        __hypervector__<D, hvtype>* shifted_hv_ptr, size_t shifted_size, int index
        ){

#ifndef NODFG
    void* section = __hetero_section_begin();

    void* task1 = __hetero_task_begin(
            3,  base_ptr, base_size, encoding_scheme_ptr, encoded_scheme_size, index,
        /* Output Buffers: 1*/ 1, base_ptr, base_size,
        "produce_dot_prod_inner_task_1"
    );


    __hetero_hint(DEVICE);
#endif

        auto base_hv = __hetero_hdc_get_matrix_row<K,D,hvtype>(*encoding_scheme_ptr, K,D,index);
      __hypervector__<D, hvtype>  row = __hetero_hdc_wrap_shift<D, hvtype>(base_hv, index);
      *base_ptr = row;

#ifndef NODFG
    __hetero_task_end(task1); 
    void* task2 = __hetero_task_begin(
            5, encoded_hv_ptr, encoded_size, base_ptr, base_size, encoding_scheme_ptr, encoded_scheme_size, shifted_hv_ptr, shifted_size, index,
        /* Output Buffers: 1*/ 1, encoded_hv_ptr, encoded_size,
        "produce_dot_prod_inner_task_2"
    );
#endif



    __hypervector__<D, hvtype> product = __hetero_hdc_mul<D, hvtype>(*base_ptr, *encoded_hv_ptr); 
    *encoded_hv_ptr = product;

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
void __attribute__ ((always_inline))   encode_kmer_wrapper(
        hvtype* kmer, size_t kmer_size,
        __hypervector__<D, hvtype>* encoded_hv_ptr, size_t encoded_size, 
        __hypervector__<D, hvtype>* base_ptr, size_t base_size, 
        __hypervector__<D, hvtype>* shifted_hv_ptr, size_t shifted_size, 
        __hypermatrix__<K,D, hvtype>* encoding_scheme_ptr, size_t encoded_scheme_size
        ){


        std::cout << "encode wrapper_start: "<<  "\n";
        




    for(int i = 0; i < K; i++){

#ifndef NODFG
        void* EncodeDAG = __hetero_launch(
        (void*) encode_kmer<K,D>, 5, encoded_hv_ptr, encoded_size, base_ptr, base_size, encoding_scheme_ptr, encoded_scheme_size, shifted_hv_ptr, shifted_size, kmer[i], 1, encoded_hv_ptr, encoded_size);

        __hetero_wait(EncodeDAG);
#else
        encode_kmer<K,D>(encoded_hv_ptr, encoded_size, base_ptr, base_size, encoding_scheme_ptr, encoded_scheme_size, shifted_hv_ptr, shifted_size, kmer[i]);

#endif

    }



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
        /* Output Buffers: 1*/ 1, argmax, argmax_size,
        "produce_dot_prod_argmax_task"
    );
#endif

    *argmax = __hetero_hdc_arg_max<N, hvtype>(*output_ptr);

#ifndef NODFG

    __hetero_task_end(task2); 
    __hetero_section_end(section);
#endif

}


template<int D>
void init_encoding(
        __hypervector__<D, hvtype>* encoded_hv_ptr, size_t encoded_size
        ){

#ifndef NODFG
    void* section = __hetero_section_begin();

    void* task = __hetero_task_begin(
         1,encoded_hv_ptr, encoded_size, 
         1,encoded_hv_ptr, encoded_size, 
        "initialize_encoding_vector"
    );

    __hetero_hint(DEVICE);
#endif

#if 0
    hvtype* base_ptr = (hvtype*) encoded_hv_ptr;
    for(int i =0; i < encoded_size/sizeof(hvtype); i++){
        base_ptr[i] = (hvtype)1;

    }
#endif

#ifndef NODFG
    __hetero_task_end(task); 
    __hetero_section_end(section);
#endif

}


template<int K, int D, int N>
bool  query(hvtype* kmer, size_t kmer_size,
        __hypermatrix__<K,D, hvtype>* encoding_scheme_ptr, size_t encoded_scheme_size,
        __hypermatrix__<N,D, hvtype>* hash_table, size_t hash_table_size,
        __hypervector__<D, hvtype>* base_ptr, size_t base_size, 
        __hypervector__<D, hvtype>* shifted_hv_ptr, size_t shifted_size,
        __hypervector__<D, hvtype>* encoded_hv_handle, size_t encoded_size 
        ){



   std::cout << "Pre encode wrapper "<<std::endl;

#ifndef NODFG
        void* InitDAG = __hetero_launch(
        (void*) init_encoding<D>, 1, 
        encoded_hv_handle, encoded_size,
        1, 
        encoded_hv_handle, encoded_size
        );

        __hetero_wait(InitDAG);
#else
   init_encoding<D>(encoded_hv_handle, encoded_size);

#endif

   encode_kmer_wrapper<K,D>(kmer, kmer_size,encoded_hv_handle, encoded_size, base_ptr, base_size, shifted_hv_ptr, shifted_size, encoding_scheme_ptr, encoded_scheme_size);


    int arg_max;
    size_t arg_max_size = sizeof(int);

	__hypervector__<N, hvtype> output_hv = __hetero_hdc_hypervector<N, hvtype>();
    auto output_handle = __hetero_hdc_get_handle(output_hv);
    size_t output_size = sizeof(hvtype) * N;

#ifndef NODFG
        void* ProduceDAG = __hetero_launch(
        (void*) produce_dot_prod<K,D, N>, 4, 
        hash_table, hash_table_size,
        encoded_hv_handle, encoded_size,
        output_handle, output_size,
        &arg_max, arg_max_size, 
        1, &arg_max, arg_max_size);

        __hetero_wait(ProduceDAG);
#else
    produce_dot_prod<K,D, N>( 
        hash_table, hash_table_size,
        encoded_hv_handle, encoded_size,
        output_handle, output_size,
        &arg_max, arg_max_size) ;
#endif


    hvtype* output_base = (hvtype*) output_handle;

    return output_base[arg_max] > (0.8 * D);



}
