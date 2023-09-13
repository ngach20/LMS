#include "student.h"
#include <cstring>


Student::Student(unsigned long long s_id, std::string s_name, bool book_issued, unsigned long long b_id){
    this->s_id = s_id;
    
    this->s_name = s_name.substr(0, 31);

    this->book_issued = book_issued;
    this->b_id = b_id;
}

unsigned long long Student::get_sid(){
    return this->s_id;
}

std::string Student::get_sname(){
    return this->s_name;
}

bool Student::has_book_issued(){
    return this->book_issued;
}

unsigned long long Student::get_bid(){
    return this->b_id;
}

StudentData Student::get_student_data(){
    StudentData s_data;
    memset(&s_data, 0, sizeof(StudentData));
    s_data.b_id = this->b_id;
    s_data.book_issued = this->book_issued;
    strcpy((char*)s_data.s_name, this->s_name.c_str());
    return s_data;
}