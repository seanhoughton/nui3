/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiHTMLView.h"
#include "nuiHTTP.h"
#include "nuiFontManager.h"
#include "nuiUnicode.h"

#include "nuiHTMLContext.h"
#include "nuiHTMLItem.h"
#include "nuiHTMLBox.h"
#include "nuiHTMLHeader.h"
#include "nuiHTMLText.h"
#include "nuiHTMLImage.h"
#include "nuiHTMLFont.h"

#include "nuiAsyncIStream.h"
#include "nuiStopWatch.h"

/////////////////////////////// nuiHTMLView
nuiHTMLView::nuiHTMLView(float IdealWidth)
{
  if (SetObjectClass(_T("nuiHTMLView")))
    InitAttributes();
  
  mpHTML = NULL;
  mpRootBox = NULL;
  mIdealWidth = IdealWidth;
  mVSpace = 2.0f;
  mHSpace = 0.0f;
  mpContext = new nuiHTMLContext();
  mClicked = false;
  mAutoActivateLink = true;
  mpCurrentAnchor = NULL;
  mpStream = NULL;
  
  mMouseX = 0;
  mMouseY = 0;
  
  mDebugBoxes = false;
  mAutoIdealWidth = true;
  
  mSlotSink.Connect(LinkActivated, nuiMakeDelegate(this, &nuiHTMLView::_AutoSetURL));
}

nuiHTMLView::~nuiHTMLView()
{
  delete mpHTML;
  delete mpContext;

  Cancel();
}

void nuiHTMLView::Cancel()
{
  if (mpStream)
  {
    mpStream->Cancel();
    mpStream = NULL;
  }
}

void nuiHTMLView::InitAttributes()
{
  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("Text")), nuiUnitName,
                //nuiMakeDelegate(this, &nuiHTMLView::_GetFont), 
                nuiMakeDelegate(this, &nuiHTMLView::_SetText)));
  
  AddAttribute(new nuiAttribute<float>
               (nglString(_T("Width")), nuiUnitName,
                nuiMakeDelegate(this, &nuiHTMLView::GetIdealWidth), 
                nuiMakeDelegate(this, &nuiHTMLView::SetIdealWidth)));
  
  AddAttribute(new nuiAttribute<float>
               (nglString(_T("VSpace")), nuiUnitName,
                nuiMakeDelegate(this, &nuiHTMLView::GetVSpace), 
                nuiMakeDelegate(this, &nuiHTMLView::SetVSpace)));
  
  AddAttribute(new nuiAttribute<float>
               (nglString(_T("HSpace")), nuiUnitName,
                nuiMakeDelegate(this, &nuiHTMLView::GetHSpace), 
                nuiMakeDelegate(this, &nuiHTMLView::SetHSpace)));  
  
  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("Font")), nuiUnitName,
                nuiMakeDelegate(this, &nuiHTMLView::_GetFont), 
                nuiMakeDelegate(this, &nuiHTMLView::_SetFont)));
  
  AddAttribute(new nuiAttribute<const nuiColor&>
   (nglString(_T("TextColor")), nuiUnitNone,
    nuiMakeDelegate(this, &nuiHTMLView::GetTextColor), 
    nuiMakeDelegate(this, &nuiHTMLView::SetTextColor)));
  
  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("URL")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiHTMLView::GetURL), 
                nuiMakeDelegate(this, &nuiHTMLView::_SetURL)));

  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("AutoIdealWidth")), nuiUnitName,
                nuiMakeDelegate(this, &nuiHTMLView::GetAutoIdealWidth), 
                nuiMakeDelegate(this, &nuiHTMLView::SetAutoIdealWidth)));  
  
}


void nuiHTMLView::SetFont(nuiFont* pFont, bool AlreadyAcquired)
{
  if (!pFont)
  {
    AlreadyAcquired = true;
    pFont = nuiFont::GetFont(14.0f);
  }
  
  mpContext->mFont = nuiFontRequest(pFont, false);
  if (AlreadyAcquired)
    pFont->Release();
  
  InvalidateLayout();
}

void nuiHTMLView::SetFont(nuiFontRequest& rFontRequest)
{
  mpContext->mFont = rFontRequest;
  InvalidateLayout();
}

void nuiHTMLView::SetFont(const nglString& rFontSymbol)
{
  nuiFont* pFont = nuiFont::GetFont(rFontSymbol);
  if (pFont)
    SetFont(pFont, true);
}


void nuiHTMLView::_SetFont(const nglString& rFontSymbol)
{
  SetFont(rFontSymbol);
}

const nglString& nuiHTMLView::_GetFont() const
{
  return mpContext->mFont.mName.mElement;
}

const nuiColor& nuiHTMLView::GetTextColor() const
{
  return mpContext->mTextFgColor;
}

void nuiHTMLView::SetTextColor(const nuiColor& Color)
{
  mpContext->mTextFgColor = Color;
  ReLayout();
  Invalidate();
}


nuiRect nuiHTMLView::CalcIdealSize()
{
  float IdealWidth = mIdealWidth;
  if (mRect.GetWidth() > 0)
    IdealWidth = mRect.GetWidth();
  Clear();

  mpContext->mMaxWidth = IdealWidth;
  if (!mpRootBox)
    return nuiRect(IdealWidth, 400.0f);
  
  nuiHTMLContext context(*mpContext);
  mpRootBox->Layout(context);
  return nuiRect(mpRootBox->GetIdealRect().GetWidth(), mpRootBox->GetIdealRect().GetHeight());
}

void nuiHTMLView::ReLayout()
{
  if (!mpRootBox)
    return;
  
  nuiHTMLContext context(*mpContext);
  mpRootBox->Layout(context);
  mpRootBox->SetLayout(mpRootBox->GetIdealRect());
  mLastVisibleRect = nuiRect();
}


bool nuiHTMLView::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);

  if (mAutoIdealWidth)
    SetIdealWidth(rRect.GetWidth());
  
  ReLayout();
  return true;
}

bool nuiHTMLView::Draw(nuiDrawContext* pContext)
{
  if (!mpRootBox)
    return true;
  
  if (!(mLastVisibleRect == mVisibleRect))
  {
    mpRootBox->UpdateVisibility(mVisibleRect);
    mLastVisibleRect = mVisibleRect;
  }
  
  nuiSimpleContainer::Draw(pContext);
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->EnableBlending(true);
  if (mpRootBox)
  {
    mpRootBox->CallDraw(pContext);
  }
  
  if (mDebugBoxes)
  {
    std::vector<nuiHTMLItem*> items;
    mpRootBox->GetItemsAt(items, mMouseX, mMouseY);
    if (!items.empty())
    {
      float alpha = 0.5f;
      for (uint32 i = 0; i < items.size(); i++)
      {
        int32 ii = items.size() - 1 - i;
        nuiHTMLItem* pItem = items[ii];
        
        pContext->SetFillColor(nuiColor(0.0, 0.0, 1.0, alpha * .5f));
        pContext->SetStrokeColor(nuiColor(0.0, 0.0, 1.0, alpha));

        nuiRect r(pItem->GetGlobalRect());
        pContext->DrawRect(r, eStrokeAndFillShape);
        
        alpha *= 0.5f;
      }
    }
  }
  return true;
}

void nuiHTMLView::SetIdealWidth(float IdealWidth)
{
  if (mIdealWidth == IdealWidth)
    return;
  mIdealWidth = IdealWidth;
  InvalidateLayout();
}

float nuiHTMLView::GetIdealWidth() const
{
  return mIdealWidth;
}

float nuiHTMLView::GetVSpace() const
{
  return mVSpace;
}

float nuiHTMLView::GetHSpace() const
{
  return mHSpace;
}

void nuiHTMLView::SetVSpace(float VSpace)
{
  mVSpace = VSpace;
  InvalidateLayout();
}

void nuiHTMLView::SetHSpace(float HSpace)
{
  mHSpace = HSpace;
  InvalidateLayout();
}

bool nuiHTMLView::SetText(const nglString& rHTMLText)
{
  Cancel();
  Clear();
  
  if (rHTMLText.IsNull())
    return true;
  
  nuiHTML* pHTML = new nuiHTML();
  
  std::string str(rHTMLText.GetStdString());
  nglIMemory mem(&str[0], str.size());
  bool res = pHTML->Load(mem);
  
  if (res)
  {
    Clear();
    delete mpHTML;
    mpHTML = pHTML;
    mpRootBox = new nuiHTMLBox(mpHTML, mpCurrentAnchor, false);
    mpRootBox->SetLayoutChangedDelegate(nuiMakeDelegate(this, &nuiHTMLView::InvalidateLayout));
    mpRootBox->SetDisplayChangedDelegate(nuiMakeDelegate(this, &nuiHTMLView::Invalidate));
    ParseTree(mpHTML, mpRootBox);

    nuiHTMLContext context(*mpContext);
    mpRootBox->Layout(context);
    InvalidateLayout();
    SetHotRect(nuiRect());
  }
  return res;
}

bool nuiHTMLView::SetURL(const nglString& rURL)
{
  Cancel();
  
  nglString url(rURL);
  mTempURL = rURL;

  mpStream = new nuiAsyncIStream(rURL, true);
  mSlotSink.Connect(mpStream->StreamReady, nuiMakeDelegate(this, &nuiHTMLView::StreamDone));
  return true;
}

void nuiHTMLView::StreamDone(nuiAsyncIStream* pStream)
{
//  App->GetLog().SetLevel(_T("StopWatch"), 50);
//  nuiStopWatch watch(_T("nuiHTMLView::StreamDone"));
  mpStream = NULL;
  nglString url(mTempURL);
  const nuiHTTPResponse* pResponse = pStream->GetHTTPResponse();
  URLChanged(url);
  
  
  nglTextEncoding encoding = eEncodingUnknown;

  if (pResponse)
  {
    //NGL_OUT(_T("\n\nHTTP Headers:\n%ls\n\n"), pResponse->GetHeadersRep().GetChars());
    const nuiHTTPHeaderMap& rHeaders(pResponse->GetHeaders());
    nuiHTTPHeaderMap::const_iterator it = rHeaders.find(_T("location"));
    if (it != rHeaders.end())
    {
      nglString newurl = it->second;
      if (newurl[0] == '/')
      {
        url.TrimRight('/');
        url += newurl;
      }
      else
      {
        url = newurl;
      }
      //NGL_OUT(_T("\n\nNew location: %ls\n\n"), url.GetChars());
      
      SetURL(url);
      return;
    }
    
    it = rHeaders.find(_T("content-type"));
    
    if (it != rHeaders.end())
    {  
      nglString contents(it->second);
      contents.ToUpper();
      int32 pos = contents.Find(_T("CHARSET="));
      if (pos >= 0)
      {
        nglString enc(contents.Extract(pos + 8));
        enc.Trim();
        encoding = nuiGetTextEncodingFromString(enc);
        //NGL_OUT(_T("\n\nHTTP Encoding: %ls - %d\n\n"), enc.GetChars(), encoding);
        
      }
    }
  }  
  
  nuiHTML* pHTML = new nuiHTML();
  pHTML->SetSourceURL(url);
  bool res = pHTML->Load(*pStream, encoding);

  //  watch.AddIntermediate(_T("HTML Loaded"));

  if (res)
  {
    Clear();
    delete mpHTML;
    mpHTML = pHTML;
    mpRootBox = new nuiHTMLBox(mpHTML, mpCurrentAnchor, false);
    mpRootBox->SetLayoutChangedDelegate(nuiMakeDelegate(this, &nuiHTMLView::InvalidateLayout));
    mpRootBox->SetDisplayChangedDelegate(nuiMakeDelegate(this, &nuiHTMLView::Invalidate));
    ParseTree(mpHTML, mpRootBox);

    //    watch.AddIntermediate(_T("HTML Tree Parsed"));
    
    nuiHTMLContext context(*mpContext);
    mpRootBox->Layout(context);
    //    watch.AddIntermediate(_T("HTML Layouted"));
    InvalidateLayout();
    SetHotRect(nuiRect());
  }
}

const nglString& nuiHTMLView::GetURL() const
{
  if (mpHTML)
    return mpHTML->GetSourceURL();
  return nglString::Null;
}

void nuiHTMLView::ParseTree(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  uint32 count = pNode->GetNbChildren();
  for (uint32 i = 0; i < count; i++)
  {
    nuiHTMLNode* pChild = pNode->GetChild(i);
    switch (pChild->GetTagType())
    {
      case nuiHTML::eTag_HTML:
        {
          ParseHTML(pChild, pBox);
          return;
        }
        break;
      default:
        {
//          printf("tree??? '%ls'\n", pChild->GetName().GetChars());
//          ParseTree(pChild, pBox); // Try all children!!!
        }
        break;
    }
  }
}

void nuiHTMLView::ParseHTML(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  uint32 count = pNode->GetNbChildren();
  for (uint32 i = 0; i < count; i++)
  {
    nuiHTMLNode* pChild = pNode->GetChild(i);
    switch (pChild->GetTagType())
    {
      case nuiHTML::eTag_HEAD:
        ParseHead(pChild, pBox);
        break;
        
      case nuiHTML::eTag_BODY:
        ParseBody(pChild, pBox);
        break;
        
      default:
        {        
          //printf("html??? '%ls'\n", pChild->GetName().GetChars());
        }
        break;
    }
  }
}

void nuiHTMLView::ParseHead(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  uint32 count = pNode->GetNbChildren();
  for (uint32 i = 0; i < count; i++)
  {
    nuiHTMLNode* pChild = pNode->GetChild(i);
    switch (pChild->GetTagType())
    {
      case nuiHTML::eTag_TITLE:
        ParseTitle(pChild, pBox);
        break;
      default:
        {        
          //printf("head??? '%ls'\n", pChild->GetName().GetChars());
        }
        break;
    }
  }
}

void nuiHTMLView::ParseTitle(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
}

void nuiHTMLView::ParseBody(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  uint32 count = pNode->GetNbChildren();
  for (uint32 i = 0; i < count; i++)
  {
    nuiHTMLNode* pChild = pNode->GetChild(i);
    switch (pChild->GetTagType())
    {
      case nuiHTML::eTag_DIV:
        ParseDiv(pChild, pBox);
        break;
      case nuiHTML::eTag_TABLE:
        ParseTable(pChild, pBox);
        break;
      case nuiHTML::eTag_IMG:
        ParseImage(pChild, pBox);
        break;
      case nuiHTML::eTag_UL:
      case nuiHTML::eTag_OL:
      case nuiHTML::eTag_DL:
        ParseList(pChild, pBox);
        break;
      case nuiHTML::eTag_P:
        ParseP(pChild, pBox);
        break;
      case nuiHTML::eTag_H1:
      case nuiHTML::eTag_H2:
      case nuiHTML::eTag_H3:
      case nuiHTML::eTag_H4:
      case nuiHTML::eTag_H5:
      case nuiHTML::eTag_H6:
        ParseHeader(pChild, pBox);
        break;
      case nuiHTML::eTag_I:
      case nuiHTML::eTag_B:
      case nuiHTML::eTag_U:
      case nuiHTML::eTag_STRIKE:
      case nuiHTML::eTag_STRONG:
      case nuiHTML::eTag_EM:
        ParseFormatTag(pChild, pBox);
        break;
      case nuiHTML::eTag_BR:
        ParseBr(pChild, pBox);
        break;
      case nuiHTML::eTag_A:
        ParseA(pChild, pBox);
        break;
      case nuiHTML::eTag_SPAN:
        ParseSpan(pChild, pBox);
        break;
      case nuiHTML::eTag_FONT:
        ParseFont(pChild, pBox);
        break;
      case nuiHTML::eTag_SCRIPT:
      case nuiHTML::eTag_COMMENT:
        // Skip those tags
        break;
      default:
        if (pChild->GetName().IsEmpty())
          ParseText(pChild, pBox);
        else
        {
          //printf("body??? '%ls'\n", pChild->GetName().GetChars());
          ParseBody(pChild, pBox);
        }
        break;
    }
  }
}

void nuiHTMLView::ParseText(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  const nglString& rText(pNode->GetText());
  std::vector<nglString> words;
  rText.Tokenize(words);
  
  for (uint32 i = 0; i < words.size(); i++)
  {
    pBox->AddItem(new nuiHTMLText(pNode, mpCurrentAnchor, words[i]));
  }
  //ParseBody(pNode, pBox);
}

void nuiHTMLView::ParseDiv(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  nuiHTMLBox* pNewBox = new nuiHTMLBox(pNode, mpCurrentAnchor, false);
  pBox->AddItem(pNewBox);
  
  ParseBody(pNode, pNewBox);
}

void nuiHTMLView::ParseTable(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  nuiHTMLBox* pNewBox = new nuiHTMLBox(pNode, mpCurrentAnchor, false);
  pBox->AddItem(pNewBox);
  
  uint32 count = pNode->GetNbChildren();
  for (uint32 i = 0; i < count; i++)
  {
    nuiHTMLNode* pChild = pNode->GetChild(i);
    switch (pChild->GetTagType())
    {
      case nuiHTML::eTag_TR:
        ParseTableRow(pChild, pNewBox);
        break;
    }
  }
}

void nuiHTMLView::ParseImage(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  nuiHTMLImage* pImg = new nuiHTMLImage(pNode, mpCurrentAnchor);
  pBox->AddItem(pImg);
}

void nuiHTMLView::ParseTableRow(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  nuiHTMLBox* pNewBox = new nuiHTMLBox(pNode, mpCurrentAnchor, false);
  pBox->AddItem(pNewBox);
  
  uint32 count = pNode->GetNbChildren();
  for (uint32 i = 0; i < count; i++)
  {
    nuiHTMLNode* pChild = pNode->GetChild(i);
    switch (pChild->GetTagType())
    {
      case nuiHTML::eTag_TD:
        ParseBody(pChild, pNewBox);
        break;
    }
  }
}


void nuiHTMLView::ParseList(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  //printf("html list\n");
  nuiHTMLBox* pListBox = new nuiHTMLBox(pNode, mpCurrentAnchor, false);
  pBox->AddItem(pListBox);
  
  uint32 count = pNode->GetNbChildren();
  for (uint32 i = 0; i < count; i++)
  {
    nuiHTMLNode* pListItem = pNode->GetChild(i);
    switch (pListItem->GetTagType())
    {
      case nuiHTML::eTag_LI:
      {
        nuiHTMLBox* pListItemBox = new nuiHTMLBox(pListItem, mpCurrentAnchor, false);
        pListBox->AddItem(pListItemBox);
        ParseBody(pListItem, pListItemBox);
      }
      break;
    }
  }
  //printf("html /list\n");
}

void nuiHTMLView::ParseP(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  nuiHTMLBox* pNewBox = new nuiHTMLBox(pNode, mpCurrentAnchor, false);
  pNewBox->ForceLineBreak(true);
  pBox->AddItem(pNewBox);
  
  ParseBody(pNode, pNewBox);
}

void nuiHTMLView::ParseHeader(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  nuiHTMLBox* pNewBox = new nuiHTMLHeader(pNode, mpCurrentAnchor);
  pBox->AddItem(pNewBox);
  
  ParseBody(pNode, pNewBox);
}

void nuiHTMLView::ParseFormatTag(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  pBox->AddItem(new nuiHTMLItem(pNode, mpCurrentAnchor, true));
  ParseBody(pNode, pBox);
  pBox->AddItemEnd(new nuiHTMLItem(pNode, mpCurrentAnchor, true));
}

void nuiHTMLView::ParseA(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  mpCurrentAnchor = pNode;
  pBox->AddItem(new nuiHTMLItem(pNode, mpCurrentAnchor, true));
  ParseBody(pNode, pBox);
  pBox->AddItemEnd(new nuiHTMLItem(pNode, mpCurrentAnchor, true));
  mpCurrentAnchor = NULL;
}

void nuiHTMLView::ParseBr(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  pBox->AddItem(new nuiHTMLItem(pNode, mpCurrentAnchor, false));
}

void nuiHTMLView::ParseSpan(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  ParseBody(pNode, pBox);
}

void nuiHTMLView::ParseFont(nuiHTMLNode* pNode, nuiHTMLBox* pBox)
{
  pBox->AddItem(new nuiHTMLFont(pNode, mpCurrentAnchor));
  
  ParseBody(pNode, pBox);
  pBox->AddItemEnd(new nuiHTMLFont(pNode, mpCurrentAnchor));
}

void nuiHTMLView::_SetText(const nglString& rHTMLText)
{
  SetText(rHTMLText);
}

bool nuiHTMLView::MouseClicked(const nglMouseInfo& rInfo)
{
  if (!mpRootBox)
    return false;
  
  if (rInfo.Buttons & nglMouseInfo::ButtonLeft)
  {
    Grab();
    mClicked = true;
  }
  return false;
}

bool nuiHTMLView::MouseUnclicked(const nglMouseInfo& rInfo)
{
  if (mClicked)
  {
    if (rInfo.Buttons & nglMouseInfo::ButtonLeft)
    {
      std::vector<nuiHTMLItem*> items;
      mpRootBox->GetItemsAt(items, rInfo.X, rInfo.Y);
      if (!items.empty())
      {
        for (uint32 i = 0; i < items.size(); i++)
        {
          int32 ii = items.size() - 1 - i;
          nuiHTMLItem* pItem = items[ii];
          nuiHTMLNode* pNode = pItem->GetAnchor();
          if (pNode)
          {
            nuiHTMLAttrib* pAttrib = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_HREF);
            if (pAttrib)
            {
              nglString url(pAttrib->GetValue());
              nuiHTML::GetAbsoluteURL(mpHTML->GetSourceURL(), url);

              LinkActivated(url);
              return true;
            }
          }
        }
      }
      Ungrab();
    }
  }
  return false;
}

bool nuiHTMLView::MouseMoved(const nglMouseInfo& rInfo)
{
  if (!mpRootBox)
    return false;
  
  mMouseX = rInfo.X;
  mMouseY = rInfo.Y;

  if (mDebugBoxes)
    Invalidate();
  
  std::vector<nuiHTMLItem*> items;
  mpRootBox->GetItemsAt(items, rInfo.X, rInfo.Y);
  if (!items.empty())
  {
    for (uint32 i = 0; i < items.size(); i++)
    {
      int32 ii = items.size() - 1 - i;
      nuiHTMLItem* pItem = items[ii];
      nuiHTMLNode* pNode = pItem->GetAnchor();
      if (pNode)
      {
        nuiHTMLAttrib* pAttrib = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_HREF);
        if (pAttrib)
        {
          nglString url(pAttrib->GetValue());
          nuiHTML::GetAbsoluteURL(mpHTML->GetSourceURL(), url);

          SetToolTip(url);
          SetMouseCursor(eCursorArrow);
          return true;
        }
      }
    }
  }

  SetToolTip(nglString::Empty);
  SetMouseCursor(eCursorCaret);
  return false;
}


void nuiHTMLView::_SetURL(const nglString& rURL)
{
  SetURL(rURL);
}

void nuiHTMLView::_AutoSetURL(const nglString& rURL)
{
  if (mAutoActivateLink)
  {
    SetURL(rURL);
  }
}

void nuiHTMLView::SetAutoActivateLink(bool set)
{
  mAutoActivateLink = set;
}

bool nuiHTMLView::IsAutoActivateLink() const
{
  return mAutoActivateLink;
}

void nuiHTMLView::SetDebugBoxes(bool set)
{
  mDebugBoxes = set;
  Invalidate();
}

bool nuiHTMLView::GetDebugBoxed() const
{
  return mDebugBoxes;
}

void nuiHTMLView::SetAutoIdealWidth(bool set)
{
  mAutoIdealWidth = set;
  if (set)
    InvalidateLayout();
}

bool nuiHTMLView::GetAutoIdealWidth() const
{
  return mAutoIdealWidth;
}


