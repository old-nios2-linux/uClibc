#include <errno.h>
#include "dirstream.h"

int dirfd(DIR * dir)
{
	if (!dir || dir->dd_fd == -1) {
		errno = EBADF;
		return -1;
	}

	return dir->dd_fd;
}
