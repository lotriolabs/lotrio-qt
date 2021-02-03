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

#include "main_window.h"

#include <QApplication>
#include <QMenuBar>
#include <QScreen>

#include "about_dialog.h"
#include "colophon_dialog.h"
#include "preferences_dialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowIcon(QIcon(QStringLiteral(":/icons/apps/512/lotrio.svg")));

    createActions();
    createMenus();

    readSettings();
}

MainWindow::~MainWindow()
{
}


void MainWindow::createActions()
{
    // Actions: Application
    m_actionAbout = new QAction(tr("About %1").arg(QApplication::applicationName()), this);
    m_actionAbout->setObjectName(QStringLiteral("actionAbout"));
    m_actionAbout->setIcon(QIcon(QStringLiteral(":/icons/apps/512/lotrio.svg")));
    m_actionAbout->setIconText(tr("About"));
    m_actionAbout->setToolTip(tr("Brief description of the application"));
    connect(m_actionAbout, &QAction::triggered, this, &MainWindow::onActionAboutTriggered);

    m_actionColophon = new QAction(tr("Colophon"), this);
    m_actionColophon->setObjectName(QStringLiteral("actionColophon"));
    m_actionColophon->setToolTip(tr("Lengthy description of the application"));
    connect(m_actionColophon, &QAction::triggered, this, &MainWindow::onActionColophonTriggered);

    m_actionPreferences = new QAction(tr("Preferences…"), this);
    m_actionPreferences->setObjectName(QStringLiteral("actionPreferences"));
    m_actionPreferences->setIcon(QIcon::fromTheme(QStringLiteral("configure"), QIcon(QStringLiteral(":/icons/actions/16/application-configure.svg"))));
    m_actionPreferences->setIconText(tr("Preferences"));
    m_actionPreferences->setToolTip(tr("Customize the appearance and behavior of the application"));
    connect(m_actionPreferences, &QAction::triggered, this, &MainWindow::onActionPreferencesTriggered);

    m_actionQuit = new QAction(tr("Quit"), this);
    m_actionQuit->setObjectName(QStringLiteral("actionQuit"));
    m_actionQuit->setIcon(QIcon::fromTheme(QStringLiteral("application-exit"), QIcon(QStringLiteral(":/icons/actions/16/application-exit.svg"))));
    m_actionQuit->setIconText(tr("Quit"));
    m_actionQuit->setShortcut(QKeySequence::Quit);
    m_actionQuit->setToolTip(tr("Quit the application [%1]").arg(m_actionQuit->shortcut().toString(QKeySequence::NativeText)));
    connect(m_actionQuit, &QAction::triggered, this, &MainWindow::close);
}


void MainWindow::createMenus()
{
    // Menu: Application
    auto *menuApplication = menuBar()->addMenu(tr("Application"));
    menuApplication->setObjectName(QStringLiteral("menuApplication"));
    menuApplication->addAction(m_actionAbout);
    menuApplication->addAction(m_actionColophon);
    menuApplication->addSeparator();
    menuApplication->addAction(m_actionPreferences);
    menuApplication->addSeparator();
    menuApplication->addAction(m_actionQuit);
}


void MainWindow::setApplicationState(const QByteArray &state)
{
    if (!state.isEmpty()) {
        restoreState(state);
    }
}


QByteArray MainWindow::applicationState(bool isDefault) const
{
    if (isDefault)
        return QByteArray();

    return saveState();
}


void MainWindow::setApplicationGeometry(const QByteArray &geometry)
{
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    }
    else {
        const auto availableGeometry = screen()->availableGeometry();
        resize(availableGeometry.width() * 2/3, availableGeometry.height() * 2/3);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    }
}


QByteArray MainWindow::applicationGeometry(bool isDefault) const
{
    if (isDefault)
        return QByteArray();

    return saveGeometry();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if (true) {
        writeSettings();
        event->accept();
    }
    else {
        event->ignore();
    }
}


void MainWindow::readSettings()
{
    QSettings settings;

    m_settings.load(settings);

    // Application properties
    setApplicationState(settings.value(QStringLiteral("Application/State"), QByteArray()).toByteArray());
    setApplicationGeometry(settings.value(QStringLiteral("Application/Geometry"), QByteArray()).toByteArray());
    m_aboutDialogGeometry = settings.value(QStringLiteral("AboutDialog/Geometry"), QByteArray()).toByteArray();
    m_colophonDialogGeometry = settings.value(QStringLiteral("ColophonDialog/Geometry"), QByteArray()).toByteArray();
    m_preferencesDialogGeometry = settings.value(QStringLiteral("PreferencesDialog/Geometry"), QByteArray()).toByteArray();
}


void MainWindow::writeSettings()
{
    QSettings settings;

    m_settings.save(settings);

    // Application properties
    settings.setValue(QStringLiteral("Application/State"), applicationState(!m_settings.restoreApplicationState()));
    settings.setValue(QStringLiteral("Application/Geometry"), applicationGeometry(!m_settings.restoreApplicationGeometry()));
    settings.setValue(QStringLiteral("AboutDialog/Geometry"), m_aboutDialogGeometry);
    settings.setValue(QStringLiteral("ColophonDialog/Geometry"), m_colophonDialogGeometry);
    settings.setValue(QStringLiteral("PreferencesDialog/Geometry"), m_preferencesDialogGeometry);
}


void MainWindow::onActionAboutTriggered()
{
    AboutDialog dialog(this);
    dialog.setDialogGeometry(m_aboutDialogGeometry);
    dialog.exec();

    m_aboutDialogGeometry = dialog.dialogGeometry();
}


void MainWindow::onActionColophonTriggered()
{
    ColophonDialog dialog(this);
    dialog.setDialogGeometry(m_colophonDialogGeometry);
    dialog.exec();

    m_colophonDialogGeometry = dialog.dialogGeometry();
}


void MainWindow::onActionPreferencesTriggered()
{
    PreferencesDialog dialog(this);
    dialog.setDialogGeometry(m_preferencesDialogGeometry);
    dialog.setSettings(m_settings);
    dialog.exec();

    m_settings = dialog.settings();
    m_preferencesDialogGeometry = dialog.dialogGeometry();
}
