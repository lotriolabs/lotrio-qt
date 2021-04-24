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

#ifndef PREFERENCES_PAGE_GENERAL_H
#define PREFERENCES_PAGE_GENERAL_H

#include <QButtonGroup>
#include <QCheckBox>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>


class PreferencesPageGeneral : public QWidget
{
    Q_OBJECT

public:
    explicit PreferencesPageGeneral(QWidget *parent = nullptr);

    void setZeroMargins();

    QString title() const;

    void setRestoreApplicationGeometry(const bool checked);
    bool restoreApplicationGeometry() const;

    void setRestoreApplicationState(const bool checked);
    bool restoreApplicationState() const;

    void setDefaultTabPositionLotteries(const QTabWidget::TabPosition type);
    QTabWidget::TabPosition defaultTabPositionLotteries() const;

signals:
    void preferencesChanged();

private slots:
    void onPreferencesChanged();

private:
    QVBoxLayout *m_layout;

    QCheckBox *m_chkRestoreApplicationGeometry;
    QCheckBox *m_chkRestoreApplicationState;

    QButtonGroup *m_grpDefaultTabPositionLotteries;
};

#endif // PREFERENCES_PAGE_GENERAL_H
