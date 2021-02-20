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

#include "preferences.h"
#include "preferences_draws_page.h"
#include "preferences_general_page.h"
#include "preferences_lotteries_page.h"
#include "preferences_plays_page.h"


class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = nullptr);

    void setDialogGeometry(const QByteArray &geometry = QByteArray());
    QByteArray dialogGeometry() const;

    void setPreferences(const Preferences &preferences);
    Preferences preferences() const;

private slots:
    void onPreferencesChanged();

    void onButtonDefaultsClicked();
    void onButtonOkClicked();
    void onButtonApplyClicked();

private:
    Preferences m_preferences;

    void updatePreferences(bool isDefault = false);
    void savePreferences();

    QPushButton *m_buttonApply;

    PreferencesGeneralPage *m_generalPage;
    PreferencesLotteriesPage *m_lotteriesPage;
    PreferencesDrawsPage *m_drawsPage;
    PreferencesPlaysPage *m_playsPage;
};

#endif // PREFERENCES_DIALOG_H
