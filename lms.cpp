#include <iostream>
#include <fstream>
#include "student.h"
#include "book.h"
#include "database/repo.h"
#include "database/db.h"
#include "cmd.cpp"

const std::string books = "book.db";
const std::string students = "student.db";

Database<BookData> b_db;//Books database
Database<StudentData> s_db;//Students database

void admin_menu(Repo<BookData> &books_repo, Repo<StudentData> &students_repo)
{
    int opt = 0;
    while (1)
    {
        common::clrscr();
        std::cout << "\t\t\t\t***Admin Menu***\n\n\n";

        std::cout << "\t\t\t\t1. New Book Entry\n";
        std::cout << "\t\t\t\t2. Update Book Entry\n";
        std::cout << "\t\t\t\t3. Delete Book Entry\n";
        std::cout << "\t\t\t\t4. Show Book Entry\n";
        std::cout << "\t\t\t\t5. List All Book Entries\n";
        std::cout << "\t\t\t\t6. New Student Entry\n";
        std::cout << "\t\t\t\t7. Update Student Entry\n";
        std::cout << "\t\t\t\t8. Delete Student Entry\n";
        std::cout << "\t\t\t\t9. Show Student Entry\n";
        std::cout << "\t\t\t\t10. List All Student Entries\n";
        std::cout << "\t\t\t\t11. Back to main menu\n"
                    << std::endl;
        std::cout << "\t\t\t\tChoose an option (1-11): ";

        std::string input;
        std::getline(std::cin, input);

        try
        {
            opt = std::stoi(input);
        }
        catch (std::exception const &e)
        {
            continue;
        }

        switch (opt)
        {
            case 1:
                cmd::admin::new_book_entry(books_repo);
                break;
            case 2:
                cmd::admin::update_book_entry(books_repo);
                break;
            case 3:
                cmd::admin::delete_book_entry(books_repo);
                break;
            case 4:
                cmd::admin::show_book_entry(books_repo);
                break;
            case 5:
                cmd::admin::list_all_book_entries(books_repo);
                break;
            case 6:
                cmd::admin::new_student_entry(students_repo);
                break;
            case 7:
                cmd::admin::update_student_entry(students_repo);
                break;
            case 8:
                cmd::admin::delete_student_entry(students_repo);
                break;
            case 9:
                cmd::admin::show_student_entry(students_repo);
                break;
            case 10:
                cmd::admin::list_all_student_entries(students_repo);
                break;
            case 11:
                return;
            default:
                break;
        }
    }
}

int main(int argc, char** argv){
    try{
        b_db.init(books);
    }catch(std::runtime_error const &e){
        std::cout << "\t\t\tFailed to initialize database." << std::endl;
        std::cerr << "\n\t\t\t" << e.what() << std::endl;
        common::enter_to_cont();
        exit(-1);
    }

    try{
        s_db.init(students);
    }catch(std::runtime_error const &e){
        std::cout << "\t\t\tFailed to initialize database." << std::endl;
        std::cerr << "\n\t\t\t" << e.what() << std::endl;
        common::enter_to_cont();
        exit(-1);
    }

    Repo<BookData> books_repo(b_db);
    Repo<StudentData> students_repo(s_db);

    int opt = 0;
    while(1){
        common::clrscr();
        std::cout << "\t\t\t***Welcome to the Library Management System***\n\n\n";

        std::cout << "\t\t\t1. Book Issue\n";
        std::cout << "\t\t\t2. Book Deposit\n";
        std::cout << "\t\t\t3. Admin Menu\n";
        std::cout << "\t\t\t4. Exit\n" << std::endl;
        std::cout << "\t\t\tChoose an option (1-4): ";

        std::string input;
        std::getline(std::cin, input);

        try{
            opt = std::stoi(input);
        }catch(std::exception const &e){
            continue;
        }

        switch(opt){
            case 1:
                cmd::main::book_issue(books_repo, students_repo);
                break;
            case 2:
                cmd::main::book_deposit(students_repo);
                break;
            case 3:
                admin_menu(books_repo, students_repo);
                break;
            case 4:
                common::clrscr();
                exit(0);
                break;
            default:
                break;
        }
    }


    return 0;
}