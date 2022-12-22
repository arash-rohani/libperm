#include <stdio.h>

#include "perm.h"

int main(int argc, char *argv[]) {
	long input;
	int r;
	size_t i, n, len;
	char *arr, *perms;

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

	/* allocate memory for permutation array */
	perms = (char *) malloc(perm_size(n, 1));
	if (perms == NULL) {
		free(arr);
		fprintf(stderr, "Insufficient memory!\n");
		return PERM_ERR_MALLOC;
	}

	/* create the permutations */
	r = perm_set(arr, n, 1, perms);
	if (r != PERM_OK) {
		fprintf(stderr, "perm_set() encountered error: ");
		switch (r) {
			case PERM_ERR_INPUT:  fprintf(stderr, "bad input!\n");      break;
			case PERM_ERR_MALLOC: fprintf(stderr, "malloc failure!\n"); break;
		}
		free(arr);
		free(perms);
		return r;
	}

	/* print all permutations */
	len = perm_length(n);
	for (i = 0; i < len; i += n) {
		fwrite(&perms[i], 1, n, stdout);
		fputc('\t', stdout);
	}
	fputc('\n', stdout);

	/* free allocated memory and exit */
	free(perms);
	free(arr);
	return PERM_OK;
}
