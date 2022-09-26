#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUF_SIZE 1024 * 1024

int main(int argc, const char* argv[]){
	if(argc!=3){
		printf("Usage: %s <source> <destination>\n", argv[0]);
		return -1;
	}

	int fd_in = open(argv[1], O_RDONLY);
	if(fd_in==-1){
		perror("readwrite: open");
		return -1;
	}

	struct stat stbuf;
	fstat(fd_in, &stbuf);

	int fd_out = creat(argv[2], 0644);
	if(fd_out==-1){
		perror("readwrite: creat");
		return -1;
	}

	size_t leftsize = stbuf.st_size;
	size_t n = 0;

	while(leftsize > 0){
		n = BUF_SIZE;

		if(n > leftsize) n = leftsize;

		n = sendfile(fd_out, fd_in, 0, n);
		if(n < 0) printf("sendfile failed.");

		leftsize -= n;

	}

	close(fd_in);
	close(fd_out);

	return 0;
}
