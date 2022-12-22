#include <stdio.h>

#include "perm.h"

typedef struct custom_t {
	int num;
	char chr;
} custom_t;

int main(int argc, char *argv[]) {
	long input;
	int r;
	size_t i, n, count;
	custom_t *arr, *perms;

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
	arr = (custom_t *) malloc(n * sizeof(custom_t));
	for (i = 0; i < n; i++) {
		arr[i].num = (int) (i + 1);
		arr[i].chr = (char) (i + 65);
	}

	/* allocate memory for permutation array */
	perms = (custom_t *) malloc(perm_size(n, sizeof(custom_t)));
	if (perms == NULL) {
		free(arr);
		fprintf(stderr, "Insufficient memory!\n");
		return PERM_ERR_MALLOC;
	}

	/* create the permutations */
	r = perm_set(arr, n, sizeof(custom_t), perms);
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
	count = perm_count(n);
	for (i = 0; i < count; i++) {
		size_t ni = n * i;
		size_t j;
		for (j = 0; j < n; j++) printf("[%d: %c] ", perms[ni + j].num, perms[ni + j].chr);
		fputc('\n', stdout);
	}

	/* free allocated memory and exit */
	free(perms);
	free(arr);
	return PERM_OK;
}
