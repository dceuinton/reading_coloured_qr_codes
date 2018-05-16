
#include <stdio.h>
#include <string>


int main(int argc, char const *argv[])
{
	string filename;
	if (argc == 2) {
		filename = argv[1];
		printf("%s\n", filename);
	} else {
		printf("Usage: main <filename>\n");
		return 1;
	}
	return 0;
}