// TransferDll.cpp: определяет процедуры инициализации для библиотеки DLL.
//

#include "pch.h"
#include "framework.h"
#include "TransferDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: если эта библиотека DLL динамически связана с библиотеками DLL MFC,
//		все функции, экспортированные из данной DLL-библиотеки, которые выполняют вызовы к
//		MFC, должны содержать макрос AFX_MANAGE_STATE в
//		самое начало функции.
//
//		Например:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// тело нормальной функции
//		}
//
//		Важно, чтобы данный макрос был представлен в каждой
//		функции до вызова MFC.  Это означает, что
//		должен стоять в качестве первого оператора в
//		функции и предшествовать даже любым объявлениям переменных объекта,
//		поскольку их конструкторы могут выполнять вызовы к MFC
//		DLL.
//
//		В Технических указаниях MFC 33 и 58 содержатся более
//		подробные сведения.
//

// CTransferDllApp

BEGIN_MESSAGE_MAP(CTransferDllApp, CWinApp)
END_MESSAGE_MAP()


// Создание CTransferDllApp

CTransferDllApp::CTransferDllApp()
{
	// TODO: добавьте код создания,
	// Размещает весь важный код инициализации в InitInstance
}


// Единственный объект CTransferDllApp

CTransferDllApp theApp;


// Инициализация CTransferDllApp

BOOL CTransferDllApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}



struct header
{
	int addr;
	int size;
};

HANDLE g_mutex = CreateMutex(NULL, FALSE, NULL);


HANDLE startEvent = ::CreateEvent(NULL, FALSE, FALSE, "StartEvent");
HANDLE confirmEvent = ::CreateEvent(NULL, FALSE, FALSE, "ConfirmEvent");
HANDLE stopEvent = ::CreateEvent(NULL, FALSE, FALSE, "CloseProc");
HANDLE sendEvent = ::CreateEvent(NULL, FALSE, FALSE, "SendEvent");
HANDLE exitEvent = ::CreateEvent(NULL, FALSE, FALSE, "ExitProc");



extern "C"	__declspec(dllexport) void startThread()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	::SetEvent(startEvent);
	::WaitForSingleObject(confirmEvent, INFINITE);
}

extern "C" __declspec(dllexport) void stopThread()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	::SetEvent(stopEvent);
	::WaitForSingleObject(confirmEvent, INFINITE);
}


extern "C" __declspec(dllexport) void stopAllThreads()
{
	::SetEvent(exitEvent);
	::WaitForSingleObject(confirmEvent, INFINITE);
}


extern "C" __declspec(dllexport) void sendMessage(int addr, const char* str)
{

	::WaitForSingleObject(g_mutex, INFINITE);

	header h = { addr, strlen(str) + 1 };
	HANDLE hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, h.size + sizeof(header), "MyMap");
	char* buff = (char*)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, h.size + sizeof(header));

	memcpy(buff, &h, sizeof(header));
	memcpy(buff + sizeof(header), str, h.size);

	::ReleaseMutex(g_mutex);

	UnmapViewOfFile(buff);


	::SetEvent(sendEvent);
	::WaitForSingleObject(confirmEvent, INFINITE);
	//CloseHandle(hFileMap);
}

__declspec(dllexport) std::string getMessage(header& h)
{
	::WaitForSingleObject(g_mutex, INFINITE);

	HANDLE hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(header), "MyMap");
	LPVOID buff = MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(header));
	h = *((header*)buff);
	UnmapViewOfFile(buff);
	//CloseHandle(hFileMap);

	int n = h.size + sizeof(header);
	hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, n, "MyMap");
	buff = MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, n);

	std::string s((char*)buff + sizeof(header), h.size);


	//CloseHandle(hFileMap);
	UnmapViewOfFile(buff); 

	//CloseHandle(hFileMap);
	ReleaseMutex(g_mutex);
	return s;
}
