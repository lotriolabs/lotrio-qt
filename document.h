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

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QCloseEvent>
#include <QWidget>

#include "preferences.h"


class Document : public QWidget
{
    Q_OBJECT

public:
    explicit Document(QWidget *parent = nullptr);

    void setPreferences(const Preferences &preferences);

    void setCanonicalName(const QString &canonicalName);
    QString canonicalName() const;

    QString documentTitle() const;
    void updateDocumentTitle();

    bool load(const QString &canonicalName);

signals:
    void aboutToClose(QString);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Preferences m_preferences;

    QString m_canonicalName;
};

#endif // DOCUMENT_H
