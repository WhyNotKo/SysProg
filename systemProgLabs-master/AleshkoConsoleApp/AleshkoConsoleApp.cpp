// AleshkoConsoleApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "AleshkoConsoleApp.h"
#include "Session.h"
#include "../TransportDLL/Sockets.h"
#include "SafeWriting.h"
#include <list>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;




std::list<Session*> sessions;
static int threadCounter = 0;



DWORD WINAPI workFunc(Session* session)
{

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
				HANDLE hFile = CreateFile((to_string(session->sessionID) + ".txt").c_str(),
					GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, 0);
				SetFilePointer(hFile, 0, 0, FILE_END);
				WriteFile(hFile, m.data.c_str(), m.data.size(), NULL, NULL);
				SafeWrite("session", session->sessionID, "data", m.data);
				CloseHandle(hFile);
				break;
			}
			}
		}
	}
	
	return 0;
}


void ProcessClient(SOCKET hSock)
{
	CSocket socket;
	socket.Attach(hSock);
	while (true)
	{
		Message mes = receiveMessage(socket);

		if (mes.header.messageType == MT_START)
		{
			sessions.push_back(new Session(++threadCounter));
			
			thread t(workFunc, sessions.back());
			
			t.detach();
			sendMessage(socket, Message(MT_DATA, threadCounter, "Ok"));
		}
		else if (mes.header.messageType == MT_GET)
		{
			std::string ids = "";
			std::for_each(sessions.begin(), sessions.end(), [&ids](Session* session)
				{
					ids += std::to_string(session->sessionID) + ",";
				});

			sendMessage(socket, Message(MT_DATA, mes.header.addr, ids));
		}
		else if (mes.header.messageType == MT_CLOSE)
		{
			if (sessions.size() == 0)
			{
				sendMessage(socket, Message(MT_DATA, 0, "Empty thread list"));
				continue;
			}

			Session* stop_candidate = nullptr;
			std::for_each(sessions.begin(), sessions.end(), [&mes, &stop_candidate, &socket](Session* session) {
				if (session->sessionID == mes.header.addr)
				{
					session->addMessage(MT_CLOSE);
					
					stop_candidate = session;
					return;
				}
				});
			if (stop_candidate)
			{
				sessions.remove(stop_candidate);
				sendMessage(socket, Message(MT_DATA, mes.header.addr, "Ok"));
			}
			else
			{
				sendMessage(socket, Message(MT_DATA, -1, "Thread not found"));
			}
		}
		else if (mes.header.messageType == MT_DATA)
		{
			bool foundRecipient = false;
			if (mes.header.addr == -1)
			{
				//SafeWrite("Main received data: ", mes.data);
				std::for_each(sessions.begin(), sessions.end(), [&mes](Session* s) {s->addMessage(mes); });
				foundRecipient = true;
			}
			else
				std::for_each(sessions.begin(), sessions.end(), [&mes, &foundRecipient](Session* s) {
				if (s->sessionID == mes.header.addr)
				{
					s->addMessage(mes);
					foundRecipient = true;
					return;
				}
					});
			if (foundRecipient)
				sendMessage(socket, Message(MT_DATA, mes.header.addr, "Ok"));
			else
				sendMessage(socket, Message(MT_DATA, -1, "Thread not found"));
		}
		else
		{
			SafeWrite("Error! Something wrong");
			sendMessage(socket, Message(MT_DATA, -1, "Error"));
		}
		
	}
}




void Server()
{
	AfxSocketInit();

	CSocket Server;
	Server.Create(12345);


	while (true)
	{
		if (!Server.Listen())
			break;
		CSocket s;
		Server.Accept(s);
		SafeWrite("Request Accepted ");
		thread t(ProcessClient, s.Detach());
		t.detach();
	}
}


int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {

        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
        {
			Server();
        }
    }
    else
    {
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}
