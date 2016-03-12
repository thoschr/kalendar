#include <QDebug>
#include "pmanager.h"

PManager::PManager()
{
    /* Open the database (will be created if it doesn't exist) */
    this->db_path = string(getpwuid(getuid())->pw_dir) + string("/" FOLDER_NAME "/" DATABASE_NAME);
    ifstream dbfile(this->db_path.c_str());
    bool db_no_exists = !dbfile;
    if (db_no_exists) {
        mkdir((string(getpwuid(getuid())->pw_dir) + string("/" FOLDER_NAME)).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        ofstream new_dbfile(this->db_path.c_str());
        new_dbfile.close();
    }

    int rc = sqlite3_open(this->db_path.c_str(), &this->db);
    char *err_msg = 0;

    if (rc != SQLITE_OK) {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(this->db));
        sqlite3_close(this->db);
    }

    if (db_no_exists) {
        const char *sql = "CREATE TABLE Categories(id UNSIGNED INTEGER PRIMARY KEY, name TEXT, color TEXT);"
                          "CREATE TABLE Events(id UNSIGNED INTEGER PRIMARY KEY, name TEXT, description TEXT, category TEXT, start DATETIME, end DATETIME);"
                          "INSERT INTO Categories VALUES(1, 'Default', '#1022A0');";

        rc = sqlite3_exec(this->db, sql, 0, 0, &err_msg);

        if (rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", err_msg);
            sqlite3_free(err_msg);
            sqlite3_close(this->db);
        }
    }
}

PManager::~PManager() {
    sqlite3_close(this->db);
}

bool PManager::add_event(Event *e) {
    char *err_msg = 0;
    char sql[1024];
    if ((e->getName().length() < 3) || (e->getStart() > e->getEnd())) return false;
    snprintf(sql, 1024, "INSERT INTO Events VALUES(%u, '%s', '%s', '%s', '%lu', '%lu');", e->getId(), e->getName().c_str(), e->getDescription().c_str(), e->getCategory().c_str(), e->getStart(), e->getEnd());
    int rc = sqlite3_exec(this->db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(this->db);
        return false;
    }
    return true;
}

std::list<Event*> PManager::get_events_of_month(int month, int year) {
    list<Event*> result;
    std::tm tm;
    tm.tm_sec = 0;
    tm.tm_min = 0;
    tm.tm_hour = 0;
    tm.tm_mday = 1;
    tm.tm_mon = month - 1;    // Assuming your month represents Jan with 1
    tm.tm_year = year - 1900; // Assuming your year is the AD year number
    long start_time = static_cast<long> (mktime(&tm));
    tm.tm_mon = tm.tm_mon + 1;
    long end_time = static_cast<long> (mktime(&tm));
    sqlite3_stmt *res;
    char sql[1024];
    snprintf(sql, 1024, "SELECT * FROM Events WHERE start >= %lu AND start < %lu;", start_time, end_time);
    int rc = sqlite3_prepare_v2(this->db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        return result;
    }
    while (rc = sqlite3_step(res) == SQLITE_ROW) {

        Event *e = new Event( sqlite3_column_int(res, 0),
                              string((const char*)sqlite3_column_text(res, 1)),
                              string((const char*)sqlite3_column_text(res, 2)),
                              string((const char*)sqlite3_column_text(res, 3)),
                              (unsigned long)sqlite3_column_int64(res, 4),
                              (unsigned long)sqlite3_column_int64(res, 5));

        result.push_front(e);
    }
    sqlite3_finalize(res);
    return result;
}

bool PManager::remove_all() {
    /* Delete the database file, but not the folder */
    return (std::remove(this->db_path.c_str()) == 0);
}

bool PManager::remove_event(Event *e) {
    char *err_msg = 0;
    char sql[1024];
    snprintf(sql, 1024, "DELETE FROM Events WHERE id = %u;", e->getId());
    int rc = sqlite3_exec(this->db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(this->db);
        return false;
    }
    return true;
}

bool PManager::add_category(Category *c) {
    char *err_msg = 0;
    char sql[1024];
    if (c->getName().length() < 6) return false;
    snprintf(sql, 1024, "INSERT INTO Categories VALUES(%u, '%s', '%s');", c->getId(), c->getName().c_str(), c->getColor().c_str());
    int rc = sqlite3_exec(this->db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(this->db);
        return false;
    }
    return true;
}

list<Category*> PManager::get_categories() {
    list<Category*> result;
    sqlite3_stmt *res;
    char sql[1024];
    snprintf(sql, 1024, "SELECT * FROM Categories;");
    int rc = sqlite3_prepare_v2(this->db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        return result;
    }
    while (rc = sqlite3_step(res) == SQLITE_ROW) {

        Category *c = new Category( sqlite3_column_int(res, 0),
                              string((const char*)sqlite3_column_text(res, 1)),
                              string((const char*)sqlite3_column_text(res, 2)));

        result.push_front(c);
    }
    sqlite3_finalize(res);
    return result;
}

bool PManager::remove_category(Category *c) {
    char *err_msg = 0;
    char sql[1024];
    snprintf(sql, 1024, "DELETE FROM Categories WHERE id = %u;", c->getId());
    int rc = sqlite3_exec(this->db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(this->db);
        return false;
    }
    return true;
}
