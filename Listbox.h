#include <wx/wx.h>
#include <wx/listbox.h>

class MyPanel : public wxPanel
{
public:
    MyPanel(wxPanel *parent);

	void OnSearchByProf(wxCommandEvent& event);
	void OnSearchByDepartment(wxCommandEvent& event);
	void OnMax(wxCommandEvent& event);
	void OnMin(wxCommandEvent& event);
	void OnAvg(wxCommandEvent& event);

	wxListBox *m_better_lb;

	wxButton *m_professorb;
	wxButton *m_departmentb;
	wxButton *m_maxb;
	wxButton *m_minb;
	wxButton *m_avgb;

};

class StaticText : public wxFrame
{
public:
	StaticText(const wxString& title, const wxString& info);
};

class Listbox : public wxFrame
{
public:
    Listbox(const wxString& title);

    void OnDblClick(wxCommandEvent& event);

    wxListBox *listbox;
    MyPanel *btnPanel;

};

const int ID_PROFESSOR = 7001;
const int ID_DEPARTMENT = 7002;
const int ID_MAX = 7003;
const int ID_MIN = 7004;
const int ID_LISTBOX = 7005;
const int ID_AVG = 7006;