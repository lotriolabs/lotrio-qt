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

#ifndef COLOPHON_DIALOG_H
#define COLOPHON_DIALOG_H

#include <QDialog>


class ColophonDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColophonDialog(QWidget *parent = nullptr);

    void setDialogGeometry(const QByteArray &geometry = QByteArray());
    QByteArray dialogGeometry() const;
};

#endif // COLOPHON_DIALOG_H
