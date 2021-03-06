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

#include "colophon_dialog.h"

#include <QDialogButtonBox>
#include <QTabWidget>
#include <QVBoxLayout>

#include "colophon_pages.h"
#include "dialog_title_box.h"


ColophonDialog::ColophonDialog(QWidget *parent)
    : QDialog(parent)
{
    setMinimumSize(640, 480);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Colophon"));

    // Title box
    auto *titleBox = new DialogTitleBox;


    //
    // Content

    auto *pageAbout = new ColophonPageAbout;
    auto *pageEnvironment = new ColophonPageEnvironment;
    auto *pageLicense = new ColophonPageLicense;
    auto *pageAuthors = new ColophonPageAuthors;
    auto *pageCredits = new ColophonPageCredits;

    auto *tabBox = new QTabWidget;
    tabBox->addTab(pageAbout, pageAbout->title());
    tabBox->addTab(pageEnvironment, pageEnvironment->title());
    tabBox->addTab(pageLicense, pageLicense->title());
    tabBox->addTab(pageAuthors, pageAuthors->title());
    tabBox->addTab(pageCredits, pageCredits->title());


    // Button box
    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ColophonDialog::close);

    // Main layout
    auto *layout = new QVBoxLayout(this);
    layout->addWidget(titleBox);
    layout->addWidget(tabBox);
    layout->addWidget(buttonBox);
}
