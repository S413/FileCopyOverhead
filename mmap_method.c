#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, const char* argv[]){
	if(argc!=3){
		printf("Usage %s <source> <destination>\n", argv[0]);
		return -1;
	}

	int fd_in = open(argv[1], O_RDONLY);
	if(fd_in==-1){
		perror("readwrite: open");
		return -1;
	}

	int fd_out = open("tmpfs_mount/filecopy.txt", O_RDWR | O_CREAT, 0666);
	if(fd_out==-1){
		perror("readwrite: creat");
		return -1;
	}

	size_t filesize = lseek(fd_in, 0, SEEK_END);

	ftruncate(fd_out, filesize);

	char* src = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd_in, 0);
	char* dest = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd_out, 0);

	memcpy(dest, src, filesize);

	munmap(src, filesize);
	munmap(dest, filesize);

	close(fd_in);
	close(fd_out);

	return 0;
}
