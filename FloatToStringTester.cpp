#include <stdio.h>
#include <math.h>

void ftoa(double f, char *out, int afterpoint);

int test_main() {
	char c[50];
	double f = -232132.451;
	ftoa(f, c, 20);
	printf("%f: %s\n", f, c);
	return 0;
}


void ftoa(double f, char *out, int maxLen) {
	int exp = 0, idx = 0;
	const int base = 10;
	double throwaway;
	if (f < 0) {
		out[idx++] = '-';
		f = -f;
	}
	if (f < 1)
		out[idx++] = '0';
	else {
		do {
			f /= base;
			exp++;
		} while (f >= 1);
	}
	// Digits in front of point
	while (idx < maxLen) {
		f *= base;
		out[idx++] = (int)f + 48;
		f = modf(f, &throwaway);
		out[idx] = '\0';
		if (exp >= 0) {
			if (exp == 0)
				out[idx++] = '.';
			exp--;
		} else {
			if (f == 0)
				break;
		}
	}
	out[idx] = '\0';
}