#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
	int t;
	if (argc < 2)	{
		fprintf(2, "Usage: sleep...\n");
		exit(1);
	}
	t = atoi(argv[1]);
	sleep(t);	
	fprintf(2, "(nothing happens for a little while)");

	exit(0);
}
