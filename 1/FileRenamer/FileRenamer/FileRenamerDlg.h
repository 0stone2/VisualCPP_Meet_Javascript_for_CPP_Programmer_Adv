
// FileRenamerDlg.h : ��� ����
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




// CFileRenamerDlg ��ȭ ����
class CFileRenamerDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CFileRenamerDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILERENAMER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
