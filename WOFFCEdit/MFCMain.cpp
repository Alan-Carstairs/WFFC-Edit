#include "MFCMain.h"
#include "resource.h"
#include "InspectorDialogue.h"


BEGIN_MESSAGE_MAP(MFCMain, CWinApp)
	ON_COMMAND(ID_FILE_QUIT,	&MFCMain::MenuFileQuit)
	ON_COMMAND(ID_FILE_SAVETERRAIN, &MFCMain::MenuFileSaveTerrain)
	ON_COMMAND(ID_EDIT_SELECT, &MFCMain::MenuEditSelect)
	ON_COMMAND(ID_BUTTON40001,	&MFCMain::ToolBarButton1)
	ON_COMMAND(ID_FUNCTIONS_TERRAINSCULPTING, &MFCMain::FunctionTerrainSculpt)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TOOL, &CMyFrame::OnUpdatePage)
	ON_COMMAND(ID_INSPECTOR_OBJECTINSPECTOR, &MFCMain::InspectorFunction)
	
END_MESSAGE_MAP()

BOOL MFCMain::InitInstance()
{
	//instanciate the mfc frame
	m_frame = new CMyFrame();
	m_pMainWnd = m_frame;

	m_frame->Create(	NULL,
					_T("World Of Flim-Flam Craft Editor"),
					WS_OVERLAPPEDWINDOW,
					CRect(100, 100, 1024, 768),
					NULL,
					NULL,
					0,
					NULL
				);

	//show and set the window to run and update. 
	m_frame->ShowWindow(SW_SHOW);
	m_frame->UpdateWindow();


	//get the rect from the MFC window so we can get its dimensions
	m_toolHandle = m_frame->m_DirXView.GetSafeHwnd();				//handle of directX child window
	m_frame->m_DirXView.GetClientRect(&WindowRECT);
	m_width		= WindowRECT.Width();
	m_height	= WindowRECT.Height();

	m_ToolSystem.onActionInitialise(m_toolHandle, m_width, m_height);


	m_ToolInspectorDialogue = std::make_unique<InspectorDialogue>();
	m_ToolInspectorDialogue->Create(IDD_INSPECTOR, CWnd::FromHandle(m_toolHandle));
	m_ToolInspectorDialogue->ShowWindow(SW_HIDE);


	return TRUE;
}

int MFCMain::Run()
{
	//MSG msg;
	//BOOL bGotMsg;

	//PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	//while (WM_QUIT != msg.message)
	//{
	//	if (true)
	//	{
	//		bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);
	//	}
	//	else
	//	{
	//		bGotMsg = (GetMessage(&msg, NULL, 0U, 0U) != 0);
	//	}

	//		   
	//	if (bGotMsg)
	//	{
	//		switch (msg.message)
	//		{
	//			//////////////////////////////////////////////////////////////////////////
	//			// Handle custom messages
	//			//////////////////////////////////////////////////////////////////////////
	//		case WM_SELECTION_NEW:
	//			m_ToolInspectorDialogue->ShowWindow(SW_SHOW);
	//			//if (m_ToolInspectorDialogue)
	//			//{
	//			//	m_ToolInspectorDialogue->LoadSelectedObjectData(NULL/*((SelectedObject*)msg.lParam)->object*/);
	//			//}
	//			break;

	//		case WM_SELECTION_ADDED:
	//			break;

	//		case WM_SELECTION_REMOVED:
	//			break;

	//		case WM_SELECTION_OVER:
	//			m_ToolInspectorDialogue->ShowWindow(SW_HIDE);
	//			break;

	//		case WM_OBJECT_EDITED:
	//			if (m_ToolInspectorDialogue)
	//			{
	//				m_ToolInspectorDialogue->UpdateFromCurrentSelection();
	//			}
	//			break;

	//			//////////////////////////////////////////////////////////////////////////
	//			// Handle window messages
	//			//////////////////////////////////////////////////////////////////////////
	//		default:
	//			TranslateMessage(&msg);
	//			DispatchMessage(&msg);
	//			m_ToolSystem.UpdateInput(&msg);
	//			break;
	//		}
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//		m_ToolSystem.UpdateInput(&msg);
	//	}
	MSG msg;
	BOOL bGotMsg;

	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	while (WM_QUIT != msg.message)
	{
		if (true)
		{
			bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);
		}
		else
		{
			bGotMsg = (GetMessage(&msg, NULL, 0U, 0U) != 0);
		}

		if (bGotMsg)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			m_ToolSystem.UpdateInput(&msg);
		}
		else
		{	
			
			if (m_ToolSystem.m_sceneGraph.size() > 0 && m_ToolSystem.m_selectedObjectVector.size() > 0 && m_ToolInspectorDialogue)
			{
				m_ToolInspectorDialogue->setSelectedObj(&m_ToolSystem.m_sceneGraph[m_ToolSystem.m_selectedObjectVector.back()]);
				m_ToolInspectorDialogue->LoadSelectedObjectData(NULL/*((SelectedObject*)msg.lParam)->object)*/);

				if (m_ToolInspectorDialogue->madeChange)
				{
					m_ToolInspectorDialogue->madeChange = false;
					m_ToolSystem.madeChange = true;
				}
			}
			/*int ID = m_ToolSystem.getCurrentSelectionID();
			std::wstring statusString = L"Selected Object: " + std::to_wstring(ID);*/



			int ID = 0;


			std::wstring statusString = L"Selected Object: ";
			std::wstring appendString = L", ";
			//for (int ID : m_ToolSystem.getCurrentSelectionID()) {
				//int ID = m_ToolSystem.getCurrentSelectionID();
			if (m_ToolSystem.getCurrentSelectionVectorID().size() > 0) {
				for (int ID : m_ToolSystem.getCurrentSelectionVectorID())
					statusString += std::to_wstring(ID) + appendString;
			
				CRect mainWindowRect;
				GetWindowRect(m_toolHandle, &mainWindowRect);
				m_ToolInspectorDialogue->Update(mainWindowRect);

				m_ToolSystem.Tick(&msg);
				//send current object ID to status bar in The main frame
				m_frame->m_wndStatusBar.SetPaneText(1, statusString.c_str(), 1);
			}
			else
			{
				std::wstring statusString = L"Selected Object: " + std::to_wstring(m_ToolSystem.getCurrentSelectionVectorID().size());
				m_ToolSystem.Tick(&msg);

			//send current object ID to status bar in The main frame
			m_frame->m_wndStatusBar.SetPaneText(1, statusString.c_str(), 1);	
			}
			
		}
	}

	return (int)msg.wParam;
}

void MFCMain::MenuFileQuit()
{
	//will post message to the message thread that will exit the application normally
	PostQuitMessage(0);
}

void MFCMain::MenuFileSaveTerrain()
{
	m_ToolSystem.onActionSaveTerrain();
}

void MFCMain::MenuEditSelect()
{
	//SelectDialogue m_ToolSelectDialogue(NULL, &m_ToolSystem.m_sceneGraph);		//create our dialoguebox //modal constructor
	//m_ToolSelectDialogue.DoModal();	// start it up modal

	//modeless dialogue must be declared in the class.   If we do local it will go out of scope instantly and destroy itself
	m_ToolSelectDialogue.Create(IDD_DIALOG1);	//Start up modeless
	
	m_ToolSelectDialogue.ShowWindow(SW_SHOW);	//show modeless
	m_ToolSelectDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph, &m_ToolSystem.m_selectedObject);
}

void MFCMain::ToolBarButton1()
{
	
	m_ToolSystem.onActionSave();
}

void MFCMain::FunctionTerrainSculpt()
{
	m_ToolSystem.SculptFunc = !m_ToolSystem.SculptFunc;
}

void MFCMain::InspectorFunction()
{
	//m_ToolInspectorDialogue->Create(IDD_INSPECTOR);	//Start up modeless
	m_ToolInspectorDialogue->EnableWindow();
	m_ToolInspectorDialogue->ShowWindow(SW_SHOW);	//show modeless
	
	
	//m_ToolSelectDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph, &m_ToolSystem.m_selectedObject);
}

MFCMain::MFCMain()
{
}


MFCMain::~MFCMain()
{
}
