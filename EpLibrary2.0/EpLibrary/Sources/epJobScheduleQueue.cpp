/*! 
JobScheduleQueue for the EpLibrary

The MIT License (MIT)

Copyright (c) 2012-2013 Woong Gyu La <juhgiyo@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "epJobScheduleQueue.h"
#include "epQuickSort.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;
JobScheduleQueue::JobScheduleQueue(LockPolicy lockPolicyType) :ThreadSafePQueue<BaseJob*, BaseJob>(lockPolicyType)
{
}

JobScheduleQueue::~JobScheduleQueue()
{
	while(!IsEmpty())
	{
		Pop();
	}
}

void JobScheduleQueue::Push(BaseJob* const &data, BaseJob::JobStatus status)
{
	data->RetainObj();
	ThreadSafePQueue::Push(data);
	if(status!=BaseJob::JOB_STATUS_NONE)
	{
		data->JobReport(status);
	}
}
void JobScheduleQueue::Pop()
{
	BaseJob* jobObj=Front();
	jobObj->ReleaseObj();
	ThreadSafePQueue::Pop();

}

bool JobScheduleQueue::Erase(BaseJob * const object)
{
	LockObj lock(m_queueLock);
	if(m_queue.empty())
		return false;
	std::vector<BaseJob*>::iterator iter;
	for(iter=m_queue.begin();iter!=m_queue.end();iter++)
	{
		if(*iter==object)
		{
			(*iter)->JobReport(BaseJob::JOB_STATUS_TIMEOUT);
			(*iter)->ReleaseObj();
			m_queue.erase(iter);
			return true;
		}
	}
	return false;
}

void JobScheduleQueue::ReportAllJob(const BaseJob::JobStatus status)
{
	std::vector<BaseJob *>::iterator iter;
	m_queueLock->Lock();
	std::vector<BaseJob *> queue=m_queue;
	m_queueLock->Unlock();
	for(iter=queue.begin();iter!=queue.end();iter++)
	{
		(*iter)->JobReport(BaseJob::JOB_STATUS_INCOMPLETE);
	}

}