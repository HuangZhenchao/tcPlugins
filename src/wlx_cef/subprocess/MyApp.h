#pragma once
#include "../Win32CEFTest/stdafx.h"

class MyApp : public CefApp, public CefRenderProcessHandler
{

public:
	virtual ~MyApp() {}

private:

	IMPLEMENT_REFCOUNTING(MyApp);
};