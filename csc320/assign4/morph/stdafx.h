#ifdef _WIN32

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__BA777DC2_8328_11D3_8E86_0800690F85CA__INCLUDED_)
#define AFX_STDAFX_H__BA777DC2_8328_11D3_8E86_0800690F85CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

#include <stdlib.h> 
#include <stdio.h>
#include <conio.h> 
#include <iostream.h>

// Rounding may not work as expected in windows - Vista uses the rint() function to
// round "to an integral value according to the current IEEE754 rounding
// direction."  rint() is not part of the ANSI C specification, and is
// not implemented in VC++.  All references to rint() were replaced using
// the macro:
#define rint(a) floor((a)+.5)


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__BA777DC2_8328_11D3_8E86_0800690F85CA__INCLUDED_)

#endif

