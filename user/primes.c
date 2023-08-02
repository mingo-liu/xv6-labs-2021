#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int isPrime(int x) {
	int i;
	if (x <= 1) return 0;
	for(i = 2; i*i <= x; i++) {
		if (x % i == 0)
			return 0;
	}
	return 1;
}

int main() {
	int i;
	int p[2], pid;
	for (i = 2;i <= 35; i++) {
		if (isPrime(i)) {
			pipe(p);
			if ((pid = fork())){
				int re;

				close(p[1]);
				read(p[0], &re, sizeof(re));
				close(p[0]);

				printf("prime %d\n", re);
				exit(0);
			} else {
				close(p[0]);	
				write(p[1], &i, sizeof(i));
				close(p[1]);

				wait(0);
			}
		}
	}
	exit(0);
}

