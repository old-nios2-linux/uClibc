/* vi: set sw=4 ts=4: */
/*
 * dup() for uClibc
 *
 * Copyright (C) 2000-2004 by Erik Andersen <andersen@codpoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 */

#include "syscalls.h"
#include <unistd.h>
_syscall1(int, dup, int, oldfd);
