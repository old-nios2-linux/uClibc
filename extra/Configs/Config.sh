# Library Configuration rules for uClibc
#
# This file contains rules which are shared between multiple Makefiles.  Feel
# free to adjust to taste...  
#  -Erik Andersen <andersen@lineo.com> <andersee@debian.org>
# 
# Copyright (C) 2000 by Lineo, inc.
# Copyright (C) 2001 by Hewlett-Packard Australia
#
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU Library General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your option) any
# later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU Library General Public License for more
# details.
#
# You should have received a copy of the GNU Library General Public License
# along with this program; if not, write to the Free Software Foundation, Inc.,
# 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
#
# Derived in part from the Linux-8086 C library, the GNU C Library, and several
# other sundry sources.  Files within this library are copyright by their
# respective copyright holders.

# What arch do you want to compile for...
#TARGET_ARCH=arm
#TARGET_ARCH=i386
#TARGET_ARCH=m68k
#TARGET_ARCH=powerpc
TARGET_ARCH=sh
#TARGET_ARCH=sparc

# If you are running a cross compiler, you may want to set this
# to something more interesting...
NATIVE_CC = gcc
CROSS = /usr/cygnus/yapp-001013/H-i686-pc-linux-gnulibc2.1/bin/sh-linux-gnu-
CC = $(CROSS)gcc
AR = $(CROSS)ar
LD = $(CROSS)ld
NM = $(CROSS)nm
STRIPTOOL = $(CROSS)strip
#STRIPTOOL = /bin/true

# Set the following to `true' to make a debuggable build, and `false' for
# production builds.
DODEBUG = false

# Compiler warnings you want to see 
WARNINGS=-Wall

# Enable support for shared libraries?
HAVE_SHARED = false

# Note that the kernel source you use to compile with should be the same as the
# Linux kernel you run your apps on.  uClibc doesn't even try to achieve binary
# compatibility across kernel versions.  So don't expect, for example, uClibc
# compiled with Linux kernel 2.0.x to implement lchown properly, since 2.0.x
# can't do that. Similarly, if you compile uClibc vs Linux 2.4.x kernel headers,
# but then run on Linux 2.0.x, lchown will be compiled into uClibc, but won't
# work at all.  You have been warned.
KERNEL_SOURCE=../../../linux

#
# ARCH_CFLAGS if your have something special to add to the CFLAGS
#
ARCH_CFLAGS  = -DNO_UNDERSCORES

ifeq ($(strip $(TARGET_ARCH)),sh)
ifeq ($(strip $(TARGET_PROC)),SH2_BIG_THAMIS)
ARCH_CFLAGS += -DHIOS -mb -m2 -specs=/usr/cygnus/yapp-001013/H-i686-pc-linux-gnulibc2.1/lib/gcc-lib/sh-linux-gnu/2.96-yapp-001013/specs_sh2
HAS_MMU = false
endif
ifeq ($(strip $(TARGET_PROC)),SH3_BIG_UCLINUX)
ARCH_CFLAGS += -mb
HAS_MMU = false
endif
ifeq ($(strip $(TARGET_PROC)),SH3_LITTLE_UCLINUX)
ARCH_CFLAGS += -ml
HAS_MMU = false
endif
ifeq ($(strip $(TARGET_PROC)),SH3)
ARCH_CFLAGS += -ml
HAS_MMU = true
endif
endif

# Set this to `false' if you don't have/need basic floating point support
# support in libc (strtod, printf, scanf).  Set it to `true' otherwise.
# Note: If not true, Rules.mak disables libm as well.
HAS_FLOATING_POINT = true

# Set this to `false' if you don't have/need float functions in libm.
# Set it to `true' otherwise, and make sure HAS_FLOATING_POINT is true as well.
HAS_LIBM_FLOAT = false

# Set this to `false' if you don't have/need double functions in libm.
# Set it to `true' otherwise, and make sure HAS_FLOATING_POINT is true as well.
HAS_LIBM_DOUBLE = false

# Set this to `false' if you don't have/need long double functions in libm.
# Set it to `true' otherwise, and make sure HAS_FLOATING_POINT is true as well.
HAS_LIBM_LONG_DOUBLE = false

# Set this to `false' if you don't have/need "(unsigned) long long int" support.
# Set it to `true' otherwise.
# Affects *printf and *scanf functions.
# Also omits strto(u)ll, and (u)lltostr from the library if `false'.
HAS_LONG_LONG = false

# Set this to `false' if you don't have/need locale support; `true' otherwise.
# Note: Currently only affects the ctype functions.  You must also generate
# a locale file for anything but the C locale.  See directory extra/locale for
# a utility to do so.  Also see the following option.
HAS_LOCALE = false

# Set this to the path of your uClibc locale file directory.
# Warning!  This must be different than the glibc locale directory to avoid
# name conflicts, as the locale files are entirely different in format!
LOCALE_DIR = "/usr/share/uClibc-locale/"

# This specifies which malloc implementation is used.
# "malloc-simple" is very, very small, but is also very, very dumb 
# and does not try to make good use of memory or clean up after itself.
#
# "malloc" on the other hand is a bit bigger, but is pretty smart thereby
# minimizing memory wastage and reusing already allocated memory.  This 
# can be lots faster and safer IMHO.
#
# "malloc-930716" is from libc-5.3.12 and was/is the standard gnu malloc.
# It is actually smaller than "malloc", at least on i386.  Right now, it
# only works on i386 (and maybe m68k) because it needs sbrk.
MALLOC = malloc-simple
#MALLOC = malloc 
#MALLOC = malloc-930716

# If you want to collect common syscall code into one function, set to this to
# `true'.  Set it to false otherwise.
# On i386 this saves about than 2.8k over all syscalls.
# The idea came from the implementation in dietlibc.
# At present, only affects i386.
UNIFIED_SYSCALL = false

# If you want large file summit support (greater then 2 Gib), 
# turn this on.  This has no effect unless your kernel supports 
# lfs.  This currently does nothing, but may someday...
DOLFS = false

# If you want to include RPC support, enable this.  RPC is almost never used 
# for anything except NFS support, so unless you plan to use NFS, leave this
# disabled.  This is off by default.
INCLUDE_RPC = false

# If you want to include support for the next version of the Internet
# Protocol: IP version 6, enable this.  This is off by default.
INCLUDE_IPV6 = false

# If you want to compile the library as PIC code, turn this on.
DOPIC = false

# PREFIX is the directory prefix that is applied to all installed
# files.  Typically, it is set to /usr or /usr/local, although it
# could also be /opt/vendor_name_here/some/random/path/.  The
# install_runtime makefile target installs a few symbolic links
# based on PREFIX, not DEVEL_PREFIX.
#
# DEVEL_PREFIX is the directory into which the development environment
# will be installed.  Include files are installed into
# $(DEVEL_PREFIX)/include, libraries into $(DEVEL_PREFIX)/lib, etc.
# This directory is compiled into the uclibc cross compiler spoofer.
# Generally, DEVEL_PREFIX should be $(PREFIX)/$(TARGET_ARCH)-uclibc-linux.
# DEVEL_PREFIX is used by the Makefile install targets install_gcc,
# install_dev, and install_runtime.
# 
# TARGET_PREFIX is the directory into which the target runtime
# environment is installed.  The target runtime environment is
# what one would use for a embedded system where uclibc is the
# native libaray.  This will typically be a staging area for
# creating a root filesystem for the target system, so the default
# is in the local directory.
# 
# If you want to install to a temporary directory before copying
# files to their final location, you can change PREFIX after build
# but before 'make install'.
PREFIX = /opt/uClinux
DEVEL_PREFIX = $(PREFIX)/$(TARGET_ARCH)-elf
TARGET_PREFIX = $(TOPDIR)/_install

