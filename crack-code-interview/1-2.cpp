//#include <iostream>
#include <stdio.h>
#include <string.h>

void reverse(char* str) {
	char* end = str;
	char  tmp;
	if (str) {
		while (*end)
			++end;
		--end;
		while (str < end) {
			tmp = *str;
			*str++ = *end;
			*end-- = tmp;
		}
	}
}

void test(char* str) {
	printf("BEFORE: %s\n", str);
	reverse(str);
	printf(" AFTER: %s\n\n", str);
}

int main() {
	char str[80];

	strcpy(str, "123");
	test(str);

	strcpy(str, "ab");
	test(str);

	strcpy(str, "qwe123");
	test(str);

	strcpy(str, "");
	test(str);

	return 0;
}