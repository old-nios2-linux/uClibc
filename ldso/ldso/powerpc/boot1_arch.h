/* Any assmbly language/system dependent hacks needed to setup boot1.c so it
 * will work as expected and cope with whatever platform specific wierdness is
 * needed for this architecture.  */

/* Overrive the default _dl_boot function, and replace it with a bit of asm.
 * Then call the real _dl_boot function, which is now named _dl_boot2. */

asm("" \
"	.text\n"			\
"	.globl	_dl_boot\n"		\
"_dl_boot:\n"				\
"	mr	3,1\n"		\
"	li	4,0\n"			\
"	addi	1,1,-16\n"		\
"	stw	4,0(1)\n"		\
"	bl      _dl_boot2\n"		\
".previous\n"				\
);

#define DL_BOOT(X) static void __attribute__ ((unused)) _dl_boot2(X)
