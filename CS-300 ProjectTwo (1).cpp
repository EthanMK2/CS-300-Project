// CS-300 Project 2
// Ethan Klukkert
// 16 October 2022

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Course {
private:
    string courseId;
    string courseName;
    vector<string> prereqList;
public:
    Course() {
        this->courseId = "NoID";
        this->courseName = "NoName";
    }

    Course(string id, string name) {  // constructor for courses with no prerequisites 
        this->courseId = id;
        this->courseName = name;
    }

    Course(string id, string name, vector<string> prereqs) {
        this->courseId = id;
        this->courseName = name;
        this->prereqList = prereqs;
    }

    string getId() {
        return this->courseId;
    }

    string getName() {
        return this->courseName;
    }

    vector<string> getPrereqList() {
        return this->prereqList;
    }

};

// taken from vector assignment and adapted for this program
void selectionSort(vector<string>& courseNames) {  // sorts alphanumerically
    if (courseNames.size() == 0) {
        return;
    }
    for (int i = 0; i < courseNames.size() - 1; i++) {
        int min_index = i;
        for (int j = min_index + 1; j < courseNames.size(); j++) {
            if ((courseNames.at(j).compare(courseNames.at(min_index))) < 0) {
                min_index = j;
            }
        }
        swap(courseNames.at(i), courseNames.at(min_index));  // move minimum in unsorted part down to min_index
    }
}

// ==========================================
// Hash Table
// ==========================================
class HashTable {

private:
    struct Node {
        Course course;
        unsigned int key;
        Node* next;
        vector<Course> chain; // each node has a list (Chaining technique for collisions)

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a course
        Node(Course aCourse) : Node() {
            course = aCourse;
        }

        // initialize with a course and a key
        Node(Course aCourse, unsigned int aKey) : Node(aCourse) {
            key = aKey;
        }
    };

    vector<Node> nodes;

    unsigned int tableSize = 20;

    unsigned int hash(string courseId);

public:
    HashTable();
    HashTable(unsigned int size);
    void Insert(Course course);
    Course Search(string courseId);
};

HashTable::HashTable() {
    this->nodes.resize(this->tableSize);
    // Initalize node structure by resizing with tableSize
}

HashTable::HashTable(unsigned int size) {
    this->nodes.resize(size);
}

unsigned int HashTable::hash(string courseId) {  // try block here instead of overcomplicating the loadCourses func
    string lastThreeNums = courseId.substr(4, 3);
    int key;

    try {
        key = stoi(lastThreeNums);
    } 
    catch (...)  {
        cout << "Course does not exist." << endl;
        return -1;
    }

    return key % this->tableSize; // indexes are determined by a modulo hash function.
}

void HashTable::Insert(Course course) {
    int hashIndex = hash(course.getId());  // hash the key for index value
    this->nodes.at(hashIndex).chain.push_back(course);  // insert bid at end of linked list (chain) at hash index
}

Course HashTable::Search(string courseId) {

    if (courseId.length() == 7) {
        int hashIndex = hash(courseId);

        if (hashIndex < 0) {  // stoi() in hash function failed
            return Course();
        }

        if (this->nodes.at(hashIndex).chain.size() != 0) {  // make sure it has items at index

            vector<Course> vectorToSearch = this->nodes.at(hashIndex).chain;

            for (int i = 0; i < vectorToSearch.size(); i++) {  // look for Course in vector

                if (vectorToSearch.at(i).getId() == courseId) {
                    return vectorToSearch.at(i);  // found course
                }
            }
            cout << "Course does not exist." << endl;
            return Course();
        }
    }

    cout << "Course Does not exist." << endl;
    return Course();
}

// loadCourses function works for course id's, names, and prequisites of any size, given one course per line.
// The Hash Table, however, is NOT dynamic, and is restricted to hashing courseId's of fours letter and three
// numbers like this: ABCD123
bool loadCourses(string fileName, vector<string>& courseNameList, HashTable& courseHashTable) {
    fstream textFile;
    textFile.open(fileName, ios::in);
    
    if (textFile.is_open()) {

        string line;
        int i = 0;

        while (getline(textFile, line)) {
            
            // MAY HAVE TO COMMENT OUT THIS IF CHECK
            // the first few chars are not the first course ID, (weird characters?)
            // so I added a newline in the file in order to skip them here
            // ==============
            if (i == 0) {
                i++;
                continue;
            }
            // ==============

            char curChar = 'a';

            int j = 0;
            string courseId = "";
            while (j < line.length()) {
                curChar = line.at(j);
                courseId += curChar;
                j++;
                curChar = line.at(j);  // assigned again so that the comma is not copied to courseID

                if (curChar == ',') {
                    // the first comma. (next characters thru end of line OR new comma is: name of course)
                    string courseName = "";
                    j++;
                    curChar = line.at(j);  // next char after the comma we detected
                    while (j < line.length() && curChar != ',') {
                        courseName += curChar;
                        j++;

                        if (j >= line.length() || curChar == ',') {

                            break;
                        }
                        curChar = line.at(j);
                    }

                    if (j >= line.length()) {  // when no prereqs exist
                        // course constructor no prereq
                        Course* myCourse = new Course(courseId, courseName);

                        // Add string to vector for sorting later
                        courseNameList.push_back("" + courseId + ", " + courseName);

                        // Add course to Hash Table
                        courseHashTable.Insert(*myCourse);

                        continue;  // Move to next line in file
                    }
                    // if not done with line, there are prerequisites
                    else if (curChar == ',') {

                        // comma after course name. (next characters thru end of line OR to new comma is: a prerequisite)
                        j = j + 1;  // index after comma
                        vector<string> prereqList;
                        string coursePrereq = "";
                        curChar = line.at(j);

                        while (j < line.length()) {
                            
                            if (curChar == ',') {
                                prereqList.push_back(coursePrereq);

                                // reset values
                                coursePrereq = "";
                                j++;
                                curChar = line.at(j);
                                continue;  // go to next prerequisite
                            }

                            coursePrereq += curChar;
                            j++;

                            if (j >= line.length()) {
                                prereqList.push_back(coursePrereq);  // add prerequisite to vector

                                // course constructor with prereqs
                                Course* myCourse = new Course(courseId, courseName, prereqList);

                                // Add string to vector for sorting later
                                courseNameList.push_back("" + courseId + ", " + courseName);

                                // Insert into hash table
                                courseHashTable.Insert(*myCourse);
                                break;
                            }
                            curChar = line.at(j);  // go to next char
                        }
                    }

                }
            }

            i++;

        }

        textFile.close();
        return true;  // return true if file was successfully loaded and closed
    }
    else {
        cout << "File does not exist." << endl;
        return false;
    }
}

int main()
{
    vector<string> courseList;  // for sorting and printing list only (hash table will do searching and printing)
    HashTable courseHashTable;
    bool fileLoaded = false;  // check if a file was loaded
    
    string courseIdToFind;
    Course foundCourse;

    int answer = -1;
    while (answer != 4) {
        cout << "\nWelcome to the course planner.\n\n";
        cout << "    1. Load Hash Table"
            << endl << "    2. Print Course List"
            << endl << "    3. Print Course"
            << endl << "    4. Exit"
            << endl << endl << "What would you like to do? "
            << endl;

        cin >> answer;

        string userFileName;

        switch (answer)
        {
        case 1:
 
            if (!fileLoaded) {
                cout << "What file would you like to load? " << endl;
                cin >> userFileName;
                fileLoaded = loadCourses(userFileName, courseList, courseHashTable);
            }
            else {
                cout << "Already Loaded a Course File." << endl;
            }
            break;
        case 2:

            // I sort a vector because sorting (and printing in the same process) 
            // for hash tables directly is impossible as far as I know
            selectionSort(courseList);
            for (int i = 0; i < courseList.size(); i++) {
                cout << courseList.at(i) << endl;
            }

            break;
        case 3:
            cout << "What course do you want to know about?" << endl;
            
            cin >> courseIdToFind;

            foundCourse = courseHashTable.Search(courseIdToFind);
            if (foundCourse.getName() != "NoName") {  // course doesn't exist
                cout << "\n" + foundCourse.getId() + ", " + foundCourse.getName() << endl;
                cout << "Prerequisites:";
                for (int i = 0; i < foundCourse.getPrereqList().size(); i++) {
                    cout << " " + foundCourse.getPrereqList().at(i) + ",";
                }
                cout << endl;
            }
            break;
        case 4:
            cout << "Thank you for using the course planner!" << endl;
            return 0;
            break;
        default:
            cout << answer << " is not a valid option." << endl;
            break;
        }
    }
}
