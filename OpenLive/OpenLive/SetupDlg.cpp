// SetupDlg.cpp : implement file
//

#include "stdafx.h"
#include "OpenLive.h"
#include "SetupDlg.h"
#include "afxdialogex.h"
#include <json/json.h>
#include <IAgoraRtcEngine.h>
#include <fstream>

#include "AgoraObject.h"
// CSetupDlg dialog
static TCHAR BASED_CODE szFilter[] = _T("Json Files (*.json)|*.json|");

IMPLEMENT_DYNAMIC(CSetupDlg, CDialogEx)

CSetupDlg::CSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupDlg::IDD, pParent)
{

}

CSetupDlg::~CSetupDlg()
{
}

void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNCANCEL_SETUP, m_btnCancel);
	DDX_Control(pDX, IDC_BTNCONFIRM_SETUP, m_btnConfirm);
	DDX_Control(pDX, IDC_CKSAVESETTING_SETUP, m_ckSaveSettings);
	DDX_Control(pDX, IDC_CKSWPWH_SETUP, m_ckSwapWH);
	DDX_Control(pDX, IDC_CKFULLBAND_SETUP, m_ckFullBand);
	DDX_Control(pDX, IDC_CKSTEREO_SETUP, m_ckStereo);
	DDX_Control(pDX, IDC_CKFULLBITRATE_SETUP, m_ckFullBitrate);
	DDX_Control(pDX, IDC_COMBO_ENCODER, m_cmbEncoder);
	DDX_Control(pDX, IDC_CHECK_INTRA_REQUEST, m_chkIntraRequest);
	DDX_Control(pDX, IDC_EDIT_PARAMETER, m_edtParam);
	DDX_Control(pDX, IDC_BUTTON_SETPARAM, m_btnSetParam);
	DDX_Control(pDX, IDC_BUTTON_GETPARAM, m_btnGetParam);
	DDX_Control(pDX, IDC_BUTTON_READFILE, m_btnReadFile);
}


BEGIN_MESSAGE_MAP(CSetupDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTNCONFIRM_SETUP, &CSetupDlg::OnBnClickedBtnconfirmSetup)
//	ON_CBN_SELCHANGE(IDC_CMBRES_SETUP, &CSetupDlg::OnCbnSelchangeCmbresSetup)
//	ON_CBN_SELCHANGE(IDC_CMBCODEC_SETUP, &CSetupDlg::OnCbnSelchangeCmbresSetup)

	ON_WM_HSCROLL()
	ON_WM_LBUTTONDBLCLK()

	ON_BN_CLICKED(IDC_BUTTON1, &CSetupDlg::OnBnClickedBtncancelSetup)
	ON_BN_CLICKED(IDC_BUTTON_SETPARAM, &CSetupDlg::OnBnClickedButtonSetparam)
	ON_BN_CLICKED(IDC_BUTTON_GETPARAM, &CSetupDlg::OnBnClickedButtonGetparam)
	ON_BN_CLICKED(IDC_BUTTON_READFILE, &CSetupDlg::OnBnClickedButtonReadfile)
END_MESSAGE_MAP()


// CSetupDlg message deal with app


BOOL CSetupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  add code to initialize
	m_ftHead.CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_ftDes.CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_ftBtn.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

	m_penFrame.CreatePen(PS_SOLID, 1, RGB(0xD8, 0xD8, 0xD8));
	m_cbxVideoProfile.Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, CRect(0, 0, 300, 40), this, IDC_CMBVDOPRF_SETUP);
	SetBackgroundColor(RGB(0xFF, 0xFF, 0xFF), TRUE);
    InitData();
    InitCtrls();

	if (m_agConfig.IsAutoSaveEnabled())
		m_ckSaveSettings.SetCheck(TRUE);
	else
		m_ckSaveSettings.SetCheck(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// error:  OCX attribute pages return FALSE
}


void CSetupDlg::InitCtrls()
{
	CRect ClientRect;

	MoveWindow(0, 0, 320, 450, 1);
	GetClientRect(&ClientRect);

	int nResolutionIndex = 0;
	CString str;
	
	m_cbxVideoProfile.MoveWindow(200, 80, 200, 22, TRUE);
	m_cbxVideoProfile.SetFont(&m_ftDes);
	m_cbxVideoProfile.SetButtonImage(IDB_CMBBTN, 12, 12, RGB(0xFF, 0x00, 0xFF));
	m_cbxVideoProfile.SetFaceColor(RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF));
	m_cbxVideoProfile.SetListMaxHeight(100);


	m_ckSwapWH.MoveWindow(80, 120, 20, 20, TRUE);
	m_ckSaveSettings.MoveWindow(80, ClientRect.Height() - 95, 20, 20, TRUE);
	m_ckSaveSettings.ShowWindow(SW_HIDE);

	m_ckFullBand.MoveWindow(80, 220, 20, 20, TRUE);
	m_ckStereo.MoveWindow(80, 240, 20, 20, TRUE);
	m_ckFullBitrate.MoveWindow(80, 260, 20, 20, TRUE);
	m_ckStereo.ShowWindow(SW_HIDE);
	m_ckFullBitrate.ShowWindow(SW_HIDE);
	m_ckFullBand.ShowWindow(SW_HIDE);

	m_edtParam.MoveWindow(80, 150, 300, 40, TRUE);
	m_btnReadFile.MoveWindow(80, 200, 60, 30, TRUE);
	m_btnGetParam.MoveWindow(150, 200, 40, 30, TRUE);
	m_btnSetParam.MoveWindow(200, 200, 40, 30, TRUE);
	m_btnReadFile.ShowWindow(SW_SHOW);
	//
	m_cmbEncoder.MoveWindow(80, ClientRect.Height() - 155, 160, 40, TRUE);
	m_chkIntraRequest.MoveWindow(250, ClientRect.Height() - 160, 160, 40, TRUE);
	m_cmbEncoder.InsertString(0, LANG_STR("IDS_ENCODE_SOFTWARE"));
	m_cmbEncoder.InsertString(1, LANG_STR("IDS_ENCODE_NVIDIA"));
	m_cmbEncoder.InsertString(2, LANG_STR("IDS_ENCODE_INTEL"));
	m_cmbEncoder.SetCurSel(0);

	m_btnCancel.MoveWindow(ClientRect.Width() / 2 - 93, ClientRect.Height() - 58, 174, 36, TRUE);
	m_btnConfirm.MoveWindow(ClientRect.Width() / 2 + 93, ClientRect.Height() - 58, 174, 36, TRUE);

	for (int nIndex = 0; nIndex < 31; nIndex++) {
		m_cbxVideoProfile.InsertString(nIndex, m_szProfileDes[nIndex]);
		m_cbxVideoProfile.SetItemData(nIndex, (DWORD_PTR)m_nProfileValue[nIndex]);
	}

	m_btnCancel.SetBorderColor(RGB(0xD8, 0xD8, 0xD8), RGB(0x00, 0xA0, 0xE9), RGB(0x00, 0xA0, 0xE9), RGB(0xCC, 0xCC, 0xCC));
	m_btnCancel.SetBackColor(RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF));
	m_btnCancel.SetTextColor(RGB(0x55, 0x58, 0x5A), RGB(0x00, 0xA0, 0xE9), RGB(0x00, 0xA0, 0xE9), RGB(0xCC, 0xCC, 0xCC));

	m_btnConfirm.SetBackColor(RGB(0, 160, 239), RGB(0, 160, 239), RGB(0, 160, 239), RGB(192, 192, 192));
	m_btnConfirm.SetFont(&m_ftBtn);
	m_btnConfirm.SetTextColor(RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xC8, 0x64), RGB(0xFF, 0xC8, 0x64), RGB(0xCC, 0xCC, 0xCC));
	m_btnConfirm.SetWindowText(LANG_STR("IDS_SET_BTCONFIRM"));

    if (m_agConfig.IsAutoSaveEnabled())
        nResolutionIndex = m_agConfig.GetSolution();
    else
        nResolutionIndex = 0;
    
    m_cbxVideoProfile.SetCurSel(nResolutionIndex);
}

void CSetupDlg::InitData()
{
	int i = 0; 
	/*m_szProfileDes[i] = _T("640x360 15fps 400kbps");
	m_nProfileValue[i++] = 30;
	m_szProfileDes[i] = _T("640x360 30fps 600kbps");
	m_nProfileValue[i++] = 33;
	m_szProfileDes[i] = _T("640x360 15fps 800kbps");
	m_nProfileValue[i++] = 38;

	m_szProfileDes[i] = _T("640x480 15fps 500kbps");
	m_nProfileValue[i++] = 40;

	m_szProfileDes[i] = _T("640x480 30fps 750kbps");
	m_nProfileValue[i++] = 43;

	m_szProfileDes[i] = _T("1280x720 15fps 1130kbps");
	m_nProfileValue[i++] = 50;*/
	m_szProfileDes[i] = _T("1280x720 30fps 1710kbps");
	m_nProfileValue[i++] = 52;

	//m_szProfileDes[i] = _T("1920x1080 15fps 2080kbps");
	//m_nProfileValue[i++] = 60;
	m_szProfileDes[i] = _T("1920x1080 30fps 3150kbps");
	m_nProfileValue[i++] = 62;
	m_szProfileDes[i] = _T("1920x1080 60fps 4780kbps");
	m_nProfileValue[i++] = 64;
}

void CSetupDlg::DrawClient(CDC *lpDC)
{
	CRect	rcText;
	CRect	rcClient;
	LPCTSTR lpString = NULL;

	GetClientRect(&rcClient);

	CFont *lpDefFont = lpDC->SelectObject(&m_ftHead);
	CPen  *lpDefPen = lpDC->SelectObject(&m_penFrame);

	rcText.SetRect(rcClient.Width() / 2 - 188, 75, rcClient.Width() / 2 + 172, 107);
	lpDC->RoundRect(&rcText, CPoint(32, 32));

	lpDC->SetBkColor(RGB(0xFF, 0xFF, 0xFF));
	lpDC->SetTextColor(RGB(0xD8, 0xD8, 0xD8));

	lpDC->SetTextColor(RGB(0x44, 0x45, 0x46));
	lpString = LANG_STR("IDS_SET_RESOLUTION");
	lpDC->TextOut(80, 83, lpString, _tcslen(lpString));
	
	lpString = LANG_STR("IDS_SET_SWAPWH");
	lpDC->TextOut(100, 120, lpString, _tcslen(lpString));

	/*lpString = LANG_STR("IDS_SET_FULLBAND");
	lpDC->TextOut(100, 220, lpString, _tcslen(lpString));
	lpString = LANG_STR("IDS_SET_STEREO");
	lpDC->TextOut(100, 240, lpString, _tcslen(lpString));
	lpString = LANG_STR("IDS_SET_FULLBITRATE");
	lpDC->TextOut(100, 260, lpString, _tcslen(lpString));


	lpString = LANG_STR("IDS_SET_SAVESETTING");
	lpDC->TextOut(100, rcClient.Height() - 95, lpString, _tcslen(lpString));*/

	lpDC->SelectObject(lpDefPen);
	lpDC->SelectObject(lpDefFont);
}

void CSetupDlg::OnBnClickedBtnconfirmSetup()
{
	// TODO:  Add control notification handler code here
	GetParent()->SendMessage(WM_GOBACK, 0, 0);

	if (m_ckSaveSettings.GetCheck() == TRUE) {
		m_agConfig.EnableAutoSave(TRUE);
        m_agConfig.SetSolution(m_cbxVideoProfile.GetCurSel());
	}
	else
		m_agConfig.EnableAutoSave(FALSE);

	BOOL bFullBand = m_ckFullBand.GetCheck();
	BOOL bStereo = m_ckStereo.GetCheck();
	BOOL bFullBitrate = m_ckFullBitrate.GetCheck();

//	CAgoraObject::GetAgoraObject()->SetHighQualityAudio(bFullBand, bStereo, bFullBitrate);
}

void CSetupDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	DrawClient(&dc);
}

int CSetupDlg::GetVideoSolution()
{
    int nIndex = m_cbxVideoProfile.GetCurSel();
    return (int)m_cbxVideoProfile.GetItemData(nIndex);
}

int CSetupDlg::GetEncoderType()
{
	int nIndex = m_cmbEncoder.GetCurSel();
	return nIndex;
}

bool CSetupDlg::GetCheckIntra()
{
	return m_chkIntraRequest.GetCheck();
}

void CSetupDlg::SetVideoSolution(int nIndex)
{
	m_cbxVideoProfile.SetCurSel(nIndex);
}

CString CSetupDlg::GetVideoSolutionDes()
{
    int nIndex = m_cbxVideoProfile.GetCurSel();
    if (nIndex == -1)
        nIndex = 0;

    return (CString)m_szProfileDes[nIndex];
}

void CSetupDlg::SetWHSwap(BOOL bSwap)
{
	m_ckSwapWH.SetCheck(bSwap);
}

BOOL CSetupDlg::IsWHSwap()
{
	return (BOOL)m_ckSwapWH.GetCheck();
}

void CSetupDlg::OnBnClickedBtncancelSetup()
{
	// TODO:  Add control notification handler code here
	GetParent()->SendMessage(WM_GOBACK, 0, 0);
}


void CSetupDlg::OnBnClickedButtonSetparam()
{
	CString strExtParam;
	
	m_edtParam.GetWindowText(strExtParam);
	if (strExtParam.GetLength() > 0)
		CAgoraObject::GetAgoraObject()->SetExtentParam(strExtParam);

	m_edtParam.SetWindowText(NULL);
	// TODO: Add your control notification handler code here
}



void CSetupDlg::OnBnClickedButtonGetparam()
{
	CString strExtParam;
	CString strResult;

	m_edtParam.GetWindowText(strExtParam);
	if (strExtParam.GetLength() > 0)
		strResult = CAgoraObject::GetAgoraObject()->GetExtentParam(strExtParam);

	m_edtParam.SetWindowText(strResult);
}


void CSetupDlg::OnBnClickedButtonReadfile()
{
	CFileDialog fileDlg(TRUE, szFilter);

	if (IDOK == fileDlg.DoModal()){
		CString strFile = fileDlg.GetPathName();
		char szFile[MAX_PATH] = { 0 };
		WideCharToMultiByte(CP_ACP, 0, strFile, -1, szFile, MAX_PATH, NULL, NULL);
		std::string file = szFile;
		std::ifstream ifs(file);
		
		Json::Value jsonOffline;
		Json::CharReaderBuilder readerBuilder;
		JSONCPP_STRING errs;
		if (Json::parseFromStream(readerBuilder, ifs, &jsonOffline, &errs)){
			if (jsonOffline.isArray()){
				IRtcEngine* engine = CAgoraObject::GetAgoraObject()->GetEngine();
				AParameter apm(engine);
				int size = jsonOffline.size();
				for (size_t i = 0; i < size; i++){
					

					std::string strJson = jsonOffline[i].toString();
					CStringA csJson = strJson.c_str();

					csJson.Replace("\t", "");
					csJson.Replace("\n", "");
					
					//char szJson[MAX_PATH] = { 0 };
					//::WideCharToMultiByte(CP_UTF8, 0, csJson, -1, szJson, MAX_PATH, NULL, NULL);
					apm->setParameters(csJson);
					//::SendMessage(m_hWnd, MSGID_EVENT_RECEIVE, reinterpret_cast<WPARAM>(strJson.c_str()), static_cast<LPARAM>(strJson.length()));

				}
			}
			else{
				AfxMessageBox(_T("json必须是数组类型"));
			}
		}
		else{
			AfxMessageBox(_T("Json文件格式不正确"));
		}
	}
}
