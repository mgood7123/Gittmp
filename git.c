#include "git/git.h"

int main(int argc, char * argv[])
{
	argc--;
	argv++;
	char * a = builtin__argvtostring(argc, argv);
	git(a);
	free(a);
	return 0;
}
