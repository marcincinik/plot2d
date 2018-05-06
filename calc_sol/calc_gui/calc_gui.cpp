// calc_gui.cpp : main project file.

#include "stdafx.h"
#include "CalcViewForm.h"

using namespace calc_gui;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew CalcViewForm());
	return 0;
}
