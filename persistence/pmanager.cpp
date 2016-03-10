#include <QDebug>
#include "pmanager.h"

#define FOLDER_NAME "kalendar"
#define DATABASE_NAME "events.sql"

PManager::PManager()
{
    /* Open the database (will be created if it doesn't exist) */
    QString path = QDir::homePath() + QString("/" FOLDER_NAME "/") + QString(DATABASE_NAME);
    QFile dbfile(path);
    QDir dir;
    bool db_no_exists = !dbfile.exists();
    if (db_no_exists) {
        dir.mkdir(QDir::homePath() + QString("/" FOLDER_NAME));
        dbfile.open(QIODevice::WriteOnly);
        dbfile.close();
    }

    int rc = sqlite3_open(path.toUtf8().constData(), &this->db);
    char *err_msg = 0;

    if (rc != SQLITE_OK) {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(this->db));
        sqlite3_close(this->db);
    }

    if (db_no_exists) {
        const char *sql = "CREATE TABLE Categories(id INT PRIMARY KEY, name TEXT, colour TEXT);"
                          "CREATE TABLE Events(id INT PRIMARY KEY, name TEXT, description TEXT, category TEXT, start DATETIME, end DATETIME);"
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
    snprintf(sql, 1024, "INSERT INTO Events VALUES(%d, '%s', '%s', '%s', '%lu', '%lu');", e->getId(), e->getName()->toUtf8().data(), e->getDescription()->toUtf8().data(), e->getCategory()->toUtf8().data(), e->getStart(), e->getEnd());
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
                              new QString((const char*)sqlite3_column_text(res, 1)),
                              new QString((const char*)sqlite3_column_text(res, 2)),
                              new QString((const char*)sqlite3_column_text(res, 3)),
                              (unsigned long)sqlite3_column_int64(res, 4),
                              (unsigned long)sqlite3_column_int64(res, 5));

        result.push_front(e);
    }
    sqlite3_finalize(res);
    return result;
}

//ADD category func & test
