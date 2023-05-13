////////////////////////
// Thunk WndProc pointer
////////////////////////
// Created: 12/29/2000 {m/d/y}
// Written by: Anish Mistry http://am-productions.yi.org/
/* This code is licensed under the GNU GPL.  See License.txt or (http://www.gnu.org/copyleft/gpl.html). */

typedef unsigned char ThunkData[9];

#ifndef _THUNK_INIT_
#define _THUNK_INIT_
void inline ThunkInit(ThunkData t, void * This)
// ThunkInit created by Feng Yuan http://www.fengyuan.com/
{// begin ThunkInit
	t[0] = 0xB9; // mov ecx, 
	*((DWORD *)(t+1)) = (DWORD) This; // this
	*((DWORD *)(t+5)) = 0x20FF018B; // mov eax, [ecx] 
	// jmp [eax]
}// end ThunkInit
#endif
