#define HPVM 1

#define Dhv 1024
#define Khv 4


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


    // Read vectors from file
    hvtype* A_vec = nullptr;
    hvtype* C_vec = nullptr;
    hvtype* G_vec = nullptr;
    hvtype* T_vec = nullptr;

	__hypervector__<Dhv, hvtype> A_encoding = __hetero_hdc_create_hypervector<Dhv, hvtype>(1, (void*) copy<hvtype>, A_vec);	
	__hypervector__<Dhv, hvtype> C_encoding = __hetero_hdc_create_hypervector<Dhv, hvtype>(1, (void*) copy<hvtype>, C_vec);	

	__hypervector__<Dhv, hvtype> G_encoding = __hetero_hdc_create_hypervector<Dhv, hvtype>(1, (void*) copy<hvtype>, G_vec);	

	__hypervector__<Dhv, hvtype> T_encoding = __hetero_hdc_create_hypervector<Dhv, hvtype>(1, (void*) copy<hvtype>, T_vec);	



#ifndef NODFG
	__hpvm__cleanup();
#endif	

    return 0;
}
