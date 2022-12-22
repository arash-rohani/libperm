#include <stdio.h>

#include "perm.h"

int main(int argc, char *argv[]) {
	long input;
	int r;
	size_t i, n, count, len;
	int *arr, *super;
	size_t *indices;
	char sep, *format;

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
	arr = (int *) malloc(n * sizeof(int));
	for (i = 0; i < n; i++) arr[i] = (int) (i + 1);

	/* allocate memory for index array */
	indices = (size_t *) malloc(perm_super_indices_size(n, sizeof(size_t)));
	if (indices == NULL) {
		free(arr);
		fprintf(stderr, "Insufficient memory for indices!\n");
		return PERM_ERR_MALLOC;
	}

	/* create the permutation indices */
	r = perm_set_super_indices(n, indices);
	if (r != PERM_OK) {
		fprintf(stderr, "perm_set_super_indices() encountered error: ");
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
	super = (int *) malloc(perm_super_size(n, sizeof(int)));
	if (super == NULL) {
		free(arr);
		free(indices);
		fprintf(stderr, "Insufficient memory for superpermutation!\n");
		return PERM_ERR_MALLOC;
	}

	/* create the superpermutation */
	r = perm_set_super(arr, n, sizeof(int), super);
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

	/* specify print formats */
	if (n < 10) {
		format = "%d";
		sep = '\t';
	} else {
		format = "%d ";
		sep = '\n';
	}

	/* print the superpermutation */
	len = perm_super_length(n);
	printf("Superpermutation:\n");
	for (i = 0; i < len; i++) printf(format, super[i]);
	printf("\n\n");

	/* print all permutations */
	count = perm_count(n);
	for (i = 0; i < count; i++) {
		size_t k = indices[i];
		size_t j;
		for (j = 0; j < n; j++) printf(format, super[k + j]);
		fputc(sep, stdout);
	}
	fputc('\n', stdout);

	/* free allocated memory and exit */
	free(indices);
	free(super);
	free(arr);
	return PERM_OK;
}
