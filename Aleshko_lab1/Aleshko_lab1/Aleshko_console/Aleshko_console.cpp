#define _AFXDLL

#include "pch.h"
#include "Session.h"

#include <afxmt.h>

using namespace std;

inline void DoWrite()
{
	std::cout << std::endl;
}

template <class T, typename... Args> inline void DoWrite(T& value, Args... args)
{
	std::cout << value << " ";
	DoWrite(args...);
}

static CCriticalSection cs;
template <typename... Args> inline void SafeWrite(Args... args)
{
	cs.Lock();
	DoWrite(args...);
	cs.Unlock();
}

DWORD WINAPI workFunc(LPVOID _param)
{
	auto session = static_cast<Session*>(_param);
	SafeWrite("session", session->sessionID, "created");
	while (true)
	{
		Message m;
		if (session->getMessage(m))
		{
			switch (m.header.messageType)
			{
			case MT_CLOSE:
			{
				SafeWrite("session", session->sessionID, "closed");
				delete session;
				return 0;
			}
			case MT_DATA:
			{
				SafeWrite("session", session->sessionID, "data", m.data);
				Sleep(100 * session->sessionID);
				break;
			}
			}
		}
	}
	return 0;
}



int main()
{
	HANDLE confirmEvent = ::CreateEvent(NULL, FALSE, FALSE, "ConfirmEvent");
	HANDLE startEvent = ::CreateEvent(NULL, FALSE, FALSE, "StartEvent");
	HANDLE stopEvent = ::CreateEvent(NULL, FALSE, FALSE, "CloseProc");
	HANDLE exitEvent = ::CreateEvent(NULL, FALSE, FALSE, "ExitProc");


	HANDLE events[3];
	events[0] = exitEvent;
	events[1] = stopEvent;
	events[2] = startEvent;

	::SetEvent(confirmEvent);

	std::vector<HANDLE> hThreads;
	std::vector<Session*> sessions;

	while (true)
	{
		DWORD dwWaitRes = ::WaitForMultipleObjects(sizeof(events) / sizeof(HANDLE), events, FALSE, INFINITE);
		switch (dwWaitRes)
		{
		case WAIT_OBJECT_0:
			for (int i{ 0 }; i < sessions.size(); ++i)
			{
				sessions[i]->addMessage(MT_CLOSE);
				CloseHandle(hThreads[i]);
			}
			return 0;
		case WAIT_OBJECT_0 + 1:

			if (!hThreads.size())
			{
				::SetEvent(confirmEvent);
				return 0;
			}
			sessions.back()->addMessage(MT_CLOSE);
			sessions.pop_back();

			::CloseHandle(hThreads.back());
			hThreads.pop_back();

			break;

		case WAIT_OBJECT_0 + 2:

			sessions.push_back(new Session(hThreads.size()));
			hThreads.push_back(::CreateThread(NULL, 0, workFunc, (LPVOID)sessions.back(), 0, NULL));
			break;

		default:

			std::cout << "There was an error" << std::endl;
			return 0;
		}
		::SetEvent(confirmEvent);
	}

	WaitForMultipleObjects((DWORD)hThreads.size(), hThreads.data(), TRUE, INFINITE);

	CloseHandle(confirmEvent);
	CloseHandle(startEvent);
	CloseHandle(stopEvent);

	return 0;
}