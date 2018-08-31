#include <stdio.h>
#include "BigFloat.h" 


void checkMem() {
	// Send all reports to STDOUT
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
	printf("Checked for Memory Leaks: %d\n", _CrtDumpMemoryLeaks());
	//system("pause");
}

int _main() {
	BigFloat<> a(340);
	BigFloat<> b(-0.000051);
	BigFloat<> c(17.451);
	BigFloat<> d(2.5464);
	BigFloat<> e(3);
	BigFloat<> f = -a;


	const int LEN = 10;
	char numbers[5][LEN];
	a.toString(numbers[0], LEN);
	b.toString(numbers[1], LEN);
	c.toString(numbers[2], LEN);
	d.toString(numbers[3], LEN);
	e.toString(numbers[4], LEN);
	printf("%s, %f\n", numbers[0], a.toDouble());
	printf("%s, %f\n", numbers[1], b.toDouble());
	printf("%s, %f\n", numbers[2], c.toDouble());
	printf("%s, %f\n", numbers[3], d.toDouble());
	printf("%s, %f\n", numbers[4], e.toDouble());
	
	//printf("\nresults: %f, %f ; %f, %f\n\n", (a - b).toDouble(), (b - a).toDouble(), (-b - -a).toDouble(), (-a - -b).toDouble());  // correct

	//printf("\nresults: %f, %f\n", (a * b).toDouble(), (b * a).toDouble());
	//printf("\nresults: %f, %f\n", (d * d).toDouble(), (e * f).toDouble());
	//printf("\nresults: %f, %f, %f, %f\n", (t + c).toDouble(), (c + t).toDouble(), (a + d).toDouble(), (a + c).toDouble()); //correct


	

	printf("\nEND\n\n");
	return 0;
}
