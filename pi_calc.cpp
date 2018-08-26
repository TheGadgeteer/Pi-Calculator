//#include "BigFloat.h"
#include <stdio.h>
#include <stdlib.h>

typedef double BigFloat;

using namespace std;

void swapPointers(char **p1, char **p2) {
	char *temp = *p1;
	*p1 = *p2;
	*p2 = temp;
}

int findDiff(char* str1, char* str2) {
	int i = 0;
	char c1, c2;
	do {
		c1 = str1[i];
		c2 = str2[i];
		if (c1 != c2)
			return i;
		i++;
	} while (c1 != 0 && c2 != 0);
	return i;
}

int main(int argc, char** argv) {
	int curAccuracy = 0, maxAccuracy = 0xffffff;
	BigFloat pi(0), x;
	char *last_pi_str = new char[1000], *pi_str = new char[1000];
	last_pi_str[0] = 0
;
	if (argc > 1)
		maxAccuracy = atoi(argv[1]);
	printf("PI: \n");
	int val = 4;
	for (int n = 1; curAccuracy < maxAccuracy; n += 2) {
		x = val;
		x = x / n;
		pi = pi + x;
		val *= -1;

		swapPointers(&last_pi_str, &pi_str);
		sprintf(pi_str, "%f", pi);
		curAccuracy = findDiff(last_pi_str, pi_str);
		printf("\r%s, \t Accuracy: %d decimals", pi_str, curAccuracy);
	}
	return 0;
}
