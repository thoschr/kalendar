#include "monthview.h"
#include <QDebug>

/* Gets the current month displayed using an hack. Infact, the cell in the middle will have always a value setted.
 * This should be used when you don't care about the day */
#define CURRENT_MONTH *this->frames[21]->getDate()

#define CELL_STYLE "QFrame { background-color: #FFFFFF; border: 1px solid #555555; }" \
                   "QLabel { border: none; font-size: 16px; padding: 5px; background-color:rgba(0,0,0,0); }" \
                   "QLabel#today { background-color: #FFFF88; }" \
                   "QFrame#selected { background-color: #EEEEFF; }" \
                   "QLabel#header { font-weight: bold; }" \
                   "QFrame#header { background-color: #EEEEEE; }"

void MonthView::on_mouse_move(QFrameExtended *frame) {
    if ((frame->getDate() != NULL) && //Checks if the frame is valid
        (this->selection_start != NULL) && //and if the selection is already started
        (!this->selection_end || frame->getDate()->compareTo(*this->selection_end) != 0)) { //and if this is a new cell
        this->selection_end = frame->getDate(); //Keeps in memory where we're arrived
        for (int i = 0; i < 42; i++) {
            if (this->frames[i]->getDate() != NULL) {
                if ((this->frames[i]->getDate()->compareTo(*this->selection_start) >= 0) &&
                    (this->frames[i]->getDate()->compareTo(*frame->getDate()) <= 0)) {
                    this->frames[i]->setObjectName("selected");
                    this->frames[i]->setStyleSheet(CELL_STYLE);
                } else {
                    this->frames[i]->setObjectName("day");
                    this->frames[i]->setStyleSheet(CELL_STYLE);
                }
            }
        }
    }
}

void MonthView::on_mouse_press(QFrameExtended *frame) {
    if (frame->getDate() != NULL) //Checks if the frame is valid
        this->selection_start = frame->getDate();
}

void MonthView::on_mouse_release(QFrameExtended *frame) {
    if ((frame->getDate() != NULL) && //Checks if the frame is valid
        (this->selection_start != NULL)) { //and if the selection is already started
        this->selection_end = frame->getDate();
        /* Clean the selection */
        for (int i = 0; i < 42; i++) {
            if (this->frames[i]->getDate() != NULL) {
                this->frames[i]->setObjectName("day");
                this->frames[i]->setStyleSheet(CELL_STYLE);
            }
        }
        /* The following lines will popup the dialog to add an event with the start and the end already setted,
         * but only if the user selects a valid range.
         */
        if (this->selection_end->compareTo(*this->selection_start) >= 0) {
            EventDialog *eventDialog = new EventDialog(this, *this->selection_start, *this->selection_end);
            eventDialog->show();
        }
        this->selection_start = NULL;
    }
}

void MonthView::on_back_button_click() {
    Date newDate = DateUtil::decrease_month(CURRENT_MONTH);
    display_days(newDate);
    /* Reload events */
    display_events(newDate);
}

void MonthView::on_next_button_click() {
    /* Gets the current month displayed using an hack. Infact, the cell in the middle will have always a value setted. */
    Date newDate = DateUtil::increase_month(CURRENT_MONTH);
    display_days(newDate);
    /* Reload events */
    display_events(newDate);
}

MonthView::MonthView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MonthView)
{
    Date current_date = DateUtil::get_current_date();
    this->selection_start = NULL;
    this->selection_end = NULL;
    this->layout = new QVBoxLayout;
    this->label_date = new QLabel;
    this->label_date->setMaximumHeight(40);
    this->label_date->setStyleSheet("QLabel { padding-left: 100px; padding-right: 100px; font-size: 20px; } ");
    QPushButton *back = new QPushButton("<");
    QPushButton *next = new QPushButton(">");
    back->setMaximumWidth(60);
    next->setMaximumWidth(60);
    back->setMaximumHeight(40);
    next->setMaximumHeight(40);
    back->setShortcut(QKeySequence(Qt::Key_Left));
    next->setShortcut(QKeySequence(Qt::Key_Right));
    connect(back, &QPushButton::clicked, this, &MonthView::on_back_button_click);
    connect(next, &QPushButton::clicked, this, &MonthView::on_next_button_click);
    QHBoxLayout *hl = new QHBoxLayout;
    hl->setAlignment(Qt::AlignCenter);
    hl->addWidget(back);
    hl->addWidget(label_date);
    hl->addWidget(next);
    this->layout->addLayout(hl);

    //Create 7x7 grid
    QGridLayout *grid_layout = new QGridLayout;
    int i,j;
    //First row contains the names of the days of the week
    for (j = 0; j < 7; j++) {
        QFrame *frame = new QFrame;
        QHBoxLayout *hl = new QHBoxLayout;
        hl->setAlignment(Qt::AlignCenter);
        frame->setFixedHeight(50);
        QLabel *wday_name = new QLabel(DateUtil::numeric2literal_day_of_week(j+1).c_str());
        wday_name->setObjectName("header");
        frame->setObjectName("header");
        hl->addWidget(wday_name);
        hl->setMargin(0);
        frame->setLayout(hl);
        frame->setStyleSheet(CELL_STYLE);
        grid_layout->addWidget(frame, 0, j);
    }

    //Next rows contain the days of the selected month
    for (i = 1; i < 7; i++) { //rows
        for (j = 0; j < 7; j++) { //columns
            //Map a Time object to each frame
            QFrameExtended *frame = createQFrameExtended(NULL);
            grid_layout->addWidget(frame, i, j);
            this->frames[7*(i-1)+j] = frame;
        }
    }
    grid_layout->setHorizontalSpacing(0);
    grid_layout->setVerticalSpacing(0);

    //Remove events too old
    PManager pm;
    Date target = DateUtil::decrease_month(DateUtil::get_first_day_of_month(current_date));
    pm.remove_past_events(QDateTime(QDate(target.getYear(), target.getMonth() , target.getMonthDay())).toTime_t());

    //Fill the grid with the days of the default month (i.e. the current month)
    display_days(current_date);

    //Load the events for the current month
    display_events(current_date);

    grid_layout->setMargin(5);
    this->layout->addLayout(grid_layout);

     // Set layout in QWidget
     QWidgetExtended *window = new QWidgetExtended;
     window->setLayout(this->layout);
     window->setMinimumHeight(600);
     window->setMinimumWidth(1100);

     connect(window, &QWidgetExtended::mousePress, this, &MonthView::on_mouse_press);
     connect(window, &QWidgetExtended::mouseRelease, this, &MonthView::on_mouse_release);
     connect(window, &QWidgetExtended::mouseMove, this, &MonthView::on_mouse_move);

     createMenu();

     // Set QWidget as the central layout of the main window
     setCentralWidget(window);
}

MonthView::~MonthView()
{
    delete ui;
    /* this->selection_start and this->selection_end are pointers to dates wrapped inside some QFrameExtended widgets. When
     * Qt frees QFrameExtended widgets they will free their dates.
     */
}

void MonthView::createMenu() {
    QAction *importAct = new QAction(tr("&Import events"), this);
    importAct->setStatusTip(tr("Import events with iCalendar format"));
    connect(importAct, &QAction::triggered, this, &MonthView::importEvents);
    QAction *exportAct = new QAction(tr("E&xport events"), this);
    exportAct->setStatusTip(tr("Export events with iCalendar format"));
    connect(exportAct, &QAction::triggered, this, &MonthView::exportEvents);
    QAction *addEventAct = new QAction(tr("&Add new event"), this);
    addEventAct->setStatusTip(tr("Show a dialog to add a new event"));
    connect(addEventAct, &QAction::triggered, this, &MonthView::addEvent);
    QAction *editCategoriesAct = new QAction(tr("Edit &Categories"), this);
    editCategoriesAct->setStatusTip(tr("Show a dialog to edit the categories"));
    connect(editCategoriesAct, &QAction::triggered, this, &MonthView::editCategories);
    QMenu *fileMenu;
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(importAct);
    fileMenu->addAction(exportAct);
    QMenu *editMenu;
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(addEventAct);
    editMenu->addAction(editCategoriesAct);
    QMenu *viewsMenu;
    viewsMenu = menuBar()->addMenu(tr("&Views"));
    //TODO: Add the other future views (each view will be displayed in a different window)
}

void MonthView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.exec(event->globalPos());
}

void MonthView::importEvents() {
    //TODO: import events (iCalendar format)
}

void MonthView::exportEvents() {
    //TODO: export events (iCalendar format)
}

void MonthView::addEvent() {
    EventDialog *eventDialog = new EventDialog(this, CURRENT_MONTH, CURRENT_MONTH);
    eventDialog->show();
}

void MonthView::editCategories() {
    CategoryDialog *categoryDialog = new CategoryDialog;
    categoryDialog->show();
}

void MonthView::display_days(Date date) {
    //Update the label that contains month and year
    this->label_date->setText(QString(DateUtil::get_literal_month(date.getMonth()).c_str()) + QString("    ") + QString::number(date.getYear()));
    //The current time is needed to highlight the current day
    Date current_date = DateUtil::get_current_date();
    int tot_days = DateUtil::get_days_in_month(date.getMonth(), date.getYear());
    int i,x,start_wday;

    //first week day of the current month
    start_wday = DateUtil::get_first_day_of_month(date).getWeekDay();
    x = 1;
    for (i = 0; i < 42; i++) {
        //Set an invalid date
        this->frames[i]->setDate(NULL);

        //Delete all the event labels inside the frame
        remove_events_from_frame(i);

        QLabel *day = static_cast<QLabel*> (this->frames[i]->children().at(1));
        day->setMaximumWidth(40);
        day->setObjectName("");
        day->setText("");
        //Checks right cells that will contain the days
        if (( i > 6 || //if I'm after the first week or
            (i % 7 >= start_wday-1)) && //if I'm in the first week and I'm in the right days
            (x <= tot_days)) { //and I'm not out of bound
            this->frames[i]->setDate(new Date(x, (start_wday + (x-1)) % 7, date.getMonth(), date.getYear()));
            //I'll insert into the label the number of the day
            day->setText(QString::number(x));
            //Checks current day
            if ((x == current_date.getMonthDay()) && (date.getMonth() == current_date.getMonth()) && (date.getYear() == current_date.getYear()))
                day->setObjectName("today");
            this->frames[i]->layout()->addWidget(day);
            x++;
        }
        //Refresh the css rules
        this->frames[i]->setStyleSheet(CELL_STYLE);
    }
}

void MonthView::display_events(Date date) {
    PManager pm;
    list<Event*> event_list = pm.get_events_of_month(date.getMonth(), date.getYear());
    int start_offset;

    //Remove all displayed events
    remove_events_from_all_frames();

    //Find at which cell the month starts
    for (start_offset = 0; start_offset < 42; start_offset++) {
        //Looks where is the first day of the month
        if (this->frames[start_offset]->getDate() != NULL) break;
    }
    //Add events to the gui
    for (Event *event : event_list) {
        Date start = DateUtil::date_from_timestamp(event->getStart());
        Date end = DateUtil::date_from_timestamp(event->getEnd());
        if (((start.getMonth() < date.getMonth()) && (start.getYear() == date.getYear())) || (start.getYear() < date.getYear()))
            start = DateUtil::get_first_day_of_month(date);
        if (((end.getMonth() > start.getMonth()) && (end.getYear() == start.getYear())) || (end.getYear() > start.getYear()))
            end = DateUtil::get_last_day_of_month(start);

        for (int i = start_offset+start.getMonthDay()-1; i < (start_offset+end.getMonthDay()); i++) {
            QLabelEvent *label_event = createLabelEvent(event);
            if (this->frames[i]->children().size() == 5) {
                QPushButtonExtended *button_show_all = new QPushButtonExtended("Show All");
                button_show_all->setIndex(i);
                connect(button_show_all, &QPushButtonExtended::on_click, this, &MonthView::on_button_extended_click);
                this->frames[i]->layout()->addWidget(button_show_all);
                label_event->setHidden(true);
            } else if (this->frames[i]->children().size() > 5) label_event->setHidden(true);
            //Events will be copied and wrapped inside the QLabelEvent widgets
            (static_cast <QVBoxLayout*> (this->frames[i]->layout()))->insertWidget(1, label_event);
        }
        delete event;
    }
}

QFrameExtended* MonthView::createQFrameExtended(Date *date) {
    QFrameExtended *frame = new QFrameExtended;
    QVBoxLayout *vl = new QVBoxLayout;
    frame->setDate(date);
    frame->setObjectName("day");
    vl->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    vl->setMargin(0);
    vl->setSpacing(1);
    vl->addWidget(new QLabel);
    frame->setMinimumWidth(150);
    frame->setMinimumHeight(60);
    frame->setLayout(vl);
    frame->setStyleSheet(CELL_STYLE);
    frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    return frame;
}

void MonthView::on_button_extended_click(int index) {
    //Make a copy
    Date *date = new Date(this->frames[index]->getDate()->getMonthDay(), this->frames[index]->getDate()->getWeekDay(), this->frames[index]->getDate()->getMonth(), this->frames[index]->getDate()->getYear());
    QFrameExtended *frame = createQFrameExtended(date);
    QString text = QString(DateUtil::numeric2literal_day_of_week(frame->getDate()->getWeekDay()).c_str()) +
                   QString("    ") +
                   QString::number(frame->getDate()->getMonthDay()) + QString(" ") +
                   QString(DateUtil::get_literal_month(frame->getDate()->getMonth()).c_str()) + QString(" ") +
                   QString::number(frame->getDate()->getYear());
    QLabel *label_day = static_cast<QLabel*> (frame->children().at(1));
    label_day->setText(text);
    label_day->setStyleSheet("QLabel { background-color: #ffffb3; border-bottom: 1px solid #000000; margin-bottom: 2px; }");
    (static_cast <QVBoxLayout*> (frame->layout()))->insertWidget(1, label_day);
    for (QLabelEvent *label_event : this->frames[index]->findChildren<QLabelEvent*>()) {
        Event *event = new Event(*label_event->getEvent());
        frame->layout()->addWidget(createLabelEvent(event));
    }
    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addWidget(frame);
    CustomDialog *custom_dialog = new CustomDialog(main_layout);
    custom_dialog->setFixedWidth(300);
    custom_dialog->setMinimumHeight(400);
    custom_dialog->setWindowTitle("Day Dialog");
    custom_dialog->show();
}

QLabelEvent* MonthView::createLabelEvent(Event *event) {
    //Make a copy
    Category *newCategory = new Category(event->getCategory()->getId(), event->getCategory()->getName(), event->getCategory()->getColor());
    Event *newEvent = new Event(event->getId(), event->getName(), event->getDescription(), newCategory, event->getStart(), event->getEnd());
    QLabelEvent *label_event = new QLabelEvent;
    label_event->setText(newEvent->getName().c_str());
    label_event->setEvent(newEvent);
    QString textColor("#000000");
    if (is_color_dark(newEvent->getCategory()->getColor()))
        textColor = "#FFFFFF";
    label_event->setStyleSheet(QString("QLabel { font-size: 14px; background-color : ") + QString(newEvent->getCategory()->getColor().c_str()) + QString("; color: ") + textColor + QString("};"));
    label_event->setFixedHeight(26);
    label_event->setMargin(0);
    label_event->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    label_event->setToolTip(newEvent->getDescription().c_str());
    connect(label_event, &QLabelEvent::clicked, this, &MonthView::on_event_click);
    return label_event;
}

void MonthView::display_events(Date date, Category category) {
    //TODO: implement me
}

void MonthView::remove_events_from_all_frames() {
    int i;
    for (i = 0; i < 42; i++) {
        //Delete all the event labels inside the frame
        remove_events_from_frame(i);
    }
}

void MonthView::remove_events_from_frame(int i) {
    QLabelEvent label_event;
    QPushButtonExtended button("");
    QListIterator<QObject *> it (this->frames[i]->children());
    while (it.hasNext()) {
        QObject *o = qobject_cast<QObject*> (it.next());
        if (o->metaObject()->className() == label_event.metaObject()->className()) delete o;
        else if (o->metaObject()->className() == button.metaObject()->className()) delete o;
    }
}

bool MonthView::is_color_dark(string colorName) {
    QColor color(colorName.c_str());
    //Formula to calculate luminance from ITU-R BT.709
    int l = 0.2126 * color.red() + 0.7152 * color.green() + 0.0722 * color.blue();
    if (l < 50)
        return true;
    else
        return false;
}

void MonthView::on_event_click(Event *event) {
    EventDialog *eventDialog = new EventDialog(this);
    eventDialog->setEvent(event);
    eventDialog->show();
}

//FIXME
/* BUG:
 * When the user changes the month, opens the EventDialog window, closes it and then changes the month again, when he'll close
 * the program, will be the following error: corrupted double-linked list
 */
