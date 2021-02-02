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

#ifndef PREFERENCES_DIALOG_H
#define PREFERENCES_DIALOG_H

#include <QDialog>
#include <QPushButton>

#include "settings.h"


class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = nullptr);

    void setDialogGeometry(const QByteArray &geometry = QByteArray());
    QByteArray dialogGeometry() const;

    void setSettings(const Settings &settings);
    Settings settings() const;

private slots:
    void onSettingsChanged();

    void onButtonDefaultsClicked();
    void onButtonOkClicked();
    void onButtonApplyClicked();

private:
    Settings m_settings;

    void updateSettings(bool isDefault = false);
    void saveSettings();

    QPushButton *m_buttonApply;
};

#endif // PREFERENCES_DIALOG_H