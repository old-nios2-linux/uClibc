/* Wrapper for setting errno.
 * Copyright (C) 2000-2006 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#include <features.h>
#include <errno.h>

/* This routine is jumped to by all the syscall handlers, to stash
 * an error number into errno.  */
int attribute_hidden __syscall_error(int err_no)
{
	__set_errno(-err_no);
	return -1;
}
