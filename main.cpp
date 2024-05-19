#include <iostream>
#include <vector>
#include <any>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

class _Class
{
    private:
        string title;
    public:
        _Class(string title) : title(title) {}

        string getTitle()
        {
            return this->title;
        }
};

class Subject
{
    private:
        string title;
    public:
        Subject(string title) : title(title) {}

        string getTitle()
        {
            return this->title;
        }
};

class Score
{
    private:
        Subject subject;
        int value;
    public:
        Score(Subject subject, int value) : subject(subject), value(value) {}

        Subject getSubject()
        {
            return this->subject;
        }

        int getValue()
        {
            return this->value;
        }
};

class Person
{
    private:
        string id, name;
    public:
        Person(string id, string name) : id(id), name(name) {}

        string getId()
        {
            return this->id;
        }

        string getName()
        {
            return this->name;
        }

        virtual void displayInfo() = 0;
};

class Student : public Person
{
    private:
        _Class* _class;
        float average_score = 0;
        vector<Score> scores;
    public:
        Student(string name, string id, _Class* _class) : Person(id, name), _class(_class) {}

        _Class* getClass()
        {
            return this->_class;
        }

        float getAverageScore()
        {
            this->calculateAverageScore();
            return this->average_score;
        }

        void addScore(Subject subject, int value)
        {
            Score score(subject, value);
            scores.push_back(score);
            this->calculateAverageScore();
        }

        void calculateAverageScore()
        {
            int total = 0;
            for (auto &score : scores)
            {
                total += score.getValue();
            }
            if (!scores.empty())
            {
                this->average_score = static_cast<float>(total) / scores.size();
            }
        }

        void displayInfo() override
        {
            cout << "Student Name: " << getName() << ", ID: " << getId() << ", Class: " << _class->getTitle() << ", Average Score: " << getAverageScore() << endl;
        }
};

class Teacher : public Person
{
    private:
        Subject* subject;
        string education_level;
    public:
        Teacher(string name, string id, Subject* subject, string education_level) : Person(id, name), subject(subject), education_level(education_level) {}

        Subject* getSubject()
        {
            return this->subject;
        }

        string getEducationLevel()
        {
            return this->education_level;
        }

        void displayInfo() override
        {
            cout << "Teacher Name: " << getName() << ", ID: " << getId() << ", Subject: " << subject->getTitle() << ", Education Level: " << education_level << endl;
        }
};

class Schedule
{
    private:
        _Class _class;
        Subject subject;
        Teacher teacher;
        string day;
        string time;
        bool practicum;
    public:
        Schedule(_Class _class, Subject subject, Teacher teacher, string day, string time, bool practicum = false) : _class(_class), subject(subject), teacher(teacher), day(day), time(time), practicum(practicum) {}

        _Class getClass()
        {
            return this->_class;
        }

        Subject getSubject()
        {
            return this->subject;
        }

        Teacher getTeacher()
        {
            return this->teacher;
        }

        string getDay()
        {
            return this->day;
        }

        string getTime()
        {
            return this->time;
        }

        bool getPracticum()
        {
            return this->practicum;
        }
};

class File
{
    public:
        static void write(const string &filename, const string &data)
        {
            string filepath = "data/" + filename;
            ofstream file(filepath, ios::app);
            if (file.is_open())
            {
                file << data << endl;
                file.close();
            }
            else
            {
                cerr << "Unable to open file for writing: " << filepath << endl;
            }
        }

        static void modify(const string &filename, const string &id, const string &newData)
        {
            string filepath = "data/" + filename;
            ifstream file(filepath);
            if (!file.is_open())
            {
                cerr << "Unable to open file for reading: " << filepath << endl;
                return;
            }

            vector<string> lines;
            string line;
            bool found = false;

            while (getline(file, line))
            {
                stringstream ss(line);
                string currentId;
                getline(ss, currentId, ',');

                if (currentId == id)
                {
                    lines.push_back(newData);
                    found = true;
                }
                else
                {
                    lines.push_back(line);
                }
            }
            file.close();

            if (found)
            {
                ofstream outFile(filepath);
                if (outFile.is_open())
                {
                    for (const auto &l : lines)
                    {
                        outFile << l << endl;
                    }
                    outFile.close();
                }
                else
                {
                    cerr << "Unable to open file for writing: " << filepath << endl;
                }
            }
            else
            {
                cerr << "ID not found: " << id << endl;
            }
        }

        static vector<string> read(const string &filename)
        {
            string filepath = "data/" + filename;
            ifstream file(filepath);
            vector<string> lines;
            if (file.is_open())
            {
                string line;
                while (getline(file, line))
                {
                    lines.push_back(line);
                }
                file.close();
            }
            else
            {
                cerr << "Unable to open file for reading: " << filepath << endl;
            }
            return lines;
        }

        static bool checkId(const string& filename, const string& id)
        {
            string filepath = "data/" + filename;
            ifstream file(filepath);
            if (!file.is_open())
            {
                cerr << "Unable to open file for reading: " << filepath << endl;
                return false;
            }

            string line;
            while (getline(file, line))
            {
                stringstream ss(line);
                string currentId;
                getline(ss, currentId, ',');
                if (currentId == id)
                {
                    file.close();
                    return true;
                }
            }
            file.close();
            return false;
        }
};

struct Sort
{
    string key;
    bool asc;
};

struct RequestSearchStudent
{
    string name = "";
    string id = "";
    string _class = "";
};

struct RequestSearchTeacher
{
    string name = "";
    string id = "";
    string subject = "";
    string education_level = "";
};

struct RequestSearchSchedule
{
    string _class = "";
    string subject = "";
    vector<Sort> sort = {};
};

class School
{
    private:
        vector<_Class> classes;
        vector<Subject> subjects;
        vector<Student> students;
        vector<Teacher> teachers;
        vector<Schedule> schedules;

    public:
        School() {
            readStudentsFromFile("students.txt");
            readTeachersFromFile("teachers.txt");
        }

        void readStudentsFromFile(const string &filename)
        {
            vector<string> lines = File::read(filename);

            for (const auto &line : lines)
            {
                stringstream ss(line);
                string id, name, classTitle;
                getline(ss, id, ',');
                getline(ss, name, ',');
                getline(ss, classTitle, ',');

                _Class* _class = nullptr;
                for (_Class &check_class : classes)
                {
                    if (check_class.getTitle() == classTitle)
                    {
                        _class = &check_class;
                        break;
                    }
                }

                if (_class == nullptr) 
                {
                    _Class newClass(classTitle);
                    classes.push_back(newClass);
                    _class = &classes.back();
                }

                students.push_back(Student(name, id, _class));
            }
        }

        void readTeachersFromFile(const string &filename)
        {
            vector<string> lines = File::read(filename);

            for (const auto &line : lines)
            {
                stringstream ss(line);
                string id, name, subjectTitle, educationLevel;
                getline(ss, id, ',');
                getline(ss, name, ',');
                getline(ss, subjectTitle, ',');
                getline(ss, educationLevel, ',');

                Subject* subject = nullptr;
                for (Subject &check_subject : subjects)
                {
                    if (check_subject.getTitle() == subjectTitle)
                    {
                        subject = &check_subject;
                        break;
                    }
                }

                if (subject == nullptr) 
                {
                    Subject newSubject(subjectTitle);
                    subjects.push_back(newSubject);
                    subject = &subjects.back();
                }

                teachers.push_back(Teacher(name, id, subject, educationLevel));
            }
        }

        vector<_Class>* getClasses()
        {
            return &classes;
        }

        vector<Subject>* getSubjects()
        {
            return &subjects;
        }

        vector<Student>* getStudents()
        {
            return &students;
        }

        vector<Teacher>* getTeachers()
        {
            return &teachers;
        }

        void addClass()
        {
            string title;
            cout << "Enter class title: ";
            cin.ignore();
            getline(cin, title);
            classes.push_back(_Class(title));
        }

        void addSubject()
        {
            string title;
            cout << "Enter subject title: ";
            cin.ignore();
            getline(cin, title);
            subjects.push_back(Subject(title));
        }

        void addStudent()
        {
            string name, id, classTitle, filename = "students.txt";
            cout << "Enter student name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter student ID: ";
            getline(cin, id);

            if(File::checkId(filename, id)) {
                cout << "Canceling add student. Id " << id << " already exist." << endl;
                return;
            }

            cout << "Enter class title: ";
            getline(cin, classTitle);

            _Class* _class = nullptr;
            for (_Class &check_class : classes)
            {
                if (check_class.getTitle() == classTitle)
                {
                    _class = &check_class;
                }
            }

            if (_class == nullptr) {
                cout << "Canceling add student. Class " << classTitle << " not found." << endl;
                return;
            }

            students.push_back(Student(name, id, _class));
            string data = id + "," + name + "," + classTitle;
            File::write(filename, data);
        }

        void addTeacher()
        {
            string name, id, subjectTitle, educationLevel, filename = "teachers.txt";
            cout << "Enter teacher name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter teacher ID: ";
            getline(cin, id);

            if(File::checkId(filename, id)) {
                cout << "Canceling add teacher. Id " << id << " already exist." << endl;
                return;
            }

            cout << "Enter subject title: ";
            getline(cin, subjectTitle);

            Subject* subject = nullptr;
            for (Subject &checksubject : subjects)
            {
                if (checksubject.getTitle() == subjectTitle)
                {
                    subject = &checksubject;
                }
            }

            if (subject == nullptr) {
                cout << "Canceling add teacher. Subject " << subjectTitle << " not found." << endl;
                return;
            }

            cout << "Enter education level: ";
            getline(cin, educationLevel);

            teachers.push_back(Teacher(name, id, subject, educationLevel));
            string data = id + "," + name + "," + subjectTitle + "," + educationLevel;
            File::write(filename, data);
        }

        void editStudent()
        {
            string id, newName, newClassTitle;
            cout << "Enter student ID to edit: ";
            cin.ignore();
            getline(cin, id);
            cout << "Enter new name: ";
            getline(cin, newName);
            cout << "Enter new class title: ";
            getline(cin, newClassTitle);

            _Class* _class = nullptr;
            for (_Class &check_class : classes)
            {
                if (check_class.getTitle() == newClassTitle)
                {
                    _class = &check_class;
                }
            }

            if (_class == nullptr) {
                cout << "Canceling edit student. Class " << newClassTitle << " not found." << endl;
                return;
            }

            for (auto &student : students)
            {
                if (student.getId() == id)
                {
                    student = Student(newName, id, _class);
                    string data = id + "," + newName + "," + newClassTitle;
                    File::modify("students.txt", id, data);
                    return;
                }
            }
            cerr << "Student not found with ID: " << id << endl;
        }

        void editTeacher()
        {
            string id, newName, newSubjectTitle, newEducationLevel;
            cout << "Enter teacher ID to edit: ";
            cin.ignore();
            getline(cin, id);
            cout << "Enter new name: ";
            getline(cin, newName);
            cout << "Enter new subject title: ";
            getline(cin, newSubjectTitle);

            Subject* subject = nullptr;
            for (Subject &checksubject : subjects)
            {
                if (checksubject.getTitle() == newSubjectTitle)
                {
                    subject = &checksubject;
                }
            }

            if (subject == nullptr) {
                cout << "Canceling edit teacher. Subject " << newSubjectTitle << " not found." << endl;
                return;
            }

            cout << "Enter new education level: ";
            getline(cin, newEducationLevel);

            for (auto &teacher : teachers)
            {
                if (teacher.getId() == id)
                {
                    teacher = Teacher(newName, id, subject, newEducationLevel);
                    string data = id + "," + newName + "," + newSubjectTitle + "," + newEducationLevel;
                    File::modify("teachers.txt", id, data);
                    return;
                }
            }
            cerr << "Teacher not found with ID: " << id << endl;
        }

        void addSchedule()
        {
            string classTitle, subjectTitle, teacherId, day, time, practicumInput;
            bool practicum;

            cout << "Enter class title: ";
            cin.ignore();
            getline(cin, classTitle);

            _Class* _class = nullptr;
            for (_Class &check_class : classes)
            {
                if (check_class.getTitle() == classTitle)
                {
                    _class = &check_class;
                    break;
                }
            }

            if (_class == nullptr) {
                cout << "Canceling add schedule. Class " << classTitle << " not found." << endl;
                return;
            }

            cout << "Enter subject title: ";
            getline(cin, subjectTitle);

            Subject* subject = nullptr;
            for (Subject &check_subject : subjects)
            {
                if (check_subject.getTitle() == subjectTitle)
                {
                    subject = &check_subject;
                    break;
                }
            }

            if (subject == nullptr) {
                cout << "Canceling add schedule. Subject " << subjectTitle << " not found." << endl;
                return;
            }

            cout << "Enter teacher ID: ";
            getline(cin, teacherId);

            Teacher* teacher = nullptr;
            for (Teacher &check_teacher : teachers)
            {
                if (check_teacher.getId() == teacherId)
                {
                    teacher = &check_teacher;
                    break;
                }
            }

            if (teacher == nullptr) {
                cout << "Canceling add schedule. Teacher with ID " << teacherId << " not found." << endl;
                return;
            }

            cout << "Enter day: ";
            getline(cin, day);

            cout << "Enter time: ";
            getline(cin, time);

            cout << "Is this a practicum? (yes/no): ";
            getline(cin, practicumInput);
            practicum = (practicumInput == "yes");

            schedules.push_back(Schedule(*_class, *subject, *teacher, day, time, practicum));
        }

        void editSchedule()
        {
            string classTitle, subjectTitle, teacherId, day, time, practicumInput, searchClass, searchSubject;
            bool practicum;

            cout << "Enter class title of the schedule to edit: ";
            cin.ignore();
            getline(cin, searchClass);

            cout << "Enter subject title of the schedule to edit: ";
            getline(cin, searchSubject);

            Schedule* scheduleToEdit = nullptr;
            for (auto &schedule : schedules)
            {
                if (schedule.getClass().getTitle() == searchClass && schedule.getSubject().getTitle() == searchSubject)
                {
                    scheduleToEdit = &schedule;
                    break;
                }
            }

            if (scheduleToEdit == nullptr) {
                cout << "Canceling edit schedule. Schedule for class " << searchClass << " and subject " << searchSubject << " not found." << endl;
                return;
            }

            cout << "Enter new class title: ";
            getline(cin, classTitle);

            _Class* _class = nullptr;
            for (_Class &check_class : classes)
            {
                if (check_class.getTitle() == classTitle)
                {
                    _class = &check_class;
                    break;
                }
            }

            if (_class == nullptr) {
                cout << "Canceling edit schedule. Class " << classTitle << " not found." << endl;
                return;
            }

            cout << "Enter new subject title: ";
            getline(cin, subjectTitle);

            Subject* subject = nullptr;
            for (Subject &check_subject : subjects)
            {
                if (check_subject.getTitle() == subjectTitle)
                {
                    subject = &check_subject;
                    break;
                }
            }

            if (subject == nullptr) {
                cout << "Canceling edit schedule. Subject " << subjectTitle << " not found." << endl;
                return;
            }

            cout << "Enter new teacher ID: ";
            getline(cin, teacherId);

            Teacher* teacher = nullptr;
            for (Teacher &check_teacher : teachers)
            {
                if (check_teacher.getId() == teacherId)
                {
                    teacher = &check_teacher;
                    break;
                }
            }

            if (teacher == nullptr) {
                cout << "Canceling edit schedule. Teacher with ID " << teacherId << " not found." << endl;
                return;
            }

            cout << "Enter new day: ";
            getline(cin, day);

            cout << "Enter new time: ";
            getline(cin, time);

            cout << "Is this a practicum? (yes/no): ";
            getline(cin, practicumInput);
            practicum = (practicumInput == "yes");

            *scheduleToEdit = Schedule(*_class, *subject, *teacher, day, time, practicum);
        }

        vector<Student*> searchStudent(RequestSearchStudent request)
        {
            vector<Student*> result;
            for (auto &student : students)
            {
                if (request.name != "" && student.getName() != request.name)
                    continue;
                if (request.id != "" && student.getId() != request.id)
                    continue;
                if (request._class != "" && student.getClass()->getTitle() != request._class)
                    continue;
                result.push_back(&student);
            }
            return result;
        }

        vector<Teacher*> searchTeacher(RequestSearchTeacher request)
        {
            vector<Teacher*> result;
            for (auto &teacher : teachers)
            {
                if (request.name != "" && teacher.getName() != request.name)
                    continue;
                if (request.id != "" && teacher.getId() != request.id)
                    continue;
                if (request.subject != "" && teacher.getSubject()->getTitle() != request.subject)
                    continue;
                if (request.education_level != "" && teacher.getEducationLevel() != request.education_level)
                    continue;
                result.push_back(&teacher);
            }
            return result;
        }

        vector<Schedule> searchSchedule(RequestSearchSchedule request)
        {
            vector<Schedule> result;
            for (auto &schedule : schedules)
            {
                if (request._class != "" && schedule.getClass().getTitle() != request._class)
                    continue;
                if (request.subject != "" && schedule.getSubject().getTitle() != request.subject)
                    continue;
                result.push_back(schedule);
            }

            for (auto &request_sort : request.sort)
            {
                sort(result.begin(), result.end(), [&](Schedule &a, Schedule &b)
                    {
                        string valueA, valueB;
                        if (request_sort.key == "class")
                        {
                            valueA = a.getClass().getTitle();
                            valueB = b.getClass().getTitle();
                        }
                        else if (request_sort.key == "subject")
                        {
                            valueA = a.getSubject().getTitle();
                            valueB = b.getSubject().getTitle();
                        }
                        return request_sort.asc ? (valueA < valueB) : (valueA > valueB);
                    });
            }

            return result;
        }

        void showStudents()
        {
            cout << "Students:\n";
            for (auto& student : students)
            {
                student.displayInfo();
            }
        }

        void showTeachers()
        {
            cout << "Teachers:\n";
            for (auto& teacher : teachers)
            {
                teacher.displayInfo();
            }
        }

        void showSchedules()
        {
            cout << "Schedules:\n";
            for (auto& schedule : schedules)
            {
                cout << "Class: " << schedule.getClass().getTitle() << ", Subject: " << schedule.getSubject().getTitle() << ", Teacher: " << schedule.getTeacher().getName() << ", Day: " << schedule.getDay() << ", Time: " << schedule.getTime() << ", Practicum: " << (schedule.getPracticum() ? "Yes" : "No") << endl;
            }
        }

        void showClasses()
        {
            cout << "Classes:\n";
            for (auto& _class : classes)
            {
                cout << "Title: " << _class.getTitle() << endl;
            }
        }

        void showSubjects()
        {
            cout << "Subjects:\n";
            for (auto& subject : subjects)
            {
                cout << "Title: " << subject.getTitle() << endl;
            }
        }

        void searchAndDisplayStudents()
        {
            RequestSearchStudent request;
            cout << "Enter student name to search (or leave empty to skip): ";
            cin.ignore();
            getline(cin, request.name);
            cout << "Enter student ID to search (or leave empty to skip): ";
            getline(cin, request.id);
            cout << "Enter class title to search (or leave empty to skip): ";
            getline(cin, request._class);

            vector<Student*> results = searchStudent(request);

            if (results.empty())
            {
                cout << "No students found with the given criteria." << endl;
            }
            else
            {
                cout << "Search Results:\n";
                for (auto* student : results)
                {
                    student->displayInfo();
                }
            }
        }

        void searchAndDisplayTeachers()
        {
            RequestSearchTeacher request;
            cout << "Enter teacher name to search (or leave empty to skip): ";
            cin.ignore();
            getline(cin, request.name);
            cout << "Enter teacher ID to search (or leave empty to skip): ";
            getline(cin, request.id);
            cout << "Enter subject title to search (or leave empty to skip): ";
            getline(cin, request.subject);
            cout << "Enter education level to search (or leave empty to skip): ";
            getline(cin, request.education_level);

            vector<Teacher*> results = searchTeacher(request);

            if (results.empty())
            {
                cout << "No teachers found with the given criteria." << endl;
            }
            else
            {
                cout << "Search Results:\n";
                for (auto* teacher : results)
                {
                    teacher->displayInfo();
                }
            }
        }

        void searchAndDisplaySchedules()
        {
            RequestSearchSchedule request;
            string sortKey, sortOrder;
            char addSort = 'n';
            
            cout << "Enter class title to search (or leave empty to skip): ";
            cin.ignore();
            getline(cin, request._class);
            cout << "Enter subject title to search (or leave empty to skip): ";
            getline(cin, request.subject);

            do {
                cout << "Would you like to add sorting criteria? (y/n): ";
                cin >> addSort;
                if (addSort == 'y' || addSort == 'Y') {
                    Sort sort;
                    cout << "Enter sort key (class/subject): ";
                    cin >> sort.key;
                    cout << "Enter sort order (asc/desc): ";
                    cin >> sortOrder;
                    sort.asc = (sortOrder == "asc");
                    request.sort.push_back(sort);
                }
            } while (addSort == 'y' || addSort == 'Y');

            vector<Schedule> results = searchSchedule(request);

            if (results.empty())
            {
                cout << "No schedules found with the given criteria." << endl;
            }
            else
            {
                cout << "Search Results:\n";
                for (auto& schedule : results)
                {
                    cout << "Class: " << schedule.getClass().getTitle() << ", Subject: " << schedule.getSubject().getTitle() << ", Teacher: " << schedule.getTeacher().getName() << ", Day: " << schedule.getDay() << ", Time: " << schedule.getTime() << ", Practicum: " << (schedule.getPracticum() ? "Yes" : "No") << endl;
                }
            }
        }
};

int main()
{
    School school;

    int choice;
    bool is_choice = true;
    while (is_choice)
    {
        cout << "\nSchool Management System\n";
        cout << "1. Add Class\n";
        cout << "2. Add Subject\n";
        cout << "3. Add Student\n";
        cout << "4. Add Teacher\n";
        cout << "5. Add Schedule\n";
        cout << "6. Edit Student\n";
        cout << "7. Edit Teacher\n";
        cout << "8. Edit Schedule\n";
        cout << "9. Show Students\n";
        cout << "10. Show Teachers\n";
        cout << "11. Show Schedules\n";
        cout << "12. Show Classes\n";
        cout << "13. Show Subjects\n";
        cout << "14. Search Student\n";
        cout << "15. Search Teacher\n";
        cout << "16. Search Schedule\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                school.addClass();
                break;
            case 2:
                school.addSubject();
                break;
            case 3:
                school.addStudent();
                break;
            case 4:
                school.addTeacher();
                break;
            case 5:
                school.addSchedule();
                break;
            case 6:
                school.editStudent();
                break;
            case 7:
                school.editTeacher();
                break;
            case 8:
                school.editSchedule();
                break;
            case 9:
                school.showStudents();
                break;
            case 10:
                school.showTeachers();
                break;
            case 11:
                school.showSchedules();
                break;
            case 12:
                school.showClasses();
                break;
            case 13:
                school.showSubjects();
                break;
            case 14:
                school.searchAndDisplayStudents();
                break;
            case 15:
                school.searchAndDisplayTeachers();
                break;
            case 16:
                school.searchAndDisplaySchedules();
                break;
            case 0:
                is_choice = false;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
