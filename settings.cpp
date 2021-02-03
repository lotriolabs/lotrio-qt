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

#include "settings.h"


Settings::Settings()
{
    // General: State & Geometry
    m_restoreApplicationState = true;
    m_restoreApplicationGeometry = true;
}


void Settings::load(QSettings &settings)
{
    settings.beginGroup(QStringLiteral("Settings"));

    // General: State & Geometry
    setRestoreApplicationState(settings.value(QStringLiteral("RestoreApplicationState"), true).toBool());
    setRestoreApplicationGeometry(settings.value(QStringLiteral("RestoreApplicationGeometry"), true).toBool());

    settings.endGroup();
}


void Settings::save(QSettings &settings)
{
    settings.beginGroup(QStringLiteral("Settings"));
    settings.remove("");

    // General: State & Geometry
    settings.setValue(QStringLiteral("RestoreApplicationState"), m_restoreApplicationState);
    settings.setValue(QStringLiteral("RestoreApplicationGeometry"), m_restoreApplicationGeometry);

    settings.endGroup();
}


void Settings::setRestoreApplicationState(bool value)
{
    m_restoreApplicationState = value;
}


bool Settings::restoreApplicationState(bool isDefault)
{
    if (isDefault)
        return true;

    return m_restoreApplicationState;
}


void Settings::setRestoreApplicationGeometry(bool value)
{
    m_restoreApplicationGeometry = value;
}


bool Settings::restoreApplicationGeometry(bool isDefault)
{
    if (isDefault)
        return true;

    return m_restoreApplicationGeometry;
}
