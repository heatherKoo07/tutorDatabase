// Specification file for the Tutor class
// Created by Heather Koo
// Purpose of Tutor.h: to create tutor objects to store tutor student's information. 


#ifndef PROGRAM_DOCS_TUTOR_H
#define PROGRAM_DOCS_TUTOR_H

#include <vector>

using std::string;

class Tutor; // Forward Declaration

// Function Prototypes for Overloaded Stream Operators
std::ostream &operator << (std::ostream &, const Tutor &);

class Tutor {
private:
    string id;
    string name;
    string subject;
    std::vector<string> courses;
    std::vector<string> days;
    string startTime;
    string endTime;

public:
    Tutor() {id = ""; name = ""; subject=""; courses={}; days={}; startTime=""; endTime="";}
    Tutor(string i) {id = i; name = ""; subject=""; courses={}; days={}; startTime=""; endTime="";}

    void setId(string i) {id = i;}
    void setName(string n) {name = n;}
    void setSubject(string s) {subject = s;}
    void setCourses(string c) {courses.push_back(c);}
    void setDays(string d) {days.push_back(d);}
    void setStartTime(string st) {startTime = st;}
    void setEndTime(string et) {endTime = et;}

    string getId() const {return id;}
    string getName() const {return name;}
    string getSubject() const {return subject;}
    std::vector<string> getCourses() const {return courses;}
    std::vector<string> getDays() const {return days;}
    string getStartTime() const {return startTime;}
    string getEndTime() const {return endTime;}

};


#endif //PROGRAM_DOCS_TUTOR_H
