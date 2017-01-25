#pragma once
#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
int mythDelay(unsigned long usec);

