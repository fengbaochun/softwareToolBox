/*
This file is part of CanFestival, a library implementing CanOpen Stack.

Copyright (C): Edouard TISSERANT and Francis DUPIN
Copyright (C) Win32 Port Leonid Tochinski

See COPYING file for copyrights details.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifdef _WIN32
#include <windows.h>
#include <stdlib.h>
#include <sys/timeb.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "applicfg.h"
#include "can_driver.h"
#include "timer.h"
#include "timers_driver.h"

#ifdef __cplusplus
};
#endif


#ifdef _WIN32
DWORD timebuffer;

/* Synchronization Object Implementation */
CRITICAL_SECTION CanFestival_mutex;
HANDLE timer_thread = NULL;
HANDLE timer = NULL;

volatile int stop_timer=0;

static TimerCallback_t init_callback;
#endif

void EnterMutex(void)
{
    #ifdef _WIN32
    EnterCriticalSection(&CanFestival_mutex);
    #endif
}

void LeaveMutex(void)
{
    #ifdef _WIN32
    LeaveCriticalSection(&CanFestival_mutex);
    #endif
}

// --------------- CAN Receive Thread Implementation ---------------

void CreateReceiveTask(CAN_HANDLE fd0, TASK_HANDLE* Thread, void* ReceiveLoopPtr)
{
    #ifdef _WIN32
	unsigned long thread_id = 0;
	*Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReceiveLoopPtr, fd0, 0, &thread_id);
    #endif
}

void WaitReceiveTaskEnd(TASK_HANDLE *Thread)
{
    #ifdef _WIN32
	if(WaitForSingleObject(*Thread, 1000) == WAIT_TIMEOUT)
	{
		TerminateThread(*Thread, -1);
	}
	CloseHandle(*Thread);
    #endif
}

#if !defined(WIN32) || defined(__CYGWIN__)
int TimerThreadLoop(void)
#else
DWORD TimerThreadLoop(LPVOID arg)
#endif
{

#ifdef _WIN32
	while(!stop_timer)
	{
		WaitForSingleObject(timer, INFINITE);
		if(stop_timer)
			break;
        EnterMutex();
		timebuffer = GetTickCount();
		TimeDispatch();
        LeaveMutex();
	}
#endif
	return 0;
}

void TimerInit(void)
{
#ifdef _WIN32
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = 0;

	InitializeCriticalSection(&CanFestival_mutex);

	timer = CreateWaitableTimer(NULL, FALSE, NULL);
	if(NULL == timer)
    {
        printf("CreateWaitableTimer failed (%d)\n", GetLastError());
    }

	// Take first absolute time ref in milliseconds.
	timebuffer = GetTickCount();
#endif
}

void TimerCleanup(void)
{
    #ifdef _WIN32
	DeleteCriticalSection(&CanFestival_mutex);
    #endif
}

void StopTimerLoop(TimerCallback_t exitfunction)
{
#ifdef _WIN32
    if(timer_thread){
        EnterMutex();
        exitfunction(NULL,0);
        LeaveMutex();

        stop_timer = 1;
        setTimer(0);
        if(WaitForSingleObject(timer_thread,1000) == WAIT_TIMEOUT)
        {
            TerminateThread(timer_thread, -1);
        }
        CloseHandle(timer);
        CloseHandle(timer_thread);
        timer_thread = NULL;
    }
#endif
}

void StartTimerLoop(TimerCallback_t _init_callback)
{
    #ifdef _WIN32
	unsigned long timer_thread_id;
	stop_timer = 0;
	init_callback = _init_callback;
	EnterMutex();
		// At first, TimeDispatch will call init_callback.
	SetAlarm(NULL, 0, init_callback, 0, 0);
	LeaveMutex();
    if(!timer_thread)
        timer_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TimerThreadLoop, NULL, 0, &timer_thread_id);
#endif
}

/* Set the next alarm */
void setTimer(TIMEVAL value)
{
    #ifdef _WIN32
	if(value == TIMEVAL_MAX)
		CancelWaitableTimer(timer);
	else
	{
		LARGE_INTEGER liDueTime;

		/* arg 2 of SetWaitableTimer take 100 ns interval */
		liDueTime.QuadPart = ((long long) (-1) * value * 10000);
		//printf("SetTimer(%llu)\n", value);

		if (!SetWaitableTimer(timer, &liDueTime, 0, NULL, NULL, FALSE))
		{
			printf("SetWaitableTimer failed (%d)\n", GetLastError());
		}
	}
#endif
}

/* Get the elapsed time since the last occured alarm */
TIMEVAL getElapsedTime(void)
{
    #ifdef _WIN32
  DWORD timetmp = GetTickCount();
  return (timetmp - timebuffer);
#endif
}

