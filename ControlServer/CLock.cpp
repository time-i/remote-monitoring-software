#include "CLock.h"

CLock::~CLock()
{

}

void CLock::Lock()
{
	EnterCriticalSection(&m_cs);
}

void CLock::UnLock()
{
	LeaveCriticalSection(&m_cs);
}

CLock::CLock()
{
	InitializeCriticalSection(&m_cs);
}
