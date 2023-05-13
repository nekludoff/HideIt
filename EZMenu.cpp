//////
// EZMenu - Owner drawn menu with Icons & progressbars
// Copyright V.Lakshmi Narasimhan,ezlux@Yahoo.com.
// Feel free to use,modify,twist,turn or even 
// digest the code for any non commercial purposes.
// I would appreciate constructive suggestions & bug reports.
// Please dont delete the above lines.
///////

//Implementation file for owner drawn menu EZMenu
#include "stdafx.h" 
#include "EZMenu.h"
#include "resource.h"

CEZMenu::CEZMenu()
{
	m_bLargeIcons=FALSE;
	m_szBitmapSize=CSize(20,60);
	m_nCount=m_nItems=0;
	m_nAveHeight=16;
	m_clrExtend=RGB(0,0,255);
	CMenu::CMenu();
}


BOOL CEZMenu::SetDrawItemCallBack(LPDRAWITEMCALLBACKPROC lpfn)
{
	lpDrawItemCallBackProc = lpfn;
	return TRUE;
}

void CEZMenu::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	m_nCount++;
	LPEZMENUITEMDATA lpez=(LPEZMENUITEMDATA)lpDrawItemStruct->itemData;

	CDC* pdc=CDC::FromHandle(lpDrawItemStruct->hDC);

    if(lpez->bSeperator)
	{
		int nWidth=0;
		if(m_bitmap.m_hObject)
			nWidth+=(m_szBitmapSize.cx+lpDrawItemStruct->rcItem.left);
	    CRect rcSep(lpDrawItemStruct->rcItem);
		rcSep.left+=nWidth;
		pdc->DrawEdge(&rcSep,EDGE_ETCHED,BF_TOP);
		
		return;
	}
	
	// Rects Definition
	CRect rect(lpDrawItemStruct->rcItem);//general
	CRect rcBmp(rect);
	CRect rcIcon(rect);//Icon rect
	CRect rcProgress(rect);//Progress bar rect
	CRect rcText(rect);//Text rect;

	
	//Rects Initialization
	   
	rcBmp.right=rcBmp.left+m_szBitmapSize.cx;
	rcIcon.left=rcBmp.right/*+2*/;
	rcIcon.right=rcIcon.left+(m_bLargeIcons?32:16);
	rcProgress.left=rcIcon.right+0;
	rcProgress.right=rcIcon.right+(lpez->bProgress?100:0);
	rcProgress.top+=4;
	rcProgress.bottom-=4;
	rcText.left=rcProgress.right+8;
	
	//Drawing code
	
	
	
	HICON hItemIcon=(m_bLargeIcons?lpez->hItemIconLarge:lpez->hItemIconSmall);
	CSize szIcon=(m_bLargeIcons?CSize(32,32):CSize(16,16));
	
	//Draws the whole thing
	
	if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)
	{
		
		
	//Draw the Icon
	CBrush *pBrush0=new CBrush(::GetSysColor(COLOR_MENU));
	pdc->FillRect(rect,pBrush0);
	delete pBrush0;

	 int nOldMap=pdc->SetMapMode(MM_TEXT);
	 if(hItemIcon)
	 {		
		  // pdc->DrawState(rcIcon.TopLeft(),szIcon,hItemIcon,DST_ICON|DSS_NORMAL,(CBrush*)NULL);
			DrawIconEx(pdc->GetSafeHdc(), 
			rcIcon.TopLeft().x,
			rcIcon.TopLeft().y,
			hItemIcon,
			m_bLargeIcons?32:16,
			m_bLargeIcons?32:16,
			NULL,
			(HBRUSH) NULL,
			DI_NORMAL
			);

	 }
	 else if(lpez->bChecked)
		{
		   
		   HICON hCheck=(HICON)::LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(m_bLargeIcons?IDI_CHECK_LARGE:IDI_CHECK_SMALL),IMAGE_ICON,szIcon.cx,szIcon.cy,LR_DEFAULTCOLOR);
		   //pdc->DrawState(rcIcon.TopLeft(),szIcon,hCheck,DST_ICON|DSS_NORMAL,(CBrush*)NULL);
			DrawIconEx(pdc->GetSafeHdc(), 
			rcIcon.TopLeft().x,
			rcIcon.TopLeft().y,
			hCheck,
			m_bLargeIcons?32:16,
			m_bLargeIcons?32:16,
			NULL,
			(HBRUSH) NULL,
			DI_NORMAL
			);
	   
		}
		
		
		//Draw the Progress Bar
		
		if(lpez->bProgress)
		{
			
			pdc->Draw3dRect(&rcProgress,::GetSysColor(COLOR_HIGHLIGHT),::GetSysColor(COLOR_HIGHLIGHT));
			CRect rectProgress(rcProgress);
			rectProgress.DeflateRect(CSize(1,1));
			CBrush *pBrush=new CBrush(::GetSysColor(COLOR_MENU));
		    pdc->FillRect(rectProgress,pBrush);
		    delete pBrush;
			pBrush=new CBrush(::GetSysColor(COLOR_HIGHLIGHT)); 
			rectProgress.right-=lpez->nProgress;
			pdc->FillRect(&rectProgress,pBrush);
			delete pBrush;
			
		}
		
		
		//Finally draw the text
		
		pdc->SetMapMode(nOldMap);
		CBrush *pBrush=new CBrush(::GetSysColor(COLOR_MENU));
		pdc->FillRect(rcText,pBrush);
		delete pBrush;

		pdc->SetTextColor(::GetSysColor(COLOR_MENUTEXT));
		pdc->SetBkColor(::GetSysColor(COLOR_MENU));
		DWORD dw = lpez->strItemText.GetLength();
		pdc->DrawText(lpez->strItemText,&rcText,DT_LEFT|DT_NOCLIP|DT_SINGLELINE|DT_VCENTER);

		//Draw the bitmap at the end
//		if(m_nCount==m_nItems)
		{
			m_nCount=0;
			if(m_bitmap.m_hObject)
			{
				CDC memdc;
				memdc.CreateCompatibleDC(pdc);
				memdc.SelectObject(m_bitmap);
				if(m_nAveHeight>m_szBitmapSize.cy)
				{
					int nTop=rect.bottom-m_szBitmapSize.cy;
					pdc->BitBlt(rect.left,nTop,m_szBitmapSize.cx,m_szBitmapSize.cy,&memdc,0,0,SRCCOPY);	   
					pdc->FillSolidRect(rect.left,rect.bottom-m_nAveHeight,m_szBitmapSize.cx,m_nAveHeight-m_szBitmapSize.cy,m_clrExtend);
				}
				else
					pdc->BitBlt(rect.left,rect.bottom-m_nAveHeight,m_szBitmapSize.cx,m_nAveHeight,&memdc,0,0,SRCCOPY);
				
				
			}
		}
	}
	
	
	//Draws the selected item
	
	if ((lpDrawItemStruct->itemState & ODS_SELECTED) &&
		(lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		
		if(lpDrawItemCallBackProc != NULL)
			lpDrawItemCallBackProc(lpDrawItemStruct);

		CBrush *pBrush0=new CBrush(::GetSysColor(COLOR_ACTIVECAPTION));
		pdc->FillRect(rect,pBrush0);
		delete pBrush0;
		
		if(hItemIcon)
		{
			CRect rcIconHilite(rcIcon);
			rcIconHilite.left-=1;
			rcIconHilite.top-=1;
			//rcIconHilite.bottom+=1;
			rcIconHilite.right+=1;
//		COLORREF crOld=pdc->SetTextColor(::GetSysColor(COLOR_CAPTIONTEXT));
//		COLORREF crBgOld=pdc->SetBkColor(::GetSysColor(COLOR_ACTIVECAPTION));

			//pdc->Draw3dRect(&rcIconHilite,::GetSysColor(COLOR_ACTIVECAPTION),::GetSysColor(COLOR_3DDKSHADOW));

		//Draw the Icon
		CRect rcIconWR(rcIcon);
		rcIconWR.right=rcText.left;

//		CBrush *pBrush1=new CBrush(::GetSysColor(COLOR_ACTIVECAPTION));
//		pdc->FillRect(rcIconWR,pBrush1);
//		delete pBrush1;
//		   pdc->DrawState(rcIcon.TopLeft(),szIcon,hItemIcon,DST_ICON|DSS_NORMAL,(CBrush*)NULL);
			DrawIconEx(pdc->GetSafeHdc(), 
			rcIcon.TopLeft().x,
			rcIcon.TopLeft().y,
			hItemIcon,
			m_bLargeIcons?32:16,
			m_bLargeIcons?32:16,
			NULL,
			(HBRUSH) NULL,
			DI_NORMAL
			);

		}
		
         //Progress
		if(lpez->bProgress)
		{
			pdc->Draw3dRect(&rcProgress,::GetSysColor(COLOR_3DHILIGHT),::GetSysColor(COLOR_3DDKSHADOW));
		}

        //Text

		CRect textrect(rcText);
        
		CBrush *pBrush=new CBrush(::GetSysColor(COLOR_ACTIVECAPTION));
		pdc->FillRect(rcText,pBrush);
		delete pBrush;
		
		COLORREF crOld=pdc->SetTextColor(::GetSysColor(COLOR_CAPTIONTEXT));
		COLORREF crBgOld=pdc->SetBkColor(::GetSysColor(COLOR_ACTIVECAPTION));

		
		CFont* pFont=pdc->GetCurrentFont();
		LOGFONT* pLf=new LOGFONT;
		pFont->GetLogFont(pLf);
		//pLf->lfWeight=FW_SEMIBOLD;
		CFont* pNewFont=new CFont;
		pNewFont->CreateFontIndirect(pLf);
		pFont=pdc->SelectObject(pNewFont);
		pdc->DrawText(lpez->strItemText,&textrect,DT_LEFT|DT_NOCLIP|DT_SINGLELINE|DT_VCENTER);
		pdc->SetTextColor(crOld);
		pdc->SetBkColor(crBgOld);
		
		delete pdc->SelectObject(pFont);
		delete pLf;
		
		textrect.top+=2;
		textrect.bottom-=2;
        textrect.left-=1;  
		//pdc->Draw3dRect(&textrect,::GetSysColor(COLOR_ACTIVEBORDER),::GetSysColor(COLOR_ACTIVEBORDER));
      
	}
	
	
	//Draws the deselected item
	
	
	
	if (!(lpDrawItemStruct->itemState & ODS_SELECTED) &&
		(lpDrawItemStruct->itemAction & ODA_SELECT))
	{
		
		//Draw the Icon
		CBrush *pBrush0=new CBrush(::GetSysColor(COLOR_MENU));
		pdc->FillRect(rect,pBrush0);
		delete pBrush0;
		
		if(hItemIcon)
		{
			CRect rcIconHilite(rcIcon);
			rcIconHilite.left-=1;
			rcIconHilite.top-=1;
			//rcIconHilite.bottom+=1;
			rcIconHilite.right+=1;

		CRect rcIconWR(rcIcon);
		rcIconWR.right=rcText.left;


//		CBrush *pBrush1=new CBrush(::GetSysColor(COLOR_MENU));
//		pdc->FillRect(rcIconWR,pBrush1);
//		delete pBrush1;

//			pdc->DrawState(rcIcon.TopLeft(),szIcon,hItemIcon,DST_ICON|DSS_NORMAL,(CBrush*)NULL);
			DrawIconEx(pdc->GetSafeHdc(), 
			rcIcon.TopLeft().x,
			rcIcon.TopLeft().y,
			hItemIcon,
			m_bLargeIcons?32:16,
			m_bLargeIcons?32:16,
			NULL,
			(HBRUSH) NULL,
			DI_NORMAL
			);
		   //pdc->Draw3dRect(&rcIconHilite,::GetSysColor(COLOR_MENU),::GetSysColor(COLOR_MENU));
			
		}

		if(lpez->bProgress)
		{
			
			pdc->Draw3dRect(&rcProgress,::GetSysColor(COLOR_HIGHLIGHT),::GetSysColor(COLOR_HIGHLIGHT));

		}
		pdc->SetTextColor(::GetSysColor(COLOR_MENUTEXT));
		pdc->SetBkColor(::GetSysColor(COLOR_MENU));
		CRect textrect(rcText);
        CBrush *pBrush=new CBrush(::GetSysColor(COLOR_MENU));
		pdc->FillRect(&textrect,pBrush);
		delete pBrush;

		pdc->DrawText(lpez->strItemText,&textrect,DT_LEFT|DT_NOCLIP|DT_SINGLELINE|DT_VCENTER);
		textrect.top+=2;
		textrect.bottom-=2;
        textrect.left-=1;  
		pdc->Draw3dRect(&textrect,::GetSysColor(COLOR_MENU),::GetSysColor(COLOR_MENU));
		
	}   
}


void CEZMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	LPEZMENUITEMDATA  lpez=(LPEZMENUITEMDATA  )lpMeasureItemStruct->itemData;
	if(lpez->bSeperator) 
	{
		lpMeasureItemStruct->itemHeight=5;
		return;
	}
	CDC* pdc=new CDC;
	pdc->Attach(::GetDC(AfxGetMainWnd()->m_hWnd));
	CSize szText=pdc->GetTextExtent(lpez->strItemText);
	
	CSize szText1(0,0);
	for(int i = 1; i <= lpez->strItemText.GetLength(); i++)
	{
		szText1=szText1+pdc->GetTextExtent(lpez->strItemText.Mid(i,1));
	}
	::ReleaseDC(AfxGetMainWnd()->m_hWnd,pdc->m_hDC);
	delete pdc;
	if(lpez->bProgress)	szText.cx+=100;

	lpMeasureItemStruct->itemWidth=szText1.cx+(m_bLargeIcons?32:16)+m_szBitmapSize.cx+8;
	lpMeasureItemStruct->itemHeight=__max(m_bLargeIcons?32:16,szText.cy)+3;
    m_nAveHeight=m_nItems*lpMeasureItemStruct->itemHeight;
}
			



CEZMenu::~CEZMenu()
{
	while( m_pArrData.GetSize() != 0 ) 
	{ 
		delete (CEZMenuItemData*) m_pArrData.GetAt(0); 
		m_pArrData.RemoveAt(0);
	}
	DestroyMenu();
	CMenu::~CMenu();
}


void CEZMenu::SetMenuBitmap(HBITMAP hBitmap)
{
	m_bitmap.Attach(hBitmap);
}


void CEZMenu::SetLargeIcon(BOOL bLarge)
{
	m_bLargeIcons=bLarge;
}

void CEZMenu::AppendOwnerDrawnMenuItem(UINT nID, LPEZMENUITEMDATA lpez)
{

	VERIFY(AppendMenu(MF_ENABLED | MF_OWNERDRAW, nID,(LPCTSTR)lpez));
}

void CEZMenu::SetBitmapDimension(CSize sz)
{
	m_szBitmapSize=sz;
}

void CEZMenu::InsertOwnerDrawnMenuItem(UINT nID, LPEZMENUITEMDATA lpez,int nPos)
{
	VERIFY(InsertMenu(nPos,MF_BYPOSITION|MF_ENABLED | MF_OWNERDRAW, nID,(LPCTSTR)lpez));
}

void CEZMenu::SetExtensionColor(COLORREF clrExtend)
{
	m_clrExtend=clrExtend; 
}

void CEZMenu::SetTotalItems(int nItems)
{
	m_nItems=nItems;
}

CEZMenuItemData::CEZMenuItemData()
{
	bChecked=FALSE;
	bProgress=FALSE;
    bSeperator=FALSE;
	hItemIconLarge=hItemIconSmall=NULL;
	nProgress=0;
	strItemText.Empty();
}

void CEZMenu::ModifyOwnerDrawnMenuItem(int nPos, UINT nID, int nTotalCheck,LPEZMENUITEMDATA lpez)
{
    //Iam not sure how to implement this.Suggestions are welcome.    
	//if(GetMenuItemCount()>(UINT)nTotalCheck)?
			VERIFY(ModifyMenu(nPos,MF_BYPOSITION|MF_OWNERDRAW,nID,(LPCTSTR)lpez));
	//else
	//	InsertOwnerDrawnMenuItem(nID,lpez,nPos);
}
