/* Copyright (C) 1991, 1992 Free Software Foundation, Inc.
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
License along with the GNU C Library; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 675 Mass Ave,
Cambridge, MA 02139, USA.  */

#include <errno.h>
#include <signal.h>

#undef sigpause

/* Set the mask of blocked signals to MASK,
   wait for a signal to arrive, and then restore the mask.  */
int sigpause(int mask)
{
  sigset_t set;
  int sig;

  sigemptyset(&set);
  for (sig = 1; sig < NSIG; ++sig)
    if (mask & sigmask(sig))
      sigaddset(&set, sig);

  return sigsuspend (&set);
}
