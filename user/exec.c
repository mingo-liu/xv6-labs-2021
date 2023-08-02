#include "kernel/types.h"
#include "user/user.h"

int 
main()
{
	char *argv[] = {"echo", "this", "is", "echo", 0};
	exec("echo", argv);
	printf("exec failed!\n");

	exit(0);
}
