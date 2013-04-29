//#include <iostream>
#include <stdio.h>
#include <string.h>

bool isAnagrams(const char* s1, const char* s2) {
	/// TODO - later
}

void test(const char* s1, const char* s2, bool result) {
	if ( isAnagrams(s1, s2) != result )
		printf("isAnagrams FAILED");
}

int main() {
	test("123", "312", true);
	test("111", "111", true);
	test("11",  "111", false);

	return 0;
}