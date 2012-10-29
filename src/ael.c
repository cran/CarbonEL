#ifndef __APPLE__
#warning "This package works on Mac OS X only."
#include <R.h>
#include <Rinternals.h>
SEXP aih(SEXP tout, SEXP ap) {
  error("CarbonEL works on Mac OS X only.");
  return R_NilValue;
}
#else

#include <R.h>
#include <Rinternals.h>
#include <R_ext/eventloop.h>

#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

#include <pthread.h>

#include <Carbon/Carbon.h>

static int ifd, ofd;
static int fired = 0, active = 1;

static unsigned long intersleep = 300;

static void millisleep(unsigned long tout) {
  struct timeval tv;
  tv.tv_usec = (tout%1000)*1000;
  tv.tv_sec  = tout/1000;
  select(0, 0, 0, 0, &tv);
}

static void uih(void *data) {
  char buf[16];
  
  read(ifd, buf, 16);
  
  {
    EventRef theEvent;
    EventTargetRef theTarget;
    
    theTarget = GetEventDispatcherTarget();
    
    while (ReceiveNextEvent(0, NULL, kEventDurationNoWait ,true, &theEvent)== noErr)
      {
	EventRecord outEvent;
	bool conv = ConvertEventRefToEventRecord(theEvent, &outEvent);
	if(conv && (outEvent.what == kHighLevelEvent))
	  AEProcessAppleEvent(&outEvent);
	
	SendEventToEventTarget (theEvent, theTarget);
	ReleaseEvent(theEvent);
      }
  }
  fired=0;
}

static void *thread(void *foo) {
  char buf[16];
  while (active) {
    millisleep(intersleep);
    if (!fired) {
      fired=1; *buf=0;
      write(ofd, buf, 1);
    }
  }
  return 0;
}

SEXP cel_set_sleep(SEXP tout) {
  intersleep = (unsigned long) (asReal(tout)*1000.0 + 0.5);
  if (intersleep<0) intersleep = 300;
  return R_NilValue;
}

SEXP aih(SEXP tout, SEXP ap) {
  int fds[2];
  pipe(fds);
  ifd = fds[0];
  ofd = fds[1];

  intersleep = (unsigned long) (asReal(tout)*1000.0 + 0.5);
  if (intersleep<0) intersleep = 300;

  addInputHandler(R_InputHandlers, ifd, &uih, 32);

  if (asLogical(ap)) {
    /* hack from http://www.libsdl.org/pipermail/sdl/2006-April/073988.html */
    ProcessSerialNumber psn = { 0, kCurrentProcess };
    TransformProcessType (&psn,  kProcessTransformToForegroundApplication
			  );
    /* SetFrontProcess (&psn); */
  }

  {
    pthread_t t;
    pthread_attr_t ta;
    pthread_attr_init(&ta);
    pthread_attr_setdetachstate(&ta, PTHREAD_CREATE_DETACHED);
    pthread_create(&t,&ta,thread,0);
  }

  return R_NilValue;
}

void stopt() {
  active=0;
}

#endif
