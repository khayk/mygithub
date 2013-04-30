#include <iostream>
#include <vector>
#include <cstdio>
#include <string.h>

typedef std::vector<int>        tIntArray;
typedef std::vector<tIntArray>  tIntMatrix;

///-----------------------------------------------------------------------
void rotateImage(tIntMatrix& m) {
	size_t i, j;
	size_t n = m.size();

	for (size_t layer = 0; layer < n/2; ++layer) {
		for (size_t k = layer; k < n-layer-1; ++k) {
			i = layer;
			j = k;
			int tmp = m[i][j];
			for (size_t l = 0; l < 3; ++l) {
				m[i][j] = m[j][n-i-1];
				int s = i;
				i = j;
				j = n-s-1;
			}
			m[i][j] = tmp;
		}
	}
}

///-----------------------------------------------------------------------
void test(const tIntMatrix& a, const tIntMatrix& r) {
	if (a.size() != r.size() || a.size() == 0)
		return;

	size_t n = a.size();
	if (a[0].size() != n)
		return;

	for (int i = 0; i < n; ++i) {
		if (n != r[i].size())
			return;
		for (int j = 0; j < n; ++j) {
			if (a[i][j] != r[i][j]) {
				printf("FAILED");
				return;
			}
		}
	}
}

void print(const tIntMatrix& m) {
	for (auto i: m) {
		for (auto j: i)
			std::cout << j << ", ";
		std::cout << std::endl;
	}
}
int main() {
	tIntMatrix a;
	a.resize(3);
	a[0] = {1, 2, 3};
	a[1] = {4, 5, 6};
	a[2] = {7, 8, 9};

	print(a);
	rotateImage(a);

	tIntMatrix r;
	r.resize(4);

	r[0] = { 1,  2,  3, 4};
	r[1] = { 5,  6,  7, 8};
	r[2] = { 9, 10, 11, 12};
	r[3] = {13, 14, 15, 16};

	print(r);
	rotateImage(r);
	print(r);

	//test(a, r);
	return 0;
}
