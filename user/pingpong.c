#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main() {
	int pid, p1[2], p2[2];
	char buf[2];
	pipe(p1);
	pipe(p2);
	pid = fork();

	if (pid == 0) {
		close(p1[1]);
		read(p1[0], buf, 1);

//		printf("%s\n", buf);
		close(p1[0]);

		close(p2[0]);
		write(p2[1],buf, 1);
		close(p2[1]);

		printf("%d: received ping\n", getpid());
	} else {
		close(p1[0]);
		write(p1[1], "p", 1);
		close(p1[1]);

		wait(0);
		close(p2[1]);
		read(p2[0], buf, 1);
//		printf("%s\n", buf);
		close(p2[0]);

		printf("%d: received pong\n", getpid());
	} 
	exit(0);
}
