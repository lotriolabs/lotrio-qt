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

#ifndef LOTTERY_DOCUMENT_H
#define LOTTERY_DOCUMENT_H

#include <QCloseEvent>
#include <QTabWidget>
#include <QWidget>

#include "preferences.h"


class LotteryDocument : public QWidget
{
    Q_OBJECT

public:
    explicit LotteryDocument(QWidget *parent = nullptr);

    void setPreferences(const Preferences &preferences);

    void setCanonicalName(const QString &canonicalName);
    QString canonicalName() const;

    QString documentTitle() const;
    void updateDocumentTitle();

    void setTabPosition(const QTabWidget::TabPosition tabPosition);
    QTabWidget::TabPosition tabPosition() const;

    bool load(const QString &canonicalName);

signals:
    void aboutToClose(QString);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QTabWidget *m_tabBox;
    Preferences m_preferences;

    QString m_canonicalName;
};

#endif // LOTTERY_DOCUMENT_H
