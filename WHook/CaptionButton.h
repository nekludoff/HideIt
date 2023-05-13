//////////////////////
// CaptionButton.h //
//////////////////////

// Written by Anish Mistry
// Anish Mistry's e-mail: amistry@am-productions.yi.org
// AM Productions Website http://am-productions.yi.org/
// created 10/19/1999 mm/dd/yy

// MODIFICATION BY LIST:
// modified by: Anish Mistry 5/29/2000
// modified by: Anish Mistry 12/29/2000
// modified by: Anish Mistry 04/01/2001
// modified by: Anish Mistry 04/17/2001
// modified by: your name mm/dd/yyyy
//

/* This code is licensed under the GNU GPL.  See License.txt or (http://www.gnu.org/copyleft/gpl.html). */
/* HOWTO: (example) works in both MFC and Win32

   // declare
   CCaptionButton extra;	
   // handle to window that the captions are to be displayed
   extra.Init(hWnd);	
   // number of of other captions (ie. Close/Maximize/Minimize)
   extra.SetNumOfDefaultCaptions(3);
   // color of the transparent area of the bitmap
   extra.SetTransparentColor(RGB(255,0,255));
   // the bitmap that will be used when the caption is pressed
   extra.SetSelectionBitmap((HBITMAP)LoadImage(hInstance,MAKEINTRESOURCE(IDB_SELECTION),IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS|LR_DEFAULTCOLOR));
   
   // create a new caption
   void New(const UINT &	// ID of the caption (duplicate captions are not checked for)
   ,HBITMAP hilite	// Bitmap that is displayed when the mouse IS over the caption
   ,HBITMAP normal	// Bitmap that is displayed when the mouse is NOT over the caption
   , char*	// pointer to a string that contains the tool tip for the caption
   , int nInsertPos = -1	// this defaults to -1 which adds it to the end of the list (leftmost)
							// this is the 0 based index of where to insert the caption (right to left)
   );
   // create a new caption
   void New(const UINT &	// ID of the caption (duplicate captions are not checked for)
   ,HICON hilite	// Icon that is displayed when the mouse IS over the caption
   ,HICON normal	// Icon that is displayed when the mouse is NOT over the caption
   , char*	// pointer to a string that contains the tool tip for the caption
   , int nInsertPos = -1	// this defaults to -1 which adds it to the end of the list (leftmost)
							// this is the 0 based index of where to insert the caption (right to left)
   );

   extra.New(1,
   (HBITMAP)LoadImage(hInstance,MAKEINTRESOURCE(IDB_BITMAP1),IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS|LR_DEFAULTCOLOR),
   (HBITMAP)LoadImage(hInstance,MAKEINTRESOURCE(IDB_BITMAP2),IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS|LR_DEFAULTCOLOR),
   "Caption 1");
   extra.New(2,
   (HBITMAP)LoadImage(hInstance,MAKEINTRESOURCE(IDB_BITMAP3),IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS|LR_DEFAULTCOLOR),
   (HBITMAP)LoadImage(hInstance,MAKEINTRESOURCE(IDB_BITMAP4),IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS|LR_DEFAULTCOLOR),
   "Caption 2");
    extra.New(3,
   (HBITMAP)LoadImage(hInstance,MAKEINTRESOURCE(IDB_BITMAP5),IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS|LR_DEFAULTCOLOR),
   (HBITMAP)LoadImage(hInstance,MAKEINTRESOURCE(IDB_BITMAP6),IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS|LR_DEFAULTCOLOR),
   "Caption 3");
     extra.New(4,
   (HBITMAP)LoadImage(hInstance,MAKEINTRESOURCE(IDB_BITMAP5),IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS|LR_DEFAULTCOLOR),
   (HBITMAP)LoadImage(hInstance,MAKEINTRESOURCE(IDB_BITMAP6),IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS|LR_DEFAULTCOLOR),
   "Caption 4");

	// replacing a caption
	bool Replace(const UINT replaceCaptionID // the ID of the caption to be replaced
	,const UINT newID // ID to the new caption
	,HBITMAP newHilite	// new bitmap that is displayed when the mouse IS over the caption
	,HBITMAP newNormal	// new Bitmap that is displayed when the mouse is NOT over the caption
	,char *newToolTip	// pointer to a string that contains the new tool tip for the caption
	);

	// deleting a caption
	void Delete(const UINT	// the ID of the caption that you want to delete
	);

	Optimizing graphics use:
		As a way to reduce the number of bitmap/icons that are used the create the caption
	the following funcions were provided: IconToBitmap(), CombineBitmaps().
	To reduce the number of graphics that are used to the mouse over effect, to can use
	the CombineBitmaps() function the create a bitmap with an mouse over effect. ie.
	// choose the transparency color for the bitmap
	COLORREF crTransparent = RGB(255,0,255);
	// load our common mouseover effect
	HBITMAP hOverlay = (HBITMAP)LoadImage(hInstance,MAKEINTRESOURCE(IDB_OVERLAY),IMAGE_BITMAP,0,0,LR_DEFAULTCOLOR);
	// load our bitmap
	HBITMAP hBitmap = (HBITMAP)LoadImage(hInstance,MAKEINTRESOURCE(IDB_BITMAP),IMAGE_BITMAP,0,0,LR_DEFAULTCOLOR);
	// create a resulting mouseover bitmap using the hOverlay, and the hBitmap
	HBITMAP hMouseOver = CCaptionButton::CombineBitmaps(hBitmap,hOverlay,crTransparent);
	// create a caption
	extra.New(1,hMouseOver,hBitmap,"Some Caption");
	// destroy th overlay
	DeleteObject(hOverlay);

	KNOWN ISSUES: // this section may be modified as issues arise.
	1) The captions flicker when resizing

*/
#ifndef _CAPTIONBUTTON_H_
#define _CAPTIONBUTTON_H_


#include "Thunk.h"
#include <vector>

using namespace std;

#define WM_CBLBUTTONCLICKED WM_USER+1

class CCaptionButton
{

	// Construction
protected:
	virtual LRESULT WindowProc(HWND,UINT,WPARAM,LPARAM);
	ThunkData m_Thunk;

public:
	struct CCaption
	{ // begin CCaption
		unsigned long int nID;
		HBITMAP hNormalBitmap;
		HBITMAP hHiliteBitmap;
		HRGN hRgn;
		// CCaption constructor
		CCaption::CCaption() : nID(0) , hNormalBitmap(NULL), hHiliteBitmap(NULL), hRgn(NULL) {}
	}; // end CCaption
	CCaptionButton();
	~CCaptionButton();

// Attributes
	void Init(HWND);
	void SetSelectionBitmap(HBITMAP bitmap);
	void SetTransparentColor(const int &);
	void SetNumOfDefaultCaptions(const UINT &);
	char * GetCaptionText(unsigned long int nID,char *pBuffer);
	bool New(const UINT ,HBITMAP hilite,HBITMAP normal,char*,int nInsertPostion = -1);
	bool New(const UINT nID,HICON hHilite,HICON hNormal,char *pTooltip,const int nInsertPostion = -1);
	void Delete(const UINT &);
	bool Replace(const UINT replaceCaptionID,const UINT newID,HBITMAP newHilite,HBITMAP newNormal,char *newToolTip);
	static HBITMAP CombineBitmaps(HBITMAP hSource, HBITMAP hOverlay, COLORREF crTrans);
	static HBITMAP IconToBitmap(HICON hIcon,COLORREF crTrans);
	static void DrawTransparentBitmap(HDC hDC, HBITMAP hBitmap, int x, int y, COLORREF crColour);

protected:
	void ReInitCaptions();
	void Swap(CCaption a,CCaption b) const;
	CCaption CreateCaption(const UINT &ID,HBITMAP hilite,HBITMAP normal,char *tooltipString);
	bool UpdateRegion(HRGN hUpdateRegion);
	bool DeleteCaption(int nIndex);
	LRESULT OnLButtonUp(unsigned long int message,WPARAM wParam,LPARAM lParam);
	LRESULT OnDestroy();
	LRESULT OnNcLButtonUp(unsigned long int message,WPARAM wParam,LPARAM lParam);
	LRESULT OnNcLButtonDown(unsigned long int message,WPARAM wParam,LPARAM lParam);
	LRESULT OnMouseMove(UINT message,WPARAM wParam, LPARAM lParam);
	LRESULT OnSystemChange(void);
	LRESULT OnNcPaint(UINT message,WPARAM wParam, LPARAM lParam);
	bool OnWindowChange(void);
	RECT CalculateRect(const int &offset, bool toClient = true) const;
	HRGN CalculateRgn(const int &offset) const;
	bool Insert(CCaption *pNode,int nInsertPosition = -1);
	void DeleteNode(const UINT &);
	void Draw(int, bool) const;
	void DrawSelection(const int &num) const;
	void DeleteList();
	SIZE CenterBitmap(const SIZE &size) const;
	int GetNode(const UINT &find) const;
	SIZE GetCaptionSize() const;
	SIZE GetFrameSize() const;
	void RedrawCaptions() const;
/******* member variables ************/
	unsigned long int m_fStyles;
	unsigned long int m_fExStyles;
	int m_nLastPointNode;
	bool m_bMyRedraw;
	bool m_bCapturing;
	HWND m_hToolTipWnd;
	WNDPROC m_wpOldWndProc;
	HWND m_hWnd;
	vector <CCaption> m_llCaptions;
	int m_nDownNode;
	HBITMAP m_hSelectionBitmap;
	int m_nNumCaptions;
	UINT m_crTransparentColor;
	UINT m_nNumDefaultCaptions;
};

#endif