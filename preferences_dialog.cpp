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

    m_pageGeneral = new PreferencesPageGeneral;
    m_pageGeneral->setZeroMargins();
    connect(m_pageGeneral, &PreferencesPageGeneral::preferencesChanged, this, &PreferencesDialog::onPreferencesChanged);

    m_pageLotteries = new PreferencesPageLotteries;
    m_pageLotteries->setZeroMargins();
    connect(m_pageLotteries, &PreferencesPageLotteries::preferencesChanged, this, &PreferencesDialog::onPreferencesChanged);

    m_pageDraws = new PreferencesPageDraws;
    m_pageDraws->setZeroMargins();
    connect(m_pageDraws, &PreferencesPageDraws::preferencesChanged, this, &PreferencesDialog::onPreferencesChanged);

    m_pagePlays = new PreferencesPagePlays;
    m_pagePlays->setZeroMargins();
    connect(m_pagePlays, &PreferencesPagePlays::preferencesChanged, this, &PreferencesDialog::onPreferencesChanged);

    auto *stackedBox = new QStackedWidget;
    stackedBox->addWidget(m_pageGeneral);
    stackedBox->addWidget(m_pageLotteries);
    stackedBox->addWidget(m_pageDraws);
    stackedBox->addWidget(m_pagePlays);
    stackedBox->setCurrentIndex(0);

    auto *listBox = new QListWidget;
    listBox->addItem(m_pageGeneral->title());
    listBox->addItem(m_pageLotteries->title());
    listBox->addItem(m_pageDraws->title());
    listBox->addItem(m_pagePlays->title());
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
    m_pageGeneral->setRestoreApplicationGeometry(m_preferences.restoreApplicationGeometry(isDefault));
    m_pageGeneral->setRestoreApplicationState(m_preferences.restoreApplicationState(isDefault));

    // General: Tab Bars
    m_pageGeneral->setDefaultTabbarLotteriesPosition(m_preferences.defaultTabbarLotteriesPosition(isDefault));
    m_pageGeneral->setDefaultTabbarSheetsPosition(m_preferences.defaultTabbarSheetsPosition(isDefault));
}


void PreferencesDialog::savePreferences()
{
    // General: Geometry & State
    m_preferences.setRestoreApplicationGeometry(m_pageGeneral->restoreApplicationGeometry());
    m_preferences.setRestoreApplicationState(m_pageGeneral->restoreApplicationState());

    // General: Tab Bars
    m_preferences.setDefaultTabbarLotteriesPosition((m_pageGeneral->defaultTabbarLotteriesPosition()));
    m_preferences.setDefaultTabbarSheetsPosition((m_pageGeneral->defaultTabbarSheetsPosition()));
}
