#include <gamemath/matrix.h>

#include <gamemath/mat43.h>
#include <mic3d/state.h>

#include <stdio.h>
#include <math.h>
#include <assert.h>


#define MATRIX_STACK_COUNT 16
fix16_mat43_t __matrix_pool[MATRIX_STACK_COUNT];


#define FLOAT_FIX16_ONE 65536.0f


void __matrix_init(void)
{
	fix16_mat43_t* matrix_1 = &__matrix_pool[1];
	
	__state.mstack->pool_matrix = matrix_1;
	__state.mstack->top_matrix = matrix_1;
	__state.mstack->bottom_matrix = &__matrix_pool[(MATRIX_STACK_COUNT - 1) - 1];
	__state.mstack->stack_count = MATRIX_STACK_COUNT - 1;
	
	for(int i = 0; i < __state.mstack->stack_count; i++)
	{
		fix16_mat43_identity(&__state.mstack->top_matrix[i]);
	}
	
	fix16_mat43_identity(__matrix_view_get());
}


fix16_mat43_t* __matrix_view_get(void)
{
	return &__matrix_pool[0];
}


void matrix_push(void)
{
	matrix_ptr_push();
	
	const fix16_mat43_t* prev_matrix = __state.mstack->top_matrix - 1;
	
	fix16_mat43_dup(prev_matrix, __state.mstack->top_matrix);
}


void matrix_ptr_push(void)
{
	assert(__state.mstack->top_matrix < __state.mstack->bottom_matrix);
	
	__state.mstack->top_matrix++;
}


void matrix_pop(void)
{
	if(__state.mstack->top_matrix != __state.mstack->bottom_matrix)
	{
		__state.mstack->top_matrix--;
	}
}


fix16_mat43_t* matrix_top(void)
{
	return __state.mstack->top_matrix;
}

