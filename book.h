#ifndef BOOK
#define BOOK

#include <cstring>
#include <string>
#include <iostream>


struct BookData{
    char b_name[32];
    char a_name[32];
};

class Book{
    public:
        Book(unsigned long long b_id, std::string b_name, std::string a_name);


        /**
        * @returns book id
        */
        unsigned long long get_bid();

        /**
        * @returns book name
        */
        std::string get_bname();

        /**
        * @returns author's name
        */
        std::string get_aname();

        /**
        * @returns Book data in a BookData struct
        */
        BookData get_book_data();

        friend std::ostream& operator<<(std::ostream& os, Book const & bk) {
            os << "\t\t\t* Printing book No: " << bk.b_id << '\n';
            os << "\t\t\t* Book name: " << bk.b_name << '\n'; 
            os << "\t\t\t* Book author: " << bk.a_name << '\n' << std::endl; 
            return os;
        }

    private:
        unsigned long long b_id;
        std::string b_name; //Book name
        std::string a_name; //Author name
};

#endif