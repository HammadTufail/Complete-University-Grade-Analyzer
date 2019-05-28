#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <set>
#include "gradeutil.h"
#include <iomanip>
void summary();

void searchData();

void unknown();

void dfw();

using namespace std;
Course ParseCourse(string csvline)
{
    Course  course;
    string  value;
    stringstream  ss(csvline);
    getline(ss, course.Dept, ',');
    getline(ss, value, ',');
    course.Number = stoi(value);
    getline(ss, value, ',');
    course.Section = stoi(value);
    getline(ss, course.Title, ',');
    getline(ss, value, ',');
    course.NumA = stoi(value);
    getline(ss, value, ',');
    course.NumB = stoi(value);
    getline(ss, value, ',');
    course.NumC = stoi(value);
    getline(ss, value, ',');
    course.NumD = stoi(value);
    getline(ss, value, ',');
    course.NumF = stoi(value);
    getline(ss, value, ',');
    course.NumI = stoi(value);
    getline(ss, value, ',');
    course.NumNR = stoi(value);
    getline(ss, value, ',');
    course.NumS = stoi(value);
    getline(ss, value, ',');
    course.NumU = stoi(value);
    getline(ss, value, ',');
    course.NumW = stoi(value);
    getline(ss, course.Instructor, ',');
    return course;
}

//Dept ParseDept(string name, vector<string> csvlines)
//{
//  Dept dept;
//
//  return dept;
//}
//
//College ParseCollege(string name, vector<string> csvlines)
//{
//  College college;
//
//  return college;
//}
double GetDFWRate(const Course& c, int& DFW, int& N)
{
    DFW = 0;
    N   = 0;
    Course::GradingType grading = c.getGradingType();

    if (grading != Course::Letter)
        return 0.0;
    DFW = c.NumD + c.NumF + c.NumW;
    N   = c.NumA + c.NumB + c.NumC + c.NumD + c.NumF + c.NumW;

    if (N == 0)
        return 0.0;

    return (static_cast<double>(DFW) / static_cast<double>(N)) * 100.0;
}

double GetDFWRate(const Dept& dept, int& DFW, int& N)
{
    DFW = 0;
    N   = 0;

    for (const Course& c : dept.Courses)
    {
        if (c.getGradingType() != Course::Letter)
            continue;
        DFW += (c.NumD + c.NumF + c.NumW);
        N   += (c.NumA + c.NumB + c.NumC + c.NumD + c.NumF + c.NumW);
    }

    if (N == 0)
        return 0.0;

    return (static_cast<double>(DFW) / static_cast<double>(N)) * 100.0;
}

double GetDFWRate(const College& college, int& DFW, int& N)
{
    DFW = 0;
    N   = 0;

    for (const Dept& dept : college.Depts)
    {
        for (const Course& c : dept.Courses)
        {
            if (c.getGradingType() != Course::Letter)
                continue;
            DFW += (c.NumD + c.NumF + c.NumW);
            N   += (c.NumA + c.NumB + c.NumC + c.NumD + c.NumF + c.NumW);
        }
    }

    if (N == 0)
        return 0.0;

    return (static_cast<double>(DFW) / static_cast<double>(N)) * 100.0;
}
GradeStats GetGradeDistribution(const Course& c)
{
    GradeStats stats;
    Course::GradingType grading = c.getGradingType();

    if (grading != Course::Letter)
        return stats;
    stats.N = c.NumA + c.NumB + c.NumC + c.NumD + c.NumF;

    if (stats.N == 0)
        return stats;

    stats.NumA = c.NumA;
    stats.PercentA = (static_cast<double>(stats.NumA) / static_cast<double>(stats.N)) * 100.0;

    stats.NumB = c.NumB;
    stats.PercentB = (static_cast<double>(stats.NumB) / static_cast<double>(stats.N)) * 100.0;

    stats.NumC = c.NumC;
    stats.PercentC = (static_cast<double>(stats.NumC) / static_cast<double>(stats.N)) * 100.0;

    stats.NumD = c.NumD;
    stats.PercentD = (static_cast<double>(stats.NumD) / static_cast<double>(stats.N)) * 100.0;

    stats.NumF = c.NumF;
    stats.PercentF = (static_cast<double>(stats.NumF) / static_cast<double>(stats.N)) * 100.0;

    return stats;
}

GradeStats GetGradeDistribution(const Dept& dept)
{
    Course  all;
    for (const Course& c : dept.Courses)
    {
        if (c.getGradingType() != Course::Letter)
            continue;
        all.NumA += c.NumA;
        all.NumB += c.NumB;
        all.NumC += c.NumC;
        all.NumD += c.NumD;
        all.NumF += c.NumF;
    }
    return GetGradeDistribution(all);
}

GradeStats GetGradeDistribution(const College& college)
{
    Course  all;
    for (const Dept& dept : college.Depts)
    {
        for (const Course& c : dept.Courses)
        {
            if (c.getGradingType() != Course::Letter)
                continue;
            all.NumA += c.NumA;
            all.NumB += c.NumB;
            all.NumC += c.NumC;
            all.NumD += c.NumD;
            all.NumF += c.NumF;
        }
    }
    return GetGradeDistribution(all);
}
vector<Course> FindCourses(const Dept& dept, int courseNumber)
{
    vector<Course>  courses;
    for(const Course& course : dept.Courses)
    {
        if (course.Number == courseNumber)  // match:
        {
            courses.push_back(course);
        }
    }
    if (courses.size() > 1)
    {
        sort(courses.begin(), courses.end(),
             [](const Course& c1, const Course& c2)
             {
                 if (c1.Number < c2.Number)
                     return true;
                 else if (c1.Number > c2.Number)
                     return false;
                 else
                 if (c1.Section < c2.Section)
                     return true;
                 else
                     return false;
             }
        );
    }

    return courses;
}
vector<Course> FindCourses(const Dept& dept, string instructorPrefix)
{
    vector<Course>  courses;
    size_t len = instructorPrefix.length();
    for(const Course& course : dept.Courses)
    {
        if (course.Instructor.substr(0, len) == instructorPrefix)  // match:
        {
            courses.push_back(course);
        }
    }
    if (courses.size() > 1)
    {
        sort(courses.begin(), courses.end(),
             [](const Course& c1, const Course& c2)
             {
                 if (c1.Number < c2.Number)
                     return true;
                 else if (c1.Number > c2.Number)
                     return false;
                 else
                 if (c1.Section < c2.Section)
                     return true;
                 else
                     return false;
             }
        );
    }

    return courses;
}

vector<Course> FindCourses(const College& college, int courseNumber)
{
    vector<Course>  courses;

    for(const Dept& dept : college.Depts)
    {
        vector<Course> onedept = FindCourses(dept, courseNumber);

        for(const Course& c : onedept)
        {
            courses.push_back(c);
        }
    }

    if (courses.size() > 1)
    {
        sort(courses.begin(), courses.end(),
             [](const Course& c1, const Course& c2)
             {
                 if (c1.Dept < c2.Dept)
                     return true;
                 else if (c1.Dept > c2.Dept)
                     return false;
                 else // same dept, look at course #:
                 if (c1.Number < c2.Number)
                     return true;
                 else if (c1.Number > c2.Number)
                     return false;
                 else // same course #, look at section #:
                 if (c1.Section < c2.Section)
                     return true;
                 else
                     return false;
             });
    }
    return courses;
}
vector<Course> FindCourses(const College& college, string instructorPrefix)
{
    vector<Course>  courses;
    for(const Dept& dept : college.Depts)
    {
        vector<Course> onedept = FindCourses(dept, instructorPrefix);

        for(const Course& c : onedept)
        {
            courses.push_back(c);
        }
    }
    if (courses.size() > 1)
    {
        sort(courses.begin(), courses.end(),
             [](const Course& c1, const Course& c2)
             {
                 if (c1.Instructor < c2.Instructor)
                     return true;
                 else if (c1.Instructor > c2.Instructor)
                     return false;
                 else
                 if (c1.Number < c2.Number)
                     return true;
                 else if (c1.Number > c2.Number)
                     return false;
                 else
                 if (c1.Section < c2.Section)
                     return true;
                 else
                     return false;
             }
        );
    }

    return courses;
}
int fileSize(std::string name)
{
    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile(name);

    while (std::getline(myfile, line))
        ++number_of_lines;
    myfile.close();
    return number_of_lines;
}

void summaryShow(Dept dept)
{
    int total_courses=0,total_students=0;
    vector<Course> cc=dept.Courses;
    sort(cc.begin(),cc.end(),
         [](const Course& c1, const Course& c2)
         {
             if(c1.Instructor.compare(c2.Instructor)>0)
             {
                 return false;
             } else
             {
                 return true;
             }

         }
    );
    GradeStats gs=GetGradeDistribution(dept);
    for(const Course& c : cc)
    {
        total_students=total_students+c.getNumStudents();
    }
    int a=0,b=0;
    cout<<dept.Name<<":\n"<<" ";
    cout<<"# courses taught: "<<dept.Courses.size()<<"\n"<<" ";
    cout<<"# students taught: "<<total_students<<"\n"<<" ";
    cout<<"grade distribution (A-F): "<<gs.PercentA<<"%, "<<gs.PercentB<<"%, "<<gs.PercentC<<"%, "<<gs.PercentD<<"%, "<<gs.PercentF<<"%\n"<<" ";
    cout<<"DFW rate: "<<GetDFWRate(dept,a,b)<<"%\n";
}
Dept getDepart(vector<Dept> departs,string deptName)
{
    Dept tmp;
    for(const Dept& d : departs)
    {
        if(deptName.compare(d.Name)==0)
        {
            return d;
        }
    }
    tmp.Name="Unknown";
    return tmp;
}
void dataSummary(vector<Dept> departs)
{
    string deptName;
    cout<<"dept name, or all? ";
    cin>>deptName;
    if(deptName.compare("all")==0)
    {
        for(const Dept& d : departs)
        {
            summaryShow(d);
        }
    }
    else
    {
        summaryShow(getDepart(departs,deptName));
    }
}
string getGradingTypeStr(int gradingType)
{
    if(gradingType==0)
    {
        return "letter";
    }
    else if(gradingType==1)
    {
        return "satisfactory";
    }
    else if(gradingType=2)
    {
        return "Unknown";
    }
}

void courseSummary(vector<Course> courses)
{

    int a=0,b=0;
    for(const Course& c : courses)
    {
        cout<<c.Dept<<" "<<c.Number<<"(section "<<c.Section<<"): "<<c.Instructor<<"\n";
        cout<<" # students: "<<c.getNumStudents()<<"\n";
        cout<<" course type: "<<getGradingTypeStr(c.getGradingType())<<"\n";
        GradeStats gs = GetGradeDistribution(c);
        cout<<" grade distribution (A-F): "<<gs.PercentA<<"%, "<<gs.PercentB<<"%, "<<gs.PercentC<<"%, "<<gs.PercentD<<"%, "<<gs.PercentF<<"%\n";
        cout<<" DFW rate: "<<GetDFWRate(c,a,b)<<"%\n";
    }
}
bool contains(vector<Dept> dd,Course cc)
{
    for(const Dept& d : dd)
    {
        if(d.Name.compare(cc.Dept)==0)
        {
            return true;
        }
    }
    return false;
}
vector<Course> dfwThresholdHelper(Dept depart,double threshold)
{
    int a=0,b=0;
    vector<Course> dfwThresholdGraded;
    vector<Course> courses = depart.Courses;
        for (const Course &c : courses) {
            if (GetDFWRate(c, a, b) >= threshold) {
                dfwThresholdGraded.push_back(c);
            }
        }
    sort(dfwThresholdGraded.begin(),dfwThresholdGraded.end(),
         [](const Course& c1, const Course& c2)
         {
             int a=0,b=0;
             if(GetDFWRate(c1,a,b)-GetDFWRate(c2,a,b)>0)
             {
                 return true;
             } else
             {
                 return false;
             }

         }
    );
    return dfwThresholdGraded;
}
vector<Course> dfwThresholdHelper(vector<Dept> depart,double threshold)
{
    int a=0,b=0;
    vector<Course> dfwThresholdGraded;
    for(const Dept& d : depart) {
        vector<Course> courses = d.Courses;
        for (const Course &c : courses) {
            if (GetDFWRate(c, a, b) >= threshold) {
                dfwThresholdGraded.push_back(c);
            }
        }
    }
    sort(dfwThresholdGraded.begin(),dfwThresholdGraded.end(),
         [](const Course& c1, const Course& c2)
         {
             int a=0,b=0;
             if(GetDFWRate(c1,a,b)-GetDFWRate(c2,a,b)>0)
             {
                 return true;
             } else
             {
                 return false;
             }

         }
    );
    return dfwThresholdGraded;
}
void dfw(vector<Dept> departs)
{
    string deptName;
    double threshold;
    cout<<"dept name, or all? ";
    cin>>deptName;
    cout<<"dfw threshold? ";
    cin>>threshold;
    Dept currentDept;
    vector<Course> courses;
    vector<Course> v;
    if(deptName.compare("all")==0)
    {
        courses=dfwThresholdHelper(departs,threshold);
        courseSummary(courses);
    }
    else
    {
        currentDept=getDepart(departs,deptName);
        courses = dfwThresholdHelper(currentDept,threshold);
        courseSummary(courses);
    }
}
vector<Course> getUnknownGraded(Dept depart)
{
    vector<Course> UnknownGradedCourses;
    for(const Course& c : depart.Courses)
    {
        if(c.getGradingType()==Course::Unknown)
        {
            UnknownGradedCourses.push_back(c);
        }
    }
    sort(UnknownGradedCourses.begin(),UnknownGradedCourses.end(),
         [](const Course& c1, const Course& c2)
         {
             if(c1.Instructor.compare(c2.Instructor)>0)
             {
                 return false;
             } else
             {
                 return true;
             }

         }
    );
    return UnknownGradedCourses;
}
void unknown(vector<Dept> departs)
{
    string deptName;
    cout<<"dept name, or all? ";
    cin>>deptName;
    Dept currentDept;
    vector<Course> courses;
    if(deptName.compare("all")==0)
    {
        for(const Dept& d : departs)
        {
            currentDept = d;
            courses = getUnknownGraded(currentDept);
            courseSummary(courses);
        }
    }
    else
    {
        currentDept=getDepart(departs,deptName);
        vector<Course> tmp =getUnknownGraded(currentDept);
        courseSummary(tmp);
    }
}
void searchData(vector<Dept> departs)
{
    string deptName;
    cout<<"dept name, or all? ";
    cin>>deptName;
    string instructorPrefix;
    int courseNum;
    cout << "course # or instructor prefix? ";
    cin >> instructorPrefix;
    stringstream ss(instructorPrefix); // create stringstream object
    ss >> courseNum; // try to convert input to a course #:
    Dept currentDept;
    vector<Course> courses;
    vector<Course> vv;
    bool ok;
    if(deptName.compare("all")==0)
    {
        for(const Dept& d : departs)
        {
            currentDept=d;
            if(ss.fail())
            {
                courses=FindCourses(currentDept,instructorPrefix);
                ok=true;
            }
            else
            {
                courses=FindCourses(currentDept,courseNum);
                ok=false;
            }
            for(const Course c:courses)
            {
                vv.push_back(c);
            }
        }
        if(ok==true)
        {
            sort(vv.begin(),vv.end(),
                 [](const Course& c1, const Course& c2)
                 {
                     if(c1.Instructor.compare(c2.Instructor)>0)
                     {
                         return false;
                     } else if(c1.Instructor.compare(c2.Instructor)<0)
                         return true;
                 }
            );
        } else
        {
            sort(vv.begin(),vv.end(),
                 [](const Course& c1, const Course& c2)
                 {
                     if(c1.Dept.compare(c2.Dept)>0)
                     {
                         return false;
                     } else
                         return true;

                 }
            );
        }
        courseSummary(vv);
    }
    else
    {
        currentDept=getDepart(departs,deptName);
        if(ss.fail())
        {
            courses=FindCourses(currentDept,instructorPrefix);
        }
        else
        {
            courses=FindCourses(currentDept,courseNum);
        }
        courseSummary(courses);
    }
}
vector<Dept> getDepartmentsList(vector<Course> courses)
{
    vector<Dept> departments;
    set<string> depts;
    depts.insert(courses[0].Dept);
    for(const Course& c : courses)
    {
        depts.insert(c.Dept);
    }
    vector<string> v( depts.begin(), depts.end() );
    sort(v.begin(),v.end());

    for(const string& str : v)
    {
        Dept depart;
        depart.Name=str;
        departments.push_back(depart);
    }
    return departments;
}
vector<Dept> addCoursesToDpt(vector<Dept> departments,vector<Course> courses)
{
    courses.begin();
    for(const Course& c : courses)
    {
        for (int j = 0; j <departments.size() ; ++j)
        {
            if(c.Dept.compare(departments[j].Name)==0)
            {
                departments[j].Courses.push_back(c);
                break;
            }
        }
    }
    return departments;
}
vector<Course> letterACourseHelper(Dept depart,double letterA_threshold)
{
    vector<Course> letterAGraded;
    for(const Course& c : depart.Courses)
    {
        if(GetGradeDistribution(c).PercentA>=letterA_threshold)
        {
            letterAGraded.push_back(c);
        }
    }
    sort(letterAGraded.begin(),letterAGraded.end(),
         [](const Course& c1, const Course& c2)
         {
             GradeStats gs = GetGradeDistribution(c1);
             GradeStats gs1 = GetGradeDistribution(c2);
             if(gs.PercentA-gs1.PercentA>0)
             {
                 return true;
             }
             else if(gs.PercentA-gs1.PercentA<0)
             {
                 return false;
             } else {
                     if (c1.Number - c2.Number < 0) {
                         return true;
                     } else if (c1.Number - c2.Number > 0)
                         return false;
                     else {
                         if (c1.Section - c2.Section < 0) {
                             return true;
                         } else {
                             return false;
                         }
                     }
                 }
            }

    );
    return letterAGraded;
}
vector<Course> letterACourseHelper(vector<Dept> depart,double letterA_threshold)
{
    vector<Course> letterAGraded;
    for(const Dept& d : depart) {
        for (const Course &c : d.Courses) {
            if (GetGradeDistribution(c).PercentA >= letterA_threshold) {
                letterAGraded.push_back(c);
            }
        }
    }
    sort(letterAGraded.begin(),letterAGraded.end(),
         [](const Course& c1, const Course& c2)
         {
            GradeStats gs = GetGradeDistribution(c1);
            GradeStats gs1 = GetGradeDistribution(c2);
            if(gs.PercentA-gs1.PercentA>0)
            {
                return true;
            }
            else if(gs.PercentA-gs1.PercentA<0)
            {
                return false;
            } else {
                if (c1.Dept.compare(c2.Dept) > 0) {
                    return false;
                } else if (c1.Dept.compare(c2.Dept) < 0)
                    return true;
                else {
                    if (c1.Number - c2.Number < 0) {
                        return true;
                    } else if (c1.Number - c2.Number > 0)
                        return false;
                    else {
                        if (c1.Section - c2.Section < 0) {
                            return true;
                        } else {
                            return false;
                        }
                    }
                }
            }
         }

    );
    return letterAGraded;
}
void letterA(vector<Dept> departs)
{
    string deptName;
    double letterA;
    cout<<"dept name, or all? ";
    cin>>deptName;
    cout<<"letter A threshold? ";
    cin>>letterA;
    Dept currentDept;
    if(deptName.compare("all")==0)
    {
        vector<Course> courses;
        courses = letterACourseHelper(departs,letterA);
        courseSummary(courses);
    }
    else
    {
        vector<Course> courses;
        currentDept=getDepart(departs,deptName);
        courses = letterACourseHelper(currentDept,letterA);
        courseSummary(courses);
    }
}
vector<string> collegeSummary(string tmp)
{
    string semester,year,college_name;
    vector<string> str;
    stringstream  ss(tmp);
    getline(ss,college_name,',');
    getline(ss,semester,',');
    getline(ss,year,',');
    str.push_back(college_name);
    str.push_back(semester);
    str.push_back(year);
    return str;
}
void mainSummary(vector<Dept> depts,College college,string semester,string year)
{
    int total_students=0,total_courses=0;
    for(const Dept& d : depts)
    {
        total_courses=total_courses+d.Courses.size();
        for(const Course& c : d.Courses)
        {
            total_students=total_students+c.getNumStudents();
        }
    }
    int a=0,b=0;
    cout<<"# of courses taught: "<<total_courses<<"\n";
    cout<<"# of students taught: "<<total_students<<"\n";
    GradeStats gs= GetGradeDistribution(college);
    double d=GetDFWRate(college,a,b);
    cout<<"grade distribution (A-F): "<<gs.PercentA<<"%, "<<gs.PercentB<<"%, "<<gs.PercentC<<"%, "<<gs.PercentD<<"%, "<<gs.PercentF<<"%\n";
    cout<<"DFW rate: "<<d<<"%\n";
}
int main ()
{
    cout << std::fixed;
    cout << std::setprecision(2);
    std::string line;
    string fileName,collegeName,semester,year;
    cin>>fileName;
    ifstream file(fileName);
    College college;
    vector<Dept> departments;
    vector<Course> courses;
    Course c;
    int count=0;
    while (getline(file,line))
    {
        if(count==0)//first line for college information
        {
            cout<<"** College of ";
            cout<<collegeSummary(line)[0]<<", "<<collegeSummary(line)[1]<<" "<<stoi(collegeSummary(line)[2])<<" **\n";
            getline(file,line);// we can skip second line as it is template for next lines
            count++;
        }
        else{
            c = ParseCourse(line);
            courses.push_back(c);
        }
    }
    departments=getDepartmentsList(courses);
    departments=addCoursesToDpt(departments,courses);
    college.Depts=departments;
    mainSummary(departments,college,semester,year);
    while(true)
    {
        cout << "Enter a command> ";
        string command;
        cin >> command;
        if (command.compare("summary") == 0) {
            dataSummary(departments);
        } else if (command.compare("search") == 0) {
            searchData(departments);
        } else if (command.compare("unknown") == 0) {
            unknown(departments);
        } else if (command.compare("dfw") == 0) {
            dfw(departments);
        } else if (command.compare("letterA") == 0) {
            letterA(departments);
        } else {
            cout << "**unknown command\n";
        }
    }
    return  0;
}