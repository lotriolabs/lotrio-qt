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

#include "about_dialog.h"

#include <QApplication>
#include <QDialogButtonBox>
#include <QSettings>
#include <QVBoxLayout>

#include "colophon_about_page.h"
#include "dialog_title_box.h"


AboutDialog::AboutDialog(const bool &restoreGeometry, QWidget *parent)
    : QDialog(parent)
    , m_restoreGeometry(restoreGeometry)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("About %1").arg(QApplication::applicationName()));

    loadSettings();

    // Title box
    auto *titleBox = new DialogTitleBox;

    // Content
    auto *aboutPage = new ColophonAboutPage;
    aboutPage->setZeroMargins();

    // Button box
    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &AboutDialog::close);

    // Main layout
    auto *layout = new QVBoxLayout(this);
    layout->addWidget(titleBox);
    layout->addWidget(aboutPage);
    layout->addWidget(buttonBox);
}


void AboutDialog::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
}


void AboutDialog::loadSettings()
{
    QSettings settings;

    const auto geometry = m_restoreGeometry ? settings.value(QStringLiteral("AboutDialog/Geometry"), QByteArray()).toByteArray() : QByteArray();

    if (!geometry.isEmpty())
        restoreGeometry(geometry);
    else
        resize(480, 320);
}


void AboutDialog::saveSettings()
{
    QSettings settings;

    const auto geometry = m_restoreGeometry ? saveGeometry() : QByteArray();

    settings.setValue(QStringLiteral("AboutDialog/Geometry"), geometry);
}
