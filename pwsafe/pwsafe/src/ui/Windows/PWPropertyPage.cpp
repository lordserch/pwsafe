/*
* Copyright (c) 2003-2013 Rony Shapiro <ronys@users.sourceforge.net>.
* All rights reserved. Use of the code is allowed under the
* Artistic License 2.0 terms, as specified in the LICENSE file
* distributed with this code, or available from
* http://www.opensource.org/licenses/artistic-license-2.0.php
*/

#include "DboxMain.h"
#include "PWPropertyPage.h"
#include "GeneralMsgBox.h"

#if defined(POCKET_PC)
#error "TBD - define proper PropertyPage base class for PPC"
#endif

extern const wchar_t *EYE_CATCHER;

IMPLEMENT_DYNAMIC(CPWPropertyPage, CPropertyPage)

CPWPropertyPage::CPWPropertyPage(UINT nID)
: CPropertyPage(nID), m_pToolTipCtrl(NULL)
{
  m_psp.dwFlags |= PSP_HASHELP;
}

void CPWPropertyPage::InitToolTip(int Flags, int delayTimeFactor)
{
  m_pToolTipCtrl = new CToolTipCtrl;
  if (!m_pToolTipCtrl->Create(this, Flags)) {
    pws_os::Trace(L"Unable To create ToolTip\n");
    delete m_pToolTipCtrl;
    m_pToolTipCtrl = NULL;
  } else {
    EnableToolTips();
    // Delay initial show & reshow
    int iTime = m_pToolTipCtrl->GetDelayTime(TTDT_AUTOPOP);
    m_pToolTipCtrl->SetDelayTime(TTDT_INITIAL, iTime);
    m_pToolTipCtrl->SetDelayTime(TTDT_RESHOW, iTime);
    m_pToolTipCtrl->SetDelayTime(TTDT_AUTOPOP, iTime * delayTimeFactor);
    m_pToolTipCtrl->SetMaxTipWidth(300);
  }
}

void CPWPropertyPage::AddTool(int DlgItemID, int ResID)
{
  if (m_pToolTipCtrl != NULL) {
    const CString cs(MAKEINTRESOURCE(ResID));
    m_pToolTipCtrl->AddTool(GetDlgItem(DlgItemID), cs);
  }
}

void CPWPropertyPage::ActivateToolTip()
{
  if (m_pToolTipCtrl != NULL)
    m_pToolTipCtrl->Activate(TRUE);
}

void CPWPropertyPage::RelayToolTipEvent(MSG *pMsg)
{
  if (m_pToolTipCtrl != NULL)
    m_pToolTipCtrl->RelayEvent(pMsg);
}

LRESULT CPWPropertyPage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
  CWnd *pParent = GetParent();
  while (pParent != NULL) {
    DboxMain *pDbx = dynamic_cast<DboxMain *>(pParent);
    if (pDbx != NULL && pDbx->m_eye_catcher != NULL &&
        wcscmp(pDbx->m_eye_catcher, EYE_CATCHER) == 0) {
      pDbx->ResetIdleLockCounter(message);
      break;
    } else
      pParent = pParent->GetParent();
  }
  if (pParent == NULL)
    pws_os::Trace(L"CPWPropertyPage::WindowProc - couldn't find DboxMain ancestor\n");

  return CPropertyPage::WindowProc(message, wParam, lParam);
}

