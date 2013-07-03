#include "listbox.h"
#include <wx/textdlg.h>

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <stdio.h>
#include <ctype.h>
#include <map>
#include <cctype>
#include <iomanip>
using namespace std;

struct professor {
	string university;
	string name;
	string position;
	string salary;
	string deductible;
	int int_salary;
};
vector<professor> professors;

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

void parse_prof(string line){
	
	professor new_prof;
	vector<string> split_prof = split(line, '\t');
	
	new_prof.university = split_prof[0];
	
	new_prof.name = split_prof[2] + " " + split_prof[1];
	
	new_prof.position = split_prof[3];

	new_prof.salary = split_prof[4];

	new_prof.deductible = split_prof[5];

	// salary (string) to integer
	
	string unf_pay = split_prof[4].substr(1);

	for(string::iterator qw=unf_pay.begin(); qw!=unf_pay.end(); qw++){
		if((*qw) == ','){
			qw = unf_pay.erase(qw);
		}
	}

	double form_pay = ::atof(unf_pay.c_str());

	new_prof.int_salary = form_pay;

	professors.push_back(new_prof);
}

void createSource()
{
	string line;
	ifstream myfile("data.txt");
	vector<string> stored_profs;

	// Open the file
	if(myfile.is_open())
	{
		// While you're not at the end of the file
		while(myfile.good())
		{
			getline (myfile,line);

			stored_profs.push_back(line);
			parse_prof(line);
		}
		myfile.close();
	}
}

wxString findMax(){
	vector<professor>::iterator max_iter = professors.begin();
	professor max_prof;
	int max = 0;
	while(max_iter!=professors.end()){
		if((*max_iter).int_salary > max){
			max = (*max_iter).int_salary;
			max_prof = (*max_iter);
		}
		max_iter++;
	}
	string total = "Name: " + max_prof.name + "\n" + "Position: " + max_prof.position + "\n" + "Salary: " + max_prof.salary;
	
	wxString total_max(total);
	return total_max;
}

wxString findMin(){
	vector<professor>::iterator min_iter = professors.begin();
	professor min_prof;
	int min = 10000000000;
	while(min_iter!=professors.end()){
		if((*min_iter).int_salary < min){
			min = (*min_iter).int_salary;
			min_prof = (*min_iter);
		}
		min_iter++;
	}
	string total = "Name: " + min_prof.name + "\n" + "Position: " + min_prof.position + "\n" + "Salary: " + min_prof.salary;
	
	wxString total_min(total);
	return total_min;
}

void universityAvg(){
	vector<professor>::iterator iter = professors.begin();
	int sum = 0;
	while(iter!=professors.end()){
		sum += (*iter).int_salary;
		iter++;
	}
	double avg = sum/professors.size();
}

vector<professor> getCurrProfs(vector<string> profs){

	vector<professor> curr_profs;

	vector<string>::iterator iter = profs.begin();
	int sum = 0;
	while(iter!=profs.end()){
		vector<professor>::iterator iter2 = professors.begin();
		while(iter2!=professors.end()){
			if((*iter) == (*iter2).name){
				curr_profs.push_back((*iter2));
			}
			iter2++;
		}
		iter++;
	}
	return curr_profs;
}

wxString findAvg(vector<string> profs){

	vector<professor> curr_profs = getCurrProfs(profs);

	vector<professor>::iterator iter = curr_profs.begin();
	int sum = 0;
	while(iter!=curr_profs.end()){
		sum += (*iter).int_salary;
		iter++;
	}
	double avg = sum/curr_profs.size();
	wxString average = wxString::Format(wxT("%f"), avg);
	return average;
}

bool compareBySalary(const professor &a, const professor &b)
{
	return a.int_salary > b.int_salary;
}

vector<professor> findProf(string name_query)
{

	vector<professor> ret_profs;

	std::transform(name_query.begin(), 
		name_query.end(), 
		name_query.begin(), 
		std::toupper);
	size_t found;

	for(int i = 0; i < professors.size(); i++){
		found = professors[i].name.find(name_query);
		if(found!=std::string::npos){
			ret_profs.push_back((professors[i]));
		}
	}

	sort(ret_profs.begin(), ret_profs.end(), compareBySalary);

	return ret_profs;
}

vector<professor> findDepartment(string name_query){

	vector<professor> ret_profs;
	size_t found;

	for(int i = 0; i < professors.size(); i++){
		found = professors[i].position.find(name_query);
		if(found!=std::string::npos){
			ret_profs.push_back((professors[i]));
		}
	}

	sort(ret_profs.begin(), ret_profs.end(), compareBySalary);

	return ret_profs;
}

Listbox::Listbox(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(270, 200))
{

	wxPanel * panel = new wxPanel(this, -1);

	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

	listbox = new wxListBox(panel, ID_LISTBOX,
		wxPoint(-1, -1), wxSize(-1, -1));

	hbox->Add(listbox, 3, wxEXPAND | wxALL, 20);

	btnPanel = new MyPanel(panel);

	hbox->Add(btnPanel, 2, wxEXPAND | wxRIGHT, 10);

	Connect(wxEVT_COMMAND_LISTBOX_DOUBLECLICKED,
		wxCommandEventHandler(Listbox::OnDblClick));

	panel->SetSizer(hbox);
	Center();
}

MyPanel::MyPanel(wxPanel * parent)
	: wxPanel(parent, wxID_ANY)
{
	createSource();

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

	Listbox *lb = (Listbox *) parent->GetParent();
	m_better_lb = lb->listbox;

	m_professorb = new wxButton(this, ID_PROFESSOR, wxT("Search By Professor"));
	m_departmentb = new wxButton(this, ID_DEPARTMENT, wxT("Search By Department"));
	m_maxb = new wxButton(this, ID_MAX, wxT("Find Max"));
	m_minb = new wxButton(this, ID_MIN, wxT("Find Min"));
	m_avgb = new wxButton(this, ID_AVG, wxT("Find Avg"));

	Connect(ID_PROFESSOR, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(MyPanel::OnSearchByProf));
	Connect(ID_DEPARTMENT, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(MyPanel::OnSearchByDepartment));
	Connect(ID_MAX, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(MyPanel::OnMax));
	Connect(ID_MIN, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(MyPanel::OnMin));
	Connect(ID_AVG, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(MyPanel::OnAvg));

	vbox->Add(-1, 20);
	vbox->Add(m_professorb);
	vbox->Add(m_departmentb, 0, wxTOP, 5);
	vbox->Add(m_maxb, 0, wxTOP, 5);
	vbox->Add(m_minb, 0, wxTOP, 5);
	vbox->Add(m_avgb, 0, wxTOP, 5);

	SetSizer(vbox);
}

void MyPanel::OnSearchByProf(wxCommandEvent& event)
{
	m_better_lb->Clear();
	wxString query = wxGetTextFromUser(wxT("Enter the name of a Professor"));
	if (query.Len() > 0)
	{
		// Valid prof name
		string prof = query.ToStdString();
		vector<professor> ret_profs = findProf(prof); 

		for(int i = 0; i < ret_profs.size(); i++)
		{
			wxString wxprof = ret_profs[i].name;
			m_better_lb->Append(wxprof);
		}
	}
}

void MyPanel::OnSearchByDepartment(wxCommandEvent& event)
{
	m_better_lb->Clear();
	wxString query = wxGetTextFromUser(wxT("Enter the name of a Department"));
	if (query.Len() > 0)
	{
		// Valid dept name
		string dept = query.ToStdString();
		vector<professor> ret_profs = findDepartment(dept); 

		for(int i = 0; i < ret_profs.size(); i++)
		{
			wxString wxprof = ret_profs[i].name;
			m_better_lb->Append(wxprof);
		}
	}
}

StaticText::StaticText(const wxString& title, const wxString& info)
       : wxFrame(NULL, wxID_ANY, title)
{

	wxPanel *panel = new wxPanel(this, wxID_ANY);

	wxStaticText *st = new wxStaticText(panel, ID_MAX, info,
		wxPoint(10,10), wxDefaultSize, wxALIGN_CENTRE);

	this->SetSize(600, 110);
	this->Centre();
}


void MyPanel::OnMax(wxCommandEvent& event)
{
	wxString total = findMax();
	StaticText *st = new StaticText(wxT("Maximum Earner"), total);
	st->Show(true);
}

void MyPanel::OnMin(wxCommandEvent& event)
{
	wxString total = findMin();
	StaticText *st = new StaticText(wxT("Minimum Earner"), total);
	st->Show(true);
}

void MyPanel::OnAvg(wxCommandEvent& event)
{
	int length = m_better_lb->GetCount();
	vector<string> curr_profs;
	for(int i = 0; i < length; i++){
		wxString prof = m_better_lb->GetString(i);
		curr_profs.push_back(prof.ToStdString());
	}
	wxString curr_avg = findAvg(curr_profs);
	StaticText *st = new StaticText(wxT("Average Salary"), curr_avg);
	st->Show(true);
}

void Listbox::OnDblClick(wxCommandEvent& event)
{
	wxString prof_name;
	int sel = listbox->GetSelection();
	if (sel != -1)
	{
		prof_name = listbox->GetString(sel);
		string prof = prof_name.ToStdString();

		size_t found;
		string position = "Position: NONE";
		string salary = "Salary: NONE";

		for(int i = 0; i < professors.size(); i++){
			found = professors[i].name.find(prof);
			if(found!=std::string::npos){
				position = "Position: " + professors[i].position;
				salary = "Salary: " + professors[i].salary;
			}
		}

		prof = "Name: " + prof;

		string total = prof + "\n" + position + "\n" + salary;
		wxString prof_total(total);
		StaticText *st = new StaticText(wxT("Professor"), prof_total);
		st->Show(true);
	}
}