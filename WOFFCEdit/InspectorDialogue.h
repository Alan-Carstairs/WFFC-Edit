#pragma once
#include <afxdialogex.h>
#include <afxpropertygridctrl.h>
#include "resource.h"
#include "afxwin.h"
#include "SceneObject.h"
#include <memory>
#include <xstring>

class SceneObject;


class InspectorDialogue :
	public CDialogEx
{
	DECLARE_DYNAMIC(InspectorDialogue)

public:
	InspectorDialogue(CWnd* pParent = nullptr);
	virtual ~InspectorDialogue();
	void LoadSelectedObjectData(SceneObject* _selectedObject);
	void UpdateFromCurrentSelection();
	const int GetWidth() const { return m_Width; }
	const int GetHeight() const { return m_Height; }
	void Update(CRect _mainWindowRect);
	void setSelectedObj(SceneObject* _selectedObject) { m_SelectedObject = _selectedObject; }
	bool madeChange;
	// Inspector Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSPECTOR };
#endif


private:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Update the date in the edit controls
	void UpdateData(SceneObject& _selectedObject);

	float ValidateEditFloat(CEdit& _edit);
	void SetEditFloat(CEdit& _edit, float _value);
	float GetEditFloat(CEdit& _edit);
	int m_Width;
	int m_Height;

	SceneObject* m_SelectedObject = nullptr;
	CEdit m_PosX;
	CEdit m_PosY;
	CEdit m_PosZ;
	CEdit m_RotX;
	CEdit m_RotY;
	CEdit m_RotZ;
	CEdit m_ScaleX;
	CEdit m_ScaleY;
	CEdit m_ScaleZ;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog() override;
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonDiscardChanges();
	#
	afx_msg void OnEnChangeEditPosXUp();
	afx_msg void OnEnChangeEditPosXDown();
	
	afx_msg void OnEnChangeEditPosYUp();
	afx_msg void OnEnChangeEditPosYDown();
	
	afx_msg void OnEnChangeEditPosZUp();
	afx_msg void OnEnChangeEditPosZDown();

	afx_msg void OnEnChangeEditRotXUp();
	afx_msg void OnEnChangeEditRotXDown();
	
	afx_msg void OnEnChangeEditRotYUp();
	afx_msg void OnEnChangeEditRotYDown();

	afx_msg void OnEnChangeEditRotZUp();
	afx_msg void OnEnChangeEditRotZDown();
	
	afx_msg void OnEnChangeEditScaleXUp();
	afx_msg void OnEnChangeEditScaleXDown();

	afx_msg void OnEnChangeEditScaleYUp();
	afx_msg void OnEnChangeEditScaleYDown();


	afx_msg void OnEnChangeEditScaleZUp();
	afx_msg void OnEnChangeEditScaleZDown();

	afx_msg void OnBnClickedButtonZUp();
};

INT_PTR CALLBACK InspectorProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);