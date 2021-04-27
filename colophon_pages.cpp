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

#include "colophon_pages.h"

#include <QApplication>
#include <QTextBrowser>


//
//
// Colophon page: About
//

ColophonPageAbout::ColophonPageAbout(QWidget *parent)
    : QWidget(parent)
{
    auto *textBox = new QTextBrowser;
    textBox->setFrameStyle(QFrame::NoFrame);
    textBox->setStyleSheet(QStringLiteral("background-color:transparent;"));
    textBox->setOpenExternalLinks(true);
    textBox->setHtml(tr("<html><body>"
        "<p>%1 is an open source visualization tool written in Qt for C++ and designed for easy creation and graphical representation of lottery data.</p>"
        "<p>Copyright &copy; 2021 <a href=\"%2\" title=\"Visit organization's homepage\">%3</a>.</p>"
        "<p>This application is licensed under the terms of the <a href=\"https://www.gnu.org/licenses/gpl-3.0.en.html\" title=\"Visit license's homepage\">GNU General Public License, version 3</a>.</p>"
        "</body></html>").arg(QApplication::applicationName(), QApplication::organizationDomain(), QApplication::organizationName()));

    // Main layout
    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(textBox);
}


void ColophonPageAbout::setZeroMargins()
{
    m_layout->setContentsMargins(0, 0, 0, 0);
}


QString ColophonPageAbout::title() const
{
    return tr("About");
}


//
//
// Colophon page: Authors
//

ColophonPageAuthors::ColophonPageAuthors(QWidget *parent)
    : QWidget(parent)
{
    auto *textBox = new QTextBrowser;
    textBox->setFrameStyle(QFrame::NoFrame);
    textBox->setStyleSheet(QStringLiteral("background-color:transparent;"));
    textBox->setOpenExternalLinks(true);
    textBox->setHtml(tr("<html><body>"
        "<dl><dt><strong>NotNypical</strong></dt>"
            "<dd>Created and developed by <a href=\"https://notnypical.github.io\" title=\"Visit author's homepage\">NotNypical</a>.</dd></dl>"
        "</body></html>"));

    // Main layout
    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(textBox);
}


void ColophonPageAuthors::setZeroMargins()
{
    m_layout->setContentsMargins(0, 0, 0, 0);
}


QString ColophonPageAuthors::title() const
{
    return tr("Authors");
}


//
//
// Colophon page: Credits
//

ColophonPageCredits::ColophonPageCredits(QWidget *parent)
    : QWidget(parent)
{
    auto *textBox = new QTextBrowser;
    textBox->setFrameStyle(QFrame::NoFrame);
    textBox->setStyleSheet(QStringLiteral("background-color:transparent;"));
    textBox->setOpenExternalLinks(true);
    textBox->setHtml(tr("<html><body>"
        "<dl><dt><strong>Freepik</strong></dt>"
            "<dd>Application logo made by <a href=\"https://www.flaticon.com/authors/freepik/\" title=\"Visit author's homepage\">Freepik</a> "
                "from <a href=\"https://www.flaticon.com\" title=\"Visit organization's homepage\">www.flaticon.com</a> "
                "is licensed under <a href=\"https://file000.flaticon.com/downloads/license/license.pdf\" title=\"Visit license's homepage\">Free License (with attribution)</a>.</dd></dl>"
        "<dl><dt><strong>BreezeIcons project</strong></dt>"
            "<dd>Application icons made by <a href=\"https://api.kde.org/frameworks/breeze-icons/html/\" title=\"Visit project's homepage\">BreezeIcons project</a> "
                "from <a href=\"https://kde.org\" title=\"Visit organization's homepage\">KDE</a> "
                "are licensed under <a href=\"https://www.gnu.org/licenses/lgpl-3.0.en.html\" title=\"Visit license's homepage\">LGPLv3</a>.</dd></dl>"
        "</body></html>"));

    // Main layout
    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(textBox);
}


void ColophonPageCredits::setZeroMargins()
{
    m_layout->setContentsMargins(0, 0, 0, 0);
}


QString ColophonPageCredits::title() const
{
    return tr("Credits");
}


//
//
// Colophon page: Environment
//

ColophonEnvironmentPage::ColophonEnvironmentPage(QWidget *parent)
    : QWidget(parent)
{
    const QString qtVersion = qVersion(); // Qt version used to run Qt for C++
    const QString qtBuildVersion = QT_VERSION_STR; // Qt version used to compile Qt
    const QString osName = QSysInfo::prettyProductName();
    const QString osKernelVersion = QSysInfo::kernelVersion();
    const QString osCpuArchitecture = QSysInfo::currentCpuArchitecture();

    auto *textBox = new QTextBrowser;
    textBox->setFrameStyle(QFrame::NoFrame);
    textBox->setStyleSheet(QStringLiteral("background-color:transparent;"));
    textBox->setOpenExternalLinks(true);
    textBox->setHtml(tr("<html><body>"
        "<dl><dt><strong>Application version</strong></dt>"
            "<dd>%1</dd></dl>"
        "<dl><dt><strong>Qt for C++ version</strong></dt>"
            "<dd>Qt %2 (Built against %3)</dd></dl>"
        "<dl><dt><strong>Operation System</strong></dt>"
            "<dd>%4 (Kernel %5 on %6)</dd></dl>"
        "</body></html>").arg(QApplication::applicationVersion(), qtVersion, qtBuildVersion, osName, osKernelVersion, osCpuArchitecture));

    // Main layout
    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(textBox);
}


void ColophonEnvironmentPage::setZeroMargins()
{
    m_layout->setContentsMargins(0, 0, 0, 0);
}


QString ColophonEnvironmentPage::title() const
{
    return tr("Environment");
}


//
//
// Colophon page: License
//

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
