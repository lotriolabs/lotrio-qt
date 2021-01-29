/**
 * Copyright 2021 LotrioLabs, <https://lotriolabs.github.io>.
 *
 * This file is part of Lotrio-Qt.
 *
 * Lotrio-Qt is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Lotrio-Qt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Lotrio-Qt.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "main_window.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName(QStringLiteral("LotrioLabs"));
    app.setOrganizationDomain(QStringLiteral("https://lotriolabs.github.io"));
    app.setApplicationName(QStringLiteral("Lotrio-Qt"));
    app.setApplicationDisplayName(QStringLiteral("Lotrio-Qt"));
    app.setApplicationVersion(QStringLiteral("0.1.0"));

    MainWindow window;
    window.show();

    return app.exec();
}
