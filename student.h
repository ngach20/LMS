#ifndef STUDENT
#define STUDENT

#include <string>
#include <ostream>

struct StudentData{
    unsigned long long b_id;
    char s_name[32];
    bool book_issued;
};

class Student{
    public:
        
        Student(unsigned long long s_id = 0, std::string s_name = "", bool book_issued = 0, unsigned long long b_id = 0);


        /**
         * @returns student id
        */
        unsigned long long get_sid();

        /**
         * @returns student name
        */
        std::string get_sname();

        /**
         * @returns true if the student has issued a book
        */
        bool has_book_issued();

        /**
         * @returns issued book id
        */
        unsigned long long get_bid();

        /**
         * @returns All of the student data in the form of StudentData
        */
        StudentData get_student_data();

        friend std::ostream& operator<<(std::ostream& os, Student const & st) {
            os << "\t\t\t* Printing student No: " << st.s_id << '\n';
            os << "\t\t\t* Student's name: " << st.s_name << '\n';

            if(st.book_issued){
                os << "\t\t\t* Student has issued book No: " << st.b_id << '\n' << std::endl; 
            }else{
                os << "\t\t\t* Student has no book issued. " << '\n' << std::endl; 
            }
            return os;
        }

    private:
        unsigned long long s_id; //Student id
        std::string s_name;
        bool book_issued;
        unsigned long long b_id; //Book id that was issued, junk value if book_issued = 0.
};

#endif