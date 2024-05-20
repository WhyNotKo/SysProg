// TransportDLL.cpp : Defines the initialization routines for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "TransportDLL.h"
#include "Message.h"

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


extern "C" __declspec(dllexport) int Connect(int port = 12345)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	static bool first_connect = true;

	if (first_connect)
	{
		AfxSocketInit();
		first_connect == false;
	}
	
	CSocket socket;
	socket.Create();
	if (!socket.Connect("127.0.0.1", port))
	{
		return 0;
	}
	Message mes = Message::send(MR_BROKER, MT_INIT);
	return mes.header.to;
}

extern "C" __declspec(dllexport) void Disconnect()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	Message::send(MR_BROKER, MT_EXIT);
}

int messageDataToBuffer(Message& response ,char* buffer)
{
	if (response.header.size >= 1)
	{
		memcpy_s(buffer, response.header.size, response.data.c_str(), response.header.size);
		return response.header.size;
	}
	return 0;
}

extern "C" __declspec(dllexport) int ProcessMessages(int maxBufferLength, int& mes_code, char* buffer)
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	Message mes = Message::send(MR_BROKER, MT_GETDATA);

	switch (mes.header.messageType)
	{
	case MT_DATA:
	{
		mes_code = MT_DATA;
		return messageDataToBuffer(mes, buffer);
	}
	case MT_CLIENTS_LIST:
	{
		if (mes.header.size > 1)
		{
			mes_code = MT_CLIENTS_LIST;
			memcpy_s(buffer, maxBufferLength, mes.data.c_str(), mes.data.size()-1);
			return mes.data.size()-1;
		}
		break;
	}
	}
	return 0;
}

extern "C" __declspec(dllexport) void sendMessageTo(int addr, const char* str)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	Message::send(addr, MT_DATA, str);
}
