#include "swap.h"

void int_swap(int* a, int* b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void float_swap(float* a, float* b)
{
	float tmp = *a;
	*a = *b;
	*b = tmp;
}

void vec3_t_swap(vec3_t* a, vec3_t* b)
{
	vec3_t tmp = *a;
	*a = *b;
	*b = tmp;
}