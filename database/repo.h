#ifndef REPO
#define REPO

#include "db.h"
#include <cstring>


template <typename DataStruct>
class Repo{
    public:
        Repo(Database<DataStruct> &db) : db(db) {};
        
        /**
         * @brief Reads data from the database and returns memory pointing to it.
         * @brief Pointer ownership is transferred to the caller.
         * @throws invalid_argument, out_of_range, runtime_error.
        */
        DataStruct* get_record(unsigned long long id){
            DataStruct *data = (DataStruct*)malloc(sizeof(DataStruct));
            try{
                this->db.set_cur_id(id);

                bool exists = cur_record_exists(id);
            
                if(!exists){
                    throw std::invalid_argument("Database entry does not exist.");
                }

                memset(data, 0, sizeof(DataStruct));

                this->db.read((char*)data);
                return data;
            }
            catch(std::exception const &e){
                free(data);
                throw;
            }
        }

        /**
         * @brief Updates the entry with the id to new_record.
         * @throws invalid_argument, out_of_range, runtime_error.
        */
        void update_record(const DataStruct &new_record, unsigned long long id) {
            try{
                this->db.set_cur_id(id);
                
                bool exists = cur_record_exists(id);

                if(!exists){
                    throw std::invalid_argument("Database entry does not exist.");
                }

                this->db.write((char*)&new_record);
            }
            catch(std::exception const &e){
                throw;
            }
        }

        /**
         * @brief Deletes the entry id.
         * @throws invalid_argument, out_of_range, runtime_error.
        */
        void delete_record(unsigned long long id) {
            try{
                this->db.set_cur_id(id);
            
                bool exists = cur_record_exists(id);
                if(!exists){
                    throw std::invalid_argument("Database entry does not exist.");
                }

                DataStruct zero;
                memset((void*)&zero, 0, sizeof(DataStruct));
                this->db.write((char*)&zero);
            }catch(std::exception const &e){
                throw;
            }
        }

        /**
         * @brief Creates new record with the id max_id saved in the database.
         * @brief Increments the max_id in the database by 1.
         * @throws invalid_argument, runtime_error.
        */
        unsigned long long create_new_record(const DataStruct &rec) {
            try{
                unsigned long long max_id = this->db.get_max_entry();

                this->db.set_cur_id(max_id);

                this->db.write((char*)&rec);

                this->db.set_max_entry(max_id + 1);

                return max_id;
            }catch(std::exception const &e){
                throw;
            }

        }

    private:
        Database<DataStruct> &db;

         /**
         * @brief Checks if the current record is null or out of bounds.
         * @throws out_of_range, runtime_error.
        */
        bool cur_record_exists(unsigned long long id){
            try{
                unsigned long long max_id = this->db.get_max_entry();

                if(id >= max_id){
                    throw std::out_of_range("ID out of bounds");
                }
            }catch(std::exception const &e){
                throw;
            }

            DataStruct zero;
            memset(&zero, 0, sizeof(DataStruct));

            DataStruct data;
            try{
                this->db.read((char*)&data);
            }catch(std::runtime_error const &e){
                throw;
            }

            return (memcmp(&zero, &data, sizeof(DataStruct)) != 0);
        }

        
    
};

#endif