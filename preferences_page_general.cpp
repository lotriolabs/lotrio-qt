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

    auto *rdbDefaultTabbarLotteriesPositionTop = new QRadioButton(tr("Top"));
    rdbDefaultTabbarLotteriesPositionTop->setToolTip(tr("The lottery tabs are displayed above the pages"));

    auto *rdbDefaultTabbarLotteriesPositionBottom = new QRadioButton(tr("Bottom"));
    rdbDefaultTabbarLotteriesPositionBottom->setToolTip(tr("The lottery tabs are displayed below the pages"));

    m_grpDefaultTabbarLotteriesPosition = new QButtonGroup(this);
    m_grpDefaultTabbarLotteriesPosition->addButton(rdbDefaultTabbarLotteriesPositionTop, (int) QTabWidget::North);
    m_grpDefaultTabbarLotteriesPosition->addButton(rdbDefaultTabbarLotteriesPositionBottom, (int) QTabWidget::South);
    connect(m_grpDefaultTabbarLotteriesPosition, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, &PreferencesPageGeneral::onPreferencesChanged);

    auto *rdbDefaultTabbarSheetsPositionTop = new QRadioButton(tr("Top"));
    rdbDefaultTabbarSheetsPositionTop->setToolTip(tr("The sheet tabs are displayed above the pages"));

    auto *rdbDefaultTabbarSheetsPositionBottom = new QRadioButton(tr("Bottom"));
    rdbDefaultTabbarSheetsPositionBottom->setToolTip(tr("The sheet tabs are displayed below the pages"));

    m_grpDefaultTabbarSheetsPosition = new QButtonGroup(this);
    m_grpDefaultTabbarSheetsPosition->addButton(rdbDefaultTabbarSheetsPositionTop, (int) QTabWidget::North);
    m_grpDefaultTabbarSheetsPosition->addButton(rdbDefaultTabbarSheetsPositionBottom, (int) QTabWidget::South);
    connect(m_grpDefaultTabbarSheetsPosition, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, &PreferencesPageGeneral::onPreferencesChanged);

    auto *defaultTabbarLotteriesPositionBox = new QHBoxLayout;
    defaultTabbarLotteriesPositionBox->addWidget(rdbDefaultTabbarLotteriesPositionTop);
    defaultTabbarLotteriesPositionBox->addWidget(rdbDefaultTabbarLotteriesPositionBottom);

    auto *defaultTabbarSheetsPositionBox = new QHBoxLayout;
    defaultTabbarSheetsPositionBox->addWidget(rdbDefaultTabbarSheetsPositionTop);
    defaultTabbarSheetsPositionBox->addWidget(rdbDefaultTabbarSheetsPositionBottom);

    auto *defaultTabbarPositionLayout = new QFormLayout;
    defaultTabbarPositionLayout->addRow(tr("Position of the lottery tabs"), defaultTabbarLotteriesPositionBox);
    defaultTabbarPositionLayout->addRow(tr("Position of the sheet tabs"), defaultTabbarSheetsPositionBox);

    auto *tabBarsGroup = new QGroupBox(tr("Tab Bars"));
    tabBarsGroup->setLayout(defaultTabbarPositionLayout);


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


void PreferencesPageGeneral::setDefaultTabbarLotteriesPosition(const QTabWidget::TabPosition tabPosition)
{
    if (tabPosition != defaultTabbarLotteriesPosition())
        onPreferencesChanged();

    const QList<QAbstractButton *> buttons = m_grpDefaultTabbarLotteriesPosition->buttons();
    for (auto *button : buttons) {
        if (m_grpDefaultTabbarLotteriesPosition->id(button) == (int) tabPosition) {
            button->setChecked(true);
            break;
        }
    }
}


QTabWidget::TabPosition PreferencesPageGeneral::defaultTabbarLotteriesPosition() const
{
    return static_cast<QTabWidget::TabPosition> (m_grpDefaultTabbarLotteriesPosition->checkedId());
}


void PreferencesPageGeneral::setDefaultTabbarSheetsPosition(const QTabWidget::TabPosition tabPosition)
{
    if (tabPosition != defaultTabbarSheetsPosition())
        onPreferencesChanged();

    const QList<QAbstractButton *> buttons = m_grpDefaultTabbarSheetsPosition->buttons();
    for (auto *button : buttons) {
        if (m_grpDefaultTabbarSheetsPosition->id(button) == (int) tabPosition) {
            button->setChecked(true);
            break;
        }
    }
}


QTabWidget::TabPosition PreferencesPageGeneral::defaultTabbarSheetsPosition() const
{
    return static_cast<QTabWidget::TabPosition> (m_grpDefaultTabbarSheetsPosition->checkedId());
}
