//#include "BigFloat.h"
#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>
#include "BigFloat.h"

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
	BigFloat<20, 2> pi(0), x;
	int sgn = 1;
	char *last_pi_str = new char[1000], *pi_str = new char[1000];
	last_pi_str[0] = 0;
	if (argc > 1)
		maxAccuracy = atoi(argv[1]);
	printf("PI: \n");
	for (int n = 1; curAccuracy < maxAccuracy; n += 2) {
		x.assignFraction(1, n);
		x <<= 2;  // x *= 4
		sgn = -sgn;
		x.setSign(sgn);
		pi += x;

		swapPointers(&last_pi_str, &pi_str);
		pi.toString(pi_str, curAccuracy + 2);
		curAccuracy = findDiff(last_pi_str, pi_str);
		printf("\rAccuracy: %d decimals, n = %d:\t%s", curAccuracy, n, pi_str);
	}
	return 0;
}

/*
int exp_main(int argc, char** argv) {
	int curAccuracy = 0, maxAccuracy = 0xffffff;
	BigFloat<> e(1), x(1), nInv;
	char *last_e_str = new char[1000], *e_str = new char[1000];
	last_e_str[0] = 0;
	if (argc > 1)
		maxAccuracy = atoi(argv[1]);
	printf("eulersche Zahl: \n");
	for (int n = 1; curAccuracy < maxAccuracy; n++) {
		nInv.assignFraction(1, n);
		x *= nInv;
		e += x;

		swapPointers(&last_e_str, &e_str);
		e.toString(e_str, curAccuracy + 2);
		curAccuracy = findDiff(last_e_str, e_str);
		printf("\rAccuracy: %d decimals, n = %d:  %s\t", curAccuracy, n, e_str);
	}
	return 0;
}
*/
