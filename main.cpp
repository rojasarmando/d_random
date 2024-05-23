#include <sqlite3.h>
#include <iostream>
#include <cstdlib> 
#include <ctime> 
#include <random>

using namespace std;


const int ERROR = -1;
const int OK = 0;
const int MAX = 5 - 1;
const int MAX_RANDOM = 5000;

string top_lenguages[]  = { 
  "PHP" ,
  "Java" ,
  "Javascript" ,
  "Golang" ,
  "Rust"
};

int init_tables( sqlite3 *db);
int get_results(sqlite3 *db );
int insert_name(sqlite3 *db , string name);
int generate_random();

int main() {
    
    sqlite3 *db;
    int rc;

    rc = sqlite3_open("database.db", &db);
    
    if (rc) {
        std::cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    if(init_tables(db) == -1){

        cout<<"ERROR INIT TABLE"<<endl;
        return 0;

    }

    
    for (size_t i = 0; i < MAX_RANDOM; i++)
    {
        int indice_random = generate_random();
        insert_name(db , top_lenguages[indice_random]);
        cout<<top_lenguages[indice_random]<<endl;

    }

    cout<<endl<<endl<<"Resultados: "<<endl<<endl;

    get_results(db);

    cout<<top_lenguages<<endl;
    sqlite3_close(db);
    return 0;
}


int init_tables(sqlite3 *db){
    
    string sql = "CREATE TABLE IF NOT EXISTS top_languages ("
                  " name TEXT NOT NULL, "
                  " create_date DATE DEFAULT (datetime('now','localtime')) "
                  ");";

    int control = OK;

    int exitCode = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
    
    if(exitCode!= SQLITE_OK) {
        std::cerr << "Error creating table: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        control = ERROR;
    }

    return control;
}

int insert_name(sqlite3 *db , string name){
    
    string  sql = "INSERT INTO top_languages(name) VALUES('" + name + "');";
    int control = OK;

    int exitCode = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
    
    if(exitCode!= SQLITE_OK) {
        std::cerr << "Error inserting records: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        control = ERROR;
    }

    return control;
}

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    for(int i = 0; i < argc; i++) {
        std::cout << azColName[i] << ": " << (argv[i]? argv[i] : "NULL") << "\n";
    }
    std::cout << "\n";
    return 0;
}



int get_results(sqlite3 *db ){

    string sql = "SELECT name , count(create_date) FROM top_languages group by name order by count(create_date) DESC  limit 3;";
    int exitCode = sqlite3_exec(db, sql.c_str(), callback, nullptr, nullptr);
    int control = OK;
    
    if(exitCode!= SQLITE_OK) {
        std::cerr << "Error reading records: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        control = ERROR;
    }

    return control;

}

int generate_random(){
    
    random_device rd;  
    mt19937 gen(rd()); 
    uniform_int_distribution<> dist(0, MAX);

    int randomNumber = dist(gen);

    return randomNumber;
}

