#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>

int file_lock(char *tmpfile) {
	int fd;
	fd = open(tmpfile, O_RDWR | O_CREAT | O_CLOEXEC, S_IRUSR);
	if (fd == -1) {
		perror("open");
		return -1;
	}
	
	if (flock(fd, LOCK_EX | LOCK_NB) == -1) {
		FILE *f;
		int pid;
		perror("flock");
		f = fdopen(fd, "rw");
		if (fscanf(f, "%d", &pid) == 1)
			printf("Lock pertence a %d\n", pid);
		fclose(f);
		return -1;
	}
	return fd;
}

void file_unlock(int fd) {
	// Desnecessário (lock é removido ao fechar o arquivo)
	flock(fd, LOCK_UN);
	close(fd);
}
