/* Definitions of macros to access `dev_t' values.
   Copyright (C) 1996, 1997, 1999, 2003 Free Software Foundation, Inc.
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

#ifndef _SYS_SYSMACROS_H
#define _SYS_SYSMACROS_H	1

#include <features.h>

/* If the compiler does not know long long it is out of luck.  We are
   not going to hack weird hacks to support the dev_t representation
   they need.  */
__extension__
extern __inline unsigned int gnu_dev_major (unsigned long long int __dev)
     __THROW;
__extension__
extern __inline unsigned int gnu_dev_minor (unsigned long long int __dev)
     __THROW;
__extension__
extern __inline unsigned long long int gnu_dev_makedev (unsigned int __major,
							unsigned int __minor)
     __THROW;

#if defined __GNUC__ && __GNUC__ >= 2
__extension__ extern __inline unsigned int
gnu_dev_major (unsigned long long int __dev) __THROW
{
  return ((__dev >> 8) & 0xfff) | ((unsigned int) (__dev >> 32) & ~0xfff);
}

__extension__ extern __inline unsigned int
gnu_dev_minor (unsigned long long int __dev) __THROW
{
  return (__dev & 0xff) | ((unsigned int) (__dev >> 12) & ~0xff);
}

__extension__ extern __inline unsigned long long int
gnu_dev_makedev (unsigned int __major, unsigned int __minor) __THROW
{
  return ((__minor & 0xff) | ((__major & 0xfff) << 8)
	  | (((unsigned long long int) (__minor & ~0xff)) << 12)
	  | (((unsigned long long int) (__major & ~0xfff)) << 32));
}
#endif


/* Access the functions with their traditional names.  */
#define major(dev) gnu_dev_major (dev)
#define minor(dev) gnu_dev_minor (dev)
#define makedev(maj, min) gnu_dev_makedev (maj, min)

#endif /* sys/sysmacros.h */
