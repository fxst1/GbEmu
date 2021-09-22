#ifndef MYLIB
# include "memunit/MemUnit.hh"
# include "memunit/MemBitOrder.hh"
# include <sys/types.h>
# include <unistd.h>

# ifndef LEAKSLOOP
/*
	static int LEAKSLOOP(void) {
		#ifdef LEAKSCHECK

		dprintf(STDERR_FILENO, "<leaks pid=%d> Waiting for input\n", getpid());
		int c = 0;
		read(STDIN_FILENO, &c, sizeof(c));

		#endif
		return (1);
	}
*/
# endif

#endif
