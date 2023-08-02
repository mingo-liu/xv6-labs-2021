#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int 
main()
{
	char *argv[2];
	argv[0] = "cat";
	argv[1] = 0;
	if (fork() == 0) {
		close(0);
		open("input.txt", O_RDONLY);
		exec("cat", argv);
		printf("exec failed!\n");
	}
	exit(0);
}
