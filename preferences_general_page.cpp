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

#include "preferences_general_page.h"

#include <QGroupBox>
#include <QLabel>


PreferencesGeneralPage::PreferencesGeneralPage(QWidget *parent)
    : QWidget(parent)
{
    // Title
    auto *title = new QLabel(tr("<strong style=\"font-size:large;\">%1</strong>").arg(this->title()));

    //
    // Content: Geometry & State

    m_chkRestoreApplicationGeometry = new QCheckBox(tr("Save and restore the application geometry"));
    connect(m_chkRestoreApplicationGeometry, &QCheckBox::stateChanged, this, &PreferencesGeneralPage::onPreferencesChanged);

    m_chkRestoreApplicationState = new QCheckBox(tr("Save and restore the application state"));
    connect(m_chkRestoreApplicationState, &QCheckBox::stateChanged, this, &PreferencesGeneralPage::onPreferencesChanged);

    auto *geometryStateLayout = new QVBoxLayout;
    geometryStateLayout->addWidget(m_chkRestoreApplicationGeometry);
    geometryStateLayout->addWidget(m_chkRestoreApplicationState);

    auto *geometryStateGroup = new QGroupBox(tr("Geometry && State"));
    geometryStateGroup->setLayout(geometryStateLayout);

    // Main layout
    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(title);
    m_layout->addWidget(geometryStateGroup);
    m_layout->addStretch(1);
}


void PreferencesGeneralPage::setZeroMargins()
{
    m_layout->setContentsMargins(0, 0, 0, 0);
}


QString PreferencesGeneralPage::title() const
{
    return tr("General");
}


void PreferencesGeneralPage::onPreferencesChanged()
{
    emit preferencesChanged();
}


void PreferencesGeneralPage::setRestoreApplicationGeometry(const bool checked)
{
    m_chkRestoreApplicationGeometry->setChecked(checked);
}


bool PreferencesGeneralPage::restoreApplicationGeometry() const
{
    return m_chkRestoreApplicationGeometry->isChecked();
}


void PreferencesGeneralPage::setRestoreApplicationState(const bool checked)
{
    m_chkRestoreApplicationState->setChecked(checked);
}


bool PreferencesGeneralPage::restoreApplicationState() const
{
    return m_chkRestoreApplicationState->isChecked();
}
