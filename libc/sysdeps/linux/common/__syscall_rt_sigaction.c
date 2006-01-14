/* vi: set sw=4 ts=4: */
/*
 * __syscall_rt_sigaction() for uClibc
 *
 * Copyright (C) 2000-2006 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#include "syscalls.h"

#ifdef __NR_rt_sigaction
#include <signal.h>

#define __NR___syscall_rt_sigaction __NR_rt_sigaction
#undef sigaction
attribute_hidden _syscall4(int, __syscall_rt_sigaction, int, signum,
		  const struct sigaction *, act, struct sigaction *, oldact,
		  size_t, size);
#endif
