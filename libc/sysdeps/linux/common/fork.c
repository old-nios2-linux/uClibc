/* vi: set sw=4 ts=4: */
/*
 * fork() for uClibc
 *
 * Copyright (C) 2000-2006 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#include "syscalls.h"
#include <unistd.h>

#ifdef __ARCH_HAS_MMU__
#ifdef __NR_fork
#define __NR___libc_fork __NR_fork
_syscall0(pid_t, __libc_fork);
strong_alias(__libc_fork,fork)
libc_hidden_proto(fork)
libc_hidden_def(fork)
#endif
#endif
