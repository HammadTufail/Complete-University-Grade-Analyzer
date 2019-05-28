#include <string>
#include <vector>
using std::string;
using std::vector;
class Course
{
private:
public:
    enum GradingType { Letter, Satisfactory, Unknown };
    string  Dept;
    string  Title;
    int     Number;
    int     Section;
    string  Instructor;
    int     NumA, NumB, NumC, NumD, NumF, NumI, NumS, NumU, NumW, NumNR;
    Course()
            : Dept("?"), Title("?"), Number(0), Section(0), Instructor("?"),
              NumA(0), NumB(0), NumC(0), NumD(0), NumF(0), NumI(0), NumS(0), NumU(0), NumW(0), NumNR(0)
    { }
    Course(string dept, string title, int number, int section, string instructor,
           int A, int B, int C, int D, int F, int I, int S, int U, int W, int NR)
            : Dept(dept), Title(title), Number(number), Section(section), Instructor(instructor),
              NumA(A), NumB(B), NumC(C), NumD(D), NumF(F), NumI(I), NumS(S), NumU(U), NumW(W), NumNR(NR)
    { }
    GradingType getGradingType() const
    {
        if ((this->NumA + this->NumB + this->NumC + this->NumD + this->NumF) > 0)
            return Letter;
        else if ((this->NumS + this->NumU) > 0)
            return Satisfactory;
        else
            return Unknown;
    }

    int getNumStudents() const
    {
        int N = 0;

        N = this->NumA + this->NumB + this->NumC + this->NumD + this->NumF;

        if (N > 0)
        {
            N = N + this->NumI + this->NumNR;
            return N;
        }

        N = this->NumS + this->NumU;

        if (N > 0)
        {
            N = N + this->NumI + this->NumNR;
            return N;
        }

        N = this->NumI + this->NumNR;
        return N;
    }

};

class Dept
{
private:
public:
    string         Name;
    vector<Course> Courses;
    Dept()
            : Name("?")
    { }

    Dept(string name)
            : Name(name)
    { }

};


class College
{
private:
public:
    string        Name;
    vector<Dept>  Depts;
    College()
            : Name("?")
    { }
    College(string name)
            : Name(name)
    { }

};
class GradeStats
{
private:
public:
    int    N;  // total # of grades assigned:
    int    NumA, NumB, NumC, NumD, NumF;  // number of A's, B's, etc.
    double PercentA, PercentB, PercentC, PercentD, PercentF;  // percentage of A's, B's, etc.

    GradeStats()
            : N(0), NumA(0), NumB(0), NumC(0), NumD(0), NumF(0),
              PercentA(0.0), PercentB(0.0), PercentC(0.0), PercentD(0.0), PercentF(0.0)
    { }

    GradeStats(int n, int numA, int numB, int numC, int numD, int numF,
               double percentA, double percentB, double percentC, double percentD, double percentF)
            : N(n), NumA(numA), NumB(numB), NumC(numC), NumD(numD), NumF(numF),
              PercentA(percentA), PercentB(percentB), PercentC(percentC), PercentD(percentD), PercentF(percentF)
    { }

};
Course ParseCourse(string csvline);
double GetDFWRate(const Course& c, int& DFW, int& N);
double GetDFWRate(const Dept& dept, int& DFW, int& N);
GradeStats GetGradeDistribution(const Course& c);
GradeStats GetGradeDistribution(const Dept& dept);
GradeStats GetGradeDistribution(const College& college);
vector<Course> FindCourses(const Dept& dept, int courseNumber);
vector<Course> FindCourses(const Dept& dept, string instructorPrefix);
vector<Course> FindCourses(const College& college, int courseNumber);
vector<Course> FindCourses(const College& college, string instructorPrefix);
double GetDFWRate(const Course& c, int& DFW, int& N);
double GetDFWRate(const Dept& dept, int& DFW, int& N);
GradeStats GetGradeDistribution(const Course& c);
GradeStats GetGradeDistribution(const Dept& dept);
GradeStats GetGradeDistribution(const College& college);