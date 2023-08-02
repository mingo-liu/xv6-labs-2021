#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main() {
	int pid, p[3];
	char buf[21];
	
	pipe(p);

	pid = fork();
	if (pid == 1) {
		close(p[2]);
		read(p[1], buf, 5);
		close(p[1]);

//		printf("%s\n", buf);
		printf("%d: received ping\n", getpid());
		exit(1);
	} else {
		close(p[1]);
		write(p[2], "ping\n", 5);
		close(p[2]);

		wait(0);

		printf("%d: received pong\n", getpid());
	}
	exit(1);
}
