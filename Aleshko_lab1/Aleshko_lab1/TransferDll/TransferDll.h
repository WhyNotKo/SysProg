// TransferDll.h: основной файл заголовка для библиотеки DLL TransferDll
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CTransferDllApp
// Реализацию этого класса см. в файле TransferDll.cpp
//

class CTransferDllApp : public CWinApp
{
public:
	CTransferDllApp();

// Переопределение
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
