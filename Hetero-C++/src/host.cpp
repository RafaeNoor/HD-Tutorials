#define HPVM 1

#define D 1024
#define K 4


#ifdef HPVM
#include <heterocc.h>
#include <hpvm_hdc.h>
#include "DFG.hpp"
#endif
#include "host.h"
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



    // Read vectors from file
    hvtype* A_vec = ...;
    hvtype* C_vec = ...;
    hvtype* G_vec = ...;
    hvtype* T_vec = ...;

	__hypervector__<D, hvtype> A_encoding = __hetero_hdc_create_hypervector<D, hvtype>(1, (void*) copy<hvtype>, A_vec);	
	__hypervector__<D, hvtype> C_encoding = __hetero_hdc_create_hypervector<D, hvtype>(1, (void*) copy<hvtype>, C_vec);	

	__hypervector__<D, hvtype> G_encoding = __hetero_hdc_create_hypervector<D, hvtype>(1, (void*) copy<hvtype>, G_vec);	

	__hypervector__<D, hvtype> T_encoding = __hetero_hdc_create_hypervector<D, hvtype>(1, (void*) copy<hvtype>, T_vec);	




    return 0;
}
