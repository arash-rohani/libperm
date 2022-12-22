#include <iostream>
#include <vector>

#include "perm.h"

int main(int argc, char *argv[]) {
	using namespace std;
	vector<long> arr, super;
	vector<unsigned int> indices;

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
	for (size_t i = 0; i < n; i++) arr.push_back((long) (i + 1));

	/* create the permutation indices */
	indices.resize(perm_count(n));
	int r = perm_set_super_indices_uint(n, &indices[0]);
	if (r != PERM_OK) {
		cerr << "perm_set_super_indices_uint() encountered error: ";
		switch (r) {
			case PERM_ERR_INPUT:      cerr << "bad input!\n";        break;
			case PERM_ERR_MALLOC:     cerr << "malloc failure!\n";   break;
			case PERM_ERR_INDEX_TYPE: cerr << "small index type!\n"; break;
		}
		return r;
	}

	/* create the superpermutation */
	size_t len = perm_super_length(n);
	super.resize(len);
	r = perm_set_super(&arr[0], n, sizeof(long), &super[0]);
	if (r != PERM_OK) {
		cerr << "perm_set_super() encountered error: ";
		switch (r) {
			case PERM_ERR_INPUT:  cerr << "bad input!\n";      break;
			case PERM_ERR_MALLOC: cerr << "malloc failure!\n"; break;
		}
		return r;
	}

	/* print the superpermutation */
	cout << "Superpermutation:\n";
	if (n < 10) {
		for (size_t i = 0; i < len; i++) cout << super[i];
	} else {
		for (size_t i = 0; i < len; i++) cout << super[i] << ' ';
	}
	cout << "\n\n";

	/* print all permutations and exit */
	size_t count = perm_count(n);
	if (n < 10) {
		for (size_t i = 0; i < count; i++) {
			unsigned int k = indices[i];
			for (size_t j = 0; j < n; j++) cout << super[k + j];
			cout << '\t';
		}
		cout << '\n';
	} else {
		for (size_t i = 0; i < count; i++) {
			unsigned int k = indices[i];
			for (size_t j = 0; j < n; j++) cout << super[k + j] << ' ';
			cout << '\n';
		}
	}
	return PERM_OK;
} 
