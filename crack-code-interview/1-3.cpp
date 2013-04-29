//#include <iostream>
//#include <iostream>
#include <stdio.h>
#include <string.h>

bool allCharsUnique(const char* s) {
	if (!s)
		return false;

	bool cs[0xff] = {0};
	unsigned char ch;

	while ( (ch = static_cast<unsigned char>(*s)) != '\0' ) {
		if ( cs[ch] )
			return false;

		cs[ch] = true;
		++s;
	}
	return true;
}

void removeDuplicates(char* str) {
	if ( !str )
		return;

	char first = *str;
	char* left = str;
	char* cur  = left + 1;

	while (*left && *cur ) {
		while (*cur) {
			if (*left == *cur)
				*cur = first;
			++cur;			
		}
		++left;
		while (*left && *left == first)
			++left;
		cur = left + 1;
	}

	/// at these point all duplicates have the value 'first'
	left = str + 1;
	cur = left;
	while (*cur) {
		if (*cur != first)
			*left++ = *cur;
		++cur;
	}
	*left = 0;
}


void test(char* str) {
	removeDuplicates(str);
	if ( str && !allCharsUnique(str) )
		printf("FAILED: %s\n", str);
}

void test(char* str, const char* expected) {
	removeDuplicates(str);
	if ( strcmp(str, expected) != 0 )
		printf("WAS: %s, EXPECTED: %s\n", str, expected);
}

int main() {
	char str[80];

	strcpy(str, "123");
	//test(str);
	test(str, "123");

	strcpy(str, "abcbdv");
	//test(str);
	test(str, "abcdv");

	strcpy(str, "111a2ra1aaarr223a");
	//test(str);
	test(str, "1a2r3");

	strcpy(str, "");
	test(str, "");

	strcpy(str, "\na411122233334411114433221111abcd");
	test(str);

	strcpy(str, "aaaaa");
	test(str);

	//strcpy(str, 0);
	test(0);

	return 0;
}