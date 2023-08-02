#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


char* fmtname(char *path) {
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
//  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
	buf[strlen(p)] ='\0'; //一定要有，不然会读取buf中的残留的尾部字符
  return buf;
}

void find(char *path, char *target) {
	int fd;
	char buf[512], *p;
	struct stat st;
	struct dirent de;

	if ((fd = open(path, 0)) < 0) {
		fprintf(2, "find: cannot open %s\n", path);
		return;
	}
	
	if (fstat(fd, &st) < 0) {
		fprintf(2, "find: cannot stat %s\n", path);
		close(fd);
		return;
	}

	// for file	
	if (st.type == T_FILE) {
//		printf("%s----%s\n", fmtname(path), path);
		if (strcmp(target, fmtname(path)) == 0) { 
			write(1, path, strlen(path));
			write(1, "\n", 1);
		} 
		close(fd);											
		return;														
	}

	// for directory 
	if (st.type == T_DIR) {
		if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
			printf("find: path too long\n");
			return;
		}
		strcpy(buf, path);
		p = buf + strlen(buf);
		*p++ = '/';
		while (read(fd, &de, sizeof(de)) == sizeof(de)) {
			if (de.inum == 0)
				continue;
			if ((strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0))
				continue;
			memmove(p, de.name, DIRSIZ);
			p[DIRSIZ] = 0;
			if (stat(buf, &st) < 0) {
				printf("find: cannot stat--- %s\n", buf);
				continue;
			}
			find(buf, target);
//			printf("%s\n", buf);
		}
	}
	close(fd);
}

int main (int argc, char *argv[]) {
	if (argc < 3) {
		fprintf(2, "find: less than three parameters\n");
		exit(1);
	}
	find(argv[1], argv[2]);
	exit(0);
}
