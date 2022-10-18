#include "workerThread.h"
#include "Iocp.h"
#include "session.h"
#include <WS2tcpip.h>
#include <process.h>
#include <stdio.h>

CWorkerThread::CWorkerThread()
{
}

CWorkerThread::~CWorkerThread()
{
}

bool CWorkerThread::Start()
{
	HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, &CWorkerThread::ThreadFunc, this, 0, NULL);
	return true;
}

unsigned int _stdcall CWorkerThread::ThreadFunc(void* _pArgs)
{
	CWorkerThread* thread = (CWorkerThread*)_pArgs;

	thread->RunLoop();

	return 0;
}

void CWorkerThread::RunLoop()
{
	HANDLE hIOCP = CIocp::GetInstance()->GetHandle();
	DWORD bytesTrans;
	LPOVERLAPPED overlapped;
	CSession* pSession;

	while (1)
	{
		if (!GetQueuedCompletionStatus(hIOCP, &bytesTrans, (PULONG_PTR)&pSession, (LPOVERLAPPED*)&overlapped, INFINITE))
		{
			delete pSession;
			continue;
		}

		printf("recv ok %ld : %ld \n", pSession->GetSocket(), bytesTrans);
		if (bytesTrans == 0)
		{
			delete pSession;
			continue;
		}

		// 2022-10-18
		// RecvEvent일반 함수로 사용하고 
		// OnRecv를 만들어서 virtual로 사용한다.
		/*pSession->RecvEvent(bytesTrans); 

		pSession->WsaRecv();*/

		pSession->RecvEvent1(bytesTrans);
	}
}
