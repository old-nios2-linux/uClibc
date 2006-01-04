/*
 * Distributed under the terms of the GNU Lesser General Public License
 * $Header: $
 *
 * This is a modified version of Hiroaki Etoh's stack smashing routines
 * implemented for glibc.
 *
 * The following people have contributed input to this code.
 * Ned Ludd - <solar[@]gentoo.org>
 * Alexander Gabert - <pappy[@]gentoo.org>
 * The PaX Team - <pageexec[@]freemail.hu>
 * Peter S. Mazinger - <ps.m[@]gmx.net>
 * Yoann Vandoorselaere - <yoann[@]prelude-ids.org>
 * Robert Connolly - <robert[@]linuxfromscratch.org>
 * Cory Visi <cory[@]visi.name>
 * Mike Frysinger <vapier[@]gentoo.org>
 */

#if defined __SSP__ || defined __SSP_ALL__
#error "file must not be compiled with stack protection enabled on it. Use -fno-stack-protector"
#endif

#ifdef __PROPOLICE_BLOCK_SEGV__
# define SSP_SIGTYPE SIGSEGV
#else
# define SSP_SIGTYPE SIGABRT
#endif

#define openlog __openlog
#define syslog __syslog
#define closelog __closelog
#define sigfillset __sigfillset_internal
#define sigdelset __sigdelset_internal
#define sigaction __sigaction
#define kill __kill

#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/syslog.h>

static void block_signals(void)
{
	struct sigaction sa;
	sigset_t mask;

	sigfillset(&mask);

	sigdelset(&mask, SSP_SIGTYPE);	/* Block all signal handlers */
	__sigprocmask(SIG_BLOCK, &mask, NULL);	/* except SSP_SIGTYPE */

	/* Make the default handler associated with the signal handler */
	__memset(&sa, 0, sizeof(struct sigaction));
	sigfillset(&sa.sa_mask);	/* Block all signals */
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	sigaction(SSP_SIGTYPE, &sa, NULL);
}

static void ssp_write(int fd, const char *msg1, const char *msg2, const char *msg3)
{
	__write(fd, msg1, __strlen(msg1));
	__write(fd, msg2, __strlen(msg2));
	__write(fd, msg3, __strlen(msg3));
	__write(fd, "()\n", 3);
	openlog("ssp", LOG_CONS | LOG_PID, LOG_USER);
	syslog(LOG_INFO, "%s%s%s()", msg1, msg2, msg3);
	closelog();
}

static attribute_noreturn void terminate(void)
{
	(void) kill(__getpid(), SSP_SIGTYPE);
	_exit_internal(127);
}

void attribute_noreturn __stack_smash_handler(char func[], int damaged __attribute__ ((unused)));
void attribute_noreturn __stack_smash_handler(char func[], int damaged)
{
	static const char message[] = ": stack smashing attack in function ";

	block_signals();

	ssp_write(STDERR_FILENO, __uclibc_progname, message, func);

	/* The loop is added only to keep gcc happy. */
	while(1)
		terminate();
}

void attribute_noreturn attribute_hidden __stack_chk_fail_internal(void)
{
	static const char msg1[] = "stack smashing detected: ";
	static const char msg3[] = " terminated";

	block_signals();

	ssp_write(STDERR_FILENO, msg1, __uclibc_progname, msg3);

	/* The loop is added only to keep gcc happy. */
	while(1)
		terminate();
}
strong_alias(__stack_chk_fail_internal,__stack_chk_fail)

#if 0
void attribute_noreturn __chk_fail(void)
{
	static const char msg1[] = "buffer overflow detected: ";
	static const char msg3[] = " terminated";

	block_signals();

	ssp_write(STDERR_FILENO, msg1, __uclibc_progname, msg3);

	/* The loop is added only to keep gcc happy. */
	while(1)
		terminate();
}
#endif
