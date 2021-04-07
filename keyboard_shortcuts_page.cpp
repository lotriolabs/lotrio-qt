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

#include "keyboard_shortcuts_page.h"

#include <QAction>
#include <QHeaderView>
#include <QTableWidget>


KeyboardShortcutsPage::KeyboardShortcutsPage(QWidget *mainWindow, QWidget *parent)
    : QWidget(parent)
{
    QStringList headerLabels = { tr("Name"), tr("Shortcut"), tr("Description") };

    auto *tableBox = new QTableWidget(0, headerLabels.size());
    tableBox->setHorizontalHeaderLabels(headerLabels);
    tableBox->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    tableBox->horizontalHeader()->setStretchLastSection(true);
    tableBox->verticalHeader()->setVisible(false);
    tableBox->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableBox->setSelectionMode(QAbstractItemView::NoSelection);
    tableBox->setFocusPolicy(Qt::NoFocus);

    QList<QAction *> actionItems = mainWindow->findChildren<QAction *> (QString(), Qt::FindDirectChildrenOnly);
    for (auto *actionItem : actionItems) {

        if (!actionItem->shortcut().isEmpty()) {
            int idx = tableBox->rowCount();

            tableBox->setRowCount(idx + 1);
            tableBox->setItem(idx, 0, new QTableWidgetItem(actionItem->icon(), actionItem->text()));
            tableBox->setItem(idx, 1, new QTableWidgetItem(actionItem->shortcut().toString(QKeySequence::NativeText)));
            tableBox->setItem(idx, 2, new QTableWidgetItem(actionItem->toolTip()));
        }
    }
    tableBox->resizeColumnsToContents();

    // Main layout
    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(tableBox);
}


void KeyboardShortcutsPage::setZeroMargins()
{
    m_layout->setContentsMargins(0, 0, 0, 0);
}


QString KeyboardShortcutsPage::title() const
{
    return tr("Keyboard Shortcuts");
}
