#ifndef DB
#define DB

#include <string>
#include <fstream>
#include <iostream>


template <class DataStruct>
class Database{
    public:
        Database<DataStruct>(){};

        /**
         * @brief Initializes the database.
         * @throws runtime_error if the file with the filename can't be opened.
        */
        void init(std::string filename){
            this->db.open(filename, std::ios::binary|std::ios::out|std::ios::in);

            if(!db.is_open()){
                throw std::runtime_error("Failed to open database.");
            }
        }

        /**
         * @brief writes sizeof(DataStruct) number of bytes to the file.
         * @throws runtime_error.
        */
        void write(char *src){
            if(!this->db.is_open()){
                throw std::runtime_error("Database not open.");
            }

            this->db.seekp(sizeof(unsigned long long) + sizeof(DataStruct) * (this->cur_id - 1));

            if(this->db.fail()){
                throw std::runtime_error("Failed while seeking data position.");
            }

            this->db.write(src, sizeof(DataStruct));

            if(this->db.bad()){
                throw std::runtime_error("Failed while writing to database.");
            }
        }

        /**
         * @brief reads sizeof(DataStruct) number of bytes to dest.
         * @throws runtime_error.
        */
        void read(char *dest){
            if(!this->db.is_open()){
                throw std::runtime_error("Database not open.");
            }

            this->db.seekg(sizeof(unsigned long long) + sizeof(DataStruct) * (this->cur_id - 1));

            if(this->db.fail()){
                throw std::runtime_error("Failed while seeking data position.");
            }

            this->db.read(dest, sizeof(DataStruct));

            if(this->db.fail()){
                throw std::runtime_error("Failed while reading data from database.");
            }
        }


        /**
         * @brief sets current_id to id, from where next reads and writes occur.
         * @throws invalid_argument.
        */
        void set_cur_id(unsigned long long id){
            if(id == 0){
                throw std::invalid_argument("0 ID does not exist.");
            }

            this->cur_id = id;
        }

        /**
         * @returns id, which should be the one after the last entry.
         * @throws runtime_error.
        */
        unsigned long long get_max_entry(){
            if(!this->db.is_open()){
                throw std::runtime_error("Database not open.");
            }

            this->db.seekg(0);

            //If it there is no id stored in the database yet
            if(this->db.peek() == EOF){
                this->db.clear();
                return 1;
            }

            if(this->db.fail()){
                throw std::runtime_error("Failed while seeking data position.");
            }

            unsigned long long dest = 0;

            this->db.read((char*)&dest, sizeof(unsigned long long));

            if(this->db.fail()){
                throw std::runtime_error("Failed while reading from database.");
            }

            return dest;
        }


        /**
         * @brief sets the max_entry variable (the first 8 bytes in the database).
         * @throws runtime_error.
        */
        void set_max_entry(unsigned long long id){
            if(!this->db.is_open()){
                throw std::runtime_error("Database not open.");
            }

            this->db.seekp(0);

            if(this->db.fail()){
                throw std::runtime_error("Failed while seeking data position.");
            }

            this->db.write((char*)&id, sizeof(unsigned long long));

            if(this->db.bad()){
                throw std::runtime_error("Failed while writing to database.");
            }
        }

        ~Database<DataStruct>(){
            this->db.close();
        }

    private:
        std::fstream db;
        unsigned long long cur_id = 0;
};

#endif