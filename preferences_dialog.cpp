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

#include "preferences_dialog.h"

#include <QDialogButtonBox>
#include <QListWidget>
#include <QStackedWidget>
#include <QVBoxLayout>


PreferencesDialog::PreferencesDialog(QWidget *parent)
    : QDialog(parent)
{
    setMinimumSize(800, 600);
    setWindowTitle(tr("Preferences"));

    //
    // Content

    m_generalPage = new PreferencesGeneralPage;
    m_generalPage->setZeroMargins();
    connect(m_generalPage, &PreferencesGeneralPage::preferencesChanged, this, &PreferencesDialog::onPreferencesChanged);

    m_lotteriesPage = new PreferencesLotteriesPage;
    m_lotteriesPage->setZeroMargins();
    connect(m_lotteriesPage, &PreferencesLotteriesPage::preferencesChanged, this, &PreferencesDialog::onPreferencesChanged);

    m_drawsPage = new PreferencesDrawsPage;
    m_drawsPage->setZeroMargins();
    connect(m_drawsPage, &PreferencesDrawsPage::preferencesChanged, this, &PreferencesDialog::onPreferencesChanged);

    m_playsPage = new PreferencesPlaysPage;
    m_playsPage->setZeroMargins();
    connect(m_playsPage, &PreferencesPlaysPage::preferencesChanged, this, &PreferencesDialog::onPreferencesChanged);

    auto *stackedBox = new QStackedWidget;
    stackedBox->addWidget(m_generalPage);
    stackedBox->addWidget(m_lotteriesPage);
    stackedBox->addWidget(m_drawsPage);
    stackedBox->addWidget(m_playsPage);
    stackedBox->setCurrentIndex(0);

    auto *listBox = new QListWidget;
    listBox->addItem(m_generalPage->title());
    listBox->addItem(m_lotteriesPage->title());
    listBox->addItem(m_drawsPage->title());
    listBox->addItem(m_playsPage->title());
    listBox->setCurrentRow(stackedBox->currentIndex());
    connect(listBox, &QListWidget::currentRowChanged, stackedBox, &QStackedWidget::setCurrentIndex);

    auto *preferencesBox = new QHBoxLayout;
    preferencesBox->addWidget(listBox, 1);
    preferencesBox->addWidget(stackedBox, 3);

    // Button box
    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::RestoreDefaults | QDialogButtonBox::Ok | QDialogButtonBox::Apply | QDialogButtonBox::Cancel);
    m_buttonApply = buttonBox->button(QDialogButtonBox::Apply);
    connect(buttonBox->button(QDialogButtonBox::RestoreDefaults), &QAbstractButton::clicked, this, &PreferencesDialog::onButtonDefaultsClicked);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &PreferencesDialog::onButtonOkClicked);
    connect(m_buttonApply, &QAbstractButton::clicked, this, &PreferencesDialog::onButtonApplyClicked);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &PreferencesDialog::close);

    // Main layout
    auto *layout = new QVBoxLayout(this);
    layout->addLayout(preferencesBox);
    layout->addWidget(buttonBox);

    updatePreferences();
    m_buttonApply->setEnabled(false);
}


void PreferencesDialog::setPreferences(const Preferences &preferences)
{
    m_preferences = preferences;

    updatePreferences();
    m_buttonApply->setEnabled(false);
}


Preferences PreferencesDialog::preferences() const
{
    return m_preferences;
}


void PreferencesDialog::onPreferencesChanged()
{
    m_buttonApply->setEnabled(true);
}


void PreferencesDialog::onButtonDefaultsClicked()
{
    updatePreferences(true);
}


void PreferencesDialog::onButtonOkClicked()
{
    savePreferences();
    close();
}


void PreferencesDialog::onButtonApplyClicked()
{
    savePreferences();
    m_buttonApply->setEnabled(false);
}


void PreferencesDialog::updatePreferences(bool isDefault)
{
    // General: Geometry & State
    m_generalPage->setRestoreApplicationGeometry(m_preferences.restoreApplicationGeometry(isDefault));
    m_generalPage->setRestoreApplicationState(m_preferences.restoreApplicationState(isDefault));
}


void PreferencesDialog::savePreferences()
{
    // General: Geometry & State
    m_preferences.setRestoreApplicationGeometry(m_generalPage->restoreApplicationGeometry());
    m_preferences.setRestoreApplicationState(m_generalPage->restoreApplicationState());
}
