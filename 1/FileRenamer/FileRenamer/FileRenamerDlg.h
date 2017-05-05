
// FileRenamerDlg.h : 헤더 파일
//

#pragma once

#include "ChakraCore.h"
#pragma comment (lib, "C:/Script/ChakraCore/Build/VcBuild/bin/x86_release/ChakraCore.lib")


#define MAX_PLUGINS		10
typedef struct {
	TCHAR szPath[MAX_PATH];
	TCHAR szName[MAX_PATH];
	TCHAR szDesc[MAX_PATH];
	TCHAR szVersion[MAX_PATH];
	TCHAR szDate[MAX_PATH];
	TCHAR szAuthor[MAX_PATH];
	TCHAR szEmail[MAX_PATH];
} PLUGIN, *PPLUGIN;


#define MAX_FILES		256
typedef struct {
	wchar_t szSrcFile[MAX_PATH];
	wchar_t szDstFile[MAX_PATH];
} FILE_RENAMER, * PFILE_RENAMER;




// CFileRenamerDlg 대화 상자
class CFileRenamerDlg : public CDialogEx
{
// 생성입니다.
public:
	CFileRenamerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILERENAMER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedRenameButton();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

public:
	afx_msg void OnCbnSelchangePluginCombo();
	
private:
	UINT			m_nNumberOfPlugIns;
	PLUGIN			m_PlugIns[MAX_PLUGINS];

	UINT			m_nNumberOfFiles;
	FILE_RENAMER	m_FileList[MAX_FILES];

	LPWSTR LoadJSFile(LPWSTR pszJSFile);
	void LoadPlugInList();
	BOOL GetPlugInInfo(LPCWSTR szPlugIn, PPLUGIN pPlugIn);
	LPCWSTR LoadJSFile(LPCWSTR szJsFile);
};
