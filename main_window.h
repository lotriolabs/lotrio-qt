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

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QAction>
#include <QByteArray>
#include <QCloseEvent>
#include <QMainWindow>
#include <QToolBar>

#include "settings.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setApplicationState(const QByteArray &state = QByteArray());
    QByteArray applicationState() const;

    void setApplicationGeometry(const QByteArray &geometry = QByteArray());
    QByteArray applicationGeometry() const;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onActionAboutTriggered();
    void onActionColophonTriggered();
    void onActionPreferencesTriggered();

    void onActionLotteriesToggled(const QString &lottery, bool checked);

    void onActionFullScreenTriggered();

private:
    Settings m_settings;
    void readSettings();
    void writeSettings();

    QByteArray m_aboutDialogGeometry;
    QByteArray m_colophonDialogGeometry;
    QByteArray m_preferencesDialogGeometry;

    void createLotteries();
    QMap<QString, QStringList> m_listLotteries;

    void createActions();
    void createMenus();
    void createToolBars();

    void updateActionFullScreen();

    QToolBar *m_toolbarApplication;
    QToolBar *m_toolbarView;

    QAction *m_actionAbout;
    QAction *m_actionColophon;
    QAction *m_actionPreferences;
    QAction *m_actionQuit;

    QList<QAction *> m_actionLotteries;

    QAction *m_actionFullScreen;
    QAction *m_actionToolbarApplication;
    QAction *m_actionToolbarView;
};

#endif // MAIN_WINDOW_H
