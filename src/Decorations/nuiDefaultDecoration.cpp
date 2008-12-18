/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiDefaultDecoration.h"
#include "nuiColorDecoration.h"
#include "nuiGradientDecoration.h"
#include "nuiBorderDecoration.h"
#include "nuiMetaDecoration.h"
#include "../graphics/DefaultDecoration.h"
#include "nuiFrame.h"
#include "nuiMessageBox.h"
#include "nuiTabView.h"
#include "nuiBackgroundPane.h"
#include "nuiKnob.h"


nuiDefaultDecoration::nuiDefaultDecoration()
{
  mpKnobSequence = NULL;
}


nuiDefaultDecoration::~nuiDefaultDecoration()
{
  
} 


//**************************************************************************************************************
//
// Init
//
// connect the object class to the decoration method
//
void nuiDefaultDecoration::Init()
{

  InitColors();
  InitSelection();
  InitMaps();
  InitIcons();
    
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiWindow")), &nuiDefaultDecoration::Window);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiBackgroundPane")), &nuiDefaultDecoration::BackgroundPane);

  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiLabel")), &nuiDefaultDecoration::Label);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiEditText")), &nuiDefaultDecoration::EditText);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiEditLine")), &nuiDefaultDecoration::EditLine);
  
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiSplitterHandle")), &nuiDefaultDecoration::SplitterHandle);
  
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiTitledPane")), &nuiDefaultDecoration::TitledPane);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiFolderPane")), &nuiDefaultDecoration::FolderPane);
  
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiButton")), &nuiDefaultDecoration::Button);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiRadioButton")), &nuiDefaultDecoration::RadioButton);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiCloseButton")), &nuiDefaultDecoration::CloseButton);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiToggleButton")), &nuiDefaultDecoration::ToggleButton);
  
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiKnob")), &nuiDefaultDecoration::KnobSequence);

  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiComboBox")), &nuiDefaultDecoration::ComboBox);

  // dialogs
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiDialog::Title")), &nuiDefaultDecoration::Dialog_Title);
  
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiDialog::EditLine")), &nuiDefaultDecoration::Dialog_EditLine);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiMessageBox::Title")), &nuiDefaultDecoration::MessageBox_Title);
  
  
  // FileSelector
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiFileSelector::FolderView")), &nuiDefaultDecoration::FileSelector_FolderView);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiFileSelector::InfoView")), &nuiDefaultDecoration::FileSelector_InfoView);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiFileSelector::FolderLabel")), &nuiDefaultDecoration::FileSelector_FolderLabel);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiFileSelector::VolumeIcon")), &nuiDefaultDecoration::FileSelector_VolumeIcon);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiFileSelector::FolderIcon")), &nuiDefaultDecoration::FileSelector_FolderIcon);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiFileSelector::TreeView")), &nuiDefaultDecoration::FileSelector_TreeView);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiFileSelector::TreeFolderLabel")), &nuiDefaultDecoration::FileSelector_TreeFolderLabel);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiFileSelector::TreeFileLabel")), &nuiDefaultDecoration::FileSelector_TreeFileLabel);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiFileSelector::TreeFolderIcon")), &nuiDefaultDecoration::FileSelector_TreeFolderIcon);
  nuiWidget::SetDefaultDecoration(nuiObject::GetClassNameIndex(_T("nuiFileSelector::TreeFileIcon")), &nuiDefaultDecoration::FileSelector_TreeFileIcon);
}





//**************************************************************************************************************
//
// InitColors
//
void nuiDefaultDecoration::InitColors()
{
  nuiColor::SetColor(_T("nuiDefaultClrWindowBkg1"), nuiColor(225,230,235));
  nuiColor::SetColor(_T("nuiDefaultClrWindowBkg2"), nuiColor(255,255,255));

  nuiColor::SetColor(_T("nuiDefaultClrCaptionBkg1"), nuiColor(197,197,197));
  nuiColor::SetColor(_T("nuiDefaultClrCaptionBkg2"), nuiColor(150,150,150));
  
  nuiColor::SetColor(_T("nuiDefaultClrInactiveCaptionBkg1"), nuiColor(157,157,157));
  nuiColor::SetColor(_T("nuiDefaultClrInactiveCaptionBkg2"), nuiColor(110,110,110));
  

  nuiColor::SetColor(_T("nuiDefaultClrCaptionBorder"), nuiColor(150,150,150));
  nuiColor::SetColor(_T("nuiDefaultClrCaptionBorderLight"), nuiColor(238,238,238));
  nuiColor::SetColor(_T("nuiDefaultClrCaptionBorderDark"), nuiColor(64,64,64));
  nuiColor::SetColor(_T("nuiDefaultClrInactiveCaptionBorderLight"), nuiColor(198,198,198));
  
  nuiColor::SetColor(_T("nuiDefaultClrCaptionText"), nuiColor(16,16,16));
  nuiColor::SetColor(_T("nuiDefaultClrCaptionTextLight"), nuiColor(215,215,215));
  nuiColor::SetColor(_T("nuiDefaultClrInactiveCaptionTextLight"), nuiColor(175,175,175));
  
  nuiColor::SetColor(_T("nuiDefaultClrSelection"), nuiColor(56,117,215));

  nuiColor::SetColor(_T("nuiDefaultClrNormalTab"), nuiColor(64,64,64));
  nuiColor::SetColor(_T("nuiDefaultClrSelectedTab"), nuiColor(32,32,32));

  nuiColor::SetColor(_T("nuiDefaultClrProgressFg"), nuiColor(76,145,230));
  nuiColor::SetColor(_T("nuiDefaultClrProgressBg"), nuiColor(200,200,200));


}




//**************************************************************************************************************
//
// InitSelection
//
void nuiDefaultDecoration::InitSelection()
{
  nuiColor color;
  nuiColor::GetColor(_T("nuiDefaultClrSelection"), color);
  
  nuiColorDecoration* pDeco = new nuiColorDecoration(_T("nuiDefaultDecorationSelectionBackground"), 
                                                     nuiRect(3,3,0,0), color);
}




//**************************************************************************************************************
//
// InitMaps
//
// those maps are loaded in decorations, but are used dynamically in nuiTheme drawing methods, 
// during the application running
//
void nuiDefaultDecoration::InitMaps()
{
  // vertical scrollbar background
  nglIMemory* pIMem = new nglIMemory(gpScrollbarVerticalBkg, gScrollbarVerticalBkgSize);
  nuiTexture* pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  nuiFrame* pFrame = new nuiFrame(_T("nuiDefaultDecorationScrollBarVerticalBkg"), pTex, nuiRect(6,8,0,0));
  pFrame->UseWidgetAlpha(false);
  delete pIMem;

  // vertical scrollbar handle
  pIMem = new nglIMemory(gpScrollbarVerticalHdl, gScrollbarVerticalHdlSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationScrollBarVerticalHdl"), pTex, nuiRect(2,6,6,0));
  pFrame->UseWidgetAlpha(false);
  delete pIMem;
  
  // Horizontal scrollbar background
  pIMem = new nglIMemory(gpScrollbarHorizontalBkg, gScrollbarHorizontalBkgSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationScrollBarHorizontalBkg"), pTex, nuiRect(8,6,0,0));
  pFrame->UseWidgetAlpha(false);
  delete pIMem;
  
  // Horizontal scrollbar handle
  pIMem = new nglIMemory(gpScrollbarHorizontalHdl, gScrollbarHorizontalHdlSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationScrollBarHorizontalHdl"), pTex, nuiRect(6,2,0,6));
  pFrame->UseWidgetAlpha(false);
  delete pIMem;  
  
  
  
  
  
  
  // vertical slider background
  pIMem = new nglIMemory(gpSliderVerticalBkg, gSliderVerticalBkgSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationSliderVerticalBkg"), pTex, nuiRect(0,6,4,0));
  pFrame->UseWidgetAlpha(false);
  delete pIMem;
  
  // vertical slider handle
  pIMem = new nglIMemory(gpSliderVerticalHdl, gSliderVerticalHdlSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationSliderVerticalHdl"), pTex, nuiRect(0,0,20,20));
  pFrame->UseWidgetAlpha(false);
  delete pIMem;
  
  // Horizontal slider background
  pIMem = new nglIMemory(gpSliderHorizontalBkg, gSliderHorizontalBkgSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationSliderHorizontalBkg"), pTex, nuiRect(6,0,0,4));
  pFrame->UseWidgetAlpha(false);
  delete pIMem;
  
  // Horizontal slider handle
  pIMem = new nglIMemory(gpSliderHorizontalHdl, gSliderHorizontalHdlSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationSliderHorizontalHdl"), pTex, nuiRect(0,0,20,20));
  pFrame->UseWidgetAlpha(false);
  delete pIMem; 
  
  
  
  // popup Menu
  pIMem = new nglIMemory(gpPopupMenu, gPopupMenuSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationPopupMenu"), pTex, nuiRect(8,8,0,14));
  pFrame->UseWidgetAlpha(false);
  delete pIMem;  



  // arrow handle close
  pIMem = new nglIMemory(gpArrowClose, gArrowCloseSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationArrowClose"), pTex, nuiRect(0,0,7,6));
  pFrame->UseWidgetAlpha(false);

  delete pIMem;  
  
  
  // arrow handle open
  pIMem = new nglIMemory(gpArrowOpen, gArrowOpenSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationArrowOpen"), pTex, nuiRect(0,0,6,7));
  pFrame->UseWidgetAlpha(false);
  delete pIMem;  
  
  
  
  // Checkerboard for color alpha viewing
  pIMem = new nglIMemory(gpCheckerboardSmall, gCheckerboardSmallSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationCheckerboardSmall"), pTex, nuiRect(0,0,20,20));
  pFrame->UseWidgetAlpha(false);
  delete pIMem;  
  

}




//**************************************************************************************************************
//
// InitIcons
//
void nuiDefaultDecoration::InitIcons()
{
  // volume icon
  nglIMemory* pIMem = new nglIMemory(gpIconVolume, gIconVolumeSize);
  nuiTexture* pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  nuiFrame* pFrame = new nuiFrame(_T("nuiDefaultDecorationIconVolume"), pTex, nuiRect(0,0,18,19));
  pFrame->UseWidgetAlpha(false);
  delete pIMem; 

  // folder icon
  pIMem = new nglIMemory(gpIconFolder, gIconFolderSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationIconFolder"), pTex, nuiRect(0,0,18,16));
  pFrame->UseWidgetAlpha(false);
  delete pIMem;  
  
  // file icon
  pIMem = new nglIMemory(gpIconFile, gIconFileSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationIconFile"), pTex, nuiRect(0,0,16,20));
  pFrame->UseWidgetAlpha(false);
  delete pIMem; 
}





//**************************************************************************************************************
//
// nuiWindow
//
void nuiDefaultDecoration::Window(nuiWidget* pWidget)
{
  nuiWindow* pWindow = (nuiWindow*)pWidget;
  if (pWindow->GetFlags() & nuiWindow::DecoratedBackground)
  {
    nuiGradientDecoration* pDeco = (nuiGradientDecoration*)nuiDecoration::Get(_T("nuiDefaultDecorationWindow"));
    if (!pDeco)
    {
      nuiColor color1, color2;
      nuiColor::GetColor(_T("nuiDefaultClrWindowBkg1"), color1);
      nuiColor::GetColor(_T("nuiDefaultClrWindowBkg2"), color2);
      
      pDeco = new nuiGradientDecoration(_T("nuiDefaultDecorationWindow"), 
                                        nuiRect(0,0, 0,0), color1, color2, nuiVertical, 1, nuiColor(175,175,175), eStrokeAndFillShape);
      pDeco->SetOffset1(0.f);
      pDeco->SetOffset2(0.5f);
    }
    pWindow->SetDecoration(pDeco);
  }
  else
  {
    pWindow->SetColor(eActiveWindowBg, nuiColor(255, 255, 255));
    pWindow->SetColor(eInactiveWindowBg, nuiColor(200, 200, 200));
  }

  // see nuiTheme::DrawActiveWindow for the rest
  
}



//**************************************************************************************************************
//
// nuiBackgroundPane
//
void nuiDefaultDecoration::BackgroundPane(nuiWidget* pWidget)
{
  nuiBackgroundPane* pPane = (nuiBackgroundPane*)pWidget;

  nglString decoName;
  const char* deco;
  uint32 decoSize;
  nuiRect rect;
  
  if (pPane->GetType() == eOutterBackground)
  {
    decoName = _T("nuiDefaultDecorationOutterPane");
    deco = gpPaneOutter;
    decoSize = gPaneOutterSize;
    rect = nuiRect(12,12,0,1);
  }
  else
  {
    decoName = _T("nuiDefaultDecorationInnerPane");
    deco = gpPaneInner;
    decoSize = gPaneInnerSize;
    rect = nuiRect(6,6,0,0);
  }
  
  nuiFrame* pFrame = (nuiFrame*)nuiDecoration::Get(decoName);
  
  if (!pFrame)
  {
    nglIMemory* pIMem = new nglIMemory(deco, decoSize);
    nuiTexture* pTex = nuiTexture::GetTexture(pIMem);
    NGL_ASSERT(pTex);
    pFrame = new nuiFrame(decoName, pTex, rect);
    pFrame->UseWidgetAlpha(false);
    delete pIMem; 
  }
  NGL_ASSERT(pFrame);
  pWidget->SetDecoration(pFrame, eDecorationBorder);
}




//**************************************************************************************************************
//
// nuiLabel
//
void nuiDefaultDecoration::Label(nuiWidget* pWidget)
{
//  nuiColor cText;
//  cText.SetValue(_T("nuiDefaultColorLabelText"));
//  pWidget->SetColor(eNormalTextFg, cText);
}


//**************************************************************************************************************
//
// nuiEditText
//
void nuiDefaultDecoration::EditText(nuiWidget* pWidget)
{
  
}


//**************************************************************************************************************
//
// nuiEditLine
//
void nuiDefaultDecoration::EditLine(nuiWidget* pWidget)
{
  nuiBackgroundPane* pPane = (nuiBackgroundPane*)pWidget;
  
  nglString decoName = _T("nuiDefaultDecorationInnerPane");
  const char* deco = gpPaneInner;
  uint32 decoSize = gPaneInnerSize;
  nuiRect rect = nuiRect(6,6,0,0);
  
  nuiFrame* pFrame = (nuiFrame*)nuiDecoration::Get(decoName);
  
  if (!pFrame)
  {
    nglIMemory* pIMem = new nglIMemory(deco, decoSize);
    nuiTexture* pTex = nuiTexture::GetTexture(pIMem);
    NGL_ASSERT(pTex);
    pFrame = new nuiFrame(decoName, pTex, rect);
    pFrame->UseWidgetAlpha(false);
    delete pIMem; 
  }
  NGL_ASSERT(pFrame);
  
  pWidget->SetDecoration(pFrame, eDecorationBorder);  
}


//**************************************************************************************************************
//
// nuiSplitterHandle
//
void nuiDefaultDecoration::SplitterHandle(nuiWidget* pWidget)
{
  nuiSplitterHandle* pHandle = (nuiSplitterHandle*)pWidget;
  nuiSplitter* pSplitter = pHandle->GetSplitter();
  NGL_ASSERT(pSplitter);
  
  if (pSplitter->GetOrientation() == nuiVertical)
  {
    pHandle->SetUserSize(6,0);

    nuiMetaDecoration* pDeco = (nuiMetaDecoration*)nuiDecoration::Get(_T("nuiDefaultDecorationSplitterHandleVertical"));
    if (!pDeco)
    {
      nuiGradientDecoration* pDeco1 = new nuiGradientDecoration(_T("nuiDefaultDecorationSplitterHandleVertical1"), 
                                                                nuiRect(0, 0, 0, 0), nuiColor(232,232,232), nuiColor(196,196,196), nuiHorizontal);

      nuiBorderDecoration* pDeco2 = new nuiBorderDecoration(_T("nuiDefaultDecorationSplitterHandleVertical2"));
      pDeco2->SetBorderType(_T("All"));
      pDeco2->SetStrokeSize(1);
      pDeco2->SetStrokeLeftColor(nuiColor(190,190,190));
      pDeco2->SetStrokeRightColor(nuiColor(170,170,170));
      pDeco2->SetStrokeTopColor(nuiColor(180,180,180));
      pDeco2->SetStrokeBottomColor(nuiColor(180,180,180));
      
      pDeco = new nuiMetaDecoration(_T("nuiDefaultDecorationSplitterHandleVertical"));
      pDeco->AddDecoration(_T("nuiDefaultDecorationSplitterHandleVertical1"));
      pDeco->AddDecoration(_T("nuiDefaultDecorationSplitterHandleVertical2"));
      
    }
    pHandle->SetDecoration(pDeco, eDecorationBorder);  
    
  }
  else
  {
    pHandle->SetUserSize(0,6);
    
    nuiMetaDecoration* pDeco = (nuiMetaDecoration*)nuiDecoration::Get(_T("nuiDefaultDecorationSplitterHandleHorizontal"));
    if (!pDeco)
    {
      nuiGradientDecoration* pDeco1 = new nuiGradientDecoration(_T("nuiDefaultDecorationSplitterHandleHorizontal1"), 
                                                                nuiRect(0, 0, 0, 0), nuiColor(232,232,232), nuiColor(196,196,196), nuiVertical);
      nuiBorderDecoration* pDeco2 = new nuiBorderDecoration(_T("nuiDefaultDecorationSplitterHandleHorizontal2"));
      pDeco2->SetBorderType(_T("All"));
      pDeco2->SetStrokeSize(1);
      pDeco2->SetStrokeLeftColor(nuiColor(180,180,180));
      pDeco2->SetStrokeRightColor(nuiColor(180,180,180));
      pDeco2->SetStrokeTopColor(nuiColor(190,190,190));
      pDeco2->SetStrokeBottomColor(nuiColor(170,170,170));
      
      pDeco = new nuiMetaDecoration(_T("nuiDefaultDecorationSplitterHandleHorizontal"));
      pDeco->AddDecoration(_T("nuiDefaultDecorationSplitterHandleHorizontal1"));
      pDeco->AddDecoration(_T("nuiDefaultDecorationSplitterHandleHorizontal2"));
      
    }
   pHandle->SetDecoration(pDeco, eDecorationBorder);  
  }
  
  
}



//**************************************************************************************************************
//
// nuiTitledPane
//
void nuiDefaultDecoration::TitledPane(nuiWidget* pWidget)
{
  
}


//**************************************************************************************************************
//
// nuiFolderPane
//
void nuiDefaultDecoration::FolderPane(nuiWidget* pWidget)
{
  
}




//**************************************************************************************************************
//
// nuiButton
//
void nuiDefaultDecoration::Button(nuiWidget* pWidget)
{
  nuiStateDecoration* pDeco = (nuiStateDecoration*)nuiDecoration::Get(_T("nuiDefaultDecorationButton"));
  if (pDeco)
  {
    pWidget->SetDecoration(pDeco, eDecorationBorder);
    return;
  }
  
  nglIMemory* pIMemUp = new nglIMemory(gpButtonUp, gButtonUpSize);
  nuiTexture* pTexUp = nuiTexture::GetTexture(pIMemUp);
  NGL_ASSERT(pTexUp);
  nuiFrame* pFrameUp = new nuiFrame(_T("nuiDefaultDecorationButtonUp"), pTexUp, nuiRect(14,15,2,7));
  delete pIMemUp;

  
  nglIMemory* pIMemHover = new nglIMemory(gpButtonUp, gButtonUpSize);
  nuiTexture* pTexHover = nuiTexture::GetTexture(pIMemHover);
  NGL_ASSERT(pTexHover);
  nuiFrame* pFrameHover = new nuiFrame(_T("nuiDefaultDecorationButtonHover"), pTexHover, nuiRect(14,15,2,7));
  delete pIMemHover;
  
  
  nglIMemory* pIMemDown = new nglIMemory(gpButtonDown, gButtonDownSize);
  nuiTexture* pTexDown = nuiTexture::GetTexture(pIMemDown);
  NGL_ASSERT(pTexDown);
  nuiFrame* pFrameDown = new nuiFrame(_T("nuiDefaultDecorationButtonDown"), pTexDown, nuiRect(14,15,2,7));
  delete pIMemDown;
  
  
  
  nuiStateDecoration* pState = new nuiStateDecoration(_T("nuiDefaultDecorationButton"), 
                                                      _T("nuiDefaultDecorationButtonUp"),
                                                      _T("nuiDefaultDecorationButtonDown"),
                                                      _T("nuiDefaultDecorationButtonHover"));
  pState->SetSourceClientRect(nuiRect(14,15,2,7));

  pWidget->SetDecoration(pState, eDecorationBorder);

}





//**************************************************************************************************************
//
// nuiRadioButton
//
void nuiDefaultDecoration::RadioButton(nuiWidget* pWidget)
{
  nuiRadioButton* pBtn = (nuiRadioButton*)pWidget;
  if (pBtn->HasContents())
  {
    nuiDefaultDecoration::Button(pWidget);
    return;
  }
  
  
  nuiStateDecoration* pDeco = (nuiStateDecoration*)nuiDecoration::Get(_T("nuiDefaultDecorationRadioButton"));
  if (pDeco)
  {
    pWidget->SetDecoration(pDeco, eDecorationBorder);
    return;
  }
  
  nglIMemory* pIMem = new nglIMemory(gpRadioButtonUp, gRadioButtonUpSize);
  nuiTexture* pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  nuiFrame* pFrame = new nuiFrame(_T("nuiDefaultDecorationRadioButtonUp"), pTex, nuiRect(10,10,13,13));
  delete pIMem;
  
  
  pIMem = new nglIMemory(gpRadioButtonDown, gRadioButtonDownSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationRadioButtonDown"), pTex, nuiRect(10,10,13,13));
  delete pIMem;

  pIMem = new nglIMemory(gpRadioButtonUpDisabled, gRadioButtonUpDisabledSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationRadioButtonUpDisabled"), pTex, nuiRect(10,10,13,13));
  delete pIMem;

  pIMem = new nglIMemory(gpRadioButtonDownDisabled, gRadioButtonDownDisabledSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationRadioButtonDownDisabled"), pTex, nuiRect(10,10,13,13));
  delete pIMem;
  
  
  nuiStateDecoration* pState = new nuiStateDecoration(_T("nuiDefaultDecorationRadioButton"), 
                                                      _T("nuiDefaultDecorationRadioButtonUp"),
                                                      _T("nuiDefaultDecorationRadioButtonDown"),
                                                      _T("nuiDefaultDecorationRadioButtonUp"),/* hover up */
                                                      _T("nuiDefaultDecorationRadioButtonDown"), /* hover down*/
                                                      _T("nuiDefaultDecorationRadioButtonUpDisabled"),
                                                      _T("nuiDefaultDecorationRadioButtonDownDisabled"));
  pState->SetSourceClientRect(nuiRect(10,10,13,13));
  
  pWidget->SetDecoration(pState, eDecorationBorder);
  
}







//**************************************************************************************************************
//
// nuiCloseButton (for windows)
//
void nuiDefaultDecoration::CloseButton(nuiWidget* pWidget)
{
  
  nuiStateDecoration* pDeco = (nuiStateDecoration*)nuiDecoration::Get(_T("nuiDefaultDecorationCloseButton"));
  if (pDeco)
  {
    pWidget->SetDecoration(pDeco, eDecorationBorder);
    return;
  }
  
  nglIMemory* pIMemUp = new nglIMemory(gpCloseButtonUp, gCloseButtonUpSize);
  nuiTexture* pTexUp = nuiTexture::GetTexture(pIMemUp);
  NGL_ASSERT(pTexUp);
  nuiFrame* pFrameUp = new nuiFrame(_T("nuiDefaultDecorationCloseButtonUp"), pTexUp, nuiRect(0,0,12,15));
  delete pIMemUp;
  
  
  nglIMemory* pIMemHover = new nglIMemory(gpCloseButtonHover, gCloseButtonHoverSize);
  nuiTexture* pTexHover = nuiTexture::GetTexture(pIMemHover);
  NGL_ASSERT(pTexHover);
  nuiFrame* pFrameHover = new nuiFrame(_T("nuiDefaultDecorationCloseButtonHover"), pTexHover, nuiRect(0,0,12,15));
  delete pIMemHover;
  
  
  nglIMemory* pIMemDown = new nglIMemory(gpCloseButtonDown, gCloseButtonDownSize);
  nuiTexture* pTexDown = nuiTexture::GetTexture(pIMemDown);
  NGL_ASSERT(pTexDown);
  nuiFrame* pFrameDown = new nuiFrame(_T("nuiDefaultDecorationCloseButtonDown"), pTexDown, nuiRect(0,0,12,15));
  delete pIMemDown;
  
  
  
  nuiStateDecoration* pState = new nuiStateDecoration(_T("nuiDefaultDecorationCloseButton"), 
                                                      _T("nuiDefaultDecorationCloseButtonUp"),
                                                      _T("nuiDefaultDecorationCloseButtonDown"),
                                                      _T("nuiDefaultDecorationCloseButtonHover"));
  pState->SetSourceClientRect(nuiRect(0,0,12,15));
  
  pWidget->SetDecoration(pState, eDecorationBorder);
  
}




//**************************************************************************************************************
//
// nuiToggleButton
//
void nuiDefaultDecoration::ToggleButton(nuiWidget* pWidget)
{
  nuiToggleButton* pBtn = (nuiToggleButton*)pWidget;
  if (pBtn->HasContents())
  {
    nuiDefaultDecoration::Button(pWidget);
    return;
  }
  
  
  nuiStateDecoration* pDeco = (nuiStateDecoration*)nuiDecoration::Get(_T("nuiDefaultDecorationToggleButton"));
  if (pDeco)
  {
    pWidget->SetDecoration(pDeco, eDecorationBorder);
    return;
  }
  
  nglIMemory* pIMem = new nglIMemory(gpToggleButtonUp, gToggleButtonUpSize);
  nuiTexture* pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  nuiFrame* pFrame = new nuiFrame(_T("nuiDefaultDecorationToggleButtonUp"), pTex, nuiRect(10,10,13,13));
  delete pIMem;
  
  
  pIMem = new nglIMemory(gpToggleButtonDown, gToggleButtonDownSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationToggleButtonDown"), pTex, nuiRect(10,10,13,13));
  delete pIMem;
  
  pIMem = new nglIMemory(gpToggleButtonUpDisabled, gToggleButtonUpDisabledSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationToggleButtonUpDisabled"), pTex, nuiRect(10,10,13,13));
  delete pIMem;
  
  pIMem = new nglIMemory(gpToggleButtonDownDisabled, gToggleButtonDownDisabledSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationToggleButtonDownDisabled"), pTex, nuiRect(10,10,13,13));
  delete pIMem;
  
  
  nuiStateDecoration* pState = new nuiStateDecoration(_T("nuiDefaultDecorationToggleButton"), 
                                                      _T("nuiDefaultDecorationToggleButtonUp"),
                                                      _T("nuiDefaultDecorationToggleButtonDown"),
                                                      _T("nuiDefaultDecorationToggleButtonUp"),/* hover up */
                                                      _T("nuiDefaultDecorationToggleButtonDown"), /* hover down*/
                                                      _T("nuiDefaultDecorationToggleButtonUpDisabled"),
                                                      _T("nuiDefaultDecorationToggleButtonDownDisabled"));
  pState->SetSourceClientRect(nuiRect(10,10,13,13));
  
  pWidget->SetDecoration(pState, eDecorationBorder);
  
}



//**************************************************************************************************************
//
// nuiComboBox
//
void nuiDefaultDecoration::ComboBox(nuiWidget* pWidget)
{
 
  nuiStateDecoration* pDeco = (nuiStateDecoration*)nuiDecoration::Get(_T("nuiDefaultDecorationComboBox"));
  if (pDeco)
  {
    pWidget->SetDecoration(pDeco, eDecorationBorder);
    return;
  }
  
  nglIMemory* pIMem = new nglIMemory(gpComboUp, gComboUpSize);
  nuiTexture* pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  nuiFrame* pFrame = new nuiFrame(_T("nuiDefaultDecorationComboBoxUp"), pTex, nuiRect(12,4,2,10));
  pFrame->EnableBorder(false);
  delete pIMem;
    
  pIMem = new nglIMemory(gpComboDown, gComboDownSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(_T("nuiDefaultDecorationComboBoxDown"), pTex, nuiRect(12,4,2,10));
  pFrame->EnableBorder(false);
  delete pIMem;
  
  
  
  nuiStateDecoration* pState = new nuiStateDecoration(_T("nuiDefaultDecorationComboBox"));
  pState->SetState(nuiStateEnabled  | nuiStateReleased, nuiDecoration::Get(_T("nuiDefaultDecorationComboBoxUp")));
  pState->SetState(nuiStateEnabled  | nuiStateSelected, nuiDecoration::Get(_T("nuiDefaultDecorationComboBoxDown")));
  pWidget->SetDecoration(pState, eDecorationBorder);
  pWidget->SetUserHeight(18);
  
  pWidget->SetColor(eSelectedTextFg, nuiColor(32,32,32));
  
}



//**************************************************************************************************************
//
// nuiKnob
//

nuiImageSequence* nuiDefaultDecoration::mpKnobSequence;

void nuiDefaultDecoration::KnobSequence(nuiWidget* pWidget)
{
  nuiKnob* pKnob = (nuiKnob*)pWidget;

  if (!pKnob->GetImageSequence() && !mpKnobSequence)
  {
    nglIMemory* pIMem = new nglIMemory(gpKnobSequenceHdl, gKnobSequenceHdlSize);
    nglImage* pImage = new nglImage(pIMem);
    NGL_ASSERT(pImage);
    mpKnobSequence = new nuiImageSequence(31, pImage, nuiVertical);
    delete pIMem;     
    delete pImage;
  }

  if (pKnob->GetShowDefaultBackground())
  {
    nuiFrame* pBkgDeco = (nuiFrame*)nuiDecoration::Get(_T("nuiDefaultDecorationKnobSequenceBkg"));
    if (!pBkgDeco)
    {
      nglIMemory* pIMem = new nglIMemory(gpKnobSequenceBkg, gKnobSequenceBkgSize);
      nuiTexture* pTex = nuiTexture::GetTexture(pIMem);
      NGL_ASSERT(pTex);
      pBkgDeco = new nuiFrame(_T("nuiDefaultDecorationKnobSequenceBkg"), pTex, nuiRect(0,0,64,64));
      pBkgDeco->UseWidgetAlpha(false);
      pBkgDeco->SetLayer(eLayerBack);
      delete pIMem;  
    }
    pKnob->SetDecoration(pBkgDeco, eDecorationBorder);
  }
  
  if (!pKnob->GetImageSequence())
    pKnob->SetImageSequence(mpKnobSequence);
}




//***********************************************************************************************************************************
//***********************************************************************************************************************************
//
// FileSelector
//
//***********************************************************************************************************************************
//***********************************************************************************************************************************

void nuiDefaultDecoration::FileSelector_FolderView(nuiWidget* pWidget)
{
  nuiColorDecoration* pDeco = (nuiColorDecoration*)nuiDecoration::Get(_T("nuiDefaultDecorationFileSelector_FolderView"));
  if (!pDeco)
  {
    pDeco = new nuiColorDecoration(_T("nuiDefaultDecorationFileSelector_FolderView"), nuiRect(5,5,0,0), nuiColor(214,221,229), 1, nuiColor(139,139,139), eStrokeAndFillShape);
  }
  pWidget->SetDecoration(pDeco, eDecorationBorder);
}

void nuiDefaultDecoration::FileSelector_InfoView(nuiWidget* pWidget)
{

}


void nuiDefaultDecoration::FileSelector_FolderLabel(nuiWidget* pWidget)
{
  
}


void nuiDefaultDecoration::FileSelector_VolumeIcon(nuiWidget* pWidget)
{
  nuiFrame* pFrame = (nuiFrame*)nuiDecoration::Get(_T("nuiDefaultDecorationIconVolume"));
  NGL_ASSERT(pFrame);
  pWidget->SetDecoration(pFrame);
  pWidget->SetUserSize(18,19);
}


void nuiDefaultDecoration::FileSelector_FolderIcon(nuiWidget* pWidget)
{
  nuiFrame* pFrame = (nuiFrame*)nuiDecoration::Get(_T("nuiDefaultDecorationIconFolder"));
  NGL_ASSERT(pFrame);
  pWidget->SetDecoration(pFrame);  
  pWidget->SetUserSize(18,16);
}


void nuiDefaultDecoration::FileSelector_TreeView(nuiWidget* pWidget)
{
  nuiGradientDecoration* pDeco = (nuiGradientDecoration*)nuiDecoration::Get(_T("nuiDefaultDecorationWindow"));
  if (!pDeco)
  {
    nuiColor color1, color2;
    nuiColor::GetColor(_T("nuiDefaultClrWindowBkg1"), color1);
    nuiColor::GetColor(_T("nuiDefaultClrWindowBkg2"), color2);
    
    pDeco = new nuiGradientDecoration(_T("nuiDefaultDecorationWindow"), 
                                      nuiRect(0,0, 0,0), color1, color2, nuiVertical, 1, nuiColor(175,175,175), eStrokeAndFillShape);
    pDeco->SetOffset1(0.f);
    pDeco->SetOffset2(0.5f);
  }
  pWidget->SetDecoration(pDeco, eDecorationBorder);  
}


void nuiDefaultDecoration::FileSelector_TreeFolderLabel(nuiWidget* pWidget)
{
  
}


void nuiDefaultDecoration::FileSelector_TreeFileLabel(nuiWidget* pWidget)
{
  
}


void nuiDefaultDecoration::FileSelector_TreeFolderIcon(nuiWidget* pWidget)
{
  nuiFrame* pFrame = (nuiFrame*)nuiDecoration::Get(_T("nuiDefaultDecorationIconFolder"));
  NGL_ASSERT(pFrame);
  pWidget->SetDecoration(pFrame);    
  pWidget->SetUserSize(18,16);
}


void nuiDefaultDecoration::FileSelector_TreeFileIcon(nuiWidget* pWidget)
{
  nuiFrame* pFrame = (nuiFrame*)nuiDecoration::Get(_T("nuiDefaultDecorationIconFile"));
  NGL_ASSERT(pFrame);
  pWidget->SetDecoration(pFrame);    
  pWidget->SetUserSize(16,20);
}





//***********************************************************************************************************************************
//***********************************************************************************************************************************
//
// special cases. Those are not called through the automatic default decoration system.
// they are called from their own source code.
//
//***********************************************************************************************************************************
//***********************************************************************************************************************************


//**************************************************************************************************************
//
// nuiMainWindow
//
void nuiDefaultDecoration::MainWindow(nuiMainWindow* pWindow)
{
  nuiWidget* pCont = new nuiWidget();
  pWindow->AddChild(pCont);
  nuiColor color1, color2;
  nuiColor::GetColor(_T("nuiDefaultClrWindowBkg1"), color1);
  nuiColor::GetColor(_T("nuiDefaultClrWindowBkg2"), color2);
  nuiGradientDecoration* pDeco = new nuiGradientDecoration(_T("nuiDefaultDecorationMainWindow"), 
                                                           nuiRect(0,0, 0,0), color1, color2, nuiVertical, 0, nuiColor(0,0,0), eFillShape);
  pDeco->SetOffset1(0.f);
  pDeco->SetOffset2(0.5f);                                                           
//  pWindow->SetDecoration(pDeco);  
}


//**************************************************************************************************************
//
// nuiDialog
//
void nuiDefaultDecoration::Dialog(nuiSimpleContainer* pCont)
{
  nuiFrame* pFrame = (nuiFrame*)nuiDecoration::Get(_T("nuiDefaultDecorationOutterPane"));
  if (!pFrame)
  {
    nglIMemory* pIMem = new nglIMemory(gpPaneOutter, gPaneOutterSize);
    nuiTexture* pTex = nuiTexture::GetTexture(pIMem);
    NGL_ASSERT(pTex);
    pFrame = new nuiFrame(_T("nuiDefaultDecorationOutterPane"), pTex, nuiRect(12,12,0,1));
    pFrame->UseWidgetAlpha(false);
    delete pIMem; 
  }
  NGL_ASSERT(pFrame);
  pCont->SetDecoration(pFrame);  
}


//**************************************************************************************************************
//
// nuiDialog
//
//void nuiDefaultDecoration::DialogSelectFile(nuiSimpleContainer* pCont)
//{
//  nuiFrame* pFrame = (nuiFrame*)nuiDecoration::Get(_T("nuiDefaultDecorationOutterPane"));
//  if (!pFrame)
//  {
//    nglIMemory* pIMem = new nglIMemory(gpPaneOutter, gPaneOutterSize);
//    nuiTexture* pTex = nuiTexture::GetTexture(pIMem);
//    NGL_ASSERT(pTex);
//    pFrame = new nuiFrame(_T("nuiDefaultDecorationOutterPane"), pTex, nuiRect(12,12,0,1));
//    pFrame->UseWidgetAlpha(false);
//    pFrame->EnableBorder(false);
//    delete pIMem; 
//  }
//  NGL_ASSERT(pFrame);
//  pCont->SetDecoration(pFrame, eDecorationBorder);  
//}


//**************************************************************************************************************
//
// nuiDialog::Title
//
void nuiDefaultDecoration::Dialog_Title(nuiWidget* pWidget)
{
  nuiLabel* pLabel = (nuiLabel*)pWidget;
  
  pLabel->SetFont(nuiFont::GetFont(13), true);
  pLabel->SetBorder(0,0,10,15);
}

//**************************************************************************************************************
//
// DialogSelectFile::Title
//
//void nuiDefaultDecoration::DialogSelectFile_Title(nuiWidget* pWidget)
//{
//  nuiLabel* pLabel = (nuiLabel*)pWidget;
//  
//  pLabel->SetFont(nuiFont::GetFont(13), true);
//  pLabel->SetBorder(16,0,16,15);
//}
//





//**************************************************************************************************************
//
// nuiDialog::EditLine
//
void nuiDefaultDecoration::Dialog_EditLine(nuiWidget* pWidget)
{
  nuiFrame* pFrame = (nuiFrame*)nuiDecoration::Get(_T("nuiDefaultDecorationInnerPane"));
  if (!pFrame)
  {
    nglIMemory* pIMem = new nglIMemory(gpPaneInner, gPaneInnerSize);
    nuiTexture* pTex = nuiTexture::GetTexture(pIMem);
    NGL_ASSERT(pTex);
    pFrame = new nuiFrame(_T("nuiDefaultDecorationInnerPane"), pTex, nuiRect(6,6,0,0));
    pFrame->UseWidgetAlpha(false);
    delete pIMem; 
  }
  NGL_ASSERT(pFrame);
  pWidget->SetDecoration(pFrame, eDecorationBorder);    
}



//**************************************************************************************************************
//
// nuiMessageBox
//
void nuiDefaultDecoration::MessageBox(nuiMessageBox* pBox)
{
  nuiFrame* pFrame = (nuiFrame*)nuiDecoration::Get(_T("nuiDefaultDecorationOutterPane"));
  if (!pFrame)
  {
    nglIMemory* pIMem = new nglIMemory(gpPaneOutter, gPaneOutterSize);
    nuiTexture* pTex = nuiTexture::GetTexture(pIMem);
    NGL_ASSERT(pTex);
    pFrame = new nuiFrame(_T("nuiDefaultDecorationOutterPane"), pTex, nuiRect(12,12,0,1));
    pFrame->UseWidgetAlpha(false);
    delete pIMem; 
  }
  NGL_ASSERT(pFrame);
  pBox->SetDecoration(pFrame);
}



//**************************************************************************************************************
//
// nuiMessageBox::Title
//
void nuiDefaultDecoration::MessageBox_Title(nuiWidget* pWidget)
{
  nuiLabel* pLabel = (nuiLabel*)pWidget;
  
  pLabel->SetFont(nuiFont::GetFont(14), true);
  pLabel->SetBorder(0,0,0,15);
}



//**************************************************************************************************************
//
// nuiTabView::Tab
//
void nuiDefaultDecoration::TabView_Tab(nuiTabView* pView, nuiWidget* pTab)
{
  nglString decoName = _T("nuiDefaultDecorationTabTop");
  nglString decoUpName;
  nglString decoDownName;
  
  nuiRect frameRect;

  const char* decoUp = gpTabTopUp;
  const char* decoDown = gpTabTopDown;
  long decoUpSize = gTabTopUpSize;
  long decoDownSize = gTabTopDownSize;
  
  switch (pView->GetTabPosition())
  {
    case nuiTop:
      decoName = _T("nuiDefaultDecorationTabTop");
      decoUp = gpTabTopUp;
      decoDown = gpTabTopDown;
      decoUpSize = gTabTopUpSize;
      decoDownSize = gTabTopDownSize;
      frameRect = nuiRect(4,5,2,4);
      break;
      
    case nuiLeft:
      decoName = _T("nuiDefaultDecorationTabLeft");
      decoUp = gpTabLeftUp;
      decoDown = gpTabLeftDown;
      decoUpSize = gTabLeftUpSize;
      decoDownSize = gTabLeftDownSize;
      frameRect = nuiRect(5,4,4,2);
      break;
      
    case nuiRight:
      decoName = _T("nuiDefaultDecorationTabRight");
      decoUp = gpTabRightUp;
      decoDown = gpTabRightDown;
      decoUpSize = gTabRightUpSize;
      decoDownSize = gTabRightDownSize;
      frameRect = nuiRect(5,4,4,2);
      break;
      
    case nuiBottom:
      decoName = _T("nuiDefaultDecorationTabBottom");
      decoUp = gpTabBottomUp;
      decoDown = gpTabBottomDown;
      decoUpSize = gTabBottomUpSize;
      decoDownSize = gTabBottomDownSize;
      frameRect = nuiRect(4,5,2,4);
      break;
      
    default:
      NGL_OUT(_T("nui3 error : the nuiTabView object doesn't have a nuiPosition valid parameter!\n"));
      NGL_ASSERT(0);
      break;
  }
  
  nuiStateDecoration* pDeco = (nuiStateDecoration*)nuiDecoration::Get(decoName);
  if (pDeco)
  {
    pTab->SetDecoration(pDeco, eDecorationBorder);
    return;
  }
  
  decoUpName = decoName + _T("Up");
  decoDownName = decoName + _T("Down");
  
  nglIMemory* pIMem = new nglIMemory(decoUp, decoUpSize);
  nuiTexture* pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  nuiFrame* pFrame = new nuiFrame(decoUpName, pTex, frameRect);
  delete pIMem;
  
  pIMem = new nglIMemory(decoDown, decoDownSize);
  pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pFrame = new nuiFrame(decoDownName, pTex, frameRect);
  delete pIMem;
  
  
  
  nuiStateDecoration* pState = new nuiStateDecoration(decoName);
  pState->SetState(nuiStateEnabled  | nuiStateReleased, nuiDecoration::Get(decoUpName));
  pState->SetState(nuiStateEnabled  | nuiStateSelected, nuiDecoration::Get(decoDownName));
  
  pState->SetSourceClientRect(frameRect);
  
  pTab->SetDecoration(pState, eDecorationBorder);
  
}





//**************************************************************************************************************
//
// nuiTabView::Contents
//

void nuiDefaultDecoration::TabView_Contents(nuiTabView* pView, nuiWidget* pContents)
{
  nglString decoName = _T("nuiDefaultDecorationTabTopContents");
  
  nuiRect frameRect;
  
  const char* deco = gpTabTopContents;
  long decoSize = gTabTopContentsSize;
  
  switch (pView->GetTabPosition())
  {
    case nuiTop:
      decoName = _T("nuiDefaultDecorationTabTopContents");
      deco = gpTabTopContents;
      decoSize = gTabTopContentsSize;
      frameRect = nuiRect(4,5,2,4);
      break;
      
    case nuiLeft:
      decoName = _T("nuiDefaultDecorationTabLeftContents");
      deco = gpTabLeftContents;
      decoSize = gTabLeftContentsSize;
      frameRect = nuiRect(5,4,4,2);
      break;
      
    case nuiRight:
      decoName = _T("nuiDefaultDecorationTabRightContents");
      deco = gpTabRightContents;
      decoSize = gTabRightContentsSize;
      frameRect = nuiRect(5,4,4,2);
      break;
      
    case nuiBottom:
      decoName = _T("nuiDefaultDecorationTabBottomContents");
      deco = gpTabBottomContents;
      decoSize = gTabBottomContentsSize;
      frameRect = nuiRect(4,5,2,4);
      break;
  }
  
  nuiFrame* pDeco = (nuiFrame*)nuiDecoration::Get(decoName);
  if (pDeco)
  {
    pContents->SetDecoration(pDeco, eDecorationBorder);
    return;
  }
  
  nglIMemory* pIMem = new nglIMemory(deco, decoSize);
  nuiTexture* pTex = nuiTexture::GetTexture(pIMem);
  NGL_ASSERT(pTex);
  pDeco = new nuiFrame(decoName, pTex, frameRect);
  delete pIMem;
  
  pContents->SetDecoration(pDeco, eDecorationBorder);
  
}






