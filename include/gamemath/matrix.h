#ifndef __FAKEYAUL_MATRIX_H__
#define __FAKEYAUL_MATRIX_H__



#include <gamemath/vector.h>
#include <gamemath/mat43.h>

#include <stdint.h>



typedef struct mstack
{
	fix16_mat43_t* pool_matrix;
	fix16_mat43_t* top_matrix;
	fix16_mat43_t* bottom_matrix;
	uint32_t stack_count;
	
} mstack_t;


void __matrix_init(void);
fix16_mat43_t* __matrix_view_get(void);

void matrix_push(void);
void matrix_ptr_push(void);
void matrix_pop(void);

fix16_mat43_t* matrix_top(void);


#endif
