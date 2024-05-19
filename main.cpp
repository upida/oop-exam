#include <iostream>
#include <vector>
#include <any>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

class Score;
class _Class;
class Subject;
class Person;
class Student;
class Teacher;
class Schedule;
class File;
class School;

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
};

class Student : public Person
{
    private:
        _Class _class;
        float average_score = 0;
        vector<Score> scores;
    public:
        Student(string name, string id, _Class _class) : Person(id, name), _class(_class) {}

        _Class getClass()
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
};

class Teacher : public Person
{
    private:
        Subject subject;
        string education_level;
    public:
        Teacher(string name, string id, Subject subject, string education_level) : Person(id, name), subject(subject), education_level(education_level) {}

        Subject getSubject()
        {
            return this->subject;
        }

        string getEducationLevel()
        {
            return this->education_level;
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

        template<typename T>
        string join(const vector<T>& vec, const string& delimiter)
        {
            stringstream ss;
            for (size_t i = 0; i < vec.size(); ++i)
            {
                if (i != 0)
                    ss << delimiter;
                ss << vec[i];
            }
            return ss.str();
        }

    public:
        School() {}

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

        void addClass(const string &title)
        {
            classes.push_back(_Class(title));
        }

        void addSubject(const string &title)
        {
            subjects.push_back(Subject(title));
        }

        void addStudent(const string &name, const string &id, const string &classTitle)
        {
            _Class _class(classTitle);
            students.push_back(Student(name, id, _class));
            string data = id + "," + name + "," + classTitle;
            File::write("students.txt", data);
        }

        void addTeacher(const string &name, const string &id, const string &subjectTitle, const string &educationLevel)
        {
            Subject subject(subjectTitle);
            teachers.push_back(Teacher(name, id, subject, educationLevel));
            string data = id + "," + name + "," + subjectTitle + "," + educationLevel;
            File::write("teachers.txt", data);
        }

        void editStudent(const string &id, const string &newName, const string &newClassTitle)
        {
            for (auto &student : students)
            {
                if (student.getId() == id)
                {
                    student = Student(newName, id, _Class(newClassTitle));
                    string data = id + "," + newName + "," + newClassTitle;
                    File::modify("students.txt", id, data);
                    return;
                }
            }
            cerr << "Student not found with ID: " << id << endl;
        }

        void editTeacher(const string &id, const string &newName, const string &newSubjectTitle, const string &newEducationLevel)
        {
            for (auto &teacher : teachers)
            {
                if (teacher.getId() == id)
                {
                    teacher = Teacher(newName, id, Subject(newSubjectTitle), newEducationLevel);
                    string data = id + "," + newName + "," + newSubjectTitle + "," + newEducationLevel;
                    File::modify("teachers.txt", id, data);
                    return;
                }
            }
            cerr << "Teacher not found with ID: " << id << endl;
        }

        vector<Student> searchStudent(RequestSearchStudent request)
        {
            vector<Student> result;
            for (auto &student : students)
            {
                if (request.name != "" && student.getName() != request.name)
                    continue;
                if (request.id != "" && student.getId() != request.id)
                    continue;
                if (request._class != "" && student.getClass().getTitle() != request._class)
                    continue;
                result.push_back(student);
            }
            return result;
        }

        vector<Teacher> searchTeacher(RequestSearchTeacher request)
        {
            vector<Teacher> result;
            for (auto &teacher : teachers)
            {
                if (request.name != "" && teacher.getName() != request.name)
                    continue;
                if (request.id != "" && teacher.getId() != request.id)
                    continue;
                if (request.subject != "" && teacher.getSubject().getTitle() != request.subject)
                    continue;
                if (request.education_level != "" && teacher.getEducationLevel() != request.education_level)
                    continue;
                result.push_back(teacher);
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
};

int main()
{
    School school;
    school.addClass("10A");
    school.addClass("10B");
    school.addSubject("Mathematics");
    school.addSubject("Physics");

    school.addStudent("John Doe", "S001", "10A");
    school.addStudent("Jane Smith", "S002", "10B");
    school.addTeacher("Mr. Johnson", "T001", "Mathematics", "MSc");
    school.addTeacher("Ms. Parker", "T002", "Physics", "PhD");

    school.editStudent("S001", "Johnathan Doe", "10B");
    school.editTeacher("T001", "Mr. John Johnson", "Mathematics", "PhD");

    vector<Subject>* subjects = school.getSubjects();
    Student student = school.getStudents()->at(0);
    student.addScore(subjects->at(0), 85);

    cout << " CEK " << school.getStudents()->at(0).getAverageScore() << endl;

    auto students = school.searchStudent({ name: "Johnathan Doe"});
    for (auto &student : students)
    {
        cout << "Student: " << student.getName() << ", ID: " << student.getId() << ", Class: " << student.getClass().getTitle() << ", Average Score: " << student.getAverageScore() << endl;
    }

    auto teachers = school.searchTeacher({"Mr. John Johnson"});
    for (auto &teacher : teachers)
    {
        cout << "Teacher: " << teacher.getName() << ", ID: " << teacher.getId() << ", Subject: " << teacher.getSubject().getTitle() << ", Education Level: " << teacher.getEducationLevel() << endl;
    }

    return 0;
}
