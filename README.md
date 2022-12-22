libperm - C permutation library
===============================
libperm is a C library for creating permutations and superpermutations of any type of array.
It is written in C89 standard with both C and C++ compiler compatibility.

## Table of Contents
+ [Compilation](#compilation)
+ [Installation (Linux)](#installation-linux)
+ [Algorithms](#algorithms)
	- [Ordered Permutation Algorithm (OPA)](#ordered-permutation-algorithm-opa)
	  + [Advantages of OPA](#advantages-of-opa)
	- [Palindromic Superpermutation Indexing Algorithm (PSIA)](#palindromic-superpermutation-indexing-algorithm-psia)
	  + [Finding the superpermutation](#finding-the-superpermutation)
	  + [Indexing the permutations](#indexing-the-permutations)
	  + [Advantages of PSIA](#advantages-of-psia)
	  + [Disadvantages of PSIA](#disadvantages-of-psia)
+ [Benchmark](#benchmark)
+ [Examples](#examples)
+ [License](#license)

## Compilation
The library consists of only one header `perm.h` and one source file `perm.c`.
Instead of compiling as a shared library, it is recommended that you copy both files in your project directory and
compile `perm.c` alongside your other project files.

The library is written in C89 standard with C++ compatibility in mind so you shouldn't have any problem compiling it
with your favorite C or C++ compiler.
This has a better chance of optimization as well.

For example both of these commands should work:

	gcc -Wall -Wextra -Wconversion -O3 perm.c your_source.c
	g++ -Wall -Wextra -Wconversion -O3 perm.c your_source.cpp

## Installation (Linux)
If you don't want to statically link the library on Linux, you can use the provided makefile for installation.

	git clone https://github.com/arash-rohani/libperm.git
	cd libperm/misc/linux
	sudo make install DESTDIR=/usr

You can change or omit the `DESTDIR` option.

## Algorithms
This library uses two different algorithms for creating permutations. 

+ [Ordered Permutation Algorithm (OPA)](#ordered-permutation-algorithm-opa):
  Creates the permutations by following the order of items in the first permutation.

+ [Palindromic Superpermutation Indexing Algorithm (PSIA)](#palindromic-superpermutation-indexing-algorithm-psia):
  Creates the palindromic superpermutation of items and indexes each permutation inside that superpermutation.

### Ordered Permutation Algorithm (OPA)
To explain the algorithm, it is best to start with the finished result and try to find the patterns in it.
We'll use the example of `ABCD (N = 4)`.
The result will be all permutations of ABCD in alphabetical order.
Each new permutation uses a reference to one of the previous permutations ("Reference" column) and swaps one of its
items ("Left" column) with another one ("Right" column).

| #  | Permutation | Reference            | Left              | Right            |
|:--:|:-----------:|:--------------------:|:-----------------:|:----------------:|
| 0  | ABCD        | _                    | _                 | _                |
| 1  | ABDC        | Row - 1! = 0  (ABCD) | N - 1 - 1 = 2 (C) | Left + 1 = 3 (D) |
| 2  | ACBD        | Row - 2! = 0  (ABCD) | N - 1 - 2 = 1 (B) | Left + 1 = 2 (C) |
| 3  | ACDB        | Row - 1! = 2  (ACBD) | N - 1 - 1 = 2 (B) | Left + 1 = 3 (D) |
| 4  | ADBC        | Row - 2! = 2  (ACBD) | N - 1 - 2 = 1 (C) | Left + 2 = 3 (D) |
| 5  | ADCB        | Row - 1! = 4  (ADBC) | N - 1 - 1 = 2 (B) | Left + 1 = 3 (C) |
| 6  | BACD        | Row - 3! = 0  (ABCD) | N - 1 - 3 = 0 (A) | Left + 1 = 1 (B) |
| 7  | BADC        | Row - 1! = 6  (BACD) | N - 1 - 1 = 2 (C) | Left + 1 = 3 (D) |
| 8  | BCAD        | Row - 2! = 6  (BACD) | N - 1 - 2 = 1 (A) | Left + 1 = 2 (C) |
| 9  | BCDA        | Row - 1! = 8  (BCAD) | N - 1 - 1 = 2 (A) | Left + 1 = 3 (D) |
| 10 | BDAC        | Row - 2! = 8  (BCAD) | N - 1 - 2 = 1 (C) | Left + 2 = 3 (D) |
| 11 | BDCA        | Row - 1! = 10 (BDAC) | N - 1 - 1 = 2 (A) | Left + 1 = 3 (C) |
| 12 | CABD        | Row - 3! = 6  (BACD) | N - 1 - 3 = 0 (B) | Left + 2 = 2 (C) |
| 13 | CADB        | Row - 1! = 12 (CABD) | N - 1 - 1 = 2 (B) | Left + 1 = 3 (D) |
| 14 | CBAD        | Row - 2! = 12 (CABD) | N - 1 - 2 = 1 (A) | Left + 1 = 2 (B) |
| 15 | CBDA        | Row - 1! = 14 (CBAD) | N - 1 - 1 = 2 (A) | Left + 1 = 3 (D) |
| 16 | CDAB        | Row - 2! = 14 (CBAD) | N - 1 - 2 = 1 (B) | Left + 2 = 3 (D) |
| 17 | CDBA        | Row - 1! = 16 (CDAB) | N - 1 - 1 = 2 (A) | Left + 1 = 3 (B) |
| 18 | DABC        | Row - 3! = 12 (CABD) | N - 1 - 3 = 0 (C) | Left + 3 = 3 (D) |
| 19 | DACB        | Row - 1! = 18 (DABC) | N - 1 - 1 = 2 (B) | Left + 1 = 3 (C) |
| 20 | DBAC        | Row - 2! = 18 (DABC) | N - 1 - 2 = 1 (A) | Left + 1 = 2 (B) |
| 21 | DBCA        | Row - 1! = 20 (DBAC) | N - 1 - 1 = 2 (A) | Left + 1 = 3 (C) |
| 22 | DCAB        | Row - 2! = 20 (DBAC) | N - 1 - 2 = 1 (B) | Left + 2 = 3 (C) |
| 23 | DCBA        | Row - 1! = 22 (DCAB) | N - 1 - 1 = 2 (A) | Left + 1 = 3 (B) |

Let's look at a few rows to see if we can spot a pattern:

| #  | Permutation | Reference            | Left              | Right            |
|:--:|:-----------:|:--------------------:|:-----------------:|:----------------:|
| 0  | ABCD        | _                    | _                 | _                |
| 1  | ABDC        | Row - 1! = 0  (ABCD) | N - 1 - 1 = 2 (C) | Left + 1 = 3 (D) |
| 2  | ACBD        | Row - 2! = 0  (ABCD) | N - 1 - 2 = 1 (B) | Left + 1 = 2 (C) |
| 6  | BACD        | Row - 3! = 0  (ABCD) | N - 1 - 3 = 0 (A) | Left + 1 = 1 (B) |

Permutations 1, 2 and 6 all have 0 as their reference, which means they are ahead of the first permutation by 1, 2, 6
rows respectively. This is equal to 1, 2, and 3 factorials.
The left swappable item for each row is at index `N - 1 - 1`, `N - 1 - 2`, and `N - 1 - 3` of the reference respectively.

This pattern is repeated throughout the table.
If the reference is at `Permutation Index - X!` then the left swappable item is at index `N - 1 - X` of the reference.
The right swappable item can only occupy `X` number of spots.
We need to keep track of those moving spots for each different `X` with an array (`count[X]`).

The full algorithm:

1.  Add the first permutation and set `X` to `1` and all counts to `0`.
2.  If `count[X]` is equal to `X` continue to step 3, otherwise skip to step 6.
3.  Set `count[X]` to `0`.
4.  Increment `X` by `1`.
5.  If `X` is smaller than `N` repeat step 2, otherwise end the operation **[END]**.
6.  Increment `count[X]` by `1`.
7.  Find the reference with `Permutation Index - X!`.
8.  Find the left item index with `N - 1 - X`.
9.  Find the right item index with `N - 1 - X + count[X]`.
10. Swap the left and right items of the reference and add the permutation.
11. Set `X` to `1`.
12. Repeat step 2. 

Because `X = 1` reference is always the permutation we just added (`Permutation Index - 1!`) and it always swaps the
last two items, we can optimize the algorithm better.
Whenever we add a permutation, we can immediately swap its last two items and add it again.
And instead of setting `X` to `1` we can set it to `2`.

The optimized version:

1.  Add the first permutation and set `X` to `2` and all counts to `0`.
2.  Swap the last 2 items of the previous permutation and add that again.
3.  If `count[X]` is equal to `X` continue to step 4, otherwise skip to step 7.
4.  Set `count[X]` to `0`.
5.  Increment `X` by `1`.
6.  If `X` is smaller than `N` repeat step 3, otherwise end the operation **[END]**.
7.  Increment `count[X]` by `1`.
8.  Find the reference with `Permutation Index - X!`.
9.  Find the left item index with `N - 1 - X`.
10. Find the right item index with `N - 1 - X + count[X]`.
11. Swap the left and right items of the reference and add the permutation.
12. Swap the last 2 items of the previous permutation and add that again.
13. Set `X` to `2`.
14. Repeat step 3. 

#### Advantages of OPA
+ There's no need to sort the result.
  The algorithm gives the permutations in the same order of items in the first permutation.
  If the first permutation is ABCDE all the permutations will be given in ascending order up to EDCBA.
  If the first permutation is EDCBA all the permutations will be given in descending order down to ABCDE.

+ Its speed is comparable to Heap's Algorithm (HA).
  Both OPA and HA use only one swap to get to the next permutation.
  If implemented optimally, their speed can be very close.
  OPA can be slightly faster than HA for bigger Ns.
  But if my implementation of HA can be optimized further and the same optimization technique cannot be applied to OPA,
  then the above statement could easily be false. See [benchmark](#benchmark).


### Palindromic Superpermutation Indexing Algorithm (PSIA)
This algorithm consists of two parts:
1. [Finding the superpermutation](#finding-the-superpermutation)
2. [Indexing the permutations](#indexing-the-permutations)

#### Finding the superpermutation
The standard approach to finding the palindromic superpermutation is described 
[here](https://www.gregegan.net/SCIENCE/Superpermutations/Superpermutations.html#ALG1).
The problem with that algorithm is that it's very slow.
There is another extremely fast way to find the same superpermutation.

To explain the algorithm, it is best to start with the finished result and try to find the patterns in it.
We'll use the example of `ABCDE (N = 5)`.

The palindromic superpermutation is:

    ABCDEABCDAEBCDABECDABCEDABCADEBCADBECADBCEADBCAEDBCABDECABDCEABDCAEBDCABEDCABACDEBACDBEACDBAECDBACEDBACBDEACBDAECBDACEBDACBEDACBADECBADCEBADCBEADCBAEDCBA

To make it easier to see the pattern, we'll split the string into terms starting with the first item ("Terms" column).
Each term has a moving part (underlined) of length L ("L" column) located at index M of the term ("M" column).
New terms are created by performing one of the 3 different operations ("Opteration" column) on the reference term 
("Reference" column): shifting the moving part to the right, cutting the moving part out, or inserting the moving part
at index 1 of the reference. Insert will always happen after a cut and cut always happens when the moving part cannot be
shifted any more to the right.

| #  | Terms            | Reference             | L | M | Operation |
|:--:|:-----------------|:---------------------:|:-:|:-:|:---------:|
| 0  | ABCDE            | _                     | _ | 4 | _         |
| 1  | ABCD             | Row - 1  = 0  (ABCDE) | 1 | 4 | Cut       |
| 2  | A<ins>E</ins>BCD | Previous Row          | 1 | 1 | Insert    |
| 3  | AB<ins>E</ins>CD | Row - 1  = 2  (AEBCD) | 1 | 2 | Shift     |
| 4  | ABC<ins>E</ins>D | Row - 1  = 3  (ABECD) | 1 | 3 | Shift     |
| 5  | ABC              | Row - 5  = 0  (ABCDE) | 2 | 3 | Cut       |
| 6  | A<ins>DE</ins>BC | Previous Row          | 2 | 1 | Insert    |
| 7  | ADB<ins>E</ins>C | Row - 1  = 6  (ADEBC) | 1 | 3 | Shift     |
| 8  | ADBC<ins>E</ins> | Row - 1  = 7  (ADBEC) | 1 | 4 | Shift     |
| 9  | ADBC             | Row - 1  = 8  (ADBCE) | 1 | 4 | Cut       |
| 10 | A<ins>E</ins>DBC | Previous Row          | 1 | 1 | Insert    |
| 11 | AB<ins>DE</ins>C | Row - 5  = 6  (ADEBC) | 2 | 2 | Shift     |
| 12 | ABDC<ins>E</ins> | Row - 1  = 11 (ABDEC) | 1 | 4 | Shift     |
| 13 | ABDC             | Row - 1  = 12 (ABDCE) | 1 | 4 | Cut       |
| 14 | A<ins>E</ins>BDC | Previous Row          | 1 | 1 | Insert    |
| 15 | AB<ins>E</ins>DC | Row - 1  = 14 (ABEDC) | 1 | 2 | Shift     |
| 16 | AB               | Row - 16 = 0  (ABCDE) | 3 | 2 | Cut       |
| 17 | A<ins>CDE</ins>B | Previous Row          | 3 | 1 | Insert    |
| 18 | ACDB<ins>E</ins> | Row - 1  = 17 (ACDEB) | 1 | 4 | Shift     |
| 19 | ACDB             | Row - 1  = 18 (ACDBE) | 1 | 4 | Cut       |
| 20 | A<ins>E</ins>CDB | Previous Row          | 1 | 1 | Insert    |
| 21 | AC<ins>E</ins>DB | Row - 1  = 20 (AECDB) | 1 | 2 | Shift     |
| 22 | ACB<ins>DE</ins> | Row - 5  = 17 (ACDEB) | 2 | 3 | Shift     |
| 23 | ACBD             | Row - 1  = 22 (ACBDE) | 1 | 4 | Cut       |
| 24 | A<ins>E</ins>CBD | Previous Row          | 1 | 1 | Insert    |
| 25 | AC<ins>E</ins>BD | Row - 1  = 24 (AECBD) | 1 | 2 | Shift     |
| 26 | ACB<ins>E</ins>D | Row - 1  = 25 (ACEBD) | 1 | 3 | Shift     |
| 27 | ACB              | Row - 5  = 22 (ACBDE) | 2 | 3 | Cut       |
| 28 | A<ins>DE</ins>CB | Previous Row          | 2 | 1 | Insert    |
| 29 | ADC<ins>E</ins>B | Row - 1  = 28 (ADECB) | 1 | 3 | Shift     |
| 30 | ADCB<ins>E</ins> | Row - 1  = 29 (ADCEB) | 1 | 4 | Shift     |
| 31 | ADCB             | Row - 1  = 30 (ADCBE) | 1 | 4 | Cut       |
| 32 | A<ins>E</ins>DCB | Previous Row          | 1 | 1 | Insert    |
| 33 | A                | Row - 33 = 0  (ABCDE) | 4 | 1 | Cut       |

To find out how the references are found, let's look at all the rows that have row 0 as their reference:

| #  | Terms            | Reference             | L | M | Operation |
|:--:|:-----------------|:---------------------:|:-:|:-:|:---------:|
| 0  | ABCDE            | _                     | _ | 4 | _         |
| 1  | ABCD             | Row - 1  = 0  (ABCDE) | 1 | 4 | Cut       |
| 5  | ABC              | Row - 5  = 0  (ABCDE) | 2 | 3 | Cut       |
| 16 | AB               | Row - 16 = 0  (ABCDE) | 3 | 2 | Cut       |
| 33 | A                | Row - 33 = 0  (ABCDE) | 4 | 1 | Cut       |

We can see that whenever the same row is referenced, L (length of the moving part) increases and M (index of the
moving part) decreases.

L also directly corresponds to the distance of reference term to the current one.
If we define `f(0) = 0`, the term distance can be calculated by this formula:

    f(L) = f(L - 1) * (N + 1 - L) + 1  terms

Which for `N = 5` gives us:

    f(1) = f(1 - 1) * (5 + 1 - 1) + 1 = f(0) * 5 + 1 =  0 * 5 + 1 = 1  term 
    f(2) = f(2 - 1) * (5 + 1 - 2) + 1 = f(1) * 4 + 1 =  1 * 4 + 1 = 5  terms
    f(3) = f(3 - 1) * (5 + 1 - 3) + 1 = f(2) * 3 + 1 =  5 * 3 + 1 = 16 terms
    f(4) = f(4 - 1) * (5 + 1 - 4) + 1 = f(3) * 2 + 1 = 16 * 2 + 1 = 33 terms

But because the length of the terms are not equal, it's best to calculate the distance by number of items instead:

    f(L) =  (f(L - 1) + 1) * (N + 1 - L)  items

Which for `N = 5` gives us:

    f(1) = (f(1 - 1) + 1) * (5 + 1 - 1) = (f(0) + 1) * 5 = (0  + 1) * 5 = 5   items
    f(2) = (f(2 - 1) + 1) * (5 + 1 - 2) = (f(1) + 1) * 4 = (5  + 1) * 4 = 24  items
    f(3) = (f(3 - 1) + 1) * (5 + 1 - 3) = (f(2) + 1) * 3 = (24 + 1) * 3 = 75  items
    f(4) = (f(4 - 1) + 1) * (5 + 1 - 4) = (f(3) + 1) * 2 = (75 + 1) * 2 = 152 items

L itself also follows a nice and predictable pattern.
The number of times a specific `L` is repeated (`count[L]`) before it is incremented is always `N - L`.

Which for `N = 5` means:

    L = 1 repeats 4 times before L is incremented to 2.
    L = 2 repeats 3 times before L is incremented to 3.
    L = 3 repeats 2 times before L is incremented to 4.
    L = 4 only happens 1 time which also signals the end of the operation.

If we only count the shift operations, the number of repeats will be lowered by 2.
Which means the maximum `count[L]` will be `N - L - 2`.

The full algorithm:

1.  Add the first permutation and set `L` to `1`, `M` to `N - 1` and all counts to `0`.
2.  If `M + L` is equal to `N` continue to step 3, otherwise skip to step 10.
3.  Find the reference by going back `f(L)` terms or items.
4.  Cut the last `L` items from the reference and add the remaining term to the result.
5.  If `L` is less than `N - 1` continue to step 6, otherwise end the operation **[END]**.
6.  Insert the cut items at index `1` of the remaining term and add that to the result as well.
7.  Set `M` to `L`.
8.  Set `L` to `1`.
9.  Repeat step 2.
10. If `count[L]` is equal to `N - L - 2` continue to step 11, otherwise skip to step 14.
11. Set `count[L]` to `0`.
12. Increment `L` by `1`.
13. Repeat step 2.
14. Find the reference by going back `f(L)` terms or items.
15. Shift `L` items of the reference starting at index `M` one position to the right and add it to the result.
16. Increment `M` by `L`.
17. Increment `count[L]` by `1`.
18. Set `L` to `1`. 
19. Repeat step 2.

#### Indexing the permutations
Once we have the palindromic superpermutation, we need to index all the permutations inside it.
Superpermutation length `LEN` is calculated by the cumulative sum of `1!` to `N!`.
The first permutation is always at index `0` and the last one is at index `LEN - N`.

We will increment the first index and decrement the last one by the same amount to get the next two indices.

The increment/decrement amount follows the exact pattern we've discussed in the previous section:

> L itself also follows a nice and predictable pattern.
> The number of times a specific `L` is repeated (`count[L]`) before it is incremented is always `N - L`.
> 
> Which for `N = 5` means:
> 
>     L = 1 repeats 4 times before L is incremented to 2.
>     L = 2 repeats 3 times before L is incremented to 3.
>     L = 3 repeats 2 times before L is incremented to 4.
>     L = 4 only happens 1 time which also signals the end of the operation.

The full algorithm:

1.  Set `L` to `1`, `LEN` to `cumsum(factorials 1 to N)`, `I` to `0`, `X` to `0`, and all counts to `0`.
2.  If `count[L]` is equal to `N - L` continue to step 3, otherwise skip to step 6.
3.  Set `count[L]` to `0`.
4.  Increment `L` by `1`.
5.  If `L` is less than `N - 1` repeat step 2, otherwise follow step 6 and 7 and end the operation **[END]**.
6.  Set `indices[I]` to `X`.
7.  Set `indices[N! - 1 - I]` to `LEN - N - X`.
8.  Increment `X` by `L`.
9.  Increment `I` by `1`.
10. Increment `count[L]` by `1`.
11. Set `L` to `1`.
12. Repeat step 2.

#### Advantages of PSIA
There are only two advantages to this algorithm, both of which can be negated if not applied correctly.

+ Faster than OPA and Heap's algorithm if N is not too small.
  See [benchmark](#benchmark).

+ Uses less memory than normal permutations if correct index type is used, especially for bigger item sizes.
  See [benchmark](#benchmark).

#### Disadvantages of PSIA
In general this algorithm should be avoided unless the potential speed gain or lower memory usage are **crucial** to
your application.

- The permutations are unordered.

- Because permutations are intertwined, they can't be changed individually.
  This effectively makes the permutations read-only.

- Accessing the permutations is slightly slower because the index has to be read first and **then** the permutation.
  You have to, for example, use `super[index[i]]` instead of `super[i]`.

- Index type has to be chosen with care, especially for small Ns.
  Depending on the number and size of the items, the indices might use much more memory than the superpermutation
  itself.
  If the index type is not chosen optimally, the combination of superpermutation and the indices might use **more**
  memory than all the permutations themselves, negating one of the only advantages of PSIA.
  For example let's assume N is 4, and item size is 1 byte.
  The size of a normal permutation array would be `N! * N * item_size = 4! * 4 * 1 = 96 bytes`.
  If we use PSIA with an index size of 8, the combination of superpermutation and indices would be
  `cumsum(factorials 1 to N) * item_size + N! * index_size =  33 * 1 + 24 * 8 =  225 bytes`.
  As you can see, the required memory more than doubled because the index type we used was too big for such a
  small N.
  The 24 permutation indices could have easily fit inside a 1 byte index type. If we used `unsigned char` as the index
  type, the total size would've been `33 * 1 + 24 * 1 = 57 bytes`, which is almost half the size of a normal permutation
  array. See [benchmark](#benchmark).
  
- Because of the overhead of the two functions, it might be slower to use this algorithm over OPA for small Ns.
  See [benchmark](#benchmark).

## Benchmark
The goal of the benchmark is to measure the speed and final memory usage of the algorithms for different Ns and data
types. N ranges between 2 and 12, and the different data types are char, int, and long.

The benchmark result can be found at `misc/benchmark_output.txt`.

`time` is the average execution time of both creating the permutations and iterating through them in seconds.

`mem` is the size of the permutation array for OPA and Heap's algorithm, and the size of superpermutation array plus
the index array for PSIA.

`ERROR = 3` means the index type is too small for N.

`ERROR = 2` means there's insufficient memory.

To review the benchmark source and my implementation of Heap's algorithm you can check out `misc/linux/benchmark.c`.

## Examples
C and C++ example source files have been provided in `examples` directory.
Each uses a different data type and one of the two algorithms to find permutations.

## License
libperm is licensed under MIT license.
