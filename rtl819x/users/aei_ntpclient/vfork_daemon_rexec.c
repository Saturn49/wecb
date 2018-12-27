/* vi: set sw=4 ts=4: */
/*
 * Rexec program for system have fork() as vfork() with foreground option
 *
 * Copyright (C) Vladimir N. Oleynik <dzo@simtreas.ru>
 * Copyright (C) 2003 Russ Dill <Russ.Dill@asu.edu>
 *
 * daemon() portion taken from uClibc:
 *
 * Copyright (c) 1991, 1993
 *      The Regents of the University of California.  All rights reserved.
 *
 * Modified for uClibc by Erik Andersen <andersee@debian.org>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */

#include "busybox.h" /* uses applet tables */

/* This does a fork/exec in one call, using vfork().  Returns PID of new child,
 * -1 for failure.  Runs argv[0], searching path if that has no / in it. */
pid_t FAST_FUNC spawn(char **argv)
{
	/* Compiler should not optimize stores here */
	volatile int failed;
	pid_t pid;

	fflush_all();

	/* Be nice to nommu machines. */
	failed = 0;
	pid = vfork();
	if (pid < 0) /* error */
		return pid;
	if (!pid) { /* child */
		/* This macro is ok - it doesn't do NOEXEC/NOFORK tricks */
		BB_EXECVP(argv[0], argv);

		/* We are (maybe) sharing a stack with blocked parent,
		 * let parent know we failed and then exit to unblock parent
		 * (but don't run atexit() stuff, which would screw up parent.)
		 */
		failed = errno;
		/* mount, for example, does not want the message */
		/*bb_perror_msg("can't execute '%s'", argv[0]);*/
		_exit(111);
	}
	/* parent */
	/* Unfortunately, this is not reliable: according to standards
	 * vfork() can be equivalent to fork() and we won't see value
	 * of 'failed'.
	 * Interested party can wait on pid and learn exit code.
	 * If 111 - then it (most probably) failed to exec */
	if (failed) {
		safe_waitpid(pid, NULL, 0); /* prevent zombie */
		errno = failed;
		return -1;
	}
	return pid;
}

/* Die with an error message if we can't spawn a child process. */
pid_t FAST_FUNC xspawn(char **argv)
{
	pid_t pid = spawn(argv);
	if (pid < 0)
		bb_simple_perror_msg_and_die(*argv);
	return pid;
}

int FAST_FUNC spawn_and_wait(char **argv)
{
	int rc;
#if ENABLE_FEATURE_PREFER_APPLETS
	int a = find_applet_by_name(argv[0]);

	if (a >= 0 && (APPLET_IS_NOFORK(a)
# if BB_MMU
	               || APPLET_IS_NOEXEC(a) /* NOEXEC trick needs fork() */
# endif
	              )) {
# if BB_MMU
		if (APPLET_IS_NOFORK(a))
# endif
		{
			return run_nofork_applet(a, argv);
		}
# if BB_MMU
		/* MMU only */
		/* a->noexec is true */
		rc = fork();
		if (rc) /* parent or error */
			return wait4pid(rc);
		/* child */
		xfunc_error_retval = EXIT_FAILURE;
		run_applet_no_and_exit(a, argv);
# endif
	}
#endif /* FEATURE_PREFER_APPLETS */
	rc = spawn(argv);
	return wait4pid(rc);
}

#if !BB_MMU
void FAST_FUNC re_exec(char **argv)
{
	/* high-order bit of first char in argv[0] is a hidden
	 * "we have (already) re-execed, don't do it again" flag */
	argv[0][0] |= 0x80;
	execv(bb_busybox_exec_path, argv);
	bb_perror_msg_and_die("can't execute '%s'", bb_busybox_exec_path);
}

pid_t FAST_FUNC fork_or_rexec(char **argv)
{
	pid_t pid;
	/* Maybe we are already re-execed and come here again? */
	if (re_execed)
		return 0;
	pid = xvfork();
	if (pid) /* parent */
		return pid;
	/* child - re-exec ourself */
	re_exec(argv);
}
#endif

/* Due to a #define in libbb.h on MMU systems we actually have 1 argument -
 * char **argv "vanishes" */
void FAST_FUNC bb_daemonize_or_rexec(int flags, char **argv)
{
	int fd;

	if (flags & DAEMON_CHDIR_ROOT)
		xchdir("/");

	if (flags & DAEMON_DEVNULL_STDIO) {
		close(0);
		close(1);
		close(2);
	}

	fd = open(bb_dev_null, O_RDWR);
	if (fd < 0) {
		/* NB: we can be called as bb_sanitize_stdio() from init
		 * or mdev, and there /dev/null may legitimately not (yet) exist!
		 * Do not use xopen above, but obtain _ANY_ open descriptor,
		 * even bogus one as below. */
		fd = xopen("/", O_RDONLY); /* don't believe this can fail */
	}

	while ((unsigned)fd < 2)
		fd = dup(fd); /* have 0,1,2 open at least to /dev/null */

	if (!(flags & DAEMON_ONLY_SANITIZE)) {
		if (fork_or_rexec(argv))
			exit(EXIT_SUCCESS); /* parent */
		/* if daemonizing, detach from stdio & ctty */
		setsid();
		dup2(fd, 0);
		dup2(fd, 1);
		dup2(fd, 2);
		if (flags & DAEMON_DOUBLE_FORK) {
			/* On Linux, session leader can acquire ctty
			 * unknowingly, by opening a tty.
			 * Prevent this: stop being a session leader.
			 */
			if (fork_or_rexec(argv))
				exit(EXIT_SUCCESS); /* parent */
		}
	}
	while (fd > 2) {
		close(fd--);
		if (!(flags & DAEMON_CLOSE_EXTRA_FDS))
			return;
		/* else close everything after fd#2 */
	}
}

void FAST_FUNC bb_sanitize_stdio(void)
{
	bb_daemonize_or_rexec(DAEMON_ONLY_SANITIZE, NULL);
}
