/*
 * This file based on printf.c from 'Dlibs' on the atari ST  (RdeBath)
 *
 * 
 *    Dale Schumacher                         399 Beacon Ave.
 *    (alias: Dalnefre')                      St. Paul, MN  55104
 *    dal@syntel.UUCP                         United States of America
 *  "It's not reality that's important, but how you perceive things."
 */

/* Altered to use stdarg, made the core function vfnprintf.
 * Hooked into the stdio package using 'inside information'
 * Altered sizeof() assumptions, now assumes all integers except chars
 * will be either
 *  sizeof(xxx) == sizeof(long) or sizeof(xxx) == sizeof(short)
 *
 * -RDB
 */

/*
 *                    Manuel Novoa III   Dec 2000
 *
 * The previous vfnprintf routine was almost completely rewritten with the
 * goal of fixing some shortcomings and reducing object size.
 *
 * The summary of changes:
 *
 * Converted print conversion specification parsing from one big switch
 *   to a method using string tables.  This new method verifies that the
 *   conversion flags, field width, precision, qualifier, and specifier
 *   appear in the correct order.  Many questionable specifications were
 *   accepted by the previous code.  This new method also resulted in a
 *   substantial reduction in object size of about 330 bytes (20%) from
 *   the old version (1627 bytes) on i386, even with the following
 *   improvements.
 *
 *     Implemented %n specifier as required by the standards.
 *     Implemented proper handling of precision for int types.
 *     Implemented # for hex and pointer, fixed error for octal rep of 0.
 *     Implemented return of -1 on stream error.
 *
 * Added optional support for the GNU extension %m which prints the string
 *   corresponding the errno.
 *
 * Added optional support for long long ints and unsigned long long ints
 *   using the conversion qualifiers "ll", "L", or "q" (like glibc).
 *
 * Added optional support for doubles in a very limited form.  None of
 *   the formating options are obeyed.  The string returned by __dtostr
 *   is printed directly.
 *
 * Converted to use my (un)signed long (long) to string routines, which are
 * smaller than the previous functions and don't require static buffers.
 *
 * Other Modifications:
 *   Modified sprintf, snprintf, vsprintf, vsnprintf to share on fake-file.
 */

/*
 *                    Manuel Novoa III   Jan 2000
 *
 * Removed fake file from *s*printf functions because of possible problems
 *    if called recursively.  Instead, have sprintf, snprintf, and vsprintf
 *    call vsnprintf which allocates a fake file on the stack.
 * Removed WANT_FPUTC option.  Always use standard putc macro to avoid
 *    problems with the fake file used by the *s*printf functions.
 * Added asprintf.
 * Fixed 0-pad prefixing bug.
 * Converted sizeof(int) == sizeof(long) tests to compile time vs run time.
 *    This saves 112 bytes of code on i386.
 */

/*****************************************************************************/
/*                            OPTIONS                                        */
/*****************************************************************************/
/* The optional support for long longs and doubles comes in two forms.
 *
 *   1) Normal (or partial for doubles) output support.  Set to 1 to turn on.
 *      Adds about 70 bytes for doubles, about 220 bytes for long longs,
 *      and about 275 for both to the base code size of 1163 on i386.
 */

#define WANT_LONG_LONG         0
#define WANT_DOUBLE            0

/*   2) An error message is inserted into the stream, an arg of the
 *      appropriate size is removed from the arglist, and processing
 *      continues.  This is adds less code and may be useful in some
 *      cases.  Set to 1 to turn on.  Adds about 50 bytes for doubles,
 *      about 140 bytes for long longs, and about 175 bytes for both
 *      to the base code size of 1163 on i386.
 */

#define WANT_LONG_LONG_ERROR   0
#define WANT_DOUBLE_ERROR      0

/*
 * Set to support GNU extension of %m to print string corresponding to errno.
 *
 * Warning: This adds about 50 bytes (i386) to the code but it also pulls in
 * strerror and the corresponding string table which together are about 3.8k.
 */

#define WANT_GNU_ERRNO         0

/**************************************************************************/

#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#if WANT_GNU_ERRNO
#include <errno.h>
#endif

#ifdef __STDC__
#include <stdarg.h>
#define va_strt      va_start
#else
#include <varargs.h>
#define va_strt(p,i) va_start(p)
#endif

#include "stdio.h"

extern int vfnprintf(FILE * op, size_t max_size,
					 register __const char *fmt, register va_list ap);

#ifdef L_printf
int printf(const char *fmt, ...)
{
	va_list ptr;
	int rv;

	va_strt(ptr, fmt);
	rv = vfnprintf(stdout, -1, fmt, ptr);
	va_end(ptr);
	return rv;
}
#endif

#ifdef L_asprintf
int asprintf(char **app, const char *fmt, ...)
{
	va_list ptr;
	int rv, i;
	char *p;					/* unitialized warning is ok here */
	/*
	 * First  iteration - find out size of buffer required and allocate it.
	 * Second iteration - actually produce output.
	 */
	rv = 0;
	for (i=0 ; i<2 ; i++) {
		va_strt(ptr, fmt);
		rv = vsnprintf(p, rv, fmt, ptr);
		va_end(ptr);

		if (i==0) {				/* first time through so */
			p = malloc(++rv); /* allocate the buffer */
			*app = p;
			if (!p) {
				return -1;
			}
		}
	}
	return rv;
}
#endif

#ifdef L_sprintf
int sprintf(char *sp, const char *fmt, ...)
{
	va_list ptr;
	int rv;

	va_strt(ptr, fmt);
	rv = vsnprintf(sp, -1, fmt, ptr);
	va_end(ptr);
	return rv;
}
#endif


#ifdef L_snprintf
int snprintf(char *sp, size_t size, const char *fmt, ...)
{
	va_list ptr;
	int rv;

	va_strt(ptr, fmt);
	rv = vsnprintf(sp, size, fmt, ptr);
	va_end(ptr);
	return rv;
}
#endif

#ifdef L_fprintf
int fprintf(FILE * fp, const char *fmt, ...)
{
	va_list ptr;
	int rv;

	va_strt(ptr, fmt);
	rv = vfnprintf(fp, -1, fmt, ptr);
	va_end(ptr);
	return rv;
}
#endif

#ifdef L_vprintf
int vprintf(const char *fmt, va_list ap)
{
	return vfprintf(stdout, fmt, ap);
}
#endif

#ifdef L_vfprintf

int vfprintf(FILE * op, register __const char *fmt, register va_list ap)
{
	return (vfnprintf(op, -1, fmt, ap));
}

#endif

#ifdef L_vsprintf
int vsprintf(char *sp, __const char *fmt, va_list ap)
{
	return vsnprintf(sp, -1, fmt, ap);
}
#endif

#ifdef L_vsnprintf
int vsnprintf(char *sp, size_t size, __const char *fmt, va_list ap)
{
	int rv;
#if 0
	FILE f = {0, 0, (char *) (unsigned) -1, 0, (char *) (unsigned) -1, -1,
			  _IOFBF | __MODE_WRITE};
#else
	/* As we're only using the putc macro in vfnprintf, we don't need to
	   initialize all FILE fields. */
	FILE f;

	f.bufwrite = (char *) (unsigned) -1;
	f.bufpos = sp;
	f.mode = _IOFBF | __MODE_WRITE;
#endif

	rv = vfnprintf(&f, size, fmt, ap);
	if (size) {
		*(f.bufpos) = 0;
	}
	return rv;
}
#endif

#ifdef L_vfnprintf

extern char *__ultostr(char *buf, unsigned long uval, int base, int uppercase);
extern char *__ltostr(char *buf, long val, int base, int uppercase);
extern char *__ulltostr(char *buf, unsigned long long uval, int base, int uppercase);
extern char *__lltostr(char *buf, long long val, int base, int uppercase);
extern char *__dtostr(char *buf, double x);

enum {
	FLAG_PLUS = 0,
	FLAG_MINUS_LJUSTIFY,
	FLAG_HASH,
	FLAG_0_PAD,
	FLAG_SPACE,
};

/* layout                   01234  */
static const char spec[] = "+-#0 ";

#if WANT_LONG_LONG || WANT_LONG_LONG_ERROR
static const char qual[] = "hlLq";
#else
static const char qual[] = "hl";
#endif

#if !WANT_LONG_LONG && WANT_LONG_LONG_ERROR
static const char ll_err[] = "<LONG-LONG>";
#endif

#if !WANT_DOUBLE && WANT_DOUBLE_ERROR
static const char dbl_err[] = "<DOUBLE>";
#endif

#if WANT_DOUBLE || WANT_DOUBLE_ERROR
/* layout                     012345678901234567   */
static const char u_spec[] = "%nbopxXudicsfgGeEaA";
#else
/* layout                     0123456789012   */
static const char u_spec[] = "%nbopxXudics0";
#endif

/* WARNING: u_spec and u_radix need to stay in agreement!!! */
/* u_radix[i] <-> u_spec[i+2] for unsigned entries only */
static const char u_radix[] = "\x02\x08\x10\x10\x10\x0a";

int vfnprintf(FILE * op, size_t max_size, const char *fmt, va_list ap)
{
	int i, cnt = 0, lval;
	char *p;
	const char *fmt0;
	int buffer_mode;
	int preci, width;
#define upcase i
	int radix, dpoint /*, upcase*/;
#if WANT_LONG_LONG
	char tmp[65];
#else
	char tmp[33];
#endif
	char flag[sizeof(spec)];

	/* This speeds things up a bit for unbuffered */
	buffer_mode = (op->mode & __MODE_BUF);
	op->mode &= (~__MODE_BUF);

	while (*fmt) {
		if (*fmt == '%') {
			fmt0 = fmt;			/* save our position in case of bad format */
			++fmt;
			if (buffer_mode == _IONBF) {
				fflush(op);
			}
			width = -1;			/* min field width */
			preci = -5;			/* max string width or mininum digits */
			radix = 10;			/* number base */
			dpoint = 0;			/* found decimal point */
#if INT_MAX != LONG_MAX
			lval = 0;			/* sizeof(int) != sizeof(long) */
#else
			lval = 1;			/* sizeof(int) == sizeof(long) */
#endif

			tmp[1] = 0;			/* set things up for %c -- better done here */

			/* init flags */
			for (p =(char *) spec ; *p ; p++) {
				flag[p-spec] = '\0';
			}
			flag[FLAG_0_PAD] = ' ';

			/* process optional flags */
			for (p = (char *)spec ; *p ; ) {
				if (*fmt == *p) {
					flag[p-spec] = *fmt++;
					p = (char *)spec; /* restart scan */
				} else {
					p++;
				}
			}
			
			if (!flag[FLAG_PLUS]) {
				flag[FLAG_PLUS] = flag[FLAG_SPACE];
			}

			/* process optional width and precision */
			do {
				if (*fmt == '.') {
					++fmt;
					dpoint = 1;
				}
				if (*fmt == '*') {	/* parameter width value */
					++fmt;
					i = va_arg(ap, int);
				} else {
					for ( i = 0 ; (*fmt >= '0') && (*fmt <= '9') ; ++fmt ) {
						i = (i * 10) + (*fmt - '0');
					}
				}

				if (dpoint) {
					preci = i;
					if (i<0) {
						preci = 0;
					}
				} else {
					width = i;
					if (i<0) {
						width = -i;
						flag[FLAG_MINUS_LJUSTIFY] = 1;
					}
				}
			} while ((*fmt == '.') && !dpoint );

			/* process optional qualifier */
			for (p = (char *) qual ; *p ; p++) {
				if (*p == *fmt) {
					lval = p - qual;
					++fmt;
#if WANT_LONG_LONG || WANT_LONG_LONG_ERROR
					if ((*p == 'l') && (*fmt == *p)) {
						++lval;
						++fmt;
					}
#endif /* WANT_LONG_LONG || WANT_LONG_LONG_ERROR */
				}
			}

#if WANT_GNU_ERRNO
			if (*fmt == 'm') {
				flag[FLAG_PLUS] = '\0';
				flag[FLAG_0_PAD] = ' ';
				p = strerror(errno);
				goto print;
			}
#endif

			/* process format specifier */
			for (p = (char *) u_spec ; *p ; p++) {
				if (*fmt != *p) continue;
				if (p-u_spec < 1) {	/* print a % */
					goto charout;
				}
				if (p-u_spec < 2) {	/* store output count in int ptr */
					*(va_arg(ap, int *)) = cnt;
					goto nextfmt;
				}
				if (p-u_spec < 8) { /* unsigned conversion */
					radix = u_radix[p-u_spec-2];
					upcase = ((int)'x') - *p;
					if (*p == 'p') {
						lval = (sizeof(char *) == sizeof(long));
						upcase = 0;
					}
#if WANT_LONG_LONG || WANT_LONG_LONG_ERROR
					if (lval >= 2) {
#if WANT_LONG_LONG
						p = __ulltostr(tmp + sizeof(tmp) - 1,
									   va_arg(ap, unsigned long long),
									   radix, upcase);
#else
						(void) va_arg(ap, unsigned long long);	/* cary on */
						p = (char *) ll_err;
#endif /* WANT_LONG_LONG */
					} else {
#endif /* WANT_LONG_LONG || WANT_LONG_LONG_ERROR */
#if UINT_MAX != ULONG_MAX
						/* sizeof(unsigned int) != sizeof(unsigned long) */
						p = __ultostr(tmp + sizeof(tmp) - 1, (unsigned long)
									  ((lval)
									   ? va_arg(ap, unsigned long)
									   : va_arg(ap, unsigned int)),
									  radix, upcase);
#else
						/* sizeof(unsigned int) == sizeof(unsigned long) */
						p = __ultostr(tmp + sizeof(tmp) - 1, (unsigned long)
									  va_arg(ap, unsigned long),
									  radix, upcase);
#endif
#if WANT_LONG_LONG || WANT_LONG_LONG_ERROR
					}
#endif /* WANT_LONG_LONG || WANT_LONG_LONG_ERROR */
					flag[FLAG_PLUS] = '\0';	/* meaningless for unsigned */
					if (flag[FLAG_HASH] && (*p != '0')) { /* non-zero */
						if (radix == 8) {
							*--p = '0';	/* add leadding zero */
						} else { /* either 2 or 16 */
							flag[FLAG_PLUS] = '0';
							*--p = 'b';
							if (radix == 16) {
								*p = 'x';
								if (*fmt == 'X') {
									*p = 'X';
								}
							}
						}
					}
				} else if (p-u_spec < 10) { /* signed conversion */
#if WANT_LONG_LONG || WANT_LONG_LONG_ERROR
					if (lval >= 2) {
#if WANT_LONG_LONG
						p = __lltostr(tmp + sizeof(tmp) - 1,
									  va_arg(ap, long long), 10, 0);
#else
						(void) va_arg(ap, long long); /* carry on */
						p = (char *) ll_err;
#endif /* WANT_LONG_LONG */
					} else {
#endif /* WANT_LONG_LONG || WANT_LONG_LONG_ERROR */
#if INT_MAX != LONG_MAX
						/* sizeof(int) != sizeof(long) */
						p = __ltostr(tmp + sizeof(tmp) - 1, (long)
									 ((lval)
									  ? va_arg(ap, long)
									  : va_arg(ap, int)), 10, 0);
#else
						/* sizeof(int) == sizeof(long) */
						p = __ltostr(tmp + sizeof(tmp) - 1, (long)
									 va_arg(ap, long), 10, 0);
#endif
#if WANT_LONG_LONG || WANT_LONG_LONG_ERROR
					}
#endif /* WANT_LONG_LONG || WANT_LONG_LONG_ERROR */
				} else if (p-u_spec < 12) {	/* character or string */
					flag[FLAG_PLUS] = '\0';
					flag[FLAG_0_PAD] = ' ';
					if (*p == 'c') {	/* character */
						p = tmp;
						*p = va_arg(ap, int);
					} else {	/* string */
						p = va_arg(ap, char *);
					}
#if WANT_DOUBLE || WANT_DOUBLE_ERROR
				} else if (p-u_spec < 27) {		/* floating point */
#endif /* WANT_DOUBLE || WANT_DOUBLE_ERROR */
#if WANT_DOUBLE
					p = __dtostr(tmp + sizeof(tmp) - 1, va_arg(ap, double));
#elif WANT_DOUBLE_ERROR
					(void) va_arg(ap,double); /* carry on */
					p = (char *) dbl_err;
#endif /* WANT_DOUBLE */
				}

#if WANT_GNU_ERRNO
			print:
#endif
				{				/* this used to be printfield */
					int len;

					/* cheaper than strlen call */
					for ( len = 0 ; p[len] ; len++ ) { }

					if ((*p == '-')
#if WANT_GNU_ERRNO
						&& (*fmt != 'm')
#endif
						&& (*fmt != 's')) {
						flag[FLAG_PLUS] = *p++;
						--len;
					}
				    if (flag[FLAG_PLUS]) {
						++len;
						++preci;
						if (flag[FLAG_PLUS] == '0') { /* base 16 */
							++preci; /* account for x or X */
						}
					}

					if (preci >= 0) {
						if ((*fmt == 's')
#if WANT_GNU_ERRNO
						|| (*fmt == 'm')
#endif
						) {
							len = preci;
						}
						preci -= len;
						if (preci < 0) {
							preci = 0;
						}
						width -= preci;
					}

					width -= len;
					if (width < 0) {
						width = 0;
					}

					if (preci < 0) {
						preci = 0;
						if (!flag[FLAG_MINUS_LJUSTIFY]
							/* && flag[FLAG_PLUS] */
							&& (flag[FLAG_0_PAD] == '0')) { 
							preci = width;
							width = 0;
						}
					}

					while (width + len + preci) {
						unsigned char ch;
						/* right padding || left padding */
						if ((!len && !preci)
							|| (width && !flag[FLAG_MINUS_LJUSTIFY])) {
							ch = ' ';
							--width;
						} else if (flag[FLAG_PLUS]) {
							ch = flag[FLAG_PLUS]; /* sign */
							if (flag[FLAG_PLUS]=='0') {	/* base 16 case */
								flag[FLAG_PLUS] = *p++;	/* get the x|X */
							} else {
								flag[FLAG_PLUS] = '\0';
							}
							--len;
						} else if (preci) {
							ch = '0';
							--preci;
						} else {
							ch = *p++; /* main field */
							--len;
						}

						if (++cnt < max_size) {
							putc(ch, op);
						}
						if ((ch == '\n') && (buffer_mode == _IOLBF)) {
							fflush(op);
						}
					}
				}
				goto nextfmt;
			}

			fmt = fmt0;	/* this was an illegal format */
		}

		charout:
		if (++cnt < max_size) {
			putc(*fmt, op);	/* normal char out */
		}
		if ((*fmt == '\n') && (buffer_mode == _IOLBF)) {
			fflush(op);
		}

	nextfmt:
		++fmt;
	}

	op->mode |= buffer_mode;
	if (buffer_mode == _IONBF) {
		fflush(op);
	}
	if (buffer_mode == _IOLBF) {
		op->bufwrite = op->bufstart;
	}

	if (ferror(op)) {
		cnt = -1;
	}
	return (cnt);
}

#endif

