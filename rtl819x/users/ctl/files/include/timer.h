/******************************************************************************

                               Copyright (c) 2010
                            Lantiq Deutschland GmbH
                     Am Campeon 3; 85579 Neubiberg, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _TIMER_H
#define _TIMER_H        1

#include <time.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* Application defined realtime signal*/
#define INFI_TIMER_ONESHOT 	0
#define INFI_TIMER_PERIODIC	1
#define INFI_MSEC	10000000
#define INFI_TICK	100

#define IFX_OFFSET	3

/* Simple error handler */
int err_quit(char *);

unsigned int timerArr[10];

typedef void (*timerFuncPtr) (unsigned long);

typedef struct _timer_callback {
	timerFuncPtr func;
	unsigned long param;
	unsigned char periodicity;
//Neeraj: Changed the timer id type
//      unsigned char timer_id;
	timer_t timer_id;
} timer_callback;

/* Signal handler (sa_sigaction) */
void timer_handler(int a, siginfo_t *, void *);
unsigned int timerStart(timer_t timer_id, unsigned int t_sec,
			unsigned char periodicity);
timer_t timerCreate(unsigned char, timerFuncPtr, unsigned long);

unsigned int timerStop(timer_t timer_id);
unsigned int timerDelete(timer_t timer_id);
unsigned int timerPending(timer_t timer_id, struct itimerspec *ispec);

unsigned int timerReset(timer_t timer_id, unsigned int t_sec,
			unsigned char periodicity);
void timer_handler(int signo, siginfo_t * sig_info, void *extra);

#endif
