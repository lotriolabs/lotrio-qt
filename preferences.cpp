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

#include "preferences.h"


Preferences::Preferences()
{
    // General: State & Geometries
    m_restoreApplicationState = true;
    m_restoreApplicationGeometry = true;
    m_restoreDialogGeometry = true;
}


void Preferences::load(QSettings &settings)
{
    settings.beginGroup(QStringLiteral("Preferences"));

    // General: State & Geometries
    setRestoreApplicationState(settings.value(QStringLiteral("RestoreApplicationState"), true).toBool());
    setRestoreApplicationGeometry(settings.value(QStringLiteral("RestoreApplicationGeometry"), true).toBool());
    setRestoreDialogGeometry(settings.value(QStringLiteral("RestoreDialogGeometry"), true).toBool());

    settings.endGroup();
}


void Preferences::save(QSettings &settings)
{
    settings.beginGroup(QStringLiteral("Preferences"));
    settings.remove("");

    // General: State & Geometries
    settings.setValue(QStringLiteral("RestoreApplicationState"), m_restoreApplicationState);
    settings.setValue(QStringLiteral("RestoreApplicationGeometry"), m_restoreApplicationGeometry);
    settings.setValue(QStringLiteral("RestoreDialogGeometry"), m_restoreDialogGeometry);

    settings.endGroup();
}


void Preferences::setRestoreApplicationState(bool value)
{
    m_restoreApplicationState = value;
}


bool Preferences::restoreApplicationState(bool isDefault)
{
    if (isDefault)
        return true;

    return m_restoreApplicationState;
}


void Preferences::setRestoreApplicationGeometry(bool value)
{
    m_restoreApplicationGeometry = value;
}


bool Preferences::restoreApplicationGeometry(bool isDefault)
{
    if (isDefault)
        return true;

    return m_restoreApplicationGeometry;
}


void Preferences::setRestoreDialogGeometry(bool value)
{
    m_restoreDialogGeometry = value;
}


bool Preferences::restoreDialogGeometry(bool isDefault)
{
    if (isDefault)
        return true;

    return m_restoreDialogGeometry;
}
