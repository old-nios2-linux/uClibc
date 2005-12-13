/* Copyright (C) 1994, 1996, 1997, 1998, 2000 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <features.h>

#ifdef __UCLIBC_HAS_LFS__
#if defined _FILE_OFFSET_BITS && _FILE_OFFSET_BITS != 64 
#undef _FILE_OFFSET_BITS
#define	_FILE_OFFSET_BITS   64
#endif
#ifndef __USE_FILE_OFFSET64
# define __USE_FILE_OFFSET64	1
#endif
#ifndef __USE_LARGEFILE64
# define __USE_LARGEFILE64	1
#endif
#endif

#define __USE_GNU

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/syscall.h>

#ifdef __NR_fcntl64
extern int fcntl64 (int fd, int cmd, ...);
#define flock flock64
#define fcntl fcntl64
#define F_GETLK F_GETLK64
#define F_SETLK F_SETLK64
#endif

/* lockf is a simplified interface to fcntl's locking facilities.  */

int attribute_hidden __lockf64 (int fd, int cmd, off64_t len64)
{
    struct flock fl;
    off_t len = (off_t) len64;

    if (len64 != (off64_t) len)
    {
	/* We can't represent the length.  */
	__set_errno(EOVERFLOW);
	return -1;
    }

    __memset((char *) &fl, '\0', sizeof (fl));

    /* lockf is always relative to the current file position.  */
    fl.l_whence = SEEK_CUR;
    fl.l_start = 0;
    fl.l_len = len;

    switch (cmd)
    {
	case F_TEST:
	    /* Test the lock: return 0 if FD is unlocked or locked by this process;
	       return -1, set errno to EACCES, if another process holds the lock.  */
	    fl.l_type = F_RDLCK;
	    if (fcntl (fd, F_GETLK, &fl) < 0)
		return -1;
	    if (fl.l_type == F_UNLCK || fl.l_pid == __getpid ())
		return 0;
	    __set_errno(EACCES);
	    return -1;

	case F_ULOCK:
	    fl.l_type = F_UNLCK;
	    cmd = F_SETLK;
	    break;
	case F_LOCK:
	    fl.l_type = F_WRLCK;
	    cmd = F_SETLKW;
	    break;
	case F_TLOCK:
	    fl.l_type = F_WRLCK;
	    cmd = F_SETLK;
	    break;

	default:
	    __set_errno(EINVAL);
	    return -1;
    }

    return fcntl(fd, cmd, &fl);
}

strong_alias(__lockf64,lockf64)
