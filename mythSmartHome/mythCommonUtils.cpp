#include "mythCommonUtils.hh"

int mythDelay(unsigned long usec)
{
#ifdef WIN32
	Sleep((DWORD) (usec));
#else
	usleep(usec * 1000);
#endif
	return 0;
}
