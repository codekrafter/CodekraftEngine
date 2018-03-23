#include <iostream>
#include "EditorConfig.hpp"
#include "Engine.hpp"
#include "EditorApp.hpp"
#include "EditorFrame.hpp"

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

wxIMPLEMENT_APP_NO_MAIN(EditorApp);
wxIMPLEMENT_WX_THEME_SUPPORT;

int main(int argc, char *argv[])
{
    std::cout << "Starting Editor Version: " << EDITOR_VERSION << std::endl;
    initEngine(false);
    wxEntryStart(argc, argv);
    wxTheApp->CallOnInit();
    wxTheApp->OnRun();
    
    return 0;
}