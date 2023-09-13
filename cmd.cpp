#ifndef CMD
#define CMD

#include <iostream>
#include <string>
#include "book.h"
#include "student.h"
#include "database/repo.h"

namespace common{
    void clrscr()
    {
        #ifdef _WIN32
                system("cls");
        #endif
        #ifdef __linux__
                system("clear");
        #endif
    }

    unsigned long long get_id(std::string addressing = " ")
    {
        std::string b_id_str;
        unsigned long long b_id = 0;

        if(addressing != " "){
            addressing = " " + addressing + " ";
        }

        do
        {
            std::cout << "\t\t\tEnter the" << addressing << "id: ";
            getline(std::cin, b_id_str);
            try
            {
                b_id = std::stoull(b_id_str);
                break;
            }
            catch (std::invalid_argument const &)
            {
                std::cout << "\t\t\tNot a number!\n";
                continue;
            }
            catch (std::out_of_range const &)
            {
                std::cout << "\t\t\tYour number is too large!\n";
                continue;
            }
        } while (1);

        return b_id;
    }

    std::string get_string(std::string output, std::string output_on_fail)
    {
        std::string str;
        do
        {
            std::cout << output;
            getline(std::cin, str);

            if (str.size() + 1 > 32)
            {
                std::cout << output_on_fail << std::endl;
            }
        } while (str.size() + 1 > 32);

        return str;
    }

    void enter_to_cont(){
        std::string str;

        std::cout << "\n\t\t\tPress enter to continue: ";
        getline(std::cin, str);
    }
}

namespace cmd
{
    using namespace common;

    /**
     *   MAIN MENU OPTIONS
     */
    namespace main
    {
        void book_issue(Repo<BookData> &books_repo, Repo<StudentData> &students_repo)
        {
            clrscr();

            unsigned long long s_id, b_id = 0;
            s_id = get_id("student");
            StudentData *sdata;

            // Check if the student has already issued a book
            try{
                sdata = students_repo.get_record(s_id);

            }catch(std::exception const &fetch_e){
                std::cout << "\t\t\tFailed to get student entry." << std::endl;
                std::cout << "\n\t\t\t" << fetch_e.what() << std::endl;
                enter_to_cont();
                return;
            }

            if (sdata->book_issued)
            {
                std::cout << "\t\t\tThis student has not deposited their previous book!\n";
                free(sdata);
                enter_to_cont();
                return;
            }

            b_id = get_id("book");

            //Check if the book is in the database
            BookData *bdata;
            try{
                bdata = books_repo.get_record(b_id);
            }catch(std::exception const &e){
                std::cout << "\t\t\tFailed get book entry." << std::endl;
                std::cout << "\n\t\t\t" << e.what() << std::endl;
                enter_to_cont();
                free(sdata);
                return;
            }
            free(bdata);

            Student st(s_id, sdata->s_name, true, b_id);
            free(sdata);

            try{
                students_repo.update_record(st.get_student_data(), s_id);
                std::cout << "\t\t\tBook No: " << st.get_bid() << " has been issued to " << st.get_sname();
                enter_to_cont();
            }catch(std::exception const &update_e){
                std::cout << "\t\t\tFailed to update student entry." << std::endl;
                std::cout << "\n\t\t\t" << update_e.what() << std::endl;
                enter_to_cont();
            }
        }

        // TODO: Book quantity.
        void book_deposit(Repo<StudentData> &students_repo)
        {
            clrscr();

            unsigned long long b_id, s_id = 0;

            
            s_id = get_id("student");

            StudentData *sdata;

            try{
                // Check if the student has the book issued
                sdata = students_repo.get_record(s_id);

                if (!sdata->book_issued)
                {
                    std::cout << "\t\t\tThis student has no book to deposit!\n";
                    free(sdata);
                    enter_to_cont();
                    return;
                }
            }catch(std::exception const &e){
                std::cout << "\t\t\tFailed to get student entry." << std::endl;
                std::cout << "\n\t\t\t" << e.what() << std::endl;
                enter_to_cont();
                return;
            }

            Student st(s_id, sdata->s_name, false, 0);
            b_id = sdata->b_id;
            free(sdata);

            try{
                students_repo.update_record(st.get_student_data(), s_id);
                std::cout << "\t\t\t" << st.get_sname() << " has successfully deposited book No: " << b_id << std::endl;
                enter_to_cont();
            }catch(std::exception const &e){
                std::cout << "\t\t\tFailed to update student entry." << std::endl;
                std::cout << "\n\t\t\t" << e.what() << std::endl;
                enter_to_cont();
            }
        }
    };
    /**
     *    END OF MAIN MENU
     */

    /**
     * Admin Commands
     */
    namespace admin
    {
        /* #region Books Region */
        void new_book_entry(Repo<BookData> &b_repo)
        {
            clrscr();

            std::string book_name, author_name;

            book_name = get_string("\t\t\tEnter the book title: ",
                                   "\t\t\tBook title is too large!");

            author_name = get_string("\t\t\tEnter the author's name: ",
                                     "\t\t\tAuthor's name is too large!");

            Book bk(0, book_name, author_name);

            try{
                b_repo.create_new_record(bk.get_book_data());
                std::cout << "\n\t\t\tBook entry created\n"
                        << std::endl;
                enter_to_cont();
            }catch(std::exception const &e){
                std::cout << "\n\t\t\tFailed to create new book entry." << std::endl;
                std::cout << "\n\t\t\t" << e.what() << std::endl;
                enter_to_cont();
            }
        }

        void update_book_entry(Repo<BookData> &b_repo)
        {
            clrscr();

            unsigned long long b_id;
            std::string book_name, author_name;

            b_id = get_id();

            BookData *old;
            try{
                old = b_repo.get_record(b_id);
            }catch(std::exception const &e){
                std::cout << "\t\t\tFailed to fetch book data." << std::endl;
                std::cout << "\n\t\t\t" << e.what() << std::endl;
                enter_to_cont();
                return;
            }

            
            book_name = get_string("\t\t\tEnter the new book title (press Enter to skip): ",
                                   "\t\t\tBook title is too large!");

            author_name = get_string("\t\t\tEnter the new author's name (press Enter to skip): ",
                                     "\t\t\tAuthor's name is too large!");


            if (book_name.empty())
            {
                book_name = old->b_name;
            }

            if (author_name.empty())
            {
                author_name = old->a_name;
            }

            Book bk(0, book_name, author_name);

            free(old);

            try{
                b_repo.update_record(bk.get_book_data(), b_id);
            }catch(std::exception const &e){
                std::cout << "Failed to update book entry." << std::endl;
                std::cout << "\n\t\t\t" << e.what() << std::endl;
                enter_to_cont();
                return;
            }

            std::cout << "\t\t\tBook entry has been updated\n"
                      << std::endl;

            enter_to_cont();
        }

        void delete_book_entry(Repo<BookData> &b_repo)
        {
            clrscr();

            unsigned long long b_id = get_id();

            try{
                b_repo.delete_record(b_id);
            }catch(std::exception const &e){
                std::cout << "Failed to delete book entry." << std::endl;
                std::cout << "\n\t\t\t" << e.what() << std::endl;
                enter_to_cont();
                return;
            }

            std::cout << "\t\t\tBook entry has been deleted\n"
                      << std::endl;

            enter_to_cont();
        }

        void show_book_entry(Repo<BookData> &b_repo)
        {
            clrscr();

            unsigned long long b_id = get_id();

            try{
                BookData *b_data = b_repo.get_record(b_id);
                Book b(b_id, b_data->b_name, b_data->a_name);

                std::cout << b << std::endl;
                enter_to_cont();
            }catch(std::exception const &e){
                std::cout << "\t\t\tFailed to fetch book data." << std::endl;
                std::cout << "\n\t\t\t" << e.what() << std::endl;
                enter_to_cont();
            }

        }

        void list_all_book_entries(Repo<BookData> &b_repo)
        {
            clrscr();

            for(unsigned long long i = 1; ; ++i){
                try{
                    BookData *bdata = b_repo.get_record(i);
                    Book b(i, bdata->b_name, bdata->a_name);

                    std::cout << b << "\n\n" << std::endl;
                    free(bdata);
                }catch(std::out_of_range const &e){//Out of bounds, all records listed
                    enter_to_cont();
                    return; 
                }catch(std::invalid_argument const &e){ //Deleted entry
                    continue;
                }
            }
        }
        /* #endregion */

        /* #region Students Region */
        void new_student_entry(Repo<StudentData> &students_repo)
        {
            clrscr();

            std::string st_name;

            st_name = get_string("\t\t\tEnter the student's name: ",
                                 "\t\t\tStudent's name is too large!");

            Student st(0, st_name);

            try{
                students_repo.create_new_record(st.get_student_data());
                std::cout << "\n\t\t\tStudent entry created\n"
                        << std::endl;

                enter_to_cont();
            }catch(std::exception const &e){
                std::cout << "\n\t\t\tFailed to create a new student entry"
                      << std::endl;
                std::cout << "\n\t\t\t" << e.what() << std::endl;
                enter_to_cont();
            }

        }

        void update_student_entry(Repo<StudentData> &students_repo)
        {
            clrscr();

            unsigned long long s_id = get_id();

            StudentData *old_data;
            
            try{
                old_data = students_repo.get_record(s_id);
            }catch(std::exception const &e){
                std::cout << "\t\t\tFailed to fetch student data." << std::endl;
                std::cout << "\n\t\t\t" << e.what() << std::endl;
                enter_to_cont();
                return;
            }

            std::string st_name = get_string("\t\t\tEnter the new student's name (press Enter to skip): ",
                                             "\t\t\tStudent's name is too large!\n");

            if (st_name.empty())
            {
                st_name = old_data->s_name;
            }

            Student st(s_id, st_name, old_data->book_issued, old_data->b_id);

            try{
                students_repo.update_record(st.get_student_data(), s_id);
            }catch(std::exception const &e){
                std::cout << "Failed to update student entry." << std::endl;
                std::cout << "\n\t\t\t" << e.what() << std::endl;
                enter_to_cont();
                return;
            }

            std::cout << "\n\t\t\tStudent entry updated\n"
                      << std::endl;

            enter_to_cont();
        }

        void delete_student_entry(Repo<StudentData> &students_repo)
        {
            clrscr();

            unsigned long long s_id = get_id();

            try{
                students_repo.delete_record(s_id);
            }catch(std::exception const &e){
                std::cout << "\t\t\tFailed to delete student entry." << std::endl;
                std::cout << "\n\t\t\t" << e.what() << std::endl;
                enter_to_cont();
                return;
            }

            std::cout << "\t\t\tStudent entry has been deleted\n"
                      << std::endl;

            enter_to_cont();
        }

        void show_student_entry(Repo<StudentData> &students_repo)
        {
            clrscr();

            unsigned long long s_id = get_id();

            try{
                StudentData *s_data = students_repo.get_record(s_id);
                Student st(s_id, s_data->s_name, s_data->book_issued, s_data->b_id);

                std::cout << st << std::endl;

                enter_to_cont();
            }catch(std::exception const &e){
                std::cout << "\n\t\t\tFailed to fetch student data." << std::endl;
                std::cout << "\n\t\t\t" << e.what() << std::endl;
                enter_to_cont();
            }
        }

        void list_all_student_entries(Repo<StudentData> &students_repo) {
            clrscr();

            for(unsigned long long i = 1; ; ++i){
                try{
                    StudentData *sdata = students_repo.get_record(i);
                    Student s(i, sdata->s_name, sdata->book_issued, sdata->b_id);

                    std::cout << s << "\n\n" << std::endl;
                    free(sdata);
                }catch(std::out_of_range const &e){//Out of bounds, all records listed
                    enter_to_cont();
                    return; 
                }catch(std::invalid_argument const &e){ //Deleted entry
                    continue;
                }
            }
        }
        /* #endregion */
    }

};

#endif