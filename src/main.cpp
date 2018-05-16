
#include <stdio.h>
#include <string>

using namespace std;

int main(int argc, char const *argv[])
{
	const char *filename;
	if (argc == 2) {
		filename = argv[1];
		printf("Opening %s\n", filename);
	} else {
		printf("Usage: main <filename>\n");
		return 1;
	}
	return 0;
}

