#include "stdafx.h"
#include "InspectorDialogue.h"
#include "SceneObject.h"
#include <codecvt>
#include "DisplayObject.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


IMPLEMENT_DYNAMIC(InspectorDialogue, CDialogEx)

BEGIN_MESSAGE_MAP(InspectorDialogue, CDialogEx)
	ON_BN_CLICKED(ID_BUTTON_APPLY, &InspectorDialogue::OnBnClickedButtonApply)
	//ON_BN_CLICKED(ID_BUTTON_DISCARD_CHANGES, &InspectorDialogue::OnBnClickedButtonDiscardChanges)
	ON_BN_CLICKED(IDC_BUTTON_X_Down, &InspectorDialogue::OnEnChangeEditPosXDown)
	ON_BN_CLICKED(IDC_BUTTON_X_UP, &InspectorDialogue::OnEnChangeEditPosXUp)

	ON_BN_CLICKED(IDC_BUTTON_Y_DOWN, &InspectorDialogue::OnEnChangeEditPosYDown)
	ON_BN_CLICKED(IDC_BUTTON_Y_UP, &InspectorDialogue::OnEnChangeEditPosYUp)

	ON_BN_CLICKED(IDC_BUTTON_Z_DOWN, &InspectorDialogue::OnEnChangeEditPosZDown)
	ON_BN_CLICKED(IDC_BUTTON_Z_UP, &InspectorDialogue::OnEnChangeEditPosZUp)

	ON_BN_CLICKED(IDC_BUTTON_ROTX_DOWN, &InspectorDialogue::OnEnChangeEditRotXDown)
	ON_BN_CLICKED(IDC_BUTTON_ROTX_UP, &InspectorDialogue::OnEnChangeEditRotXUp)

	ON_BN_CLICKED(IDC_BUTTON_ROTY_DOWN, &InspectorDialogue::OnEnChangeEditRotYDown)
	ON_BN_CLICKED(IDC_BUTTON_ROTY_up, &InspectorDialogue::OnEnChangeEditRotYUp)

	ON_BN_CLICKED(IDC_BUTTON_ROTZ_DOWN, &InspectorDialogue::OnEnChangeEditRotZDown)
	ON_BN_CLICKED(IDC_BUTTON_ROTZ_UP, &InspectorDialogue::OnEnChangeEditRotZUp)

	ON_BN_CLICKED(IDC_BUTTON_SCALEX_DOWN, &InspectorDialogue::OnEnChangeEditScaleXDown)
	ON_BN_CLICKED(IDC_BUTTON_SCALEX_UP, &InspectorDialogue::OnEnChangeEditScaleXUp)

	ON_BN_CLICKED(IDC_BUTTON_SCALEY_DOWN, &InspectorDialogue::OnEnChangeEditScaleYDown)
	ON_BN_CLICKED(IDC_BUTTON_SCALEY_UP, &InspectorDialogue::OnEnChangeEditScaleYUp)

	ON_BN_CLICKED(IDC_BUTTON_SCALEZ_DOWN, &InspectorDialogue::OnEnChangeEditScaleZDown)
	ON_BN_CLICKED(IDC_BUTTON_SCALEZ_UP, &InspectorDialogue::OnEnChangeEditScaleZUp)
END_MESSAGE_MAP()


InspectorDialogue::InspectorDialogue(CWnd* pParent /*= nullptr*/)
{
	madeChange = false;
	//// #TODO
	//if (pParent)
	//{
	//	SetParent(pParent);
	//}SceneObject
}


InspectorDialogue::~InspectorDialogue()
{
	// #TODO
}


void InspectorDialogue::LoadSelectedObjectData(SceneObject* _selectedObject)
{
	if (m_SelectedObject)
	{
		// Fill inspector with selected object data
		//m_SelectedObject = _selectedObject;

		UpdateData(*m_SelectedObject);
	}
	else
	{
	//	Debug::OnScreenMessages.AddMessage(L"Cannot load selected object data. Selected object is null!", 3, MessageColor::Error);
	}
}


void InspectorDialogue::UpdateFromCurrentSelection()
{
	if (m_SelectedObject)
	{
		UpdateData(*m_SelectedObject);
	}
	else
	{
		//Debug::OnScreenMessages.AddMessage(L"Inspector data cannot be loaded. No object is selected.", 5, MessageColor::Error);
	}
}


void InspectorDialogue::Update(CRect _mainWindowRect)
{
	SetWindowPos(nullptr, _mainWindowRect.right, _mainWindowRect.top, m_Width, m_Height, 0);

	//if (GameInput::Get().keyboardTracker.IsKeyPressed(Keyboard::Enter))
	//if (Keyboard::GetState)
	//{
	//	CButton* applyButton = (CButton*)GetDlgItem(ID_BUTTON_APPLY);
	//	GotoDlgCtrl(applyButton);

	//	// #TODO: Deselect current edit control, so input is stopped
	//	OnBnClickedButtonApply();
	//}
}

void InspectorDialogue::UpdateData(SceneObject& _selectedObject)
{
	SetEditFloat(m_PosX, _selectedObject.posX);
	SetEditFloat(m_PosY, _selectedObject.posY);
	SetEditFloat(m_PosZ, _selectedObject.posZ);

	SetEditFloat(m_RotX, _selectedObject.rotX);
	SetEditFloat(m_RotY, _selectedObject.rotY);
	SetEditFloat(m_RotZ, _selectedObject.rotZ);

	SetEditFloat(m_ScaleX, _selectedObject.scaX);
	SetEditFloat(m_ScaleY, _selectedObject.scaY);
	SetEditFloat(m_ScaleZ, _selectedObject.scaZ);
}


float InspectorDialogue::ValidateEditFloat(CEdit& _edit)
{
	CString text;
	_edit.GetWindowText(text);
	bool firstDot = true;
	std::wstring floatText = L"";

	// Strip out all non-digit characters, except the first dot
	for (auto& c : std::wstring(text.GetString()))
	{
		switch (c)
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '-': // Leave for now
			floatText += c;
			break;

		case '.':
			if (firstDot)
			{
				// Keep first dot
				firstDot = false;
				floatText += c;
			}
			break;

		default:
			// Omit any other character
			break;
		}
	}

	float sign = 1.0f;

	if (floatText.empty())
	{
		floatText = L"0";
	}
	else
	{
		if (floatText[0] == '-')
		{
			sign = -1.0f;
		}

		// Remove '-' characters
		for (auto& it = floatText.begin(); it != floatText.end(); )
		{
			if (*it == '-')
			{
				it = floatText.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	try
	{
		float floatValue = std::stof(floatText);
		text = std::to_wstring(floatValue).c_str();
		_edit.SetWindowText(text);
		return sign * floatValue;
	}
	catch (std::exception& e)
	{
		std::wstring msg = L"";

		std::string errorMsg = e.what();
		for (auto& c : errorMsg)
		{
			msg += c;
		}

		//Debug::OnScreenMessages.AddMessage(msg, 3, MessageColor::Error);
	}

	return -1;
}


void InspectorDialogue::SetEditFloat(CEdit& _edit, float _value)
{
	CString text;
	text.Format(_T("%f"), _value);
	_edit.SetWindowText(text);
}


float InspectorDialogue::GetEditFloat(CEdit& _edit)
{
	CString text;
	return -1;
	// #TODO: GetEditFloat()
}


BOOL InspectorDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rect;
	GetWindowRect(&rect);
	m_Width = rect.Width();
	m_Height = rect.Height();

	return TRUE;
}


void InspectorDialogue::PostNcDestroy()
{
	// #TODO
	m_SelectedObject = nullptr;
	//Debug::OnScreenMessages.AddMessage(L"PostNcDestroy()", 3, MessageColor::Warning);
}


void InspectorDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_POS_X, m_PosX);
	DDX_Control(pDX, IDC_EDIT_POS_Y, m_PosY);
	DDX_Control(pDX, IDC_EDIT_POS_Z, m_PosZ);
	DDX_Control(pDX, IDC_EDIT_ROT_X, m_RotX);
	DDX_Control(pDX, IDC_EDIT_ROT_Y, m_RotY);
	DDX_Control(pDX, IDC_EDIT_ROT_Z, m_RotZ);
	DDX_Control(pDX, IDC_EDIT_SCALE_X, m_ScaleX);
	DDX_Control(pDX, IDC_EDIT_SCALE_Y, m_ScaleY);
	DDX_Control(pDX, IDC_EDIT_SCALE_Z, m_ScaleZ);
}


void InspectorDialogue::OnBnClickedButtonApply()
{
	// #TODO: Apply changes
	if (m_SelectedObject)
	{

		Vector3 position = {
			ValidateEditFloat(m_PosX),
			ValidateEditFloat(m_PosY),
			ValidateEditFloat(m_PosZ)
		};

		Vector3 rotation = {
			ValidateEditFloat(m_RotX),
			ValidateEditFloat(m_RotY),
			ValidateEditFloat(m_RotZ)
		};

		Vector3 scale = {
			ValidateEditFloat(m_ScaleX),
			ValidateEditFloat(m_ScaleY),
			ValidateEditFloat(m_ScaleZ)
		};

		m_SelectedObject->posX = position.x;
		m_SelectedObject->posY = position.y;
		m_SelectedObject->posZ = position.z;
		m_SelectedObject->rotX = rotation.x;
		m_SelectedObject->rotY = rotation.y;
		m_SelectedObject->rotZ = rotation.z;
		m_SelectedObject->scaX = scale.x;
		m_SelectedObject->scaY = scale.y;
		m_SelectedObject->scaZ = scale.z;

		//Debug::OnScreenMessages.AddMessage(L"Applied changes", 1, MessageColor::Success);
	}
	else
	{
		//Debug::OnScreenMessages.AddMessage(L"No selected object. Changes could not be applied.", 1, MessageColor::Error);
	}
}


void InspectorDialogue::OnBnClickedButtonDiscardChanges()
{
	// #TODO: Discard changes
	UpdateFromCurrentSelection();
	//Debug::OnScreenMessages.AddMessage(L"Discarded changes", 1, MessageColor::Warning);
}

void InspectorDialogue::OnEnChangeEditPosXUp()
{
	madeChange = true;
	m_SelectedObject->posX += 1;
	
}

void InspectorDialogue::OnEnChangeEditPosXDown()
{
	madeChange = true;
	m_SelectedObject->posX -= 1;
}

void InspectorDialogue::OnEnChangeEditPosYUp()
{
	madeChange = true;
	m_SelectedObject->posY += 1;
}

void InspectorDialogue::OnEnChangeEditPosYDown()
{
	madeChange = true;
	m_SelectedObject->posY -= 1;
}

void InspectorDialogue::OnEnChangeEditPosZUp()
{
	madeChange = true;
	m_SelectedObject->posZ += 1;
}

void InspectorDialogue::OnEnChangeEditPosZDown()
{
	madeChange = true;
	m_SelectedObject->posZ -= 1;
}

void InspectorDialogue::OnEnChangeEditRotXUp()
{
	madeChange = true;
	m_SelectedObject->rotX += 1;
}

void InspectorDialogue::OnEnChangeEditRotXDown()
{
	madeChange = true;
	m_SelectedObject->rotX -= 1;
}

void InspectorDialogue::OnEnChangeEditRotYUp()
{
	madeChange = true;
	m_SelectedObject->rotY += 1;
}

void InspectorDialogue::OnEnChangeEditRotYDown()
{
	madeChange = true;
	m_SelectedObject->rotY -= 1;
}

void InspectorDialogue::OnEnChangeEditRotZUp()
{
	madeChange = true;
	m_SelectedObject->rotZ += 1;

}

void InspectorDialogue::OnEnChangeEditRotZDown()
{
	madeChange = true;
	m_SelectedObject->rotZ -= 1;
}

void InspectorDialogue::OnEnChangeEditScaleXUp()
{
	madeChange = true;
	m_SelectedObject->scaX += 1;
}

void InspectorDialogue::OnEnChangeEditScaleXDown()
{
	madeChange = true;
	m_SelectedObject->scaX -= 1;
}

void InspectorDialogue::OnEnChangeEditScaleYUp()
{
	madeChange = true;
	m_SelectedObject->scaY += 1;
}

void InspectorDialogue::OnEnChangeEditScaleYDown()
{
	madeChange = true;
	m_SelectedObject->scaY -= 1;
}

void InspectorDialogue::OnEnChangeEditScaleZUp()
{
	madeChange = true;
	m_SelectedObject->scaZ += 1;
}

void InspectorDialogue::OnEnChangeEditScaleZDown()
{
	madeChange = true;
	m_SelectedObject->scaZ -= 1;
}

