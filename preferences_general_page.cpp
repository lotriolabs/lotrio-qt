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

#include "preferences_general_page.h"

#include <QGroupBox>
#include <QLabel>


PreferencesGeneralPage::PreferencesGeneralPage(QWidget *parent)
    : QWidget(parent)
{
    // Title
    auto *title = new QLabel(tr("<strong style=\"font-size:large;\">General</strong>"));

    // State & Geometry
    m_chkRestoreApplicationState = new QCheckBox(tr("Save and restore the application state"));
    connect(m_chkRestoreApplicationState, &QCheckBox::stateChanged, this, &PreferencesGeneralPage::onSettingsChanged);

    m_chkRestoreApplicationGeometry = new QCheckBox(tr("Save and restore the application geometry"));
    connect(m_chkRestoreApplicationGeometry, &QCheckBox::stateChanged, this, &PreferencesGeneralPage::onSettingsChanged);

    auto *stateLayout = new QVBoxLayout;
    stateLayout->addWidget(m_chkRestoreApplicationState);
    stateLayout->addWidget(m_chkRestoreApplicationGeometry);

    auto *stateGroup = new QGroupBox(tr("State"));
    stateGroup->setLayout(stateLayout);

    // Main layout
    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(title);
    m_layout->addWidget(stateGroup);
    m_layout->addStretch();
}


void PreferencesGeneralPage::setZeroMargins()
{
    m_layout->setContentsMargins(0, 0, 0, 0);
}


QString PreferencesGeneralPage::title() const
{
    return tr("General");
}


void PreferencesGeneralPage::onSettingsChanged()
{
    emit settingsChanged();
}


void PreferencesGeneralPage::setRestoreApplicationState(const bool checked)
{
    m_chkRestoreApplicationState->setChecked(checked);
}


bool PreferencesGeneralPage::restoreApplicationState() const
{
    return m_chkRestoreApplicationState->isChecked();
}


void PreferencesGeneralPage::setRestoreApplicationGeometry(const bool checked)
{
    m_chkRestoreApplicationGeometry->setChecked(checked);
}


bool PreferencesGeneralPage::restoreApplicationGeometry() const
{
    return m_chkRestoreApplicationGeometry->isChecked();
}
