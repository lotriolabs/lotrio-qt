/**
 * Copyright 2021 LotrioLabs, <https://lotriolabs.github.io>.
 *
 * This file is part of Lotrio-Qt, <https://github.com/lotriolabs/lotrio-qt>.
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

#include "colophon_license_page.h"

#include <QApplication>
#include <QTextBrowser>


ColophonLicensePage::ColophonLicensePage(QWidget *parent)
    : QWidget(parent)
{
    auto *textBox = new QTextBrowser;
    textBox->setFrameStyle(QFrame::NoFrame);
    textBox->setStyleSheet(QStringLiteral("background-color:transparent;"));
    textBox->setOpenExternalLinks(true);
    textBox->setHtml(tr("<html><body>"
        "<p>%1 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.</p>"
        "<p>%1 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.</p>"
        "<p>You should have received a copy of the GNU General Public License along with %1. If not, see <a href=\"https://www.gnu.org/licenses/\" title=\"Visit license's homepage\">https://www.gnu.org/licenses/</a>.</p>"
        "</body></html>").arg(QApplication::applicationName()));

    // Main layout
    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(textBox);
}


void ColophonLicensePage::setZeroMargins()
{
    m_layout->setContentsMargins(0, 0, 0, 0);
}


QString ColophonLicensePage::title() const
{
    return tr("License");
}
