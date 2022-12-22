#include <iostream>
#include <vector>

#include "perm.h"

int main(int argc, char *argv[]) {
	using namespace std;
	vector<int> arr, perms;

	/* get n from user */
	if (argc == 1) {
		cerr << "Missing argument: N\n";
		return PERM_ERR_INPUT;
	}
	int input = atoi(argv[1]);
	if (input < 1) {
		cerr << "Bad N argument: '" << argv[1] << "\'\n";
		return PERM_ERR_INPUT;
	}
	size_t n = (size_t) input;

	/* initialize first permutation */
	for (size_t i = 0; i < n; i++) arr.push_back((int) (i + 1));

	/* create the permutations */
	perms.resize(perm_length(n));
	int r = perm_set(&arr[0], n, sizeof(int), &perms[0]);
	if (r != PERM_OK) {
		cerr << "perm_set() encountered error: ";
		switch (r) {
			case PERM_ERR_INPUT:  cerr << "bad input!\n";      break;
			case PERM_ERR_MALLOC: cerr << "malloc failure!\n"; break;
		}
		return r;
	}

	/* print all permutations and exit */
	size_t count = perm_count(n);
	if (n < 10) {
		for (size_t i = 0; i < count; i++) {
			size_t ni = n * i;
			for (size_t j = 0; j < n; j++) cout << perms[ni + j];
			cout << '\t';
		}
		cout << '\n';
	} else {
		for (size_t i = 0; i < count; i++) {
			size_t ni = n * i;
			for (size_t j = 0; j < n; j++) cout << perms[ni + j] << ' ';
			cout << '\n';
		}
	}
	return PERM_OK;
} 
