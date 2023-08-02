#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
	int pid, i, j, n, m;
	int p[2];
	char buf[100];	
	char *args[100];

	pipe(p);
	pid = fork();

	if (pid == 0) {
		// one read cannot completly read the content of the standard input
 		while ((n = read(0, buf, sizeof(buf))) > 0) {
			write(p[1], buf, n);
		}
		n = read(p[0], buf, sizeof(buf));		
		close(p[0]);
		close(p[1]);

		for (i = 1; i < argc; i++) {
			args[i - 1] = argv[i];
		}

		j = argc - 1;
		m = 0;
		for (i = 0; i < n; i++) {
			if (buf[i] == '\n'){
				buf[i] = '\0';
				args[j++] = buf + m;	
				m = i + 1;
			}
		}

		exec(argv[1], args);
		printf("exec failed!\n");
		exit(1);
	} else {
		wait(0);
	}
	exit(0);
}
