/* Assembler macros for SH.
   Copyright (C) 1999, 2000 Free Software Foundation, Inc.
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

#define __need_uClibc_config_h 1
#include <bits/uClibc_config.h>

/* Define a macro we can use to construct the asm name for a C symbol.  */
#ifdef	__NO_UNDERSCORES__
#ifdef	__STDC__
#define C_LABEL(name)		name##:
#else
#define C_LABEL(name)		name/**/:
#endif
#else
#ifdef	__STDC__
#define C_LABEL(name)		_##name##:
#else
#define C_LABEL(name)		_/**/name/**/:
#endif
#endif

/* Mark the end of function named SYM.  This is used on some platforms
   to generate correct debugging information.  */
#ifndef END
#define END(sym)
#endif

#ifndef ASM_GLOBAL_DIRECTIVE
#define ASM_GLOBAL_DIRECTIVE .global
#endif

#ifndef C_SYMBOL_NAME
#define C_SYMBOL_NAME(name)	name
#endif

#ifdef	__ASSEMBLER__

/* Syntactic details of assembler.  */

#ifdef __HAVE_ELF__

#define ALIGNARG(log2) log2
/* For ELF we need the `.type' directive to make shared libs work right.  */
#define ASM_TYPE_DIRECTIVE(name,typearg) .type name,@##typearg;
#define ASM_SIZE_DIRECTIVE(name) .size name,.-name

#ifdef SHARED
#define PLTJMP(_x)	_x##@PLT
#else
#define PLTJMP(_x)	_x
#endif

#else

#define ALIGNARG(log2) log2
#define ASM_TYPE_DIRECTIVE(name,type)	/* Nothing is specified.  */
#define ASM_SIZE_DIRECTIVE(name)	/* Nothing is specified.  */
#define C_SYMBOL_NAME(name) name	/* Nothing is specified.  */
#define PLTJMP(_x)	_x

#endif

/* Define an entry point visible from C.  */
#define	ENTRY(name)							      \
  ASM_GLOBAL_DIRECTIVE C_SYMBOL_NAME(name);				      \
  ASM_TYPE_DIRECTIVE (C_SYMBOL_NAME(name),function)			      \
  .align ALIGNARG(5);							      \
  C_LABEL(name)								      \
  CALL_MCOUNT

#undef	END
#define END(name)							      \
  ASM_SIZE_DIRECTIVE(C_SYMBOL_NAME(name))

/* If compiled for profiling, call `mcount' at the start of each function.  */
#ifdef	PROF
#define CALL_MCOUNT	/* NOTYET */
#else
#define CALL_MCOUNT		/* Do nothing.  */
#endif

#ifdef	__NO_UNDERSCORES__
/* Since C identifiers are not normally prefixed with an underscore
   on this system, the asm identifier `syscall_error' intrudes on the
   C name space.  Make sure we use an innocuous name.  */
#define	syscall_error	__syscall_error
#define mcount		_mcount
#endif

#endif	/* __ASSEMBLER__ */
