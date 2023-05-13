// CaptionButton.cpp : implementation file
//
// Written by Anish Mistry
// Anish Mistry's e-mail: amistry@am-productions.yi.org
// AM Productions Website http://am-productions.yi.org/
/* This code is licensed under the GNU GPL.  See License.txt or (http://www.gnu.org/copyleft/gpl.html). */
// created 10/19/1999 mm/dd/yy
// updated 12/24/2000
// updated 04/01/2001
// updated 04/12/2001
// updated 04/16/2001 improved drawing and performance +100%, by replacing the 
// DrawTransparentBitmap() function.
// updated 04/17/2001 removed the unessecary SetWindowType() and SetCaptionType().
// added this detection in the Init() function.
// updated 04/28/2001 fixed the bug where the captions wouldn't show in child
// windows.  Made a modification to the CalculateRect() function.

#include <windows.h>
#include <shellapi.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <commctrl.h>
#include "CaptionButton.h"

/////////////////////////////////////////////////////////////////////////////
// CCaptionButton
CCaptionButton::CCaptionButton()
{// begin constuctor
	ThunkInit(m_Thunk,this);
	// initilize caption variables
	m_nNumCaptions = 0;
	m_crTransparentColor = NULL;	// set default no transparent color
	m_nNumDefaultCaptions = NULL;	// set to null will assume no captions
	m_fExStyles = 0;
	m_fStyles = 0;
	m_nDownNode = -1;
	m_nLastPointNode = -1;
	m_hSelectionBitmap = NULL;
	m_bCapturing = false;
	m_hWnd = NULL;
	m_bMyRedraw = false;
	m_wpOldWndProc = NULL;
	m_hToolTipWnd = NULL;
}// end constuctor

CCaptionButton::~CCaptionButton()
{// begin destructor
	// cleanup
	OnDestroy();
	// delete the linked list
	DeleteList();
}// end destructor

/***********  declareations of CCaptionButton ***********/
void CCaptionButton::Init(HWND hOwner)
{ // begin Init

	m_hWnd = hOwner;
	// subclass owner's m_hWnd message function
	m_wpOldWndProc = (WNDPROC)::SetWindowLong(m_hWnd,GWL_WNDPROC,(LONG)(void *)m_Thunk);
	if(!m_hToolTipWnd)
	{// begin init tooltips
		// Ensure that the common control DLL is loaded  
		InitCommonControls();
		// create a m_hToolTipWnd control. 
		m_hToolTipWnd = CreateWindow( TOOLTIPS_CLASS, NULL,
								WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
								CW_USEDEFAULT, CW_USEDEFAULT,
								CW_USEDEFAULT, CW_USEDEFAULT,
								m_hWnd, NULL, NULL,	NULL);

		SetWindowPos(m_hToolTipWnd, HWND_TOPMOST,0, 0, 0, 0,
					 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}// end init tooltips
	
	// get the normal styles
	m_fStyles = GetWindowLong(m_hWnd,GWL_STYLE);
	// get the extra styles
	m_fExStyles = GetWindowLong(m_hWnd,GWL_EXSTYLE);
	// get upper left corner
	RECT rWindowRect = {NULL};
	GetWindowRect(m_hWnd,&rWindowRect);
} // end Init
/*************************************************/
void CCaptionButton::Draw(int nIndex, bool bHilited) const
{ // begin Draw
	// declarations
	//SIZE szCaptionSize = GetCaptionSize();

	HDC hdcWin = ::GetWindowDC(m_hWnd);

	HBITMAP hBitmap = NULL;
	if(bHilited)
	{// begin hilited
		hBitmap = m_llCaptions[nIndex].hHiliteBitmap;
	}// end hilited
	else
	{// begin not hitiled
		hBitmap = m_llCaptions[nIndex].hNormalBitmap;
	}// end not hilited

	// get the bitmap's dimensions
	BITMAP bmDimensions = {NULL};
	::GetObject(m_llCaptions[nIndex].hNormalBitmap, sizeof( bmDimensions ), &bmDimensions );

	// center the bitmap in the caption area
	SIZE szSpacing = {bmDimensions.bmWidth,bmDimensions.bmHeight};
	szSpacing = CenterBitmap(szSpacing);

	// the the captions area
	RECT rWindowRect = {NULL};
	GetWindowRect(m_hWnd,&rWindowRect);
	// get caption rectangle
	RECT rCaptionRect = CalculateRect(nIndex+1 + m_nNumDefaultCaptions,false);
	// draw it
	DrawTransparentBitmap(hdcWin
		,hBitmap
		,szSpacing.cx+rCaptionRect.left-rWindowRect.left
		,szSpacing.cy+rCaptionRect.top-rWindowRect.top
		,m_crTransparentColor);

	// cleanup
	::ReleaseDC(m_hWnd,hdcWin);

} // end Draw
/*************************************************/
bool CCaptionButton::New(const UINT ID,HBITMAP hilite,HBITMAP normal,char *tooltipString,int nInsertPostion)
// the passed in HBITMAP's will be need to be deleted by the user if the function returns false
// not passing in nInsertPosition inserts the caption at the end of the list (left most)
{ // begin New

	if(GetNode(ID) > -1)
	{// begin already a node with that ID
		return false;
	}// end already a node with that ID

	if(!hilite)
		hilite = normal;

	CCaption cbCurrent = CreateCaption(ID,hilite,normal,tooltipString);
	Insert(&cbCurrent,nInsertPostion);
	if(nInsertPostion > -1)
		ReInitCaptions();
	RedrawCaptions();
	return true;	
} // end New
int CCaptionButton::GetNode(const UINT &find) const
{ // begin GetNode
	for(int i = 0;i < m_llCaptions.size();i++)
	{ // begin
		if(m_llCaptions[i].nID == find)
			return i+1;
	} // end
	return -1;
} // end GetNode
/*************************************************/
void CCaptionButton::DeleteList(void)
{ // begin DeleteList
	int nSize = m_llCaptions.size()-1;

	for(int i = nSize;i > -1;i--)
	{ // begin delete
		DeleteCaption(i);
	} // end delete
} // end DeleteList
/*************************************************/
void CCaptionButton::DeleteNode(const UINT &nNodeID)
{ // begin DeleteNode
	// get the index of the caption
	int nIndex = GetNode(nNodeID)-1;
	if(nIndex < 0)
		return;
	// create temp region
	HRGN hTempRgn = NULL;
	RECT rRect = {NULL};
	GetRgnBox(m_llCaptions[m_llCaptions.size()-1].hRgn,&rRect);
	hTempRgn = CreateRectRgnIndirect(&rRect);
	// free the node
	DeleteCaption(nIndex);
	m_llCaptions.erase(&m_llCaptions[nIndex]);
	// force a resize
	m_llCaptions.resize(m_llCaptions.size());
	// need to clear the caption last area since it won't be overdrawn
	UpdateRegion(hTempRgn);
	// cleanup
	DeleteObject(hTempRgn);
} // end DeleteNode
/*************************************************/
SIZE CCaptionButton::CenterBitmap(const SIZE &size) const
{// begin CenterBitmap
	SIZE temp = {(::GetSystemMetrics(SM_CXSIZE)-size.cx)/2,(::GetSystemMetrics(SM_CYSIZE)-size.cy)/2};
	return temp;
}// end CenterBitmap
/*************************************************/
bool CCaptionButton::Insert(CCaptionButton::CCaption *insert,int nInsertPosition)
{// begin Insert
	if(nInsertPosition == -1)
	{// begin insert it as the first caption
		nInsertPosition = m_nNumCaptions-1;
	}// end insert it as the first caption
	m_llCaptions.insert(m_llCaptions.begin()+nInsertPosition,*insert);
	return true;
}// end Insert
/*************************************************/
LRESULT CCaptionButton::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{// begin WindowProc
	switch(message)
	{ // begin switch

	case WM_SIZING:
	case WM_MOVING:	
	case WM_MOVE:
	case WM_SIZE:
		OnWindowChange();
		break;
	case WM_NCPAINT:
		if(m_bMyRedraw)
			break;
		return OnNcPaint(message,wParam,lParam);
	case WM_SETTEXT:
	case WM_NCACTIVATE: 
		return OnNcPaint(message,wParam,lParam);
	case WM_SYSCOLORCHANGE: case WM_SETTINGCHANGE:
		OnSystemChange();
		break;
	case WM_NCMOUSEMOVE:
		if(m_bCapturing)
			break;
	case WM_MOUSEMOVE:
		OnMouseMove(message,wParam,lParam);
		break;
	case WM_NCLBUTTONDBLCLK:
	case WM_NCLBUTTONDOWN:
		if(!OnNcLButtonDown(message,wParam,lParam))
			return NULL;
		break;
	case WM_LBUTTONUP:
		if(OnLButtonUp(message,wParam,lParam))
			break;
	case WM_NCLBUTTONUP:
		if(!OnNcLButtonUp(message,wParam,lParam))
			return NULL;
		break;
	case WM_DESTROY:
		OnDestroy();
		break;
	} // end switch

	// pass the message to the owner m_hWnd's WndProc function
	return CallWindowProc(m_wpOldWndProc,hwnd,message,wParam,lParam);
}// end WindowProc
/*************************************************/
void CCaptionButton::SetTransparentColor(const int &color)
// if color == NULL there is transparent color
// use COLORREF(r,g,b) for the color varialbe
{// begin SetTransparentColor
	m_crTransparentColor = color;
}// end SetTransparentColor
/*************************************************/
void CCaptionButton::SetNumOfDefaultCaptions(const UINT &type)
// set the number of caption spaces to leave to the right of the first caption
{// begin SetNumOfDefaultCaptions
	m_nNumDefaultCaptions = type;
}// end SetNumOfDefaultCaptions
/*************************************************/
SIZE CCaptionButton::GetCaptionSize() const
// get the size of a caption for the current window
{// begin GetCaptionSize
	SIZE szCaptionSize = {NULL};
	if(m_fExStyles & WS_EX_TOOLWINDOW)
	{// begin toolwindow
		szCaptionSize.cx = GetSystemMetrics(SM_CXSMSIZE);
		szCaptionSize.cy = GetSystemMetrics(SM_CYSMSIZE);
	}// end toolwindow
	else
	{// begin appwindow
		szCaptionSize.cx = GetSystemMetrics(SM_CXSIZE);
		szCaptionSize.cy = GetSystemMetrics(SM_CYSIZE);
	}// end appwindow
	return szCaptionSize;
}// end GetCaptionSize
/*************************************************/
SIZE CCaptionButton::GetFrameSize() const
// gets the width of the window's frame
{// begin GetFrameSize
	SIZE sFrameSize = {NULL};
	if(m_fStyles & WS_THICKFRAME)
	{// begin resizing frame
		sFrameSize.cx = GetSystemMetrics(SM_CXSIZEFRAME);
		sFrameSize.cy = GetSystemMetrics(SM_CYSIZEFRAME);
	}// end resizing frame
	else
	{// begin non resizing frame
		sFrameSize.cx = GetSystemMetrics(SM_CXFIXEDFRAME);
		sFrameSize.cy = GetSystemMetrics(SM_CYFIXEDFRAME);
	}// end non resizing frame
/*	SIZE frameSize = {NULL};
	POINT screenPoint,clientPoint;
	RECT windowRect,clientRect;
	GetWindowRect(m_hWnd,&windowRect);
	GetClientRect(m_hWnd,&clientRect);
	screenPoint.x = windowRect.left;
	screenPoint.y = windowRect.bottom;
	clientPoint.x = 0;
	clientPoint.y = clientRect.bottom;

	ClientToScreen(m_hWnd,&clientPoint);
	frameSize.cx = clientPoint.x - screenPoint.x;
	frameSize.cy = screenPoint.y - clientPoint.y;
*/
	return sFrameSize;
}// end GetFrameSize
/*************************************************/
void CCaptionButton::Delete(const UINT &ID)
// removes the caption with the ID
{// begin Delete
	DeleteNode(ID);
	m_nNumCaptions--;
	ReInitCaptions();
	RedrawCaptions();
}// end Delete
/*************************************************/
void CCaptionButton::RedrawCaptions() const
// redraw all of the captions, taking special care of the currently selected one
{// begin RedrawCaptions
	for(int i = 0;i < m_llCaptions.size();i++)
	{ // begin
		if(m_nLastPointNode == i)
			Draw(i,true);
		else
			Draw(i,false);
	} // end
	// draw the node with the focus
	if(m_nDownNode != -1)
		DrawSelection(m_nDownNode);
}// end RedrawCapions
/*************************************************/
HRGN CCaptionButton::CalculateRgn(const int &offset) const
// creates a HRGN for a caption a the offset
{// begin CalculateRgn
	RECT captionRect = CalculateRect(offset,false);
	return CreateRectRgnIndirect(&captionRect);
}// end CalculateRgn
/* // old & very slow transparent drawing technique
void CCaptionButton::DrawTransparentBitmap(HDC hdc, HBITMAP hBitmap,const int &xStart,
                           const int &yStart,const COLORREF &cTransparentColor)
// code from MSDN Article ID: Q79212 
 {// begin DrawTransparentBitmap
    BITMAP     bm;   COLORREF   cColor;
    HBITMAP    bmAndBack, bmAndObject, bmAndMem, bmSave;
    HGDIOBJ    bmBackOld, bmObjectOld, bmMemOld, bmSaveOld, hdcTempOld;
    HDC        hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;   
    POINT      ptSize;
    
    hdcTemp = CreateCompatibleDC(hdc);
    hdcTempOld = SelectObject(hdcTemp, hBitmap);   // Select the bitmap
    GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);
    ptSize.x = bm.bmWidth;            // Get width of bitmap
    ptSize.y = bm.bmHeight;           // Get height of bitmap
    DPtoLP(hdcTemp, &ptSize, 1);      // Convert from device
                                      // to logical points
    // Create some DCs to hold temporary data.
    hdcBack   = CreateCompatibleDC(hdc);
	hdcObject = CreateCompatibleDC(hdc);
    hdcMem    = CreateCompatibleDC(hdc);
	hdcSave   = CreateCompatibleDC(hdc);
    // Create a bitmap for each DC. DCs are required for a number of
    // GDI functions.   // Monochrome DC
    bmAndBack   = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
    // Monochrome DC
    bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
    bmAndMem    = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
    bmSave      = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
    // Each DC must select a bitmap object to store pixel data.
    bmBackOld   = SelectObject(hdcBack, bmAndBack);
    bmObjectOld = SelectObject(hdcObject, bmAndObject);
    bmMemOld    = SelectObject(hdcMem, bmAndMem);
    bmSaveOld   = SelectObject(hdcSave, bmSave);
	// Set proper mapping mode.
    SetMapMode(hdcTemp, GetMapMode(hdc));
    // Save the bitmap sent here, because it will be overwritten.
    BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);
    // Set the background color of the source DC to the color.
    // contained in the parts of the bitmap that should be transparent
    cColor = SetBkColor(hdcTemp, cTransparentColor);
    // Create the object mask for the bitmap by performing a BitBlt
    // from the source bitmap to a monochrome bitmap.
    BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0,          SRCCOPY);
    // Set the background color of the source DC back to the original   // color.
    SetBkColor(hdcTemp, cColor);   // Create the inverse of the object mask.
    BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, NOTSRCCOPY);
    // Copy the background of the main DC to the destination.
    BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdc, xStart, yStart, SRCCOPY);   // Mask out the places where the bitmap will be placed.
    BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);
    // Mask out the transparent colored pixels on the bitmap.
    BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack, 0, 0, SRCAND);
    // XOR the bitmap with the background on the destination DC.
    BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);
    // Copy the destination to the screen.
    BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y, hdcMem, 0, 0, SRCCOPY);
    // Place the original bitmap back into the bitmap sent here.
    BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave, 0, 0, SRCCOPY);
    // Delete the memory bitmaps.
	SelectObject(hdcBack, bmBackOld);
    SelectObject(hdcObject, bmObjectOld);
    SelectObject(hdcMem, bmMemOld);
    SelectObject(hdcSave, bmSaveOld);
	SelectObject(hdcTemp,hdcTempOld);
	// delete the bitmaps
	DeleteObject(bmAndBack);
 	DeleteObject(bmAndObject);
	DeleteObject(bmAndMem);   
	DeleteObject(bmSave);

	DeleteObject(bmBackOld);
 	DeleteObject(bmObjectOld);
	DeleteObject(bmMemOld);   
	DeleteObject(bmSaveOld);
	DeleteObject(hdcTempOld);
	// Delete the memory DCs.
    DeleteDC(hdcMem);
	DeleteDC(hdcBack);
	DeleteDC(hdcObject);
    DeleteDC(hdcSave);
	DeleteDC(hdcTemp);   
}// end DrawTransparentBitmap
*/
RECT CCaptionButton::CalculateRect(const int &offset,bool toClient) const
// calculate the rectangular area of the caption at the offset
{// begin CalculateRect
	
	RECT windowRect;
	SIZE frameSize = GetFrameSize();
	SIZE captionSize = GetCaptionSize();
	::GetWindowRect(m_hWnd,&windowRect);

	RECT captionRect = {NULL};
	captionRect.left = windowRect.right-(captionSize.cx*offset+frameSize.cx);
	captionRect.top = windowRect.top+frameSize.cy;
	captionRect.right = captionRect.left+captionSize.cx;
	captionRect.bottom = captionRect.top+captionSize.cy;

	// check for partially covered up menu, and compensate
	RECT menuRect = {NULL};
	HMENU windowMenu = GetMenu(m_hWnd);
	if(windowMenu)
	{
		GetMenuItemRect(m_hWnd,windowMenu,0,&menuRect);
		if(menuRect.bottom != 0)	// if it is not a child window
		{
			captionRect.bottom = menuRect.top-1;
			captionRect.top = captionRect.bottom-captionSize.cy;
		}
	}

	if(toClient)
	{
		POINT topLeft,bottomRight;
		topLeft.x = captionRect.left;
		topLeft.y = captionRect.top;
		bottomRight.x = captionRect.right;
		bottomRight.y = captionRect.bottom;

		ScreenToClient(m_hWnd,&topLeft);
		ScreenToClient(m_hWnd,&bottomRight);
		captionRect.left = topLeft.x;
		captionRect.top = topLeft.y;
		captionRect.right = bottomRight.x;
		captionRect.bottom = bottomRight.y;
	}
	return captionRect;
}// end CalculateRect

void CCaptionButton::DrawSelection(const int &num) const
// draw the selection state bitmap
{// begin DrawSelection
	// declarations
	//SIZE captionSize = GetCaptionSize();

	HDC hdcWin = ::GetWindowDC(m_hWnd);

	// get bitmap's dimensions
	BITMAP bitmapDimentions = {NULL};
	::GetObject(m_hSelectionBitmap, sizeof( bitmapDimentions ), &bitmapDimentions );
	SIZE spacingSize = {bitmapDimentions.bmWidth,bitmapDimentions.bmHeight}; 

	// center the bitmap in the caption area
	spacingSize = CenterBitmap(spacingSize);

	RECT windowRect = {NULL};
	GetWindowRect(m_hWnd,&windowRect);
	// get caption rectangle
	RECT captionRect = CalculateRect(num+1 + m_nNumDefaultCaptions,false);
	// draw the selection bitmap
	DrawTransparentBitmap(hdcWin
		,m_hSelectionBitmap
		,spacingSize.cx+captionRect.left-windowRect.left
		,spacingSize.cy+captionRect.top-windowRect.top
		,m_crTransparentColor);

	// cleanup
	::ReleaseDC(m_hWnd,hdcWin);
}// end DrawSelection

void CCaptionButton::SetSelectionBitmap(HBITMAP bitmap)
// sets the mousedown bitmap
{// begin SetSelectionBitmap
	if(m_hSelectionBitmap)
		DeleteObject(m_hSelectionBitmap);
	m_hSelectionBitmap = bitmap;
}// end SetSelectionBitmap

bool CCaptionButton::Replace(const UINT replaceCaptionID,const UINT newID,HBITMAP newHilite,HBITMAP newNormal,char *newToolTip)
// replaces a current caption with a new one
{// begin Replace
	// find the node to replace
	for(int i = 0;i < m_llCaptions.size();i++)
	{// begin traverse
		if(m_llCaptions[i].nID == replaceCaptionID)
		{// begin replace caption
			// update tooltip text
			TOOLINFO ti,newTip;
			ti.cbSize = sizeof(TOOLINFO); 
			ti.hwnd = m_hWnd; 
			ti.uId = (UINT) m_llCaptions[i].nID;
			// close the tooltip (if it is open)
			::SendMessage(m_hToolTipWnd, TTM_POP , 0, 0);
			// delete the tooltip
			::SendMessage(m_hToolTipWnd, TTM_DELTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);
			// set new tooltip parameters
			newTip.cbSize = sizeof(TOOLINFO); 
			newTip.hwnd = m_hWnd; 
			newTip.uId = (UINT) m_llCaptions[i].nID;
			newTip.hinst = NULL; 
			newTip.uFlags = TTF_SUBCLASS; 
			newTip.rect = CalculateRect(i+1+m_nNumDefaultCaptions);
			newTip.lpszText = (LPSTR) newToolTip;
			newTip.uId = newID;
			// add tool tip
			::SendMessage(m_hToolTipWnd, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &newTip);
			// clean up the old caption's bitmaps
			::DeleteObject(m_llCaptions[i].hHiliteBitmap);
			::DeleteObject(m_llCaptions[i].hNormalBitmap);
			// set the new bitmaps
			m_llCaptions[i].hHiliteBitmap = newHilite;
			m_llCaptions[i].hNormalBitmap = newNormal;
			// set the new ID
			m_llCaptions[i].nID = newID;
			// this clear the region so the non - overlapping part does not show
			UpdateRegion(m_llCaptions[i].hRgn);
			// draw it
			Draw(i,false);
			// reset current node
			m_nLastPointNode = -1;
			return true;
		}// end replace caption
	}// end traverse
	return false;
}// end Replace

bool CCaptionButton::OnWindowChange()
// called when an aspect of the window's position changes
{// begin OnWindowChange
	ReInitCaptions();
	return true;
}// end OnWindowChange

LRESULT CCaptionButton::OnNcPaint(UINT message,WPARAM wParam, LPARAM lParam)
// called when the non-client region needs to be painted
{// begin OnNcPaint
	LRESULT lResult = CallWindowProc(m_wpOldWndProc,m_hWnd,message,wParam,lParam);
	RedrawCaptions();
	return lResult;
}// end OnNcPaint

LRESULT CCaptionButton::OnSystemChange()
// called when an aspect of the system changes
{// begin OnSystemChange
	RedrawCaptions();
	return 0;
}// end OnSystemChange

LRESULT CCaptionButton::OnMouseMove(UINT message,WPARAM wParam, LPARAM lParam)
// called when the cursor moves
{// begin OnMouseMove
	POINTS pt = {NULL};
	POINT pnt = {NULL};
	if(m_bCapturing)
	{// begin convert coords
		// convert cursor coordinates
		pnt.x = LOWORD(lParam);
		pnt.y = HIWORD(lParam);
		ClientToScreen(m_hWnd,&pnt);
		pt.x = (short)pnt.x;
		pt.y = (short)pnt.y;
	}// end convert coords
	else if(message != WM_MOUSEMOVE )
	{// begin not capturing
		pt = MAKEPOINTS(lParam);			
	}// end not capturing
	int nCurrentNode = -1;
	for(int i = 0;i < m_nNumCaptions;i++)
	{ // begin get current node
		if(nCurrentNode == -1 && ::PtInRegion(m_llCaptions[i].hRgn, pt.x,pt.y))
		{
			// set this as the current caption
			nCurrentNode = i;
			// done with this
			break;
		}
	} // end end get current node
	if(nCurrentNode != -1 && m_nLastPointNode != nCurrentNode)
	{ // begin Point in Region
			// check to see if the bitmaps are different
			if(m_llCaptions[nCurrentNode].hHiliteBitmap != m_llCaptions[nCurrentNode].hNormalBitmap)
			{// begin bitmaps are different
				// clear the drawing area
				UpdateRegion(m_llCaptions[nCurrentNode].hRgn);
				// the point is in the current region so draw it in hilited mode
				if(m_nDownNode != -1 && m_nDownNode != nCurrentNode)
					Draw(nCurrentNode,false);
				else
					Draw(nCurrentNode,true);
			}// end bitmaps are different
			// draw it in mouse down mode
			if(m_nDownNode == nCurrentNode)
				DrawSelection(nCurrentNode);
	} // end Point in Region
	if(m_nLastPointNode != -1 && m_nLastPointNode != nCurrentNode)
	{
		if(m_llCaptions[m_nLastPointNode].hHiliteBitmap != m_llCaptions[m_nLastPointNode].hNormalBitmap || m_nDownNode == m_nLastPointNode)
		{// begin bitmaps are different
			UpdateRegion(m_llCaptions[m_nLastPointNode].hRgn);
			// draw it in normal state
			Draw(m_nLastPointNode,false);
		}// end bitmaps are different
	}
	// set the last caption
	m_nLastPointNode = nCurrentNode;
	return 1;
}// end OnMouseMove

LRESULT CCaptionButton::OnNcLButtonDown(unsigned long message, WPARAM wParam, LPARAM lParam)
// called when the left mouse button is pressed in the non-client area
{// begin OnNcLButtonDown
	POINTS pt = MAKEPOINTS(lParam);
	for(int i = 0;i < m_llCaptions.size();i++)
	{ // begin traverse
		if(::PtInRegion(m_llCaptions[i].hRgn, pt.x,pt.y))
		{ // begin caption has been pressed
			// capture the mouse messages so that the cursor can be tracked outside of the window
			SetCapture(m_hWnd);
			m_bCapturing = true;
			m_nDownNode = i;
			DrawSelection(i);
			return 0;
		} // end caption has been pressed
	} // end traverse
	return 1;
}// end OnNcLButtonDown

LRESULT CCaptionButton::OnNcLButtonUp(unsigned long message, WPARAM wParam, LPARAM lParam)
// called when the left mouse button is released in the non-client area
{// begin OnNcLButtonUp
	POINTS pt = {NULL};
	if(message == WM_LBUTTONUP)
	{// begin convert point
		// convert client cursor coordinates to screen coordinates
		POINT point;
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
		ClientToScreen(m_hWnd,&point);
		pt.x = (short)point.x;
		pt.y = (short)point.y;
	}// end convert point
	else
		pt = MAKEPOINTS(lParam);
	if(m_nDownNode != -1)	
	{// begin the mouse was down
		if(::PtInRegion(m_llCaptions[m_nDownNode].hRgn, pt.x,pt.y))
		{ // begin mouse on  same caption
			if(GetNode(m_llCaptions[m_nDownNode].nID) == -1)
				return 0;
			// send the message to the window that a caption was pressed
			::SendMessage(m_hWnd,WM_CBLBUTTONCLICKED,m_llCaptions[m_nDownNode].nID,lParam);
			// clear the region
			UpdateRegion(m_llCaptions[m_nDownNode].hRgn);
			// draw it
			Draw(m_nDownNode,true);
		} // end mouse on  same caption
	}// end the mouse was down
	m_nDownNode = -1;
	return 1;
}// end OnNcLButtonUp

LRESULT CCaptionButton::OnDestroy()
// called when the window is about to be destroyed
{// begin OnDestroy
	// remove subclass of owner's m_hWnd message function
	if(m_wpOldWndProc)
	{
		SetWindowLong(m_hWnd,GWL_WNDPROC,(DWORD)m_wpOldWndProc);
	}
	// destroy the tooltip window
	if(m_hToolTipWnd)
	{
		SendMessage(m_hToolTipWnd,WM_DESTROY,NULL,NULL);
		m_hToolTipWnd = NULL;
	}
	// delete the selection bitmap
	if(m_hSelectionBitmap)
	{
		DeleteObject(m_hSelectionBitmap);
		m_hSelectionBitmap = NULL;
	}
	return 1;
}// end OnDestroy

LRESULT CCaptionButton::OnLButtonUp(unsigned long message, WPARAM wParam, LPARAM lParam)
// called when the left mouse button is press
// releases the capture of the mouse messages
{// begin OnLButtonUp
	if(m_bCapturing)
	{// begin currently capturing mouse
		ReleaseCapture();
		m_bCapturing = false;
		// reset the last node which was down
		m_nLastPointNode = -1;
		return 0;
	}// end currently capturing mouse
	return 1;
}// end OnLButtonUp

bool CCaptionButton::DeleteCaption(int nIndex)
// cleans up the caption at nIndex, but does no release the structure's memory
{// begin DeleteCaption
	// remove the tooltip
	TOOLINFO ti = {NULL};
	ti.hwnd = m_hWnd; 
	ti.uId = (UINT) m_llCaptions[nIndex].nID;
	ti.cbSize = sizeof(ti); 
	::SendMessage(m_hToolTipWnd, TTM_DELTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);
	// release all memory in node
	DeleteObject(m_llCaptions[nIndex].hNormalBitmap);
	DeleteObject(m_llCaptions[nIndex].hHiliteBitmap);
	DeleteObject(m_llCaptions[nIndex].hRgn);
	return true;
}// end DeleteCaption

bool CCaptionButton::UpdateRegion(HRGN hUpdateRegion)
// clears the non-client drawing area of hUpdateRegion
// non-client region that is to be updated
{// begin UpdateRegion
	if(!hUpdateRegion)
		return false;
	m_bMyRedraw = true;
	// this clear the region so the non-overlapping part does not show
	::SendMessage(m_hWnd,WM_NCPAINT,(WPARAM)hUpdateRegion,NULL);
	m_bMyRedraw = false;
	return true;
}// end UpdateRegion

CCaptionButton::CCaption CCaptionButton::CreateCaption(const UINT &ID, HBITMAP hilite, HBITMAP normal, char *tooltipString)
// returns a CCaption
{// begin CreateCaption
	CCaption cbNewCaption;
	// Calculate next caption pos
	m_nNumCaptions++;
	int captionOffset = m_nNumCaptions + m_nNumDefaultCaptions;
	// initilize caption params
	cbNewCaption.nID = ID;
	cbNewCaption.hNormalBitmap = normal;
	cbNewCaption.hHiliteBitmap = hilite;
	cbNewCaption.hRgn = CalculateRgn(captionOffset);
	
	// Create a new tool tip
	TOOLINFO ti = {NULL};
	ti.cbSize = sizeof(ti); 
	ti.uFlags = TTF_SUBCLASS; 
	ti.hwnd = m_hWnd; 
	ti.hinst = NULL; 
	ti.uId = (UINT) ID;
	ti.lpszText = (LPSTR) tooltipString;
	ti.rect = CalculateRect(captionOffset);
	// activate tool tip
    ::SendMessage(m_hToolTipWnd, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);
	return cbNewCaption;
}// end CreateCaption

void CCaptionButton::Swap(CCaption a, CCaption b) const
// duh...swap two captions
{// begin Swap
	CCaption temp;
	temp = a;
	a = b;
	b = temp;
}// end Swap

void CCaptionButton::ReInitCaptions()
// recalculates all of the captions' regions and updates the tooltips accordingly
{// begin ReInitCaptions
	int captionOffset = 0;
	// Recalculate caption pos
	for(int i = 0;i < m_llCaptions.size();i++)
	{ // begin traverse
		// calc the caption position
		captionOffset = i+1 + m_nNumDefaultCaptions;
		// clean up the previous region
		DeleteObject(m_llCaptions[i].hRgn);
		// create the new region
		m_llCaptions[i].hRgn = CalculateRgn(captionOffset);
		// prepare the tooltip
		TOOLINFO ti = {NULL};
		ti.cbSize = sizeof(TOOLINFO);
		ti.hwnd = m_hWnd;
		// calc the caption's rectangle
		ti.rect = CalculateRect(captionOffset);
		ti.uId = m_llCaptions[i].nID;
		ti.uFlags = TTF_SUBCLASS; 
		ti.hinst = NULL;
		// set the new position
		SendMessage(m_hToolTipWnd, TTM_NEWTOOLRECT, 0, (LPARAM) (LPTOOLINFO) &ti);	
	} // end traverse
}// end ReInitCaptions

bool CCaptionButton::New(const UINT nID,HICON hHilite,HICON hNormal,char *pTooltip,const int nInsertPostion)
// created a new caption using HICONs
// the HICON's are destroyed by this function
{// begin New
	HBITMAP hHiliteBitmap = IconToBitmap(hHilite,m_crTransparentColor);
	HBITMAP hNormalBitmap = IconToBitmap(hNormal,m_crTransparentColor);
	// clean up the icons
	DestroyIcon(hHilite);
	DestroyIcon(hNormal);
	return New(nID,hHiliteBitmap,hNormalBitmap,pTooltip,nInsertPostion);
}// end New

HBITMAP CCaptionButton::IconToBitmap(HICON hIcon, COLORREF crTrans)
// converts a HICON to a HBITMAP using the crTrans as the transparent color for the bitmap
// returns null if there is no icon
{// begin IconToBitmap
	if(!hIcon)
		return NULL;
	SIZE m_sDimensions = {16,16};
	// create DC
	HDC hScreenDC = ::GetDC(NULL);
	HDC hBitmapDC = CreateCompatibleDC(hScreenDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC,m_sDimensions.cx,m_sDimensions.cy);
	// select the bitmap into the DC
	HGDIOBJ hOldBitmap = SelectObject(hBitmapDC,hBitmap);
	// release screen dc
	ReleaseDC(NULL,hScreenDC);
	// init dc to the transparent color
	HBRUSH hBrush = CreateSolidBrush(crTrans);
	// draw the icon
	DrawIconEx(hBitmapDC,0,0,hIcon,m_sDimensions.cx,m_sDimensions.cy,NULL,hBrush,DI_NORMAL|DI_IMAGE);
	// release the bitmap
	SelectObject(hBitmapDC,hOldBitmap);
	// clean up
	DeleteObject(hBrush);
	DeleteDC(hBitmapDC);
	// return our bitmap
	return hBitmap;
}// end IconToBitmap

HBITMAP CCaptionButton::CombineBitmaps(HBITMAP hSource, HBITMAP hOverlay, COLORREF crTrans)
{// begin CombineBitmaps
	if(!hSource || !hOverlay)
		return NULL;
	// get the image dimensions
	BITMAP bm = {NULL};
    GetObject(hSource, sizeof(BITMAP), (LPSTR)&bm);
	SIZE m_sDimensions = {bm.bmWidth,bm.bmHeight};
	// create DC
	HDC hScreenDC = ::GetDC(NULL);
	HDC hBitmapDC = CreateCompatibleDC(hScreenDC);
	// select the bitmap into the DC
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC,m_sDimensions.cx,m_sDimensions.cy);
	HGDIOBJ hOldBitmap = SelectObject(hBitmapDC,hBitmap);
	// release screen dc
	ReleaseDC(NULL,hScreenDC);
	// fill the background to the transparent color
	RECT rImage = {0,0,m_sDimensions.cx,m_sDimensions.cy};
	HBRUSH hBrush = CreateSolidBrush(crTrans);
	FillRect(hBitmapDC,&rImage,hBrush);
	// draw the source
	DrawTransparentBitmap(hBitmapDC,hSource,0,0,crTrans);
	// draw the overlay
	DrawTransparentBitmap(hBitmapDC,hOverlay,0,0,crTrans);
	// release the bitmap
	SelectObject(hBitmapDC,hOldBitmap);
	// clean up
	DeleteDC(hBitmapDC);
	DeleteObject(hBrush);
	// return our new bitmap
	return hBitmap;
}// end CombineBitmaps

char * CCaptionButton::GetCaptionText(unsigned long int nID,char *pBuffer)
{// begin GetCaptionText
	// set the default info
	TOOLINFO ti = {NULL};
	ti.hwnd = m_hWnd;
	ti.uId = nID;
	ti.lpszText = pBuffer;
	ti.cbSize = sizeof(TOOLINFO);
	// send the info
	SendMessage(m_hToolTipWnd, TTM_GETTEXT, 0, (LPARAM) (LPTOOLINFO) &ti);
	// return the buffer
	return pBuffer;
}// end GetCaptionText

void CCaptionButton::DrawTransparentBitmap(HDC hDC, HBITMAP hBitmap, int x, int y, COLORREF crColour)
// written by Paul Reynolds: Paul.Reynolds@cmgroup.co.uk
// CodeGuru article: "Transparent Bitmap - True Mask Method"
// http://codeguru.earthweb.com/bitmap/CISBitmap.shtml
{
	COLORREF crOldBack = SetBkColor(hDC,RGB(255,255,255));
	COLORREF crOldText = SetTextColor(hDC,RGB(0,0,0));
	HDC dcImage, dcTrans;

	// Create two memory dcs for the image and the mask
	dcImage = CreateCompatibleDC(hDC);
	dcTrans = CreateCompatibleDC(hDC);

	// Select the image into the appropriate dc
	HGDIOBJ hOldBitmapImage = SelectObject(dcImage,hBitmap);

	// Create the mask bitmap
	HBITMAP bitmapTrans = NULL;
	// get the image dimensions
	BITMAP bm = {NULL};
    GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);
	int nWidth = bm.bmWidth;
	int nHeight = bm.bmHeight;
	bitmapTrans = CreateBitmap(nWidth, nHeight, 1, 1, NULL);

	// Select the mask bitmap into the appropriate dc
	HGDIOBJ hOldBitmapTrans = SelectObject(dcTrans,bitmapTrans);

	// Build mask based on transparent colour
	SetBkColor(dcImage,crColour);
	BitBlt(dcTrans, 0, 0, nWidth, nHeight, dcImage, 0, 0, SRCCOPY);

	// Do the work - True Mask method - cool if not actual display
	BitBlt(hDC,x, y, nWidth, nHeight, dcImage, 0, 0, SRCINVERT);
	BitBlt(hDC,x, y, nWidth, nHeight, dcTrans, 0, 0, SRCAND);
	BitBlt(hDC,x, y, nWidth, nHeight, dcImage, 0, 0, SRCINVERT);

	// Restore settings
	// don't delete this, since it is the bitmap
	SelectObject(dcImage,hOldBitmapImage);
	// delete bitmapTrans
	DeleteObject(SelectObject(dcTrans,hOldBitmapTrans));
	SetBkColor(hDC,crOldBack);
	SetTextColor(hDC,crOldText);
	// clean up
	DeleteDC(dcImage);
	DeleteDC(dcTrans);
}

