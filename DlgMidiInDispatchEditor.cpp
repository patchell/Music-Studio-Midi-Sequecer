// DlgMidiInDispatchEditor.cpp : implementation file
//

#include "pch.h"

struct DispatchControlEnableFlags {
	unsigned m_WindowCommand : 1;
	unsigned m_MidiStatus : 1;
	unsigned m_MidiChannel : 1;
};

// CDlgMidiInDispatchEditor dialog

IMPLEMENT_DYNAMIC(CDlgMidiInDispatchEditor, CDialog)

CDlgMidiInDispatchEditor::CDlgMidiInDispatchEditor(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MIDI_IN_DISPATCH, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	m_DeviceID = 0;
	m_pMIMD = 0;
	m_MID = 0;
	m_ppChildViews = 0;
}

CDlgMidiInDispatchEditor::~CDlgMidiInDispatchEditor()
{
}

void CDlgMidiInDispatchEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DEST_WINDOW, m_Combo_DestinationWindow);
	DDX_Control(pDX, IDC_COMBO_DISPATCH_COMMAND, m_Combo_WM_DispatchCommand);
	DDX_Control(pDX, IDC_COMBO_DISPATCH_MIDUCHANNEL, m_Combo_MidiChannel);
	DDX_Control(pDX, IDC_COMBO_DISPATCH_STATUS, m_Combo_MidiStatusByte);
	DDX_Control(pDX, IDC_EDIT_DISPATCH_DATAONE_HIGH, m_Edit_Data1_High);
	DDX_Control(pDX, IDC_EDIT_DISPATCH_DATAONE_LOW, m_Edit_Data1_Low);
	DDX_Control(pDX, IDC_EDIT_DISPATCH_DATATWO_HIGH, m_Edit_Data2_High);
	DDX_Control(pDX, IDC_EDIT_DISPATCH_DATATWO_LOW, m_Edit_Data2_Low);
	DDX_Control(pDX, IDC_STATIC_DISPATCH_DATA1, m_Static_Data1);
	DDX_Control(pDX, IDC_STAIC_DISPATCH_DATA2, m_Static_Data2);
	DDX_Control(pDX, IDC_EDIT_DISPATCH_NAME, m_Edit_Name);
	DDX_Control(pDX, IDC_BUTTON_PREV_DISPATCH, m_Button_Prev);
	DDX_Control(pDX, IDC_BUTTON_NEXT_DISPATCH, m_Button_Next);
	DDX_Control(pDX, IDC_BUTTON_DELETEDISPATCHER, m_Button_Delete);
	DDX_Control(pDX, IDC_STATIC_CHANNEL, m_static_Channel);
	DDX_Control(pDX, IDC_STATIC_COMMAND, m_static_Command);
	DDX_Control(pDX, IDC_STATIC_HIGH_VAL, m_static_HighValue);
	DDX_Control(pDX, IDC_STATIC_LOW_VAL, m_Static_LowValue);
	DDX_Control(pDX, IDC_STATIC_STATUS_BYTE, m_static_StatusByte);
}


BEGIN_MESSAGE_MAP(CDlgMidiInDispatchEditor, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PREV_DISPATCH, &CDlgMidiInDispatchEditor::OnBnClickedButtonPrevDispatch)
	ON_BN_CLICKED(IDC_BUTTON_DELETEDISPATCHER, &CDlgMidiInDispatchEditor::OnBnClickedButtonDeletedispatcher)
	ON_BN_CLICKED(IDC_BUTTON_NEWDISPATCHER, &CDlgMidiInDispatchEditor::OnBnClickedButtonNewdispatcher)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_DISPATCH, &CDlgMidiInDispatchEditor::OnBnClickedButtonNextDispatch)
	ON_CBN_SELCHANGE(IDC_COMBO_DEST_WINDOW, &CDlgMidiInDispatchEditor::OnCbnSelchangeComboDestWindow)
	ON_CBN_SELCHANGE(IDC_COMBO_DISPATCH_COMMAND, &CDlgMidiInDispatchEditor::OnCbnSelchangeComboDispatchCommand)
	ON_CBN_SELCHANGE(IDC_COMBO_DISPATCH_STATUS, &CDlgMidiInDispatchEditor::OnCbnSelchangeComboDispatchStatus)
	ON_CBN_SELCHANGE(IDC_COMBO_DISPATCH_MIDUCHANNEL, &CDlgMidiInDispatchEditor::OnCbnSelchangeComboDispatchMiduchannel)
	ON_EN_KILLFOCUS(IDC_EDIT_DISPATCH_DATAONE_HIGH, &CDlgMidiInDispatchEditor::OnKillfocusEditDispatchDataoneHigh)
	ON_EN_KILLFOCUS(IDC_EDIT_DISPATCH_DATAONE_LOW, &CDlgMidiInDispatchEditor::OnKillfocusEditDispatchDataoneLow)
	ON_EN_KILLFOCUS(IDC_EDIT_DISPATCH_DATATWO_HIGH, &CDlgMidiInDispatchEditor::OnKillfocusEditDispatchDatatwoHigh)
	ON_EN_KILLFOCUS(IDC_EDIT_DISPATCH_DATATWO_LOW, &CDlgMidiInDispatchEditor::OnKillfocusEditDispatchDatatwoLow)
	ON_EN_KILLFOCUS(IDC_EDIT_DISPATCH_NAME, &CDlgMidiInDispatchEditor::OnKillfocusEditDispatchName)
	ON_CBN_KILLFOCUS(IDC_COMBO_DEST_WINDOW, &CDlgMidiInDispatchEditor::OnCbnKillfocusComboDestWindow)
	ON_CBN_KILLFOCUS(IDC_COMBO_DISPATCH_COMMAND, &CDlgMidiInDispatchEditor::OnCbnKillfocusComboDispatchCommand)
	ON_CBN_KILLFOCUS(IDC_COMBO_DISPATCH_STATUS, &CDlgMidiInDispatchEditor::OnCbnKillfocusComboDispatchStatus)
	ON_CBN_KILLFOCUS(IDC_COMBO_DISPATCH_MIDUCHANNEL, &CDlgMidiInDispatchEditor::OnCbnKillfocusComboDispatchMiduchannel)
END_MESSAGE_MAP()


// CDlgMidiInDispatchEditor message handlers


BOOL CDlgMidiInDispatchEditor::OnInitDialog()
{
	CString csString;;
	unsigned n;
	CChildFrameBase* pCFB;
	CString csCaption;

	CDialog::OnInitDialog();

	//----------------------------------
	// Initialize the Combo box that
	// will be used to select the view
	// that the midi message will be
	// Sent to
	//----------------------------------
	n = GETMAINFRAME->GetNumberOfChildren();
	m_ppChildViews = new CChildViewBase * [n];
	pCFB = GETMAINFRAME->GetHead();
	for (size_t i = 0; i < n; i++)
	{
		m_ppChildViews[i] = pCFB->GetView();
		pCFB->GetWindowTextW(csCaption);
		m_Combo_DestinationWindow.InsertString(i, csCaption);
		pCFB = pCFB->GetNext();
	}
	InitializeMidiStatusByteComboBox();
	InitializeMidiChannelComboBox();
	//----------------------------------------
	if (GETAPP->GetMidiInTable()->GetMidiInDevice(m_DeviceID)->GetDispatcherTail())
	{
		//--------------------------------
		// If there are any dispatch 
		// objects, then load the last one
		//-----------------------------------
		m_pMIMD = GETAPP->GetMidiInTable()->GetMidiInDevice(m_DeviceID)->GetDispatcherTail();
		//------------------------------------
		// Fill in the fields in the dialog
		//------------------------------------
		UpdateDialogFields();
		EnableCommand(1);
		EnableChannel(1);
		EnableStatusByte(1);
		UpdateDataEditBoxes(m_Combo_MidiStatusByte.GetCurSel());
	}
	else
	{
		CreateANewDispatcher();
	}
	m_Edit_Name.SetFocus();
	return TRUE;  // return true unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return false
}

void CDlgMidiInDispatchEditor::InitializeMidiChannelComboBox()
{
	//--------------------------------------
	// Initialize the combo box that will be
	// used to select the midi channel to 
	// trigger the event
	//--------------------------------------
	CString csString;

	for (unsigned i = 0; i < 16; i++)
	{
		csString.Format(_T("%d"), i + 1);
		m_Combo_MidiChannel.InsertString(i, csString);
	}
	m_Combo_MidiChannel.SetCurSel(0);

}

void CDlgMidiInDispatchEditor::InitializeMidiStatusByteComboBox()
{
	//------------------------------------
	// Initialize the combo box that will
	// a selection of Midi Status words that
	// will trigger sending the message
	//--------------------------------------
	for (unsigned i = 0; i < DISPATCHEDIT__NUM_MIDI_CMDS; i++)
	{
		m_Combo_MidiStatusByte.InsertString(i, MidiStatusCommands[i].m_csName);
	}
	m_Combo_MidiStatusByte.SetCurSel(0);

}

void CDlgMidiInDispatchEditor::UpdateDialogFields()
{
	CString s;

//	printf("****** Update Dialog Fields %S ******\n", m_pMIMD->GetDispatchName().GetString());
	if (m_pMIMD)
	{
		if (m_pMIMD->DestWindowISset())
		{
//			printf("Destination Window is Set\n");
			m_Combo_DestinationWindow.SetCurSel(m_pMIMD->GetDestinationID());
			if (m_pMIMD->WMmessageISset())
			{
//				printf("WM_message is Set\n");
				m_Combo_WM_DispatchCommand.SetCurSel(m_pMIMD->GetWindowMessageID());
				EnableCommand(true);
				if (m_pMIMD->MidiStatusByteISset())
				{
//					printf("Midi Status is Set\n");
					m_Combo_MidiStatusByte.SetCurSel(m_pMIMD->GetMidiStatusID());
					EnableStatusByte(true);
					if (MidiStatusCommands[m_pMIMD->GetMidiStatusID()].m_Channel_flag)
					{
						EnableChannel(true);
					}
					UpdateDataEditBoxes(m_pMIMD->GetMidiStatusID());
				}
				else
				{
					EnableChannel(false);
					UpdateDataEditBoxes(-1);
				}
			}
			else
			{
				EnableStatusByte(false);
				EnableChannel(false);
				UpdateDataEditBoxes(-1);
			}
		}
		else
		{
			m_Combo_DestinationWindow.SetCurSel(-1);
			EnableCommand(false);
			EnableStatusByte(false);
			EnableChannel(false);
			UpdateDataEditBoxes(-1);
		}
		m_Edit_Name.SetWindowTextW(CString(m_pMIMD->GetDispatchName()));
	}
}


void CDlgMidiInDispatchEditor::OnBnClickedButtonPrevDispatch()
{
	if (m_pMIMD->GetPrev())
	{
		m_pMIMD = m_pMIMD->GetPrev();
		UpdateDialogFields();
		m_Edit_Name.SetFocus();
		if (m_pMIMD->GetPrev() == 0)
		{
			m_Button_Prev.EnableWindow(0);
			if (m_pMIMD->GetNext())
				m_Button_Next.EnableWindow(1);
			else
				m_Button_Next.EnableWindow(0);
		}
		else
		{
			m_Button_Prev.EnableWindow(1);
			if (m_pMIMD->GetNext())
				m_Button_Next.EnableWindow(1);
			else
				m_Button_Next.EnableWindow(0);

		}
	}
	else
	{
		m_Button_Prev.EnableWindow(0);
		if (m_pMIMD->GetNext())
			m_Button_Next.EnableWindow(1);
		else
			m_Button_Next.EnableWindow(0);
	}
}


void CDlgMidiInDispatchEditor::OnBnClickedButtonDeletedispatcher()
{
	CMidiInMessageDispatcher* pTemp;

	if (m_pMIMD->GetNext())
	{
		pTemp = m_pMIMD->GetNext();
	}
	else if (m_pMIMD->GetPrev())
		pTemp = m_pMIMD->GetPrev();
	else
		pTemp = NULL;
	GETAPP->GetMidiInTable()->GetMidiInDevice(m_DeviceID)->RemoveDispatcher(m_pMIMD);
	m_pMIMD = pTemp;
	UpdateDialogFields();;
}


void CDlgMidiInDispatchEditor::OnBnClickedButtonNewdispatcher()
{
	//----------------------------
	// Create a New Dispatcher
	// and add it to the Device
	//-----------------------------
	CreateANewDispatcher();
	m_Button_Prev.EnableWindow(1);
}


void CDlgMidiInDispatchEditor::OnBnClickedButtonNextDispatch()
{
	if (m_pMIMD->GetNext())
	{
		m_pMIMD = m_pMIMD->GetNext();
		UpdateDialogFields();
		m_Edit_Name.SetFocus();
		if (m_pMIMD->GetNext() == 0)
		{
			m_Button_Next.EnableWindow(0);
			if (m_pMIMD->GetPrev())
				m_Button_Prev.EnableWindow(1);
			else
				m_Button_Prev.EnableWindow(0);
		}
		else
		{
			m_Button_Next.EnableWindow(1);
			if (m_pMIMD->GetPrev())
				m_Button_Prev.EnableWindow(1);
			else
				m_Button_Prev.EnableWindow(0);
		}
	}
	else
	{
		m_Button_Next.EnableWindow(0);
		if (m_pMIMD->GetPrev())
			m_Button_Prev.EnableWindow(1);
		else
			m_Button_Prev.EnableWindow(0);
	}
}

//--------------- Destination Window Combo Box ----------------

void CDlgMidiInDispatchEditor::OnCbnSelchangeComboDestWindow()
{
	unsigned DestWindowID;
	unsigned n;

	DestWindowID = m_Combo_DestinationWindow.GetCurSel();
	m_pMIMD->SetDestinationWindow(m_ppChildViews[DestWindowID], DestWindowID);

	n = m_ppChildViews[DestWindowID]->GetNumOfMessageEntries();
	m_Combo_WM_DispatchCommand.ResetContent();
	for (size_t i = 0; i < n; i++)
	{
		m_Combo_WM_DispatchCommand.InsertString(
			i,
			m_ppChildViews[DestWindowID]->GetMessageName(i)
		);
	}
	m_Combo_WM_DispatchCommand.SetCurSel(0);
	EnableCommand(1);
	m_Combo_WM_DispatchCommand.SetFocus();
}

void CDlgMidiInDispatchEditor::OnCbnKillfocusComboDestWindow()
{
	unsigned DestWindowID;
	unsigned n;

	DestWindowID = m_Combo_DestinationWindow.GetCurSel();
	if (DestWindowID != CB_ERR)
	{
		m_pMIMD->SetDestinationWindow(m_ppChildViews[DestWindowID], DestWindowID);

		n = m_ppChildViews[DestWindowID]->GetNumOfMessageEntries();
		m_Combo_WM_DispatchCommand.ResetContent();
		for (size_t i = 0; i < n; i++)
		{
			m_Combo_WM_DispatchCommand.InsertString(
				i,
				m_ppChildViews[DestWindowID]->GetMessageName(i)
			);
		}
		m_Combo_WM_DispatchCommand.SetCurSel(0);
		EnableCommand(1);
	}
}

//-------------- WM<command> Combo Box --------------------

void CDlgMidiInDispatchEditor::OnCbnSelchangeComboDispatchCommand()
{
	unsigned  WM_messageID = m_Combo_WM_DispatchCommand.GetCurSel();
	if (CB_ERR != WM_messageID)
	{
		m_pMIMD->SetWindowMessageID(WM_messageID);
		EnableStatusByte(1);
		m_Combo_MidiStatusByte.SetFocus();
	}
}

void CDlgMidiInDispatchEditor::OnCbnKillfocusComboDispatchCommand()
{
	unsigned  WM_messageID = m_Combo_WM_DispatchCommand.GetCurSel();
	if (CB_ERR != WM_messageID)
	{
		m_pMIMD->SetWindowMessageID(WM_messageID);
		EnableStatusByte(1);
	}
}

//----------------- Midi Status Byte ComboBox ----------------------------

void CDlgMidiInDispatchEditor::OnCbnSelchangeComboDispatchStatus()
{
	int MidiStatusID = m_Combo_MidiStatusByte.GetCurSel();
	m_pMIMD->SetMidiStatusID(MidiStatusID);
	UpdateDataEditBoxes(MidiStatusID);
	if (MidiStatusCommands[MidiStatusID].m_Channel_flag)
	{
		EnableChannel(1);
		m_Combo_MidiChannel.SetFocus();
	}
	else
		EnableChannel(0);
}

void CDlgMidiInDispatchEditor::OnCbnKillfocusComboDispatchStatus()
{
	unsigned MidiStatusID = m_Combo_MidiStatusByte.GetCurSel();
	if (CB_ERR != MidiStatusID)
	{
		m_pMIMD->SetMidiStatusID(MidiStatusID);
		UpdateDataEditBoxes(MidiStatusID);
		if (MidiStatusCommands[MidiStatusID].m_Channel_flag)
			EnableChannel(1);
		else
			EnableChannel(0);
	}
}


//------------------ Midi Channel ComboBox --------------

void CDlgMidiInDispatchEditor::OnCbnSelchangeComboDispatchMiduchannel()
{
	m_pMIMD->SetMidiChannel(m_Combo_MidiChannel.GetCurSel());
}

void CDlgMidiInDispatchEditor::OnCbnKillfocusComboDispatchMiduchannel()
{
	m_pMIMD->SetMidiChannel(m_Combo_MidiChannel.GetCurSel());
}

//------------------ OK Button -----------------------
void CDlgMidiInDispatchEditor::OnOK()
{
	CString csString;

	CWnd* pW = GetFocus();
	int Id = pW->GetDlgCtrlID();
	switch (Id)
	{
	case IDC_EDIT_DISPATCH_DATAONE_HIGH:
	case IDC_EDIT_DISPATCH_DATAONE_LOW:
	case IDC_EDIT_DISPATCH_DATATWO_HIGH:
	case IDC_EDIT_DISPATCH_DATATWO_LOW:
		pW = GetNextDlgTabItem(pW);
		pW->SetFocus();
		break;
	case IDC_EDIT_DISPATCH_NAME:
		pW = GetNextDlgTabItem(pW);
		pW->SetFocus();
		break;
	default:
		CDialog::OnOK();
		break;
	}
}


void CDlgMidiInDispatchEditor::OnKillfocusEditDispatchDataoneHigh()
{
	CString csString;
	int v;

	m_Edit_Data1_High.GetWindowTextW(csString);
	v = _ttoi(csString);
	m_pMIMD->SetMidiData1Max(v);
}


void CDlgMidiInDispatchEditor::OnKillfocusEditDispatchDataoneLow()
{
	CString csString;
	int v;

	m_Edit_Data1_Low.GetWindowTextW(csString);
	v = _ttoi(csString);
	m_pMIMD->SetMidiData1Min(v);
}


void CDlgMidiInDispatchEditor::OnKillfocusEditDispatchDatatwoHigh()
{
	CString csString;
	int v;

	m_Edit_Data2_High.GetWindowTextW(csString);
	v = _ttoi(csString);
	m_pMIMD->SetData2Max(v);
}


void CDlgMidiInDispatchEditor::OnKillfocusEditDispatchDatatwoLow()
{
	CString csString;
	int v;

	m_Edit_Data2_Low.GetWindowTextW(csString);
	v = _ttoi(csString);
	m_pMIMD->SetMidiData2Min(v);
}


void CDlgMidiInDispatchEditor::OnKillfocusEditDispatchName()
{
	CString csString;

	m_Edit_Name.GetWindowTextW(csString);
	if (m_pMIMD)
		m_pMIMD->SetDispatchName(csString);
}

void CDlgMidiInDispatchEditor::UpdateDataEditBoxes(int statusID)
{
	CString s;

	if (statusID < 0)
	{
		//-------------------------------
		// if the MidiStatusID is less
		// than 0, then everything with
		// a couple of exceptions needs
		// to be disabled
		//--------------------------------
		m_Edit_Data1_Low.ShowWindow(0);
		m_Edit_Data1_High.ShowWindow(0);
		m_Static_Data1.ShowWindow(0);
		m_Edit_Data2_Low.ShowWindow(0);
		m_Edit_Data2_High.ShowWindow(0);
		m_Static_Data2.ShowWindow(0);
		m_static_HighValue.ShowWindow(0);
		m_Static_LowValue.ShowWindow(0);
	}
	else
	{
		//-----------------------------------
		// Check to see if the first data byte
		// is turned on for this status byte
		//-----------------------------------
		if (MidiStatusCommands[statusID].m_Data1_flag)
		{
			m_Edit_Data1_Low.ShowWindow(1);
			m_Edit_Data1_High.ShowWindow(1);
			s.Format(_T("%d"), m_pMIMD->GetMidiData1Max());
			m_Edit_Data1_High.SetWindowTextW(s);
			s.Format(_T("%d"), m_pMIMD->GetMidiData1Min());
			m_Edit_Data1_Low.SetWindowTextW(s);

			m_Static_Data1.SetWindowTextW(MidiStatusCommands[statusID].m_csData1);
			m_Static_Data1.ShowWindow(1);

			m_static_HighValue.ShowWindow(1);
			m_Static_LowValue.ShowWindow(1);
			//----------------------------------------
			// Check to see if data byte #2 is turned
			// on
			//-----------------------------------------
			if (MidiStatusCommands[statusID].m_Data2_flag)
			{
				// show Data2 field
				m_Edit_Data2_Low.ShowWindow(1);
				m_Edit_Data2_High.ShowWindow(1);
				s.Format(_T("%d"), m_pMIMD->GetMidiData2Min());
				m_Edit_Data2_Low.SetWindowTextW(s);
				s.Format(_T("%d"), m_pMIMD->GetMidiData2Max());
				m_Edit_Data2_High.SetWindowTextW(s);

				m_Static_Data2.SetWindowTextW(MidiStatusCommands[statusID].m_csData2);
				m_Static_Data2.ShowWindow(1);
			}
			else
			{
				m_Static_Data2.ShowWindow(0);
				m_Edit_Data2_Low.ShowWindow(0);
				m_Edit_Data2_High.ShowWindow(0);

			}
		}
		else
		{
//			printf("Everybody Off\n");
			//-------------------------------
			// Data byte 1 is not turned on
			// so everybody is OFF
			//--------------------------------
			m_Edit_Data1_Low.ShowWindow(0);
			m_Edit_Data1_High.ShowWindow(0);
			m_Edit_Data2_Low.ShowWindow(0);
			m_Edit_Data2_High.ShowWindow(0);

			m_Static_Data1.ShowWindow(0);
			m_Static_Data2.ShowWindow(0);

			m_static_HighValue.ShowWindow(0);
			m_Static_LowValue.ShowWindow(0);
		}
	}
}

void CDlgMidiInDispatchEditor::EnableCommand(int enable)
{
	m_static_Command.ShowWindow(enable);
	m_Combo_WM_DispatchCommand.ShowWindow(enable);

	unsigned DestWindowID = m_pMIMD->GetDestinationID();
	unsigned n = m_ppChildViews[DestWindowID]->GetNumOfMessageEntries();
	m_Combo_WM_DispatchCommand.ResetContent();
	for (unsigned i = 0; i < n; i++)
	{
		m_Combo_WM_DispatchCommand.InsertString(
			i,
			m_ppChildViews[DestWindowID]->GetMessageName(i)
		);
	}
	m_Combo_WM_DispatchCommand.SetCurSel(0);
//	EnableCommand(1);
}

void CDlgMidiInDispatchEditor::EnableChannel(int enable)
{
	m_static_Channel.ShowWindow(enable);
	m_Combo_MidiChannel.ShowWindow(enable);
}

void CDlgMidiInDispatchEditor::EnableStatusByte(int enable)
{
	m_static_StatusByte.ShowWindow(enable);
	m_Combo_MidiStatusByte.ShowWindow(enable);
}

void CDlgMidiInDispatchEditor::CreateANewDispatcher()
{
	//--------------------------------
	// if there are NO dispatchers
	// create one to start things off
	//---------------------------------
	m_pMIMD = new CMidiInMessageDispatcher;
//	memset(m_pMIMD, 0, sizeof(CMidiInMessageDispatcher));
	m_pMIMD->SetDispatchName(CString("Enter a Name Here"));
	m_Edit_Name.SetWindowTextW(CString(m_pMIMD->GetDispatchName()));
	GETAPP->GetMidiInTable()->GetMidiInDevice(m_DeviceID)->AddDispatcherToTail(m_pMIMD);
	//-----------------------------------------
	// Disable controls that need to be diabled
	//---------------------------------------------
	m_Button_Next.EnableWindow(0);
	m_Button_Prev.EnableWindow(0);
	UpdateDialogFields();
	EnableCommand(0);
	EnableChannel(0);
	EnableStatusByte(0);
	UpdateDataEditBoxes(-1);
}