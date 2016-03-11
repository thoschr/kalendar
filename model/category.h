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

    bool equals(Category *c) {
        return (this->name->compare(c->getName()) == 0) && (this->color->compare(c->getColor()) == 0);
    }

    int hashcode() {
        return static_cast<int> (hash<string>()(this->name + this->color));
    }
};

#endif // CATEGORY_H
