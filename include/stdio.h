/* Define ISO C stdio on top of C++ iostreams.
   Copyright (C) 1991, 1994-1999, 2000 Free Software Foundation, Inc.

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

/*
 *	ISO C Standard: 4.9 INPUT/OUTPUT	<stdio.h>
 */

#ifndef _STDIO_H
#define _STDIO_H

#include <features.h>
#include <stdarg.h>
#include <sys/types.h>

__BEGIN_DECLS


/* when you add or change fields here, be sure to change the initialization
 * in stdio_init and fopen */
struct __stdio_file {
  unsigned char *bufpos;   /* the next byte to write to or read from */
  unsigned char *bufread;  /* the end of data returned by last read() */
  unsigned char *bufwrite; /* 1 + highest address writable by macro */
  unsigned char *bufstart; /* the start of the buffer */
  unsigned char *bufend;   /* the end of the buffer; ie the byte after the last
                              malloc()ed byte */
  struct __stdio_file * next;

  int fd; /* the file descriptor associated with the stream */

  unsigned char mode;
  unsigned char ungot;
  char unbuf[2];	   /* The buffer for 'unbuffered' streams */
};

typedef struct __stdio_file FILE;

/* Default buffer size.  */
#define BUFSIZ	    (512)

/* Define EOF and NULL */
#define EOF	(-1)
#ifndef NULL
#define NULL	(0)
#endif

/* The possibilities for the third argument to `setvbuf'.  */
#define _IOFBF 0 		/* Fully buffered.  */
#define _IOLBF 1		/* Line buffered.  */
#define _IONBF 2		/* No buffering.  */

/* Possible states for a file stream -- internal use only */
#define __MODE_BUF		0x03	/* Modal buffering dependent on isatty */
#define __MODE_FREEBUF	0x04	/* Buffer allocated by stdio code, can free */
#define __MODE_FREEFIL	0x08	/* FILE allocated by stdio code, can free */
#define __MODE_UNGOT	0x10	/* Buffer has been polluted by ungetc */
#define __MODE_TIED 	0x20	/* FILE is tied with stdin/stdout */
#define __MODE_EOF		0x40	/* EOF status */
#define __MODE_ERR		0x80	/* Error status */

/* The possibilities for the third argument to `fseek'.
   These values should not be changed.  */
#define SEEK_SET	0	/* Seek from beginning of file.  */
#define SEEK_CUR	1	/* Seek from current position.  */
#define SEEK_END	2	/* Seek from end of file.  */

/* Default path prefix for `tempnam' and `tmpnam'.  */
#define P_tmpdir	"/tmp"
/* Get the values:
   L_tmpnam	How long an array of chars must be to be passed to `tmpnam'.
   TMP_MAX	The minimum number of unique filenames generated by tmpnam
   		(and tempnam when it uses tmpnam's name space),
		or tempnam (the two are separate).
   L_ctermid	How long an array to pass to `ctermid'.
   L_cuserid	How long an array to pass to `cuserid'.
   FOPEN_MAX	Minimum number of files that can be open at once.
   FILENAME_MAX	Maximum length of a filename.  */
#define __need_FOPEN_MAX
#include <bits/stdio_lim.h>
#undef __need_FOPEN_MAX

/* Standard streams (internal).  */
extern FILE *_stdin;
extern FILE *_stdout;
extern FILE *_stderr;

/* C89/C99 say they're macros.  Make them happy.  */
#define stdin  _stdin
#define stdout _stdout
#define stderr _stderr

/* Remove file FILENAME.  */
extern int remove __P ((__const char *__filename));
/* Rename file OLD to NEW.  */
extern int rename __P ((__const char *__old, __const char *__new));


/* Create a temporary file and open it read/write.  */
extern FILE *tmpfile __P ((void));
#ifdef __USE_LARGEFILE64
extern FILE *tmpfile64 __P ((void));
#endif
/* Generate a temporary filename.  */
extern char *tmpnam __P ((char *__s));

#ifdef __USE_MISC
/* This is the reentrant variant of `tmpnam'.  The only difference is
   that it does not allow S to be NULL.  */
extern char *tmpnam_r __P ((char *__s));
#endif


#if defined __USE_SVID || defined __USE_XOPEN
/* Generate a unique temporary filename using up to five characters of PFX
   if it is not NULL.  The directory to put this file in is searched for
   as follows: First the environment variable "TMPDIR" is checked.
   If it contains the name of a writable directory, that directory is used.
   If not and if DIR is not NULL, that value is checked.  If that fails,
   P_tmpdir is tried and finally "/tmp".  The storage for the filename
   is allocated by `malloc'.  */
extern char *tempnam __P ((__const char *__dir, __const char *__pfx));
#endif


/* Close STREAM.  */
extern int fclose __P ((FILE *__stream));
/* Flush STREAM, or all streams if STREAM is NULL.  */
extern int fflush __P ((FILE *__stream));

/* Used internally to actuall open files */
extern FILE *__fopen __P((__const char *__restrict __filename, int __fd, 
	    FILE *__restrict __stream, __const char *__restrict __mode));
/* Open a file and create a new stream for it.  */
extern FILE *fopen __P ((__const char *__restrict __filename,
			 __const char *__restrict __mode));
#define fopen(__file, __mode)         __fopen((__file), -1, (FILE*)0, (__mode))
/* Open a file, replacing an existing stream with it. */
extern FILE *freopen __P ((__const char *__restrict __filename,
			   __const char *__restrict __mode,
			   FILE *__restrict __stream));

#ifdef __USE_MISC
/*
 * Open a file using an automatically (stack) or statically allocated FILE.
 * The FILE * returned behaves just as any other FILE * with respect to the
 * stdio functions, but be aware of the following:
 * NOTE: The buffer used for the file is FILE's builtin 2-byte buffer, so
 *       setting a new buffer is probably advisable.
 * NOTE: This function is primarily intended to be used for stack-allocated
 *       FILEs when uClibc stdio has been built with no dynamic memory support.
 *       For the statically allocated case, it is probably better to increase
 *        the value of FIXED_STREAMS in stdio.c.
 * WARNING: If allocated on the stack, make sure you call fclose before the
 *          stack memory is reclaimed!
 */
extern FILE *fsfopen __P ((__const char *__restrict __filename,
			   __const char *__restrict __mode,
			   FILE *__restrict __stream));
#endif


#ifdef __USE_LARGEFILE64
extern FILE *fopen64 __P ((__const char *__restrict __filename,
			   __const char *__restrict __mode));
extern FILE *freopen64 __P ((__const char *__restrict __filename,
			     __const char *__restrict __mode,
			     FILE *__restrict __stream));
#endif

#ifdef	__USE_POSIX
/* Create a new stream that refers to an existing system file descriptor.  */
extern FILE *fdopen __P ((int __fd, __const char *__mode));
#define fdopen(__file, __mode)  __fopen((char*)0, (__file), (FILE*)0, (__mode))
#endif


/* Make STREAM use buffering mode MODE.
   If BUF is not NULL, use N bytes of it for buffering;
   else allocate an internal buffer N bytes long.  */
extern int setvbuf __P ((FILE *__restrict __stream, char *__restrict __buf,
			 int __mode, size_t __n));

/* If BUF is NULL, make STREAM unbuffered.
   Else make it use buffer BUF, of size BUFSIZ.  */
extern void setbuf __P ((FILE *__restrict __stream, char *__restrict __buf));

#ifdef	__USE_BSD
/* If BUF is NULL, make STREAM unbuffered.
   Else make it use SIZE bytes of BUF for buffering.  */
extern void setbuffer __P ((FILE *__restrict __stream, char *__restrict __buf,
			    size_t __size));

/* Make STREAM line-buffered.  */
extern void setlinebuf __P ((FILE *__stream));
#endif


/* Write formatted output to STREAM.  */
extern int fprintf __P ((FILE *__restrict __stream,
			 __const char *__restrict __format, ...));
/* Write formatted output to stdout.  */
extern int printf __P ((__const char *__restrict __format, ...));
/* Write formatted output to S.  */
extern int sprintf __P ((char *__restrict __s,
			 __const char *__restrict __format, ...));

/* Write formatted output to a file descriptor */
extern int vdprintf __P((int fd, __const char *__restrict __format,
						 va_list __arg));

/* Write formatted output to a buffer S dynamically allocated by asprintf.  */
extern int asprintf __P ((char **__restrict __s,
			 __const char *__restrict __format, ...));

/* Write formatted output to S from argument list ARG.  */
extern int vfprintf __P ((FILE *__restrict __s,
			  __const char *__restrict __format,
			  va_list __arg));
/* Write formatted output to stdout from argument list ARG.  */
extern int vprintf __P ((__const char *__restrict __format,
			 va_list __arg));
/* Write formatted output to S from argument list ARG.  */
extern int vsprintf __P ((char *__restrict __s,
			  __const char *__restrict __format,
			  va_list __arg));

/* Maximum chars of output to write in MAXLEN.  */
extern int snprintf __P ((char *__restrict __s, size_t __maxlen,
			  __const char *__restrict __format, ...))
     __attribute__ ((__format__ (__printf__, 3, 4)));

extern int __vsnprintf __P ((char *__restrict __s, size_t __maxlen,
			     __const char *__restrict __format,
			     va_list __arg))
     __attribute__ ((__format__ (__printf__, 3, 0)));
extern int vsnprintf __P ((char *__restrict __s, size_t __maxlen,
			   __const char *__restrict __format,
			   va_list __arg))
     __attribute__ ((__format__ (__printf__, 3, 0)));

/* Read formatted input from STREAM.  */
extern int fscanf __P ((FILE *__restrict __stream,
			__const char *__restrict __format, ...));
/* Read formatted input from stdin.  */
extern int scanf __P ((__const char *__restrict __format, ...));
/* Read formatted input from S.  */
extern int sscanf __P ((__const char *__restrict __s,
			__const char *__restrict __format, ...));

/* Read formatted input from S into argument list ARG.  */
extern int vfscanf __P ((FILE *__restrict __s,
			 __const char *__restrict __format,
			 va_list __arg))
     __attribute__ ((__format__ (__scanf__, 2, 0)));

/* Read formatted input from stdin into argument list ARG.  */
extern int vscanf __P ((__const char *__restrict __format, va_list __arg))
     __attribute__ ((__format__ (__scanf__, 1, 0)));

/* Read formatted input from S into argument list ARG.  */
extern int vsscanf __P ((__const char *__restrict __s,
			 __const char *__restrict __format,
			 va_list __arg))
     __attribute__ ((__format__ (__scanf__, 2, 0)));


/* Read a character from STREAM.  */
extern int fgetc __P ((FILE *__stream));
extern int getc __P ((FILE *__stream));

/* Read a character from stdin.  */
extern int getchar __P ((void));
#define getchar() getc(_stdin)

/* The C standard explicitly says this is a macro, so be that way */
#define getc(stream)	\
  (((stream)->bufpos >= (stream)->bufread) ? fgetc(stream):		\
    (*(stream)->bufpos++))

/* Write a character to STREAM.  */
extern int fputc __P ((int __c, FILE *__stream));
extern int putc __P ((int __c, FILE *__stream));

/* Write a character to stdout.  */
extern int putchar __P ((int __c));
/* Beware! stdout can be redefined! */
#define putchar(c) putc((c), _stdout)

/* The C standard explicitly says this can be a macro, so be that way */
#define putc(c, stream)	\
    (((stream)->bufpos >= (stream)->bufwrite) ? fputc((c), (stream))	\
                          : (unsigned char) (*(stream)->bufpos++ = (c))	)

#if defined __USE_SVID || defined __USE_MISC || defined __USE_XOPEN
/* Get a word (int) from STREAM.  */
extern int getw __P ((FILE *__stream));

/* Write a word (int) to STREAM.  */
extern int putw __P ((int __w, FILE *__stream));
#endif


/* Get a newline-terminated string of finite length from STREAM.  */
extern char *fgets __P ((char *__restrict __s, int __n,
			 FILE *__restrict __stream));

/* Get a newline-terminated string from stdin, removing the newline.
   DO NOT USE THIS FUNCTION!!  There is no limit on how much it will read.  */
extern char *gets __P ((char *__s));


/* Read up to (and including) a DELIMITER from STREAM into *LINEPTR
   (and null-terminate it). *LINEPTR is a pointer returned from malloc (or
   NULL), pointing to *N characters of space.  It is realloc'd as
   necessary.  Returns the number of characters read (not including the
   null terminator), or -1 on error or EOF.  */
extern ssize_t __getdelim __P ((char **__restrict __lineptr,
				    size_t *__restrict __n, int __delimiter,
				    FILE *__restrict __stream));
extern ssize_t getdelim __P ((char **__restrict __lineptr,
				  size_t *__restrict __n, int __delimiter,
				  FILE *__restrict __stream));

/* Like `getdelim', but reads up to a newline.  */
extern ssize_t getline __P ((char **__restrict __lineptr,
				 size_t *__restrict __n,
				 FILE *__restrict __stream));


/* Write a string to STREAM.  */
extern int fputs __P ((__const char *__restrict __s,
		       FILE *__restrict __stream));

/* Write a string, followed by a newline, to stdout.  */
extern int puts __P ((__const char *__s));


/* Push a character back onto the input buffer of STREAM.  */
extern int ungetc __P ((int __c, FILE *__stream));


/* Read chunks of generic data from STREAM.  */
extern size_t fread __P ((void *__restrict __ptr, size_t __size,
			  size_t __n, FILE *__restrict __stream));
/* Write chunks of generic data to STREAM.  */
extern size_t fwrite __P ((__const void *__restrict __ptr, size_t __size,
			   size_t __n, FILE *__restrict __s));

/* Rewind to the beginning of STREAM.  */
extern void rewind __P ((FILE *__stream));

/* Seek to a certain position on STREAM.  */
extern int fseek __P ((FILE *__stream, long int __off, int __whence));
/* Return the current position of STREAM.  */
extern long int ftell __P ((FILE *__stream));

/* The Single Unix Specification, Version 2, specifies an alternative,
   more adequate interface for the two functions above which deal with
   file offset.  `long int' is not the right type.  These definitions
   are originally defined in the Large File Support API.  */

/* Types needed in these functions.  */
#ifndef off_t
typedef __off_t off_t;
# define off_t off_t
#endif

#if defined __USE_LARGEFILE64 && !defined off64_t
typedef __off64_t off64_t;
# define off64_t off64_t
#endif

#ifndef fpos_t
typedef off_t fpos_t;
#define fpos_t fpos_t
#endif

/* Seek to a certain position on STREAM.  */
extern int fsetpos __P((FILE *__stream, __const fpos_t *__pos));
/* Return the current position of STREAM.  */
extern int fgetpos __P((FILE *__stream, fpos_t *__pos));

/* Clear the error and EOF indicators for STREAM.  */
extern void clearerr __P ((FILE *__stream));
/* Return the EOF indicator for STREAM.  */
extern int feof __P ((FILE *__stream));
/* Return the error indicator for STREAM.  */
extern int ferror __P ((FILE *__stream));

/* Macro versions of the 3 previous functions */
/* If fp is NULL... */
#define clearerr(fp) ((fp)->mode &= ~(__MODE_EOF|__MODE_ERR), (void)0)
#define feof(fp)   	((fp)->mode&__MODE_EOF)
#define ferror(fp)	((fp)->mode&__MODE_ERR)

/* Print a message describing the meaning of the value of errno.  */
extern void perror __P ((__const char *__s));

/* These variables normally should not be used directly.  The `strerror'
   function provides all the needed functionality.  */
extern int sys_nerr;
extern __const char *__const sys_errlist[];

#ifdef	__USE_POSIX
/* Return the system file descriptor for STREAM.  */
extern int fileno __P ((FILE *__stream));
/* Only use the macro below if you know fp is a valid FILE for a valid fd. */
#define __fileno(fp)	((fp)->fd)
#endif /* Use POSIX.  */

#if (defined __USE_POSIX2 || defined __USE_SVID  || defined __USE_BSD || \
     defined __USE_MISC)
/* Create a new stream connected to a pipe running the given command.  */
extern FILE *popen __P ((__const char *__command, __const char *__mode));

/* Close a stream opened by popen and return the status of its child.  */
extern int pclose __P ((FILE *__stream));
#endif


#ifdef	__USE_POSIX
/* Return the name of the controlling terminal.  */
extern char *ctermid __P ((char *__s));
#endif /* Use POSIX.  */


#ifdef __USE_XOPEN
/* Return the name of the current user.  */
extern char *cuserid __P ((char *__s));
#endif /* Use X/Open.  */


#if defined __USE_POSIX || defined __USE_MISC
/* These are defined in POSIX.1:1996.  */

/* Acquire ownership of STREAM.  */
extern void flockfile __P ((FILE *__stream));

/* Try to acquire ownership of STREAM but do not block if it is not
   possible.  */
extern int ftrylockfile __P ((FILE *__stream));

/* Relinquish the ownership granted for STREAM.  */
extern void funlockfile __P ((FILE *__stream));
#endif /* POSIX || misc */

#if defined __USE_XOPEN && !defined __USE_GNU
/* The X/Open standard requires some functions and variables to be
   declared here which do not belong into this header.  But we have to
   follow.  In GNU mode we don't do this nonsense.  */
# define __need_getopt
# include <getopt.h>
#endif

/* If we are compiling with optimizing read this file.  It contains
   several optizing inline functions and macros.  */
#ifdef __USE_EXTERN_INLINES
# include <bits/stdio.h>
#endif

__END_DECLS

#endif /* !_STDIO_H */
