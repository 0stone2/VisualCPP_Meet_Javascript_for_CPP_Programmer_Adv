
// FileRenamerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "FileRenamer.h"
#include "FileRenamerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



JsValueRef CALLBACK DbgString(
	_In_ JsValueRef callee,
	_In_ bool isConstructCall,
	_In_ JsValueRef *arguments,
	_In_ unsigned short argumentCount,
	_In_opt_ void *callbackState)
{
	JsErrorCode		nError;
	int				nIndex{ 0 };
	JsValueRef		ReturnValue;
	JsValueType		ArgumentType;
	const wchar_t * pszDbgString;
	size_t			nSize;

	for (nIndex = 1; nIndex < argumentCount; nIndex++)
	{
		nError = JsGetValueType(arguments[nIndex], &ArgumentType);
		if (JsString == ArgumentType)
		{
			JsStringToPointer(arguments[nIndex], &pszDbgString, &nSize);
			if (nSize > 0)
			{
				OutputDebugStringW(pszDbgString);
				OutputDebugStringW(L"\n");
			}
		}
	}

	return JS_INVALID_REFERENCE;
}

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileRenamerDlg 대화 상자



CFileRenamerDlg::CFileRenamerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FILERENAMER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileRenamerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFileRenamerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_RENAME_BUTTON, &CFileRenamerDlg::OnBnClickedRenameButton)
	ON_BN_CLICKED(IDOK, &CFileRenamerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CFileRenamerDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_PLUGIN_COMBO, &CFileRenamerDlg::OnCbnSelchangePluginCombo)
END_MESSAGE_MAP()


// CFileRenamerDlg 메시지 처리기

BOOL CFileRenamerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	::DragAcceptFiles(m_hWnd, true);


	// ChakraCore.DLL을 지연 로드 DLL로 등록하고, 여기서 DLL 검색 경로를 추가한다. 
	SetDllDirectory(L"C:/Script/ChakraCore/Build/VcBuild/bin/x86_release");


	((CListCtrl *)GetDlgItem(IDC_FROM_LIST)) -> InsertColumn(0, L"번호", LVCFMT_LEFT, 40, -1);
	((CListCtrl *)GetDlgItem(IDC_FROM_LIST)) -> InsertColumn(1, L"파일 이름", LVCFMT_LEFT, 200, -1);

	((CListCtrl *)GetDlgItem(IDC_TO_LIST))->InsertColumn(0, L"번호", LVCFMT_LEFT, 40, -1);
	((CListCtrl *)GetDlgItem(IDC_TO_LIST))->InsertColumn(1, L"파일 이름", LVCFMT_LEFT, 200, -1);

	LoadPlugInList();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CFileRenamerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CFileRenamerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CFileRenamerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFileRenamerDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDialogEx::OnDropFiles(hDropInfo);

	int nItem{ 0 };
	int nIndex{ 0 };
	int nFiles{ 0 };
	wchar_t szSrcFile[MAX_PATH]{ 0, };
	wchar_t szFname[MAX_PATH]{ 0, };
	wchar_t	szExt[MAX_PATH]{ 0, };

	wchar_t szIndex[MAX_PATH]{ 0 };


	nItem = ((CListCtrl *)GetDlgItem(IDC_FROM_LIST))->GetItemCount();
	nFiles = DragQueryFile(hDropInfo, -1, 0, 0);

	for (nIndex = 0; nIndex < nFiles; nIndex++)
	{
		::DragQueryFile(hDropInfo, nIndex, szSrcFile, sizeof(szSrcFile));

		wcscpy_s(m_FileList[nItem].szSrcFile, MAX_PATH, szSrcFile);

		_wsplitpath(szSrcFile, NULL, NULL, szFname, szExt);
		swprintf_s(szSrcFile, MAX_PATH, L"%s%s", szFname, szExt);

		swprintf_s(szIndex, MAX_PATH, L"%d", nItem + 1);
		((CListCtrl *)GetDlgItem(IDC_FROM_LIST))->InsertItem(nItem, szIndex, 0);
		((CListCtrl *)GetDlgItem(IDC_FROM_LIST))->SetItem(nItem, 1, LVIF_TEXT, szSrcFile, 0, 0, 0, 0);

		nItem++;
	}

	m_nNumberOfFiles = nItem;
}


void AddFunction(JsValueRef	GlobalObject, wchar_t *szFunction, JsNativeFunction pFunction)
{
	JsErrorCode		nError{ JsNoError };

	JsValueRef		Function;
	JsValueRef		FunctionName;
	JsPropertyIdRef FunctionId;
	void *			callbackState = NULL;


	JsPointerToString(szFunction, wcslen(szFunction), &(FunctionName));
	JsGetPropertyIdFromName(szFunction, &FunctionId);
	nError = JsCreateNamedFunction(FunctionName, pFunction, callbackState, &Function);
	JsSetProperty(GlobalObject, FunctionId, Function, true);
}

void CFileRenamerDlg::OnBnClickedRenameButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	BOOL bSuccess{ FALSE };

	LPCWSTR szPlugIn{ NULL };
	LPCWSTR pszScript{ NULL };
	int		nPlugIn{ -1 };

	JsErrorCode		nError{ JsNoError };
	JsRuntimeHandle Runtime = JS_INVALID_RUNTIME_HANDLE;
	JsContextRef	Context = JS_INVALID_REFERENCE;
	unsigned		SourceContext{ 0 };
	wchar_t *		srcUri = L"";
	JsValueRef		Result;

	JsValueRef		Function;
	JsValueRef		FunctionName;
	JsPropertyIdRef FunctionId;
	void *			callbackState = NULL;


	JsPropertyIdRef		Rename;
	JsValueType			Rename_Type;
	JsValueRef			Rename_Value;

	JsValueRef			GlobalObject;


	size_t				nSize{ 0 };
	JsValueRef			ReturnValue;
	const wchar_t *		pszStringValue;

	int nIndex{ 0 };

	int nItem{ 0 };
	wchar_t szIndex[MAX_PATH]{ 0, };
	wchar_t szFname[MAX_PATH]{ 0, };
	wchar_t szExt[MAX_PATH]{ 0, };
	wchar_t szDstFile[MAX_PATH]{ 0, };
	
	__try {
		int nPlugIn = ((CComboBox *)GetDlgItem(IDC_PLUGIN_COMBO))->GetCurSel();
		((CListCtrl *)GetDlgItem(IDC_TO_LIST))->DeleteAllItems();


		if (-1 == nPlugIn) __leave;

		pszScript = LoadJSFile(m_PlugIns[nPlugIn].szPath);
		if (NULL == pszScript) __leave;

		JsCreateRuntime(JsRuntimeAttributeNone, NULL, &Runtime);
		if (JS_INVALID_RUNTIME_HANDLE == Runtime) __leave;

		JsCreateContext(Runtime, &Context);
		if (JS_INVALID_REFERENCE == Context) __leave;

		JsSetCurrentContext(Context);

		nError = JsGetGlobalObject(&GlobalObject);

		AddFunction(GlobalObject, L"DbgString", (JsNativeFunction)DbgString);

		nError = JsRunScript(pszScript + 1, SourceContext++, srcUri, &Result);
		if (JsNoError != nError) __leave;


		JsValueRef Args[2];
		nError = JsPointerToString(L"self", wcslen(L"self"), &(Args[0]));	// 모든 함수의 첫번째 인자는 항상 thisArg다

		for(nIndex = 0; nIndex < m_nNumberOfFiles; nIndex++)
		{
			nError = JsPointerToString(m_FileList[nIndex].szSrcFile, wcslen(m_FileList[nIndex].szSrcFile), &(Args[1]));	// 모든 함수의 첫번째 인자는 항상 thisArg다


			nError = JsGetPropertyIdFromName(L"Rename", &Rename);
			nError = JsGetProperty(GlobalObject, Rename, &Rename_Value);
			nError = JsGetValueType(Rename_Value, &Rename_Type);
			nError = JsCallFunction(Rename_Value, Args, 2, &Result);

			JsStringToPointer(Result, &pszStringValue, &nSize);

			wcscpy_s(m_FileList[nIndex].szDstFile, MAX_PATH, pszStringValue);
			/////////////////////////////////////////////////////////////////////////////

			swprintf_s(szIndex, MAX_PATH, L"%d", nItem + 1);
			((CListCtrl *)GetDlgItem(IDC_TO_LIST))->InsertItem(nItem, szIndex, 0);

			_wsplitpath(pszStringValue, NULL, NULL, szFname, szExt);
			swprintf_s(szDstFile, MAX_PATH, L"%s%s", szFname, szExt);

			((CListCtrl *)GetDlgItem(IDC_TO_LIST))->SetItem(nItem, 1, LVIF_TEXT, szDstFile, 0, 0, 0, 0);

			nItem++;
		}
	}
	__finally {
		if (NULL != pszScript) free((void *)pszScript);

		if (JS_INVALID_RUNTIME_HANDLE != Runtime)
		{
			JsSetCurrentContext(JS_INVALID_REFERENCE);
			JsDisposeRuntime(Runtime);
		}
	}
}


void CFileRenamerDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	OutputDebugString(L"\n===========================================\n");
	for (int nIndex = 0; nIndex < m_nNumberOfFiles; nIndex++)
	{
		OutputDebugString(m_FileList[nIndex].szSrcFile);
		OutputDebugString(L"\n");
		OutputDebugString(m_FileList[nIndex].szDstFile);
		OutputDebugString(L"\n===========================================\n");
		_wrename(m_FileList[nIndex].szSrcFile, m_FileList[nIndex].szDstFile);
	}

	//CDialogEx::OnOK();
}


void CFileRenamerDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


// Javascript 파일을 메모리로 로드한다
LPWSTR CFileRenamerDlg::LoadJSFile(LPWSTR pszJSFile)
{
	BOOL	bSuccess{ FALSE };
	LPWSTR	pJSContents{ nullptr };

	HANDLE	hJSFile{ INVALID_HANDLE_VALUE };
	DWORD	dwSize{ 0 };
	DWORD	NumberOfBytesRead{ 0 };

	__try {
		hJSFile = CreateFile(pszJSFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hJSFile) __leave;

		dwSize = GetFileSize(hJSFile, NULL);
		if (0 == dwSize) __leave;

		pJSContents = (LPWSTR)malloc(dwSize + 2);
		if (NULL == pJSContents) __leave;
		ZeroMemory(pJSContents, dwSize + 2);

		ReadFile(hJSFile, pJSContents, dwSize, &NumberOfBytesRead, NULL);
		if (NumberOfBytesRead != dwSize) __leave;

		bSuccess = TRUE;
	}
	__finally {
		if (INVALID_HANDLE_VALUE != hJSFile) CloseHandle(hJSFile);
		if (FALSE == bSuccess)
		{
			if (NULL != pJSContents) free(pJSContents);
		}
	}

	return pJSContents;
}

void CFileRenamerDlg::OnCbnSelchangePluginCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}


// PlugIn 목록을 얻어 온다...
void CFileRenamerDlg::LoadPlugInList()
{
	// 1. PlugIn 폴더에 있는 특정 확장자(js)를 가진 파일들을 목록을 구한다.
	CFileFind	PlugInFinder;
	BOOL		bFound{ FALSE };
	BOOL		bSuccess{ FALSE };
	PLUGIN		PlugIn{ 0, };
	UINT		nIndex{ 0 };

	bFound = PlugInFinder.FindFile(L"./PlugIn/*.js");

	while (bFound)
	{
		bFound = PlugInFinder.FindNextFile();

		// 2. 얻어온 파일들에서 특정 함수 (GetInfo)를 호출한다. 
		// 이름, 설명, 버전, 날짜, 만든이
		ZeroMemory(&PlugIn, sizeof(PlugIn));
		bSuccess = GetPlugInInfo((LPCTSTR)PlugInFinder.GetFilePath(), &PlugIn);

		
		if (TRUE == bSuccess)
		{
			// 이 함수가 리턴한 문자열을 콤보 박스에 추가한다.....
			((CComboBox *)GetDlgItem(IDC_PLUGIN_COMBO))->AddString(PlugIn.szName);


			// 3. PlugIn 리스트에 보관한다.
			memcpy(&(m_PlugIns[nIndex]), &PlugIn, sizeof(PLUGIN));


			nIndex++;
		}
		

		if (nIndex >= MAX_PLUGINS) break;
	}



}

BOOL CFileRenamerDlg::GetPlugInInfo(LPCWSTR szPlugIn, PPLUGIN pPlugIn)
{
	BOOL bSuccess{ FALSE };

	LPCWSTR pszScript = NULL;


	JsErrorCode		nError{ JsNoError };
	JsRuntimeHandle Runtime = JS_INVALID_RUNTIME_HANDLE;
	JsContextRef	Context = JS_INVALID_REFERENCE;
	unsigned		SourceContext{ 0 };
	wchar_t *		srcUri = L"";
	JsValueRef		Result;

	JsPropertyIdRef		GetInfo;
	JsValueType			GetInfo_Type;
	JsValueRef			GetInfo_Value;

	size_t				nSize{ 0 };
	JsValueRef			nIndex;
	JsValueRef			ReturnValue;
	const wchar_t *		pszStringValue;

	JsValueRef			GlobalObject;
	

	__try {
		pszScript = LoadJSFile(szPlugIn);
		if (NULL == pszScript) __leave;

		JsCreateRuntime(JsRuntimeAttributeNone, NULL, &Runtime);
		if (JS_INVALID_RUNTIME_HANDLE == Runtime) __leave;

		JsCreateContext(Runtime, &Context);
		if (JS_INVALID_REFERENCE == Context) __leave;

		JsSetCurrentContext(Context);

		nError = JsGetGlobalObject(&GlobalObject);

		AddFunction(GlobalObject, L"DbgString", (JsNativeFunction)DbgString);

		nError = JsRunScript(pszScript + 1, SourceContext++, srcUri, &Result);
		if (JsNoError != nError) __leave;

		

		JsValueRef Args[1];
		nError = JsPointerToString(L"self", wcslen(L"self"), &(Args[0]));	// 모든 함수의 첫번째 인자는 항상 thisArg다

		

		nError = JsGetPropertyIdFromName(L"GetInfo", &GetInfo);
		nError = JsGetProperty(GlobalObject, GetInfo, &GetInfo_Value);
		nError = JsGetValueType(GetInfo_Value, &GetInfo_Type);
		nError = JsCallFunction(GetInfo_Value, Args, 1, &Result);



		JsIntToNumber(0, &nIndex);
		nError = JsGetIndexedProperty(Result, nIndex, &ReturnValue);
		JsStringToPointer(ReturnValue, &pszStringValue, &nSize);
		wcscpy_s(pPlugIn->szName, MAX_PATH, pszStringValue);


		JsIntToNumber(1, &nIndex);
		nError = JsGetIndexedProperty(Result, nIndex, &ReturnValue);
		JsStringToPointer(ReturnValue, &pszStringValue, &nSize);
		wcscpy_s(pPlugIn->szDesc, MAX_PATH, pszStringValue);


		JsIntToNumber(2, &nIndex);
		nError = JsGetIndexedProperty(Result, nIndex, &ReturnValue);
		JsStringToPointer(ReturnValue, &pszStringValue, &nSize);
		wcscpy_s(pPlugIn->szVersion, MAX_PATH, pszStringValue);


		JsIntToNumber(3, &nIndex);
		nError = JsGetIndexedProperty(Result, nIndex, &ReturnValue);
		JsStringToPointer(ReturnValue, &pszStringValue, &nSize);
		wcscpy_s(pPlugIn->szDate, MAX_PATH, pszStringValue);


		JsIntToNumber(4, &nIndex);
		nError = JsGetIndexedProperty(Result, nIndex, &ReturnValue);
		JsStringToPointer(ReturnValue, &pszStringValue, &nSize);
		wcscpy_s(pPlugIn->szAuthor, MAX_PATH, pszStringValue);


		JsIntToNumber(5, &nIndex);
		nError = JsGetIndexedProperty(Result, nIndex, &ReturnValue);
		JsStringToPointer(ReturnValue, &pszStringValue, &nSize);
		wcscpy_s(pPlugIn->szEmail, MAX_PATH, pszStringValue);


		wcscpy_s(pPlugIn->szPath, MAX_PATH, szPlugIn);
		bSuccess = TRUE;
	}
	__finally {
		if (NULL != pszScript) free((void *)pszScript);

		if (JS_INVALID_RUNTIME_HANDLE != Runtime)
		{
			JsSetCurrentContext(JS_INVALID_REFERENCE);
			JsDisposeRuntime(Runtime);
		}

	}

	return bSuccess;
}


LPCWSTR CFileRenamerDlg::LoadJSFile(LPCWSTR szJsFile)
{
	BOOL bSuccess{ FALSE };
	wchar_t *pJSContents{ NULL };

	HANDLE	hJSFile{ INVALID_HANDLE_VALUE };
	DWORD	dwSize{ 0 };
	DWORD	NumberOfBytesRead{ 0 };

	__try {
		hJSFile = CreateFile(szJsFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hJSFile) __leave;

		dwSize = GetFileSize(hJSFile, NULL);
		if (0 == dwSize) __leave;

		pJSContents = (wchar_t *)malloc(dwSize + 2);
		if (NULL == pJSContents) __leave;
		ZeroMemory(pJSContents, dwSize + 2);

		ReadFile(hJSFile, pJSContents, dwSize, &NumberOfBytesRead, NULL);
		if (NumberOfBytesRead != dwSize) __leave;

		bSuccess = TRUE;
	}
	__finally {
		if (INVALID_HANDLE_VALUE != hJSFile) CloseHandle(hJSFile);
		if (FALSE == bSuccess)
		{
			if (NULL != pJSContents) free(pJSContents);
		}
	}

	return pJSContents;
}
