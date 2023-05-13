//////
// EZMenu - Owner drawn menu with Icons & progressbars
// Copyright V.Lakshmi Narasimhan,ezlux@Yahoo.com.
// Feel free to use,modify,twist,turn or even 
// digest the code for any non commercial purposes.
// I would appreciate constructive suggestions & bug reports.
// Please dont delete the above lines.
///////
#ifndef __EZMENU_H__
#define __EZMENU_H__


#if _MSC_VERSION>=1000
#pragma once
#endif// _MSC_VERSION>=1000


class CEZMenuItemData
{
public:

	CEZMenuItemData();

	HICON   hItemIconLarge;//Large Icon Handle
	
	HICON   hItemIconSmall;//Small Icon Handle

    CString strItemText;//Menu string

	BOOL    bChecked;//Is item checked?

	BOOL    bSeperator;	
	//Why do you need an Owner drawn menu to draw a **seperator**?

	BOOL    bProgress;//Progressbar needed?

	int     nProgress; //if bProgress what is the percentage?
	
};

//Useful typedef
typedef  CEZMenuItemData*  LPEZMENUITEMDATA;

typedef void (CALLBACK* LPDRAWITEMCALLBACKPROC)(LPDRAWITEMSTRUCT);

//Header file for EZMenu Self drawn menu
class CEZMenu :public CMenu
{
public:

	CEZMenu();
	
	void InsertOwnerDrawnMenuItem(UINT nID,LPEZMENUITEMDATA lpez,int nPos=0);

	void AppendOwnerDrawnMenuItem(UINT nID,LPEZMENUITEMDATA lpez);

	void ModifyOwnerDrawnMenuItem(int nPos,UINT nID,int nTotalCheck,LPEZMENUITEMDATA lpez);
	

	void SetTotalItems(int nItems);//necessary. 

	void SetMenuBitmap(HBITMAP hBitmap);

	void SetExtensionColor(COLORREF clrExtend);
	//if bitmap is shorter than menu height

	void SetBitmapDimension(CSize sz);
		
    void SetLargeIcon(BOOL bLarge=FALSE);//Large or Small?

    void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	BOOL SetDrawItemCallBack(LPDRAWITEMCALLBACKPROC lpfn);

	~CEZMenu();

public:
	CPtrArray m_pArrData;
private:

	int m_nCount;//temp

	int m_nAveHeight;//average height of each menu item

	int m_nItems;//No of items

	COLORREF m_clrExtend;//extension color

	BOOL m_bLargeIcons;

	CSize m_szBitmapSize;

	CBitmap m_bitmap;
	
	LPDRAWITEMCALLBACKPROC lpDrawItemCallBackProc;
};					



















#endif //__EZMENU_H__