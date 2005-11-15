/* Copyright (C) 1991,1994-1997,2001-2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <errno.h>
#include <signal.h>

/* Set the mask of blocked signals to MASK, returning the old mask.  */
int sigsetmask (int mask)
{
    register unsigned int sig;
    sigset_t set, oset;

    if (__sigemptyset (&set) < 0)
	return -1;

    if (sizeof (mask) == sizeof (set))
	*(int *) &set = mask;
    else if (sizeof (unsigned long int) == sizeof (set))
	*(unsigned long int *) &set = (unsigned int) mask;
    else
	for (sig = 1; sig < NSIG && sig <= sizeof (mask) * 8; ++sig)
	    if ((mask & sigmask (sig)) && __sigaddset (&set, sig) < 0)
		return -1;

    if (sigprocmask (SIG_SETMASK, &set, &oset) < 0)
	return -1;

    if (sizeof (mask) == sizeof (oset))
	mask = *(int *) &oset;
    else if (sizeof (unsigned long int) == sizeof (oset))
	mask = *(unsigned long int *) &oset;
    else
	for (sig = 1, mask = 0; sig < NSIG && sig <= sizeof (mask) * 8; ++sig)
	    if (__sigismember (&oset, sig))
		mask |= sigmask (sig);

    return mask;
}
