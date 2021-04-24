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

#include "preferences_page_general.h"

#include <QGroupBox>
#include <QFormLayout>
#include <QLabel>
#include <QRadioButton>


PreferencesPageGeneral::PreferencesPageGeneral(QWidget *parent)
    : QWidget(parent)
{
    // Title
    auto *title = new QLabel(tr("<strong style=\"font-size:large;\">%1</strong>").arg(this->title()));


    //
    // Content: Geometry & State

    m_chkRestoreApplicationGeometry = new QCheckBox(tr("Save and restore the application geometry"));
    connect(m_chkRestoreApplicationGeometry, &QCheckBox::stateChanged, this, &PreferencesPageGeneral::onPreferencesChanged);

    m_chkRestoreApplicationState = new QCheckBox(tr("Save and restore the application state"));
    connect(m_chkRestoreApplicationState, &QCheckBox::stateChanged, this, &PreferencesPageGeneral::onPreferencesChanged);

    auto *geometryStateLayout = new QVBoxLayout;
    geometryStateLayout->addWidget(m_chkRestoreApplicationGeometry);
    geometryStateLayout->addWidget(m_chkRestoreApplicationState);

    auto *geometryStateGroup = new QGroupBox(tr("Geometry && State"));
    geometryStateGroup->setLayout(geometryStateLayout);


    //
    // Content: Tab Bars

    auto *rdbDefaultTabPositionLotteriesTop = new QRadioButton(tr("Top"));
    rdbDefaultTabPositionLotteriesTop->setToolTip(tr("The lottery tabs are drawn above the pages."));

    auto *rdbDefaultTabPositionLotteriesBottom = new QRadioButton(tr("Bottom"));
    rdbDefaultTabPositionLotteriesBottom->setToolTip(tr("The lottery tabs are drawn below the pages."));

    m_grpDefaultTabPositionLotteries = new QButtonGroup(this);
    m_grpDefaultTabPositionLotteries->addButton(rdbDefaultTabPositionLotteriesTop, (int) QTabWidget::North);
    m_grpDefaultTabPositionLotteries->addButton(rdbDefaultTabPositionLotteriesBottom, (int) QTabWidget::South);
    connect(m_grpDefaultTabPositionLotteries, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, &PreferencesPageGeneral::onPreferencesChanged);

    auto *rdbDefaultTabPositionSheetsTop = new QRadioButton(tr("Top"));
    rdbDefaultTabPositionSheetsTop->setToolTip(tr("The sheet tabs are drawn above the pages."));

    auto *rdbDefaultTabPositionSheetsBottom = new QRadioButton(tr("Bottom"));
    rdbDefaultTabPositionSheetsBottom->setToolTip(tr("The sheet tabs are drawn below the pages."));

    m_grpDefaultTabPositionSheets = new QButtonGroup(this);
    m_grpDefaultTabPositionSheets->addButton(rdbDefaultTabPositionSheetsTop, (int) QTabWidget::North);
    m_grpDefaultTabPositionSheets->addButton(rdbDefaultTabPositionSheetsBottom, (int) QTabWidget::South);
    connect(m_grpDefaultTabPositionSheets, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, &PreferencesPageGeneral::onPreferencesChanged);

    auto *defaultTabPositionLotteriesBox = new QHBoxLayout;
    defaultTabPositionLotteriesBox->addWidget(rdbDefaultTabPositionLotteriesTop);
    defaultTabPositionLotteriesBox->addWidget(rdbDefaultTabPositionLotteriesBottom);

    auto *defaultTabPositionSheetsBox = new QHBoxLayout;
    defaultTabPositionSheetsBox->addWidget(rdbDefaultTabPositionSheetsTop);
    defaultTabPositionSheetsBox->addWidget(rdbDefaultTabPositionSheetsBottom);

    auto *defaultTabPositionLayout = new QFormLayout;
    defaultTabPositionLayout->addRow(tr("Position of the lottery tabs"), defaultTabPositionLotteriesBox);
    defaultTabPositionLayout->addRow(tr("Position of the sheet tabs"), defaultTabPositionSheetsBox);

    auto *tabBarsGroup = new QGroupBox(tr("Tab Bars"));
    tabBarsGroup->setLayout(defaultTabPositionLayout);


    // Main layout
    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(title);
    m_layout->addWidget(geometryStateGroup);
    m_layout->addWidget(tabBarsGroup);
    m_layout->addStretch(1);
}


void PreferencesPageGeneral::setZeroMargins()
{
    m_layout->setContentsMargins(0, 0, 0, 0);
}


QString PreferencesPageGeneral::title() const
{
    return tr("General");
}


void PreferencesPageGeneral::onPreferencesChanged()
{
    emit preferencesChanged();
}


void PreferencesPageGeneral::setRestoreApplicationGeometry(const bool checked)
{
    m_chkRestoreApplicationGeometry->setChecked(checked);
}


bool PreferencesPageGeneral::restoreApplicationGeometry() const
{
    return m_chkRestoreApplicationGeometry->isChecked();
}


void PreferencesPageGeneral::setRestoreApplicationState(const bool checked)
{
    m_chkRestoreApplicationState->setChecked(checked);
}


bool PreferencesPageGeneral::restoreApplicationState() const
{
    return m_chkRestoreApplicationState->isChecked();
}


void PreferencesPageGeneral::setDefaultTabPositionLotteries(const QTabWidget::TabPosition type)
{
    if (type != defaultTabPositionLotteries())
        onPreferencesChanged();

    const QList<QAbstractButton *> buttons = m_grpDefaultTabPositionLotteries->buttons();
    for (auto *button : buttons) {
        if (m_grpDefaultTabPositionLotteries->id(button) == (int) type)
            button->setChecked(true);
    }
}


QTabWidget::TabPosition PreferencesPageGeneral::defaultTabPositionLotteries() const
{
    return static_cast<QTabWidget::TabPosition> (m_grpDefaultTabPositionLotteries->checkedId());
}


void PreferencesPageGeneral::setDefaultTabPositionSheets(const QTabWidget::TabPosition type)
{
    if (type != defaultTabPositionSheets())
        onPreferencesChanged();

    const QList<QAbstractButton *> buttons = m_grpDefaultTabPositionSheets->buttons();
    for (auto *button : buttons) {
        if (m_grpDefaultTabPositionSheets->id(button) == (int) type)
            button->setChecked(true);
    }
}


QTabWidget::TabPosition PreferencesPageGeneral::defaultTabPositionSheets() const
{
    return static_cast<QTabWidget::TabPosition> (m_grpDefaultTabPositionSheets->checkedId());
}
