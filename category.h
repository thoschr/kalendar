#ifndef CATEGORY_H
#define CATEGORY_H

using namespace std;

class Category
{
private:
    string *name;
    string *color;

public:
    Category(string *name, string *color) {
        this->name = name;
        this->color = color;
    }

    ~Category() {
        delete name;
        delete color;
    }

    string* getName() { return name; }
    string* getColor() { return color; }
};

#endif // CATEGORY_H
