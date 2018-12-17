/**
 CIS 22C: Team Project

 This application helps De Anza College to have a centralized tutor database throughout all departments.
 Tutor profiles with availability and subjects they tutor.
 Search for tutor by student id
 Search for tutor by name

 IDE: CLion
 Written By: Heather Koo

 **/

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include "BinarySearchTree.h"
#include "Tutor.h"
#include "HashTable.h"

const char INSERT_CHOICE = 'A',
           DELETE_CHOICE = 'D',
           SEARCH_CHOICE = 'S',
           DISPLAY_CHOICE = 'L',
           SAVE_CHOICE = 'F',
           SHOW_MENU_CHOICE = 'M',
           SHOW_STATISTICS = 'C',
           QUIT_CHOICE = 'Q';

const char BY_ID_CHOICE = 'P',
           BY_NAME_CHOICE = 'N';

const char DISPLAY_UNSORTED = 'U',
           DISPLAY_INDENTED_CHOICE = 'T',
           DISPLAY_DEVELOPERS = 'Z';

using namespace std;

/**~*~*
  hashKey structure returns Tutor object's ID
  to use it as a hash table key.
*~**/
struct hashKey {
    size_t operator()(const Tutor* a) const {
        return atoi(a->getId().c_str());
    }
};

/**~*~*
  TutorEqual structure returns true
  when the left tutor object's ID is the same as the right.
*~**/
struct TutorEqual {
    bool operator()(const Tutor* a, const Tutor* b) const {
        return (a->getId() == b->getId());
    }
};

/**~*~*
  TutorEqual structure returns true
  when the left tutor object's ID is less than the right.
*~**/
struct TutorLess1 {
    bool operator()(const Tutor* a, const Tutor* b) const {
        return (a->getId() < b->getId());
    }
};

/**~*~*
  TutorEqual structure returns true
  when the left tutor object's name is less than the right.
*~**/
struct TutorLess2 {
    bool operator()(const Tutor* a, const Tutor* b) const {
        return (a->getName() < b->getName());
    }
};

bool isPrime(int);
int getHashSize(string);
void readData(string, BinarySearchTree<Tutor*, TutorLess1>*, BinarySearchTree<Tutor*, TutorLess2>*, HashTable<Tutor*, hashKey, TutorEqual>*);
void display(Tutor*&);
void displayList(Tutor*&);
void menu();
char getValidInput(string validChar = "ADSLFCMQ");
void insertData(BinarySearchTree<Tutor*, TutorLess1>*, BinarySearchTree<Tutor*, TutorLess2>*, HashTable<Tutor*, hashKey, TutorEqual>*);
bool isTimeValid(string);
bool isValidDay(string);
bool deleteData(BinarySearchTree<Tutor*, TutorLess1>*, BinarySearchTree<Tutor*, TutorLess2>*, HashTable<Tutor*, hashKey, TutorEqual>*);
void displayHead();
void displayTail(HashTable<Tutor*, hashKey, TutorEqual>*);
void displayData(BinarySearchTree<Tutor*, TutorLess1>*, BinarySearchTree<Tutor*, TutorLess2>*, HashTable<Tutor*, hashKey, TutorEqual>*);
void searchData(BinarySearchTree<Tutor*, TutorLess2>*, HashTable<Tutor*, hashKey, TutorEqual>*);
void searchID(HashTable<Tutor*, hashKey, TutorEqual>*, void display(Tutor*&));
void searchName(BinarySearchTree<Tutor*, TutorLess2>*, void display(Tutor*&));
void getStatistics(HashTable<Tutor*, hashKey, TutorEqual>*);
void fileDisplay(Tutor* &person, ostream & os);
void createFile(HashTable<Tutor*, hashKey, TutorEqual>*);

int main() {

    cout << "\n\n\t\t *~~*~~* WELCOME *~~*~~*\n";
    cout << "\n\nThis application helps De Anza College to have a centralized tutor database throughout all departments\n"
         << "\tTutor profiles with availability and subjects they tutor\n"
         << "\tSearch for tutor by student id\n"
         << "\tSearch for tutor by name\n";

    char inputFileName[] = "tutors.txt";
    BinarySearchTree<Tutor*, TutorLess1>* tutorBSTPtr1 = new BinarySearchTree<Tutor*, TutorLess1>();
    BinarySearchTree<Tutor*, TutorLess2>* tutorBSTPtr2 = new BinarySearchTree<Tutor*, TutorLess2>();
    HashTable<Tutor*, hashKey, TutorEqual>* tutorHashPtr = new HashTable<Tutor*, hashKey, TutorEqual>(getHashSize(inputFileName));

    readData(inputFileName, tutorBSTPtr1, tutorBSTPtr2, tutorHashPtr);

    char choice;
    menu();

    while ((choice = getValidInput()) != QUIT_CHOICE) {
        switch (choice) {
            case INSERT_CHOICE:
                insertData(tutorBSTPtr1, tutorBSTPtr2, tutorHashPtr);
                break;
            case DELETE_CHOICE:
                deleteData(tutorBSTPtr1, tutorBSTPtr2, tutorHashPtr);
                break;
            case SEARCH_CHOICE:
                searchData(tutorBSTPtr2, tutorHashPtr);
                break;
            case DISPLAY_CHOICE:
                displayData(tutorBSTPtr1, tutorBSTPtr2, tutorHashPtr);
                break;
            case SAVE_CHOICE:
                createFile(tutorHashPtr);
                break;
            case SHOW_STATISTICS:
                getStatistics(tutorHashPtr);
                break;
            case SHOW_MENU_CHOICE:
                menu();
                break;
        }
    }
    cout << "\nBefore you go...";
    createFile(tutorHashPtr);
    cout << "\nProgram ending. Good bye\n";

    delete tutorBSTPtr1;
    delete tutorBSTPtr2;
    delete tutorHashPtr;

    return 0;
}

/**~*~*
   This function returns the size of the hash table
   by multiplying the input file size by 2 and finding the nearest prime number.
*~**/
int getHashSize(string filename) {
    ifstream inputFile;
    inputFile.open(filename);

    if (inputFile.fail()) {
        cout << "\nThe file failed to open! Please try again...\n";
        exit(EXIT_FAILURE);
    }

    int count = 0;
    string s;
    while (getline(inputFile, s)) {
        count++;
    }

    count = count * 2 + 1;

    while (true) {
        if (isPrime(count)) break;
        count += 2;
    }
    //cout << "Hash size: " << count << endl;
    return count;
}

/**~*~*
   This function returns true if the input argument is the prime number,
   and false if it's not.
*~**/
bool isPrime(int num) {
    int i = num-1;
    while (i>1) {
        if (num % i == 0) return false;
        i--;
    }
    return true;
}

/**~*~*
   This function reads input file and inserts the pointers of the tutor objects
   into two binary search trees and a hash table.
*~**/
void readData(string filename, BinarySearchTree<Tutor*, TutorLess1>* tutorBSTPtr1, BinarySearchTree<Tutor*, TutorLess2>* tutorBSTPtr2, HashTable<Tutor*, hashKey, TutorEqual>* tutorHashPtr) {
    ifstream inputFile;
    inputFile.open(filename);

    if (inputFile.fail()) {
        cout << "\nThe file failed to open! Please try again...\n";
        exit(EXIT_FAILURE);
    }

    string id, name, subject, course, day, startTime, endTime;

    while (inputFile >> id) {
        Tutor* tempTutorPtr = new Tutor;

        inputFile.ignore();
        getline(inputFile, name, ';');
        inputFile >> subject;

        getline(inputFile, course, ';');
        int i = -1;
        while ((i = course.find(" ", i+1)) != -1) {
            int j = course.find(" ", i+1);
            tempTutorPtr->setCourses(course.substr(i+1, j-i-1));
        }

        getline(inputFile, day, ';');
        i = -1;
        while ((i = day.find(" ", i+1)) != -1) {
            int j = day.find(" ", i+1);
            tempTutorPtr->setDays(day.substr(i+1, j-i-1));
        }

        inputFile >> startTime >> endTime;

        tempTutorPtr->setId(id);
        tempTutorPtr->setName(name);
        tempTutorPtr->setSubject(subject);
        tempTutorPtr->setStartTime(startTime);
        tempTutorPtr->setEndTime(endTime);

        tutorBSTPtr1->insert(tempTutorPtr);
        tutorBSTPtr2->insert(tempTutorPtr);
        tutorHashPtr->hashInsert(tempTutorPtr);

    }
    inputFile.close();
}

/**~*~*
   This function displays input tutor's information as a list.
*~**/
void displayList(Tutor* &person) {
    cout << setw(11) << left << person->getId()
         << setw(20) << person->getName()
         << setw(8) << person->getSubject();

    for(int i =0; i< person->getCourses().size(); ++i)
        cout << person->getCourses()[i] << " ";
    cout << endl;
}

/**~*~*
   This function displays input tutor's id and name.
*~**/
void display(Tutor*& aTutor)
{
    cout << aTutor->getId() << " " << aTutor->getName() << endl;
}

/**~*~*
   The menu function displays the menu.
*~**/
void menu() {
    const string line = "\n*******************************************\n";
    cout << line;
    cout << "\nWhat do you want to do?\n\t"
         << "A - add new tutor\n\t"
         << "D - delete a tutor\n\t"
         << "S - search (by ID or name)\n\t"
         << "L – display a list of tutors\n\t"
         << "F – save data to a file\n\t"
         << "C – show statistics\n\t"
         << "M - show Menu\n\t"
         << "Q – quit the program\n";
    cout << line;
}

/**~*~*
 This function prompts the user to enter a character within given choices.
 If the input is not valid (not a character or if it is outside of the choices) it
 prompts the user to enter a new character, until the input is valid.
 *~**/
char getValidInput(string validChar) {
    char choice;

    do {
        cout << "\nPlease enter a character: ";
        cin >> choice;
        cin.clear();          // to clear the error flag
        cin.ignore(80, '\n'); // to discard the unwanted input from the input buffer
    } while (cin.fail() || !strchr(validChar.c_str(), choice = toupper(choice)));

    return choice;
}

/**~*~*
 This function lets the user enter the information of the new tutor,
 and inserts the pointer into two BST and a hash table.
*~**/
void insertData(BinarySearchTree<Tutor*, TutorLess1>* tutorBSTPtr1, BinarySearchTree<Tutor*, TutorLess2>* tutorBSTPtr2, HashTable<Tutor*, hashKey, TutorEqual>* tutorHashPtr) {
    Tutor* newTutor = new Tutor;
    string id, name, subject, course, day, startTime, endTime;

    cout << "\nEnter a new tutor record for insert.\nTutor ID: ";
    getline(cin, id);
    Tutor tempTutor(id);
    Tutor* returnedTutor;
    bool isDuplicate = tutorHashPtr->hashSearch(&tempTutor, returnedTutor);
    int int_id = atoi(id.c_str());

    while (isDuplicate || id.size() != 8 || int_id < 10000000 || int_id > 99999999) {
        if (isDuplicate) cout << "duplicate id" << endl;
        else cout << "Tutor ID must have exactly 8 digits." << endl;

        cout << "\nTutor ID: ";
        getline(cin, id);
        Tutor tempTutor(id);
        isDuplicate = tutorHashPtr->hashSearch(&tempTutor, returnedTutor);
        int_id = atoi(id.c_str());
    }

    cout << "\nTutor name: ";
    getline(cin, name);

    cout << "\nTutor subject: ";
    getline(cin, subject); //need to validate

    do {
        cout << "\nCourse you've taken (or Q for stop inserting): ";
        getline(cin, course);
        newTutor->setCourses(course);
    } while(course[0] = toupper(course[0]), course != "Q");

    cout << "\nAvailable day (or Q for stop inserting): ";
    getline(cin, day);
    while(day[0] = toupper(day[0]), day != "Q") {
        if (day.length() < 6 || day.length() > 9 || !isValidDay(day))
            cout << "Wrong format. Please enter again.\n";
        else newTutor->setDays(day);

        cout << "\nAvailable day (or Q for stop inserting): ";
        getline(cin, day);
    }

    cout << "\nstart time: ";
    getline(cin, startTime);

    while (!isTimeValid(startTime)) {
        cout << "\nstart time: ";
        getline(cin, startTime);
    }

    cout << "\nend time: ";
    getline(cin, endTime);

    while (!isTimeValid(endTime)) {
        cout << "\nend time: ";
        getline(cin, endTime);
    }

    newTutor->setId(id);
    newTutor->setName(name);
    newTutor->setSubject(subject);
    newTutor->setStartTime(startTime);
    newTutor->setEndTime(endTime);

    tutorBSTPtr1->insert(newTutor);
    tutorBSTPtr2->insert(newTutor);
    tutorHashPtr->hashInsert(newTutor);
}

/**~*~*
 This function returns true if the given time has the right format,
 and false if it's wrong.
*~**/
bool isTimeValid(string t) {
    int idx = t.find(':');
    if (t.length() > 5 || idx == -1) {
        cout << "\nTime format must be ##:##. Please enter again.\n";
        return false;
    } else {
        int hr = atoi(t.substr(0, idx).c_str());
        int min = atoi(t.substr(idx).c_str());
        if (hr <= 0 || hr >= 24 || min < 0 || min >= 60) {
            cout << "Invalid time. Please enter again.\n";
            return false;
        }
    }
    return true;
}

/**~*~*
 This function returns true if the given day has the right format,
 and false if it's wrong.
*~**/
bool isValidDay(string input) {
    char day[] = "MondayTuesdayWednesdayThursdayFriday";

    input[0] = toupper(input[0]);
    for (int i = 1; i < input.length(); ++i)
        input[i] = tolower(input[i]);

    if(strstr(day, input.c_str())) return true;
    return false;
}

/**~*~*
 This function lets the user enter the id of the tutor and
 returns true if it's deleted successfully, and false if it's not.
*~**/
bool deleteData(BinarySearchTree<Tutor*, TutorLess1>* tutorBSTPtr1, BinarySearchTree<Tutor*, TutorLess2>* tutorBSTPtr2, HashTable<Tutor*, hashKey, TutorEqual>* tutorHashPtr) {
    string targetID;
    cout << "\nEnter a Tutor ID for delete: ";
    getline(cin, targetID);
    Tutor targetTutor(targetID);
    Tutor* returnedTutor;

    if (tutorHashPtr->hashSearch(&targetTutor, returnedTutor)) {
        tutorBSTPtr1->remove(returnedTutor);
        tutorBSTPtr2->remove(returnedTutor);
        tutorHashPtr->hashErase(returnedTutor);

        delete returnedTutor;
        cout << "\nThe tutor has been deleted!\n";
        return true;
    }
    cout << "\nThis tutor was not found\n";
    return false;
}

/**~*~*
 This function displays head of each row
 when printing the tutor list in the database.
*~**/
void displayHead() {
    const string LINE = "=========  =================== ======= ========================== \n";

    cout << setprecision(2) << fixed;
    cout << LINE;
    cout << setw(11) << left << "ID"
         << setw(20) << "Name"
         << setw(5) << right << "Subject"
         << setw(10) << "Courses" << endl;
    cout << LINE;
}

/**~*~*
 This function displays tail including the number of data
 when printing the tutor list in the database.
*~**/
void displayTail(HashTable<Tutor*, hashKey, TutorEqual>* tutorHashPtr) {
    const string LINE = "=========  =================== ======= ========================== \n";
    cout << LINE;

    cout << "There are a total of " << tutorHashPtr->getTotalCount() << " tutors in database.\n";
}

/**~*~*
 This function shows the submenu of the display menu
 and implements it according to each choice.
*~**/
void displayData(BinarySearchTree<Tutor*, TutorLess1>* tutorBSTPtr1, BinarySearchTree<Tutor*, TutorLess2>* tutorBSTPtr2, HashTable<Tutor*, hashKey, TutorEqual>* tutorHashPtr) {
    cout << "\nDISPLAY SUBMENU\n\t"
         << "U - list unsorted data\n\t"
         << "P - list data sorted by ID\n\t"
         << "N - list data sorted by name\n\t"
         << "T - print data as an indented list\n\t"
         << "Z - display developers\n\t"
         << "Q – leave the submenu\n\t";

    char choice = getValidInput("UPNTZQ");

    switch (choice) {
        case DISPLAY_UNSORTED:
            displayHead();
            tutorHashPtr->hashDisplay(displayList);
            displayTail(tutorHashPtr);
            break;
        case BY_ID_CHOICE:
            displayHead();
            tutorBSTPtr1->inOrder(displayList);
            displayTail(tutorHashPtr);
            break;
        case BY_NAME_CHOICE:
            displayHead();
            tutorBSTPtr2->inOrder(displayList);
            displayTail(tutorHashPtr);
            break;
        case DISPLAY_INDENTED_CHOICE:
            tutorBSTPtr1->printIndentedList(display);
            break;
        case DISPLAY_DEVELOPERS:
            cout << "\nDeveloped by Heather Koo and Habin Cho\n";
            break;
        case QUIT_CHOICE:
            cout << "Come back to the main menu\n";
            break;

    }
}

/**~*~*
 This function shows the submenu of the search menu
 and implements it according to each choice.
*~**/
void searchData(BinarySearchTree<Tutor*, TutorLess2>* tutorBSTPtr2, HashTable<Tutor*, hashKey, TutorEqual>* tutorHashPtr) {
    cout << "\nSEARCH SUBMENU\n\t"
         << "P - list data sorted by ID\n\t"
         << "N - list data sorted by name\n\t"
         << "Q – leave the submenu\n\t";

    char choice = getValidInput("PNQ");

    switch (choice) {
        case BY_ID_CHOICE:
            searchID(tutorHashPtr, display);
            break;
        case BY_NAME_CHOICE:
            searchName(tutorBSTPtr2, display);
            break;
        case QUIT_CHOICE:
            cout << "Come back to the main menu\n";
            break;
    }
}

/**~*~*
 This function searches a tutor by ID in the hash table.
*~**/
void searchID(HashTable<Tutor*, hashKey, TutorEqual>* tutorHashPtr, void display(Tutor*& aTutor)) {
    int targetID;
    cout << "Enter the tutor ID for search: ";
    cin >> targetID;

    while(cin.fail() || targetID < 10000000 || targetID > 99999999) {
        cin.clear();
        cin.ignore(80, '\n');
        cout << "\nID must be a 8-digit integer.\n\nEnter the tutor ID for search: ";
        cin >> targetID;
    }
    cin.ignore(80, '\n');

    Tutor targetTutor(to_string(targetID));
    Tutor* returnedTutor;
    if (tutorHashPtr->hashSearch(&targetTutor, returnedTutor)) {
        cout << "FOUND\n";
        displayList(returnedTutor);
    } else
        cout << "The tutor ID could not be identified!" << endl;
}

/**~*~*
 This function searches a tutor by name in the BST.
*~**/
void searchName(BinarySearchTree<Tutor*, TutorLess2>* tutorBSTPtr2, void display(Tutor*& aTutor)) {
    string targetName;
    cout << "Enter the tutor name for search: ";
    getline(cin, targetName);

    Tutor targetTutor;
    targetTutor.setName(targetName);
    tutorBSTPtr2->printSameEntry(&targetTutor, displayList);
}

/**~*~*
 This function displays statistics related to the hash table.
*~**/
void getStatistics(HashTable<Tutor*, hashKey, TutorEqual>* tutorHashPtr) {
    cout << "Number of collision: " << tutorHashPtr->getCollisionNum() << endl;
    cout << "Load factor: " << tutorHashPtr->getLoadFactor() << "%" << endl;
}

/**~*~*
 This function displays tutor information with the same format as the input file.
*~**/
void fileDisplay(Tutor* &person, ostream & os) {
    os << person->getId()  << " " << person->getName()  << "; " << person->getSubject() << " ";
    vector<string> courses = person->getCourses();
    vector<string> days = person->getDays();
    os << courses[0];
    for ( int i = 1 ; i < courses.size() ; ++i)
    {
        os << " " << courses[i];
    }
    os << "; " << days[0];
    for (int i = 1; i < days.size() ; ++i)
    {
        os << " " << days[i];
    }
    os << "; ";
    os << person->getStartTime() << " " << person->getEndTime();
    os << endl;
}

/**~*~*
   This function saves the updated list to an output file.
*~**/
void createFile(HashTable<Tutor*, hashKey, TutorEqual>* tutorHashPtr) {
    string fname;
    cout << "\nSave the updated tutor list to a file";
    cout << "\nEnter a file name to write list :";
    cin >> fname;
    cin.ignore();
    ofstream outFile(fname);

    if(outFile.is_open()) {
        cout << "Now writing data to the file named " << fname << endl;
        tutorHashPtr->writeItems(fileDisplay, outFile);
        cout << "Done\n";
        outFile.close();
    } else cout << "Unable to open file" << endl;
}

