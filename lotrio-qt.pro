#
# Copyright 2021 LotrioLabs, <https://lotriolabs.github.io>.
#
# This file is part of Lotrio-Qt, <https://github.com/lotriolabs/lotrio-qt>.
#
# Lotrio-Qt is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Lotrio-Qt is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Lotrio-Qt.  If not, see <https://www.gnu.org/licenses/>.
#

QT += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    about_dialog.cpp \
    colophon_dialog.cpp \
    colophon_pages.cpp \
    dialog_title_box.cpp \
    document.cpp \
    keyboard_shortcuts_dialog.cpp \
    keyboard_shortcuts_page.cpp \
    main.cpp \
    main_window.cpp \
    preferences.cpp \
    preferences_dialog.cpp \
    preferences_draws_page.cpp \
    preferences_general_page.cpp \
    preferences_lotteries_page.cpp \
    preferences_plays_page.cpp

HEADERS += \
    about_dialog.h \
    colophon_dialog.h \
    colophon_pages.h \
    dialog_title_box.h \
    document.h \
    keyboard_shortcuts_dialog.h \
    keyboard_shortcuts_page.h \
    main_window.h \
    preferences.h \
    preferences_dialog.h \
    preferences_draws_page.h \
    preferences_general_page.h \
    preferences_lotteries_page.h \
    preferences_plays_page.h

TRANSLATIONS += \
    translations/lotrio-qt_de.ts \
    translations/lotrio-qt_en.ts \
    translations/lotrio-qt_he.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
