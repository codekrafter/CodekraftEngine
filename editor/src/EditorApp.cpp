// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "EditorApp.hpp"
#include "EditorFrame.hpp"
wxBEGIN_EVENT_TABLE(EditorFrame, wxFrame)
    EVT_MENU(ID_Hello,   EditorFrame::OnHello)
    EVT_MENU(wxID_EXIT,  EditorFrame::OnExit)
    EVT_MENU(wxID_ABOUT, EditorFrame::OnAbout)
wxEND_EVENT_TABLE()
bool EditorApp::OnInit()
{
    EditorFrame *frame = new EditorFrame( "Codekraft Engine", wxPoint(50, 50), wxSize(450, 340) );
    frame->Show( true );
    return true;
};