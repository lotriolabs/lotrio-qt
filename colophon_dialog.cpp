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

#include "colophon_dialog.h"

#include <QApplication>
#include <QDialogButtonBox>
#include <QVBoxLayout>


ColophonDialog::ColophonDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Colophon"));


    // Button box
    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ColophonDialog::close);

    // Main layout
    auto *layout = new QVBoxLayout(this);
    layout->addStretch(1);
    layout->addWidget(buttonBox);
}


void ColophonDialog::setDialogGeometry(const QByteArray &geometry)
{
    if (!geometry.isEmpty())
        restoreGeometry(geometry);
    else
        resize(640, 480);
}


QByteArray ColophonDialog::dialogGeometry() const
{
    return saveGeometry();
}
