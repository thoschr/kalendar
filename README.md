# Kalendar

Kalendar is a calendar application written in C++ with the QT library for the gui. This project **is not related** with KDE, but this desktop environment is the main target of this project. I'm try to keep Kalendar as simple as possible and I want to avoid annoying dependencies (so you can easly install it everywhere).

**The project is currently under heavy development** and requires a lot of work to be ready for the first release. Stay tuned.

The final result should be something similar to gnome-calendar.

TODO:
* Fix issues related to daylight saving time
* External plugins support
* Support iCalendar format [as plugin]
* Notify system [as plugin]
* Year view for long-term events
* Meteo plugin

### Screenshot

![screenshot](https://raw.githubusercontent.com/echo-devim/kalendar/master/screenshot.png)
![screenshot2](https://raw.githubusercontent.com/echo-devim/kalendar/master/screenshot2.png)
![screenshot3](https://raw.githubusercontent.com/echo-devim/kalendar/master/screenshot3.png)

### Motivation
I need to schedule my time in a more efficient way. I don't like online calendar like google calendar for the privacy issues. I looked for some good calendar application, but seems there are just a couple of such calendars. This is a shame, because linux exists from so many years and the users need to have a good calendar application. The two main calendar applications are: gnome-desktop and korganizer. Their problems are a lot of dependencies and complicated compilation. So I decided to develop a simple but powerful calendar with painless compilation, without dependencies and desktop environment independent.

### Compilation

You need to have `qmake`. In Fedora you can find it inside the `qt-devel` package.

You need to install also `sqlite3` developer package. In Fedora the package is called `sqlite-devel`.

After you've installed these packages, you can run `qmake` (or `qmake-qt5` in Fedora) inside the src folder and then the `make` command.
You should get the Kalendar executable inside the same folder.

Note: Kalendar will create the following folder: ~/kalendar, so remember to delete it if you decide to delete Kalendar.

### License
GPLv3
