// TransportDLL.cpp : Defines the initialization routines for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "TransportDLL.h"
#include "Sockets.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CTransportDLLApp

BEGIN_MESSAGE_MAP(CTransportDLLApp, CWinApp)
END_MESSAGE_MAP()


// CTransportDLLApp construction

CTransportDLLApp::CTransportDLLApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTransportDLLApp object

CTransportDLLApp theApp;


// CTransportDLLApp initialization

BOOL CTransportDLLApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

#pragma section("Shared")
std::unordered_set<int> threadIDs= std::unordered_set<int>();
#pragma section()

#pragma(linker, "/SECTION: Shared RWS")


CSocket myconnection;
std::unordered_set<int> l_threadIDs;	

extern "C" __declspec(dllexport) int Connect(int port = 12345)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	AfxSocketInit();

	myconnection.Create();
	if (!myconnection.Connect("127.0.0.1", port))
	{
		return 0;
	}

	return 1;
}

int messageDataToBuffer(Message& response ,char* buffer)
{
	if (response.header.size > 1)
	{
		memcpy(buffer, response.data.c_str(), response.data.size());
		return response.data.size();
	}
	return 0;
}

extern "C"	__declspec(dllexport) int startThread(char* buffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	Message message(MT_START);
	sendMessage(myconnection, message);	
	Message response = receiveMessage(myconnection);

	return messageDataToBuffer(response, buffer);
}

extern "C" __declspec(dllexport) int stopThread(int addr, char* buffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	Message message(MT_CLOSE, addr); 
	sendMessage(myconnection, message);
	Message response = receiveMessage(myconnection);

	return messageDataToBuffer(response, buffer);
}

extern "C" __declspec(dllexport) int sendMessageTo(int addr, const char* str, char* buffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	Message message(MT_DATA, addr, str);
	sendMessage(myconnection, message);
	Message response = receiveMessage(myconnection);

	return messageDataToBuffer(response, buffer);
}

extern "C" __declspec(dllexport) int getWorkThreads(int maxBufferLength, char* buffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());;

	Message message(MT_GET);
	sendMessage(myconnection, message);
	Message response = receiveMessage(myconnection); //data format: id1,id2,id3,

	if (response.header.size > 1)
	{
		memcpy_s(buffer, maxBufferLength, response.data.c_str(), response.data.size() - 1);
		return response.data.size() - 1;
	}
	return 0;
}
