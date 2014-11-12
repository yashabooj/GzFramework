/*	YASH ABOOJ
CSCI 580: 3-D Graphics and Rendering
Homework 6: Aniti-Aliasing using Jittered Sub-Sampling
abooj@usc.edu */

/* CS580 Homework 6 */

// Application6.h: interface for the Application6 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPLICATION6_H__43A7FA9C_6CD6_4A79_9567_2354BFEFAFFB__INCLUDED_)
#define AFX_APPLICATION6_H__43A7FA9C_6CD6_4A79_9567_2354BFEFAFFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Application.h"

class Application6 : public Application
{
public:
	Application6();
	virtual ~Application6();

	int	Initialize();
	virtual int Render();
	int Clean();
};

#endif // !defined(AFX_APPLICATION6_H__43A7FA9C_6CD6_4A79_9567_2354BFEFAFFB__INCLUDED_)
