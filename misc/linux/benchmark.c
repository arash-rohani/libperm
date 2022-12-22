#define _POSIX_C_SOURCE 199309L /* timespec */

#include <stdio.h>
#include <time.h>
#include <stdint.h>

#include "perm.h"

/* better optimized perm_set_heap() function for memb_size = 1 */
int _perm_set_heap_one(void *f, size_t n_memb, char *d) {
	size_t nm1 = n_memb - 1;
	size_t i, *moving;

	moving = (size_t *) calloc(n_memb, sizeof(size_t));
	if (moving == NULL) return PERM_ERR_MALLOC;

	memcpy(d, f, n_memb);

	for (i = 1; i < n_memb; moving[i++] = 0) {
		for (; moving[i] < i; moving[i]++, i = 1) {
			d += n_memb;
			if (i % 2) {
				memcpy(d, d - n_memb, n_memb);
				d[moving[i]] = (d - n_memb)[i];
				d[i] = (d - n_memb)[moving[i]];
			} else {
				d[0] = (d - n_memb)[i];
				memcpy(d + 1, d - nm1, nm1);
				d[i] = (d - n_memb)[0];
			}
		}
	}
	free(moving);
	return PERM_OK;
}

/* use Heap's algorithm to create all permutations of n members with the given size starting with first */
int perm_set_heap(void *first, size_t n_memb, size_t memb_size, void *dst) {
	char *d;
	if (first == NULL || dst == NULL || n_memb == 0 || memb_size == 0) return PERM_ERR_INPUT;

	d = (char *) dst;

	if (n_memb == 1) memcpy(d, first, memb_size);
	else if (memb_size == 1) return _perm_set_heap_one(first, n_memb, d);
	else {
		size_t n_size = n_memb * memb_size;
		size_t nm1_size = n_size - memb_size;
		size_t i, *moving;

		moving = (size_t *) calloc(n_memb, sizeof(size_t));
		if (moving == NULL) return PERM_ERR_MALLOC;
		memcpy(d, first, n_size);

		for (i = 1; i < n_memb; moving[i++] = 0) {
			for (; moving[i] < i; moving[i]++, i = 1) {
				d += n_size;
				if (i % 2) {
					memcpy(d, d - n_size, n_size);
					memcpy(d + moving[i] * memb_size, d - n_size + i * memb_size, memb_size);
					memcpy(d + i * memb_size, d - n_size + moving[i] * memb_size, memb_size);
				} else {
					memcpy(d, d - n_size + i * memb_size, memb_size);
					memcpy(d + memb_size, d - nm1_size, nm1_size);
					memcpy(d + i * memb_size, d - n_size, memb_size);
				}
			}
		}
		free(moving);
	}
	return PERM_OK;
}

#define NANO_S 1000000000


#define START_CLOCK clock_gettime(CLOCK_REALTIME, &ts)
#define STOP_CLOCK \
	clock_gettime(CLOCK_REALTIME, &te); \
	nano = ((te.tv_sec - ts.tv_sec) * NANO_S + te.tv_nsec - ts.tv_nsec) / (long) repeats[n - 2]


#define PERM_BENCHMARK(name, type, func) \
	r = PERM_OK; \
	START_CLOCK; \
	for (i = 0; i < repeats[n - 2]; i++) { \
		size_t len, j; \
		type *perms; \
		/* allocate memory */ \
		sz = perm_size(n, sizeof(type)); \
		perms = (type *) malloc(sz); \
		if (perms == NULL) { \
			r = PERM_ERR_MALLOC; \
			printf( "%-14s[ERROR = %d]\n", name, r); \
			break; \
		} \
		/* create the permutations */ \
		r = func(type ## _arr, n, sizeof(type), perms); \
		if (r != PERM_OK) { \
			free(perms); \
			printf("%-14s[ERROR = %d]\n", name, r); \
			break; \
		} \
		/* iterate over the permutations */ \
		len = perm_length(n); \
		for (j = 0; j < len; j += n) type ## _busywork += perms[j]; \
		/* free allocated memory */ \
		free(perms); \
	} \
	if (r == PERM_OK) { \
		STOP_CLOCK; \
		printf("%-14s[mem = %-10lu]  [time = %lu.%09lu]\n", name, sz, nano / NANO_S, nano % NANO_S); \
	}


#define PSIA_BENCHMARK(name, type, i_type, i_func) \
	r = PERM_OK; \
	START_CLOCK; \
	for (i = 0; i < repeats[n - 2]; i++) { \
		type *super; \
		i_type *indices; \
		size_t count, j; \
		/* allocate memory for superpermutation */ \
		sz = perm_super_size(n, sizeof(type)); \
		super = (type *) malloc(sz); \
		if (super == NULL) { \
			r = PERM_ERR_MALLOC; \
			printf("%-14s[ERROR = %d]\n", name, r); \
			break; \
		} \
		/* create the superpermutation */ \
		r = perm_set_super(type ## _arr, n, sizeof(type), super); \
		if (r != PERM_OK) { \
			free(super); \
			printf("%-14s[ERROR = %d]\n", name, r); \
			break; \
		} \
		/* allocate memory for indices */ \
		ind_sz = perm_super_indices_size(n, sizeof(i_type)); \
		indices = (i_type *) malloc(ind_sz); \
		if (indices == NULL) { \
			r = PERM_ERR_MALLOC; \
			free(super); \
			printf("%-14s[ERROR = %d]\n", name, r); \
			break; \
		} \
		/* create the indices */ \
		r = i_func(n, indices); \
		if (r != PERM_OK) { \
			free(super); \
			free(indices); \
			printf("%-14s[ERROR = %d]\n", name, r); \
			break; \
		} \
		/* iterate over the permutations */ \
		count = perm_count(n); \
		for (j = 0; j < count; j++) type ## _busywork += super[indices[j]]; \
		/* free allocated memory */ \
		free(indices); \
		free(super); \
	} \
	if (r == PERM_OK) { \
		STOP_CLOCK; \
		printf("%-14s[mem = %-10lu]  [time = %lu.%09lu]\n", name, sz + ind_sz, nano / NANO_S, nano % NANO_S); \
	}


int main() {
	size_t n, i;
	char char_arr[12];
	int int_arr[12];
	long long_arr[12];

	/* repeat just to make the times more accurate */
	size_t repeats[] = {1000, 1000, 1000, 1000, 1000, 1000, 1000, 500, 200, 100, 50};

	/* busywork to prevent compiler from optimizing away the crucial benchmark code */
	char char_busywork = 0;
	int int_busywork = 0;
	long long_busywork = 0;

	/* initialize first permutations */
	for (i = 0; i < 12; i++) {
		int_arr[i] = (int) (i + 1);
		long_arr[i] = (long) (i + 1);
		char_arr[i] = (char) (i + 65);
	}

	/* print sizes */
	printf("sizeof(char)   = %lu\n", sizeof(char));
	printf("sizeof(int)    = %lu\n",  sizeof(int));
	printf("sizeof(long)   = %lu\n", sizeof(long));
	printf("sizeof(uchar)  = %lu  max = %u\n",  sizeof(unsigned char),  UCHAR_MAX);
	printf("sizeof(ushort) = %lu  max = %u\n",  sizeof(unsigned short), USHRT_MAX);
	printf("sizeof(uint)   = %lu  max = %u\n",  sizeof(unsigned int),   UINT_MAX);
	printf("sizeof(ulong)  = %lu  max = %lu\n", sizeof(unsigned long),  ULONG_MAX);
	printf("sizeof(size_t) = %lu  max = %lu\n", sizeof(size_t),         SIZE_MAX);
	printf("\n");

	for (n = 2; n <= 12; n++) {
		int r;
		struct timespec ts, te;
		long nano;
		size_t sz = 0;
		size_t ind_sz = 0;
		size_t i;

		printf("N = %lu (%ld repeats)\n", n, repeats[n - 2]);
		printf("--- CHAR -----------------------------------------------\n");
		PERM_BENCHMARK("OPA",           char, perm_set);
		PERM_BENCHMARK("HEAP'S",        char, perm_set_heap);
		PSIA_BENCHMARK("PSIA (UCHAR)",  char, unsigned char,  perm_set_super_indices_uchar);
		PSIA_BENCHMARK("PSIA (USHORT)", char, unsigned short, perm_set_super_indices_ushort);
		PSIA_BENCHMARK("PSIA (UINT)",   char, unsigned int,   perm_set_super_indices_uint);
		PSIA_BENCHMARK("PSIA (ULONG)",  char, unsigned long,  perm_set_super_indices_ulong);
		PSIA_BENCHMARK("PSIA",          char, size_t,         perm_set_super_indices);
		printf("--- INT  -----------------------------------------------\n");
		PERM_BENCHMARK("OPA",           int, perm_set);
		PERM_BENCHMARK("HEAP'S",        int, perm_set_heap);
		PSIA_BENCHMARK("PSIA (UCHAR)",  int, unsigned char,  perm_set_super_indices_uchar);
		PSIA_BENCHMARK("PSIA (USHORT)", int, unsigned short, perm_set_super_indices_ushort);
		PSIA_BENCHMARK("PSIA (UINT)",   int, unsigned int,   perm_set_super_indices_uint);
		PSIA_BENCHMARK("PSIA (ULONG)",  int, unsigned long,  perm_set_super_indices_ulong);
		PSIA_BENCHMARK("PSIA",          int, size_t,         perm_set_super_indices);
		printf("--- LONG -----------------------------------------------\n");
		PERM_BENCHMARK("OPA",           long, perm_set);
		PERM_BENCHMARK("HEAP'S",        long, perm_set_heap);
		PSIA_BENCHMARK("PSIA (UCHAR)",  long, unsigned char,  perm_set_super_indices_uchar);
		PSIA_BENCHMARK("PSIA (USHORT)", long, unsigned short, perm_set_super_indices_ushort);
		PSIA_BENCHMARK("PSIA (UINT)",   long, unsigned int,   perm_set_super_indices_uint);
		PSIA_BENCHMARK("PSIA (ULONG)",  long, unsigned long,  perm_set_super_indices_ulong);
		PSIA_BENCHMARK("PSIA",          long, size_t,         perm_set_super_indices);
		printf("--------------------------------------------------------\n\n");
	}

	/* print the garbage busywork just to keep the compiler from optimizing it away */
	printf("busywork garbage: %c %d %ld\n", char_busywork, int_busywork, long_busywork);
	
	return PERM_OK;
}
