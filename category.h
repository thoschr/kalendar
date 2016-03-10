#ifndef CATEGORY_H
#define CATEGORY_H


class Category
{
private:
    QString *name;
    QString *color;

public:
    Category(QString *name, QString *color) {
        this->name = name;
        this->color = color;
    }

    ~Category() {
        delete name;
        delete color;
    }

    QString* getName() { return name; }
    QString* getColor() { return color; }
};

#endif // CATEGORY_H
