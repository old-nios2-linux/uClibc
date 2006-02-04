/* because people like to make things difficult */

#undef do_clone

#define crappy_sizeof(s) (s == NULL ? 0 : sizeof(s))

#if defined __ia64__

# define do_clone(fn, stack, flags, arg) \
	__clone2(fn, stack, crappy_sizeof(stack), flags, arg, NULL, NULL, NULL)

#elif defined __hppa__

# define do_clone(fn, stack, flags, arg) \
	clone(fn, stack, flags, arg)

#else

# define do_clone(fn, stack, flags, arg) \
	clone(fn, stack+crappy_sizeof(stack), flags, arg)

#endif
