#include "book.h"
#include <iostream>

Book::Book(unsigned long long b_id, std::string b_name, std::string a_name){
    this->b_id = b_id;

    this->a_name = a_name.substr(0, 31);
    this->b_name = b_name.substr(0, 31);
}

unsigned long long Book::get_bid(){
    return this->b_id;
}

std::string Book::get_bname(){
    return b_name;
}

std::string Book::get_aname(){
    return a_name;
}

BookData Book::get_book_data(){
    BookData bdata;
    memset(&bdata, 0, sizeof(BookData));
    strcpy((char*)bdata.b_name, this->b_name.c_str());
    strcpy((char*)bdata.a_name, this->a_name.c_str());
    return bdata;
}