#include "pmanager.h"

#include <QDebug>
#include <filesystem>

PManager::PManager(string database)
{
    if (database != DEFAULT_DATABASE_NAME) db_name = database;
    init_db(db_name);
}

PManager::~PManager() {
   if (this->db != NULL) {
      int rc = sqlite3_close(this->db);
      if (rc != SQLITE_OK) {
        fprintf(stderr, "Error closing database: %s\n", sqlite3_errmsg(this->db));
      }
      this->db = NULL;
    }
}

string PManager::get_db_folder() {
    return this->db_folder;
}

string PManager::get_db_path() {
    return this->db_path;
}

void PManager::init_db(string db_name) {
    /* Close an already open database */
    if (this->db != NULL) {
        sqlite3_close(this->db);
    }
    /* Open the database (will be created if it doesn't exist) */
    #ifdef OS_WINDOWS
      //QT will transform the slashes for windows internally.
      this->db_folder = string(std::getenv("USERPROFILE")) + string("/" FOLDER_NAME "/");
    #else
      this->db_folder = string(getpwuid(getuid())->pw_dir) + string("/" FOLDER_NAME "/");
    #endif
    this->db_path = this->db_folder + string(db_name);
    ifstream dbfile(this->db_path.c_str());
    bool db_not_exists = !dbfile;
    if (db_not_exists) {
        #ifdef OS_WINDOWS
          //QT will transform the slashes for windows internally.
          mkdir((string(std::getenv("USERPROFILE")) + string("/" FOLDER_NAME)).c_str());
        #else
          mkdir((string(getpwuid(getuid())->pw_dir) + string("/" FOLDER_NAME)).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        #endif
        ofstream new_dbfile(this->db_path.c_str());
        new_dbfile.close();
    }

    int rc = sqlite3_open(this->db_path.c_str(), &this->db);
    char *err_msg = 0;

    if (rc != SQLITE_OK) {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(this->db));
        sqlite3_free(err_msg);
        sqlite3_close(this->db);
    }

    if (db_not_exists) {
        const char *sql = "CREATE TABLE Categories(id UNSIGNED INTEGER PRIMARY KEY, name TEXT, color TEXT);"
                          "CREATE TABLE Events(id UNSIGNED INTEGER PRIMARY KEY, name TEXT, description TEXT,"
                          "place TEXT, category UNSIGNED INTEGER, start DATETIME, end DATETIME,"
                          "child UNSIGNED INTEGER, rrule TEXT, rid UNSIGNED INTEGER, FOREIGN KEY(category) REFERENCES Categories(id) ON DELETE RESTRICT,"
                          "FOREIGN KEY(child) REFERENCES Events(id) ON DELETE CASCADE);"
                          "INSERT INTO Categories VALUES(1, 'Default', '#1022A0');"
                          "PRAGMA foreign_keys = ON;";

        rc = sqlite3_exec(this->db, sql, 0, 0, &err_msg);

        if (rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", err_msg);
            sqlite3_free(err_msg);
            sqlite3_close(this->db);
        }
    }
}

void PManager::set_db(string database) {
    db_name = database;
    init_db(db_name);
}

string PManager::get_db_name() {
    return db_name;
}

vector<string> PManager::get_db_list() {
    vector<string> db_list;
    for (std::filesystem::directory_entry e : std::filesystem::directory_iterator(this->db_path.substr(0, this->db_path.find_last_of('/')))) {
        std::filesystem::path p = e.path();
        if (p.extension() == ".sql")
            db_list.push_back(p.filename().string());
    }
    return db_list;
}

string PManager::filterSpecialChars(string str) {
    int start_pos = 0;
    string to = "’";
    while((str.find('\'', start_pos)) != std::string::npos) {
        start_pos = str.find('\'', start_pos);
        str.replace(start_pos, 1, to);
        start_pos += to.length();
    }
    return str;
}

bool PManager::add_event(Event *e, Event *child) {
    char *err_msg = 0;
    sqlite3_stmt *stmt;
    string filteredName, filteredDescription, filteredPlace;
    std::string rrule = e->getRrule().get_freq();
    if (e->getRrule().until != 0) {
        rrule += ";UNTIL=" + std::to_string(e->getRrule().until);
    }
    if ((e->getName().length() < 3) || (difftime(e->getStart(), e->getEnd()) > 0) || (e->getCategory() == NULL)) return false;
    int rc = sqlite3_prepare_v2(this->db, "INSERT INTO Events VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?);", -1, &stmt, NULL);
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
    filteredPlace = filterSpecialChars(e->getPlace());
    sqlite3_bind_text(stmt, 4, filteredPlace.c_str(), filteredPlace.length(), 0);
    sqlite3_bind_int64(stmt, 5, e->getCategory()->getId());
    sqlite3_bind_int64(stmt, 6, e->getStart());
    sqlite3_bind_int64(stmt, 7, e->getEnd());
    if (child != NULL)
        sqlite3_bind_int64(stmt, 8, child->getId());
    else
        sqlite3_bind_null(stmt, 8);
    sqlite3_bind_text(stmt, 9, rrule.c_str(), rrule.length(), 0);
    sqlite3_bind_int64(stmt, 10, e->getRid());
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

bool PManager::replace_event(Event *old_event, Event *new_event) {
    char *err_msg = 0;
    sqlite3_stmt *stmt;
    string filteredName, filteredDescription, filteredPlace;
    if ((new_event->getName().length() < 3) || (difftime(new_event->getStart(), new_event->getEnd()) > 0) || (new_event->getCategory() == NULL)) return false;
    int rc = sqlite3_prepare_v2(this->db, "UPDATE Events SET id=?, name=?, description=?, place=?, category=?, start=?, end=? WHERE id=?;", -1, &stmt, NULL);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error in prepare: %s\n", sqlite3_errmsg(this->db));
        sqlite3_free(err_msg);
        return false;
    }
    sqlite3_bind_int64(stmt, 1, new_event->getId());
    filteredName = filterSpecialChars(new_event->getName());
    sqlite3_bind_text(stmt, 2, filteredName.c_str(), filteredName.length(), 0);
    filteredDescription = filterSpecialChars(new_event->getDescription());
    sqlite3_bind_text(stmt, 3, filteredDescription.c_str(), filteredDescription.length(), 0);
    filteredPlace = filterSpecialChars(new_event->getPlace());
    sqlite3_bind_text(stmt, 4, filteredPlace.c_str(), filteredPlace.length(), 0);
    sqlite3_bind_int64(stmt, 5, new_event->getCategory()->getId());
    sqlite3_bind_int64(stmt, 6, new_event->getStart());
    sqlite3_bind_int64(stmt, 7, new_event->getEnd());
    sqlite3_bind_int64(stmt, 8, old_event->getId());
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

bool PManager::replace_category(Category *old_category, Category *new_category) {
    char *err_msg = 0;
    char sql[1024];
    if (new_category->getName().length() < 3) return false;
    snprintf(sql, 1024, "UPDATE Categories SET name='%s', color='%s' WHERE id=%u;", filterSpecialChars(new_category->getName()).c_str(), filterSpecialChars(new_category->getColor()).c_str(), old_category->getId());
    int rc = sqlite3_exec(this->db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return false;
    }
    return true;
}

list<Event*> PManager::get_events(Category *c) {
    if (c == NULL) return this->get_all_events();
    list<Event*> result;
    sqlite3_stmt *res;
    int rc = sqlite3_prepare_v2(this->db, "SELECT * FROM Events WHERE category = ?;", -1, &res, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        return result;
    }
    sqlite3_bind_int64(res, 1, (long)c->getId());
    while ((rc = sqlite3_step(res)) == SQLITE_ROW) {
        unsigned long id = (unsigned long)sqlite3_column_int(res, 0);
        string name((const char*)sqlite3_column_text(res, 1));
        string description((const char*)sqlite3_column_text(res, 2));
        string place((const char*)sqlite3_column_text(res, 3));
        Category *category = this->get_category((unsigned long)sqlite3_column_int64(res, 4));
        if (category == NULL) {
            fprintf(stderr, "Error: Received NULL category\n");
            continue;
        }
        time_t start = (unsigned long)sqlite3_column_int64(res, 5);
        time_t end = (unsigned long)sqlite3_column_int64(res, 6);
        std::string rrule = (const char*)sqlite3_column_text(res, 8);
        unsigned long rid = (unsigned long)sqlite3_column_int64(res, 9);
        Event *e = new Event(id, name, description, place, category, start, end, rrule, rid);

        result.push_front(e);
    }
    sqlite3_finalize(res);
    return result;
}

list<Event*> PManager::get_events_of_month(int month, int year) {
    list<Event*> result;

    /* A large number of countries use daylight saving time (DST) (called also summer time).
     * We need to manage DST. Most of the countries start to use DST between 1 March and 1 April,
     * but the dates where the DST ends are different. For example in Europe DST ends before 1 November, instead in U.S.A.
     * DST ends after the 1 November. We must calculate for the current user if DST ends before or after 1 November (geolocation dependent).
     * The following lines of code use localtime function that takes into account the location of the user. */
    time_t threshold = 26262000; // = 1 November 1970
    std::tm *t = localtime(&threshold);
    /* From documentation:
     * The Daylight Saving Time flag (tm_isdst) is greater than zero if Daylight Saving Time is in effect,
     * zero if Daylight Saving Time is not in effect,
     * and less than zero if the information is not available.
     */
    int s = 0;
    if (t->tm_isdst > 0) s = 1; //if tm_isdst is negative, s will have the default value

    std::tm tm;
    tm.tm_sec = 0;
    tm.tm_min = 0;
    tm.tm_hour = 0;
    tm.tm_mday = 1;
    tm.tm_mon = month - 1;    // Assuming month represents Jan with 1
    tm.tm_year = year - 1900; // Assuming year is the AD year number
    tm.tm_isdst = ((tm.tm_mon > 2) && (tm.tm_mon < 10+s)); // 10 is November
    long first_month = static_cast<long> (mktime(&tm)); //first of month
    tm.tm_mon = tm.tm_mon + 1; //This could be 12, but isn't a problem
    tm.tm_isdst = ((tm.tm_mon > 2) && (tm.tm_mon < 10+s));
    long last_month = static_cast<long> (mktime(&tm)); //last of month
    if ((first_month == -1) || (last_month == -1)) {
        fprintf(stderr, "Failed to calculate timestamp\n");
        return result;
    }
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
    while ((rc = sqlite3_step(res)) == SQLITE_ROW) {
        unsigned long id = (unsigned long)sqlite3_column_int(res, 0);
        string name((const char*)sqlite3_column_text(res, 1));
        string description((const char*)sqlite3_column_text(res, 2));
        string place((const char*)sqlite3_column_text(res, 3));
        Category *category = this->get_category((unsigned long)sqlite3_column_int64(res, 4));
        if (category == NULL) {
            fprintf(stderr, "Error: Received NULL category\n");
            continue;
        }
        time_t start = (unsigned long)sqlite3_column_int64(res, 5);
        time_t end = (unsigned long)sqlite3_column_int64(res, 6);
        Event *e = new Event(id, name, description, place, category, start, end);

        result.push_front(e);
    }
    sqlite3_finalize(res);
    return result;
}

bool PManager::remove_db() {
    if (this->db != NULL) {
      sqlite3_stmt *stmt;
      while ((stmt = sqlite3_next_stmt(this->db, NULL)) != NULL) {
          sqlite3_finalize(stmt); // Finalize all prepared statements
      }
      int rc = sqlite3_close(this->db);
      if (rc != SQLITE_OK) {
          fprintf(stderr, "Error closing database: %s\n", sqlite3_errmsg(this->db));
          return false;
      }
      this->db = NULL;
    }
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

vector<Category*> PManager::get_categories() {
    vector<Category*> result;
    sqlite3_stmt *res;
    char sql[1024];
    snprintf(sql, 1024, "SELECT * FROM Categories;");
    int rc = sqlite3_prepare_v2(this->db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        return result;
    }
    while ((rc = sqlite3_step(res)) == SQLITE_ROW) {

        Category *c = new Category( sqlite3_column_int(res, 0),
                              string((const char*)sqlite3_column_text(res, 1)),
                              string((const char*)sqlite3_column_text(res, 2)));

        result.push_back(c);
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
    snprintf(sql, 1024, "SELECT * FROM Events ORDER BY start DESC;");
    int rc = sqlite3_prepare_v2(this->db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        return result;
    }
    while ((rc = sqlite3_step(res)) == SQLITE_ROW) {
        unsigned long id = (unsigned long)sqlite3_column_int(res, 0);
        string name((const char*)sqlite3_column_text(res, 1));
        string description((const char*)sqlite3_column_text(res, 2));
        string place((const char*)sqlite3_column_text(res, 3));
        Category *category = this->get_category((unsigned long)sqlite3_column_int64(res, 4));
        if (category == NULL) {
            fprintf(stderr, "Error: Received NULL category\n");
            continue;
        }
        time_t start = (unsigned long)sqlite3_column_int64(res, 5);
        time_t end = (unsigned long)sqlite3_column_int64(res, 6);
        std::string rrule = (const char*)sqlite3_column_text(res, 8);
        unsigned int rid = (unsigned long)sqlite3_column_int64(res, 9);
        Event *e = new Event(id, name, description, place, category, start, end, rrule, rid);

        result.push_front(e);
    }
    sqlite3_finalize(res);
    return result;
}

int PManager::save_db(string path) {
    if (path.length() < 5) return 0;
    if (path.substr(path.length()-4, 4) != ".kal") path += ".kal";
    int counter = 0;
    ofstream file;
    file.open(path);
    //TODO: An event with a description too long can exceed the buffer, use sqlite3_prepare_v2
    char sql[4096];
    vector<Category*> categories_list = get_categories();
    for (Category *category : categories_list) {
        snprintf(sql, 4096, "INSERT INTO Categories VALUES(%u, '%s', '%s');", category->getId(), category->getName().c_str(), category->getColor().c_str());
        file << sql << endl;
        delete category;
        counter++;
    }
    list<Event*> events_list = get_all_events();
    for (Event *event : events_list) {
        string description_single_line = event->getDescription();
        size_t start_pos = 0;
        while((start_pos = description_single_line.find("\n", start_pos)) != std::string::npos) {
            description_single_line.replace(start_pos, 1, "\\n");
            start_pos += 2;
        }
        snprintf(sql, 1024, "INSERT INTO Events VALUES(%u, '%s', '%s', '%s', %u, %ld, %ld);", event->getId(), event->getName().c_str(), description_single_line.c_str(), event->getPlace().c_str(), event->getCategory()->getId(), event->getStart(), event->getEnd());
        file << sql << endl;
        delete event;
        counter++;
    }
    file.close();
    return counter;
}

/* it's easy load a crafted file with malicious queries (e.g. "DELETE ... ")
 * we assume the user knows the source and the content of the file.
 */
int PManager::load_db(string path) {
    if ((path.length() < 5) || (path.substr(path.length()-4, 4) != ".kal")) return 0;
    ifstream file;
    string line;
    int rc;
    int counter = 0;
    char *err_msg = 0;
    file.open(path);
    while ( getline (file,line) && (line.substr(0, 6) == "INSERT") ) {
        //Transform line into a multiline (replacing \n)
        size_t start_pos = 0;
        while((start_pos = line.find("\\n", start_pos)) != std::string::npos) {
            line.replace(start_pos, 2, "\n");
            start_pos += 1;
        }
        rc = sqlite3_exec(this->db, line.c_str(), 0, 0, &err_msg);
        if (rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", err_msg);
            sqlite3_free(err_msg);
        } else counter++;
    }
    file.close();
    return counter;
}

time_t PManager::apply_rrule(const time_t& date, const Rrule& rrule) const{
  struct tm date_tm;
  localtime_r(&date, &date_tm);
  if( rrule.get_freq() == "DAILY" ){
    date_tm.tm_mday += 1;
  } else if( rrule.get_freq() == "WEEKLY" ){
    date_tm.tm_mday += 7;
  } else if( rrule.get_freq() == "MONTHLY" ){
    date_tm.tm_mon += 1;
  } else if( rrule.get_freq() == "YEARLY" ){
    date_tm.tm_year += 1;
  }
  return mktime(&date_tm);
}

int PManager::add_recurring_event(Event *event, const Rrule& rrule) {
  int count = 0;
  this->add_event(event);
  count += 1;
  time_t start_old = event->getStart();
  time_t start_new = start_old; 
  time_t end_old = event->getEnd();
  time_t end_new = end_old;

  for( unsigned int i = 0; i < rrule.get_repetitions()-1; i++ ){
    start_new = apply_rrule(start_old, rrule);
    end_new = apply_rrule(end_old, rrule);
    if( rrule.until != 0 && rrule.until < end_new ) break;
    event->setStart(start_new);
    event->setEnd(end_new);
    event->setId(static_cast<unsigned int> (hash<string>()(event->getName() + event->getDescription() + event->getPlace())) + (event->getCategory() ? event->getCategory()->getId() : 0) + static_cast<unsigned int> ((start_new / 1000) + (end_new - start_new)));
    this->add_event(event);
    count += 1;
    start_old = start_new;
    end_old = end_new;
  }

  return count;
}

int PManager::import_db_iCal_format(string path, Category *category) {
    auto category_id = category->getId();
    if (path.length() < 5) return 0;
    if (path.substr(path.length()-4, 4) == ".ics" || path.substr(path.length()-5, 5) == ".ical"){
      ifstream file;
      string line;
      string pattern;
      string summary;
      string location;
      string description;
      bool found_description = false;
      Rrule rrule("NONE");
      unsigned int rid = 0;
      int counter = 0;
      struct tm start;
      start.tm_sec = start.tm_min = start.tm_hour = start.tm_wday = start.tm_yday = start.tm_year = start.tm_mday = start.tm_mon = 0;
      struct tm end;
      end.tm_sec = end.tm_min = end.tm_hour = end.tm_wday = end.tm_yday = end.tm_year = end.tm_mday = end.tm_mon = 0;
      /* Look at the explanation in get_events_of_month */
      time_t threshold = 26262000; // = 1 November 1970
      std::tm *t = localtime(&threshold);
      int s = 0;
      if (t->tm_isdst > 0) s = 1;

      file.open(path);
      while ( getline (file,line) ) {
          pattern = "DTSTART";
          if (line.find(pattern) == 0) { //if line starts with the pattern
              found_description = false;
              size_t pos = line.find(':');
              if (pos == std::string::npos){
                  fprintf(stderr, "Expected a colon in DTSTART line.\n");
              }
              string date = line.substr(pos+1,line.length()-pos+1);
              DateTime dtime(date);
              if (dtime.time.min != -1){//found time specification
                start.tm_hour = dtime.time.hour;
                start.tm_min = dtime.time.min;
              }
              else{
                start.tm_hour = 8;
              }
              start.tm_year = dtime.date.getYear() - 1900;
              start.tm_mon = dtime.date.getMonth() - 1;
              start.tm_mday = dtime.date.getMonthDay();
              start.tm_isdst = ((start.tm_mon > 2) && (start.tm_mon < 10+s));
              continue;
          }
          pattern = "DTEND";
          if (line.find(pattern) == 0) {
              found_description = false;
              size_t pos = line.find(':');
              if (pos == std::string::npos){
                  fprintf(stderr, "Expected a colon in DTEND line.\n");
              }
              string date = line.substr(pos+1,line.length()-pos+1);
              DateTime dtime(date);
              if (dtime.time.min != -1){//found time specification
                end.tm_hour = dtime.time.hour;
                end.tm_min = dtime.time.min;
              }
              else{
                end.tm_hour = 22;
              }
              end.tm_year = dtime.date.getYear() - 1900;
              end.tm_mon = dtime.date.getMonth() - 1;
              end.tm_mday = dtime.date.getMonthDay();
              end.tm_isdst = ((end.tm_mon > 2) && (end.tm_mon < 10+s));
              continue;
          }
          pattern = "SUMMARY:";
          if (line.find(pattern) == 0) {
              found_description = false;
              summary = line.substr(pattern.length(),line.length()-pattern.length());
              continue;
          }
          pattern = "LOCATION:";
          if (line.find(pattern) == 0) {
              found_description = false;
              location = line.substr(pattern.length(),line.length()-pattern.length());
              if (location.length() < 3) location = "";
              continue;
          }
          pattern = "DESCRIPTION:";
          if (line.find(pattern) == 0) {
              found_description = true;
              description = line.substr(pattern.length(),line.length()-pattern.length());
              if (description.length() < 3) description = "";
              continue;
          }
          pattern = "RRULE:FREQ=";
          if (line.find(pattern) == 0) {
              found_description = false;
              std::string rruleline = line.substr(pattern.length(),line.length()-pattern.length());
              rrule = Rrule(rruleline);
              continue;
          }
          pattern = "END:VEVENT";
          if (line.find(pattern) == 0) {
              found_description = false;
              Event *event = NULL;
              if (rrule.isset()) {
                rid = static_cast<unsigned int> (hash<string>()(summary + description + rrule.get_freq()));
                event = new Event(0,summary,description,location,this->get_category(category_id),mktime(&start),mktime(&end), rrule, rid);}
              else
                event = new Event(0,summary,description,location,this->get_category(category_id),mktime(&start),mktime(&end));
              if (rrule.isset()) {
                counter += this->add_recurring_event(event, rrule);
                rrule.reset();
              }
              else if (this->add_event(event))
                  counter++;
              else
                  printf("Error: %s not imported\n", summary.c_str());
              /* Reset optional variables to import the next event without old values */
              location = "";
              description = "";
              continue;
          }
          if (found_description) { /* Multi-line description */
              description = description + "\n" + line;
          }
      }
      file.close();
      return counter;}
    else {
      fprintf(stderr, "Error: File %s is not a valid iCal file\n", path.c_str());
      return 0;
    }
}

void PManager::prune_recurring_events(std::list<Event*>& events) {
  std::map<unsigned int, Event*> map_revents;
  for (auto events_it = events.begin(); events_it != events.end();) {
    if ((*events_it)->getRrule().isset()) {
      auto map_it = map_revents.find((*events_it)->getRid());
      if (map_it == map_revents.end()) {
        map_revents[(*events_it)->getRid()] = *events_it;
      } 
      else if((*events_it)->getStart() < map_it->second->getStart()) {
        // If the new event starts before the existing one, replace it
        map_it->second = *events_it;
      }
      events_it = events.erase(events_it);
    }
    else {
      ++events_it;
    }
  }
  for (auto it : map_revents) {
    events.push_back(it.second);
  }
}

int PManager::export_db_iCal_format(list<Event*> events, string path) {
    if (path.length() < 5) return 0;
    if (path.substr(path.length()-4, 4) != ".ics" && path.substr(path.length()-5, 5) != ".ical") path = path + ".ics";
    char buff[9];
    ofstream file;
    prune_recurring_events(events);
    file.open(path);
    file << "BEGIN:VCALENDAR" << endl;
    file << "CALSCALE:GREGORIAN" << endl;
    for (Event *event : events) {
        file << "BEGIN:VEVENT" << endl;
        time_t tmp = event->getStart();
        strftime(buff, sizeof(buff),"%Y%m%d",localtime((const time_t*)&tmp));
        file << "DTSTART;VALUE=DATE:" << buff << endl;
        tmp = event->getEnd();
        struct tm *end = localtime((const time_t*)&tmp);
        tmp = mktime(end);
        strftime(buff, sizeof(buff),"%Y%m%d",localtime((const time_t*)&tmp));
        file << "DTEND;VALUE=DATE:" << buff << endl;
        if( event->getRid() != 0 )
            file << "RRULE:FREQ=" << event->getRrule().get_freq();
        if( event->getRrule().until != 0 )
            file << "; UNTIL=" << event->getRrule().until << endl;
        else if ( event->getRid() != 0 )
            file << endl;
        file << "UID:" << to_string(event->getId()) << endl;
        if (! event->getDescription().empty())
          file << "DESCRIPTION:" << event->getDescription() << endl;
        if (! event->getPlace().empty())
          file << "LOCATION:" << event->getPlace() << endl;
        file << "SUMMARY:" << event->getName() << endl;
        file << "END:VEVENT" << endl;
    }
    file << "END:VCALENDAR" << endl;
    file.close();
    return events.size();
}
