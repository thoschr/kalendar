#include "pmanager.h"

#include <QDebug>

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
                          "CREATE TABLE Events(id UNSIGNED INTEGER PRIMARY KEY, name TEXT, "
                          "description TEXT, category UNSIGNED INTEGER, start DATETIME, end DATETIME,"
                          "FOREIGN KEY(category) REFERENCES Categories(id) ON DELETE RESTRICT);"
                          "INSERT INTO Categories VALUES(1, 'Default', '#1022A0');"
                          "PRAGMA foreign_keys = ON;";

        rc = sqlite3_exec(this->db, sql, 0, 0, &err_msg);

        if (rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", err_msg);
            sqlite3_free(err_msg);
        }
    }
}

PManager::~PManager() {
    sqlite3_close(this->db);
}

string PManager::filterSpecialChars(string str) {
    int start_pos = 0;
    string to = "\\'";
    while((str.find('\'', start_pos)) != std::string::npos) {
        start_pos = str.find('\'', start_pos);
        str.replace(start_pos, 1, to);
        start_pos += to.length();
    }
    return str;
}

bool PManager::add_event(Event *e) {
    char *err_msg = 0;
    sqlite3_stmt *stmt;
    string filteredName, filteredDescription;
    if ((e->getName().length() < 3) || (e->getStart() > e->getEnd()) || (e->getCategory() == NULL)) return false;
    int rc = sqlite3_prepare_v2(this->db, "INSERT INTO Events VALUES(?, ?, ?, ?, ?, ?);", -1, &stmt, NULL);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error in prepare: %s\n", sqlite3_errmsg(this->db));
        sqlite3_free(err_msg);
        sqlite3_close(this->db);
        return false;
    }
    sqlite3_bind_int64 (stmt, 1, e->getId());
    filteredName = filterSpecialChars(e->getName());
    sqlite3_bind_text(stmt, 2, filteredName.c_str(), filteredName.length(), 0);
    filteredDescription = filterSpecialChars(e->getDescription());
    sqlite3_bind_text(stmt, 3, filteredDescription.c_str(), filteredDescription.length(), 0);
    sqlite3_bind_int64(stmt, 4, e->getCategory()->getId());
    sqlite3_bind_int64(stmt, 5, e->getStart());
    sqlite3_bind_int64(stmt, 6, e->getEnd());
    //commit
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE ) {
        fprintf(stderr, "SQL error in commit: %s\n", sqlite3_errmsg(this->db));
        sqlite3_free(err_msg);
        return false;
    }
    //free memory
    sqlite3_finalize(stmt);
    return true;
}

bool PManager::edit_event(Event *before, Event *after) {
    char *err_msg = 0;
    sqlite3_stmt *stmt;
    string filteredName, filteredDescription;
    if ((after->getName().length() < 3) || (after->getStart() > after->getEnd())) return false;
    int rc = sqlite3_prepare_v2(this->db, "UPDATE Events SET id=?, name=?, description=?, category=?, start=?, end=? WHERE id=?;", -1, &stmt, NULL);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error in prepare: %s\n", sqlite3_errmsg(this->db));
        sqlite3_free(err_msg);
        return false;
    }
    sqlite3_bind_int64(stmt, 1, after->getId());
    filteredName = filterSpecialChars(after->getName());
    sqlite3_bind_text(stmt, 2, filteredName.c_str(), filteredName.length(), 0);
    filteredDescription = filterSpecialChars(after->getDescription());
    sqlite3_bind_text(stmt, 3, filteredDescription.c_str(), filteredDescription.length(), 0);
    sqlite3_bind_int64(stmt, 4, after->getCategory()->getId());
    sqlite3_bind_int64(stmt, 5, after->getStart());
    sqlite3_bind_int64(stmt, 6, after->getEnd());
    sqlite3_bind_int64(stmt, 7, before->getId());
    //commit
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE ) {
        fprintf(stderr, "SQL error in commit: %s\n", sqlite3_errmsg(this->db));
        sqlite3_free(err_msg);
        return false;
    }
    //free memory
    sqlite3_finalize(stmt);
    return true;
}

std::list<Event*> PManager::get_events_of_month(int month, int year) {
    list<Event*> result;
    std::tm tm;
    tm.tm_sec = 0;
    tm.tm_min = 0;
    tm.tm_hour = 0;
    tm.tm_mday = 1;
    tm.tm_mon = month - 1;    // Assuming month represents Jan with 1
    tm.tm_year = year - 1900; // Assuming year is the AD year number
    long first_month = static_cast<long> (mktime(&tm)); //first of month
    tm.tm_mon = tm.tm_mon + 1;
    long last_month = static_cast<long> (mktime(&tm)); //last of month
    sqlite3_stmt *res;
    int rc = sqlite3_prepare_v2(this->db, "SELECT * FROM Events WHERE (start >= ? AND start < ?) OR (end >= ? AND end < ?);", -1, &res, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        return result;
    }
    sqlite3_bind_int64(res, 1, first_month);
    sqlite3_bind_int64(res, 2, last_month);
    sqlite3_bind_int64(res, 3, first_month);
    sqlite3_bind_int64(res, 4, last_month);
    while (rc = sqlite3_step(res) == SQLITE_ROW) {
        unsigned long id = (unsigned long)sqlite3_column_int(res, 0);
        string name((const char*)sqlite3_column_text(res, 1));
        string description((const char*)sqlite3_column_text(res, 2));
        Category *category = this->get_category((unsigned long)sqlite3_column_int64(res, 3));
        if (category == NULL) {
            fprintf(stderr, "Error: Received NULL category\n");
            continue;
        }
        time_t start = (unsigned long)sqlite3_column_int64(res, 4);
        time_t end = (unsigned long)sqlite3_column_int64(res, 5);
        Event *e = new Event(id, name, description, category, start, end);

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
        return false;
    }
    return true;
}

bool PManager::add_category(Category *c) {
    char *err_msg = 0;
    char sql[1024];
    if (c->getName().length() < 3) return false;
    snprintf(sql, 1024, "INSERT INTO Categories VALUES(%u, '%s', '%s');", c->getId(), filterSpecialChars(c->getName()).c_str(), filterSpecialChars(c->getColor()).c_str());
    int rc = sqlite3_exec(this->db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
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
    snprintf(sql, 1024, "PRAGMA foreign_keys = ON; DELETE FROM Categories WHERE id = %u;", c->getId());
    int rc = sqlite3_exec(this->db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        //Don't close the db
        return false;
    }
    return true;
}

Category* PManager::get_category(unsigned int id) {
    sqlite3_stmt *res;
    char sql[1024];
    snprintf(sql, 1024, "SELECT * FROM Categories WHERE id = %u;", id);
    int rc = sqlite3_prepare_v2(this->db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    if ((rc = sqlite3_step(res)) == SQLITE_ROW) {
        Category *category = new Category( sqlite3_column_int(res, 0),
                                           string((const char*)sqlite3_column_text(res, 1)),
                                           string((const char*)sqlite3_column_text(res, 2)));
        sqlite3_finalize(res);
        return category;
    } else
        return NULL;
}

bool PManager::remove_past_events(time_t timestamp) {
    char *err_msg = 0;
    char sql[1024];
    snprintf(sql, 1024, "DELETE FROM Events WHERE end <= %ld;", static_cast <long> (timestamp));
    int rc = sqlite3_exec(this->db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return false;
    }
    return true;
}

list<Event*> PManager::get_all_events() {
    list<Event*> result;
    sqlite3_stmt *res;
    char sql[1024];
    snprintf(sql, 1024, "SELECT * FROM Events;");
    int rc = sqlite3_prepare_v2(this->db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        return result;
    }
    while (rc = sqlite3_step(res) == SQLITE_ROW) {
        unsigned long id = (unsigned long)sqlite3_column_int(res, 0);
        string name((const char*)sqlite3_column_text(res, 1));
        string description((const char*)sqlite3_column_text(res, 2));
        Category *category = this->get_category((unsigned long)sqlite3_column_int64(res, 3));
        if (category == NULL) {
            fprintf(stderr, "Error: Received NULL category\n");
            continue;
        }
        time_t start = (unsigned long)sqlite3_column_int64(res, 4);
        time_t end = (unsigned long)sqlite3_column_int64(res, 5);
        Event *e = new Event(id, name, description, category, start, end);

        result.push_front(e);
    }
    sqlite3_finalize(res);
    return result;
}
