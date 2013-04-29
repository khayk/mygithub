#include <stdio.h>
#include <string.h>

void replaceAll(char* src, const char* dst) {
	if ( !src || !dst )
		return;

	int srcLen = 0, spaces = 0;
	int dstLen = strlen(dst);

	char* tmp = src;
	while (*tmp != 0) {
		++srcLen;
		if (*tmp == ' ')
			++spaces;
		++tmp;
	}
	--tmp;
	int i = srcLen + spaces * (dstLen - 1);
	src[i--] = 0;
	while (tmp >= src) {
		if (*tmp != ' ') {
			src[i] = *tmp;
			--i;
		}
		else {
			int j = dstLen - 1;
			while (j >= 0) {
				src[i] = dst[j];
				--i;
				--j;
			}
		}
		--tmp;
	}
}

void test(char* src, const char* expected) {
	replaceAll(src, "%20");
	if ( strcmp(src, expected) != 0 )
		printf("WAS: %s, EXPECTED: %s\n", src, expected);
}

int main() {
	char str[80];

	strcpy(str, "12 3");
	test(str,  "12%203");

	strcpy(str, "ab c bdv");
	//test(str);
	test(str, "ab%20c%20bdv");

	strcpy(str, " ");
	test(str, "%20");

	return 0;
}
