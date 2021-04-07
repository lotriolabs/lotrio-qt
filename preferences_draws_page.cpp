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

#include "preferences_draws_page.h"

#include <QLabel>


PreferencesDrawsPage::PreferencesDrawsPage(QWidget *parent)
    : QWidget(parent)
{
    // Title
    auto *title = new QLabel(tr("<strong style=\"font-size:large;\">Draws</strong>"));

    // Content


    // Main layout
    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(title);
    m_layout->addStretch(1);
}


void PreferencesDrawsPage::setZeroMargins()
{
    m_layout->setContentsMargins(0, 0, 0, 0);
}


QString PreferencesDrawsPage::title() const
{
    return tr("Draws");
}


void PreferencesDrawsPage::onPreferencesChanged()
{
    emit preferencesChanged();
}
