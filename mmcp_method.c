#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

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

	struct stat sb;
	if(fstat(fd_in, &sb)==-1){
		perror("fstat");
		return -1;
	}

	char* src = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd_in, 0);
	if(src==MAP_FAILED){
		perror("mmap");
		return -1;
	}

	int fd_out = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if(fd_out==-1){
		perror("readwrite: open creat");
		return -1;
	}

	if(ftruncate(fd_out, sb.st_size)==-1){
		perror("ftruncate");
		return -1;
	}

	char* dst = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_out, 0);
	if(dst==MAP_FAILED){
		perror("dst mmap");
		return -1;
	}

	memcpy(dst, src, sb.st_size);

	if(msync(dst, sb.st_size, MS_SYNC)==-1){
		perror("msync");
		return -1;
	}

	return 0;
}

