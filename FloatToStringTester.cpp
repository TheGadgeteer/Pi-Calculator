#include <stdio.h>
#include <math.h>

void ftoa(double f, char *out, int afterpoint);

int test_main() {
	char c[50];
	double f = -232132.451;
	ftoa(f, c, 6);
	printf("%f: %s\n", f, c);
	return 0;
}


void ftoa(double f, char *out, int afterPoint) {
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
		while ((f /= base) >= 1)
			exp++;
	}
	int digitsAfterPoint = 0;
	// Digits in front of point
	while (digitsAfterPoint < afterPoint) {
		f *= base;
		int digit = (int)f;
		out[idx++] = digit + 48;
		f = modf(f, &throwaway);
		if (exp >= 0) {
			if (exp == 0)
				out[idx++] = '.';
			exp--;
		} else {
			digitsAfterPoint++;
		}
	}
	out[idx] = '\0';
}