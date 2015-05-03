#ifndef __FILE_LOCK_H__
#define __FILE_LOCK_H__

int file_lock(char *tmpfile);
void file_unlock(int fd);

#endif
