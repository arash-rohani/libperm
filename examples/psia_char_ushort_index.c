#include <stdio.h>

#include "perm.h"

int main(int argc, char *argv[]) {
	long input;
	int r;
	size_t i, n, count, super_size;
	char *arr, *super;
	unsigned short *indices;

	/* get n from user */
	if (argc == 1) {
		fprintf(stderr, "Missing argument: N\n");
		return PERM_ERR_INPUT;
	}
	input = strtol(argv[1], NULL, 10);
	if (input < 1) {
		fprintf(stderr, "Bad N argument: '%s'\n", argv[1]);
		return PERM_ERR_INPUT;
	}
	n = (size_t) input;

	/* initialize first permutation */
	arr = (char *) malloc(n);
	for (i = 0; i < n; i++) arr[i] = (char) (i + 65);

	/* allocate memory for index array */
	indices = (unsigned short *) malloc(perm_super_indices_size(n, sizeof(unsigned short)));
	if (indices == NULL) {
		free(arr);
		fprintf(stderr, "Insufficient memory for indices!\n");
		return PERM_ERR_MALLOC;
	}

	/* create the permutation indices */
	r = perm_set_super_indices_ushort(n, indices);
	if (r != PERM_OK) {
		fprintf(stderr, "perm_set_super_indices_ushort() encountered error: ");
		switch (r) {
			case PERM_ERR_INPUT:      fprintf(stderr, "bad input!\n");        break;
			case PERM_ERR_MALLOC:     fprintf(stderr, "malloc failure!\n");   break;
			case PERM_ERR_INDEX_TYPE: fprintf(stderr, "small index type!\n"); break;
		}
		free(arr);
		free(indices);
		return r;
	}

	/* allocate memory for superpermutation array */
	super_size = perm_super_size(n, 1);
	super = (char *) malloc(super_size);
	if (super == NULL) {
		free(arr);
		free(indices);
		fprintf(stderr, "Insufficient memory for superpermutation!\n");
		return PERM_ERR_MALLOC;
	}

	/* create the superpermutation */
	r = perm_set_super(arr, n, 1, super);
	if (r != PERM_OK) {
		fprintf(stderr, "perm_set_super() encountered error: ");
		switch (r) {
			case PERM_ERR_INPUT:  fprintf(stderr, "bad input!\n");      break;
			case PERM_ERR_MALLOC: fprintf(stderr, "malloc failure!\n"); break;
		}
		free(arr);
		free(indices);
		free(super);
		return r;
	}

	/* print the superpermutation */
	printf("Superpermutation:\n");
	fwrite(super, 1, super_size, stdout);
	printf("\n\n");

	/* print all permutations */
	count = perm_count(n);
	for (i = 0; i < count; i++) {
		fwrite(&super[indices[i]], 1, n, stdout);
		fputc('\t', stdout);
	}
	fputc('\n', stdout);

	/* free allocated memory and exit */
	free(indices);
	free(super);
	free(arr);
	return PERM_OK;
}
