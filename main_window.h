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

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QAction>
#include <QByteArray>
#include <QCloseEvent>
#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QToolBar>

#include "document.h"
#include "preferences.h"


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

    bool openDocument(const QString &documentName);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onActionAboutTriggered();
    void onActionColophonTriggered();
    void onActionPreferencesTriggered();

    void onActionLotteriesToggled(const QString &lottery, bool checked);

    void onActionFullScreenTriggered();

    void onDocumentActivated();

private:
    Preferences m_preferences;

    QByteArray m_applicationState;
    QByteArray m_applicationGeometry;
    QByteArray m_aboutDialogGeometry;
    QByteArray m_colophonDialogGeometry;
    QByteArray m_preferencesDialogGeometry;

    void readSettings();
    void writeSettings();

    QMap<QString, QStringList> m_listLotteries;

    void createLotteries();

    QToolBar *m_toolbarApplication;
    QToolBar *m_toolbarLotteries;
    QToolBar *m_toolbarView;

    void createActions();
    void createMenus();
    void createToolBars();

    void updateActionFullScreen();

    QAction *m_actionAbout;
    QAction *m_actionColophon;
    QAction *m_actionPreferences;
    QAction *m_actionQuit;

    QList<QAction *> m_actionLotteries;

    QAction *m_actionFullScreen;
    QAction *m_actionToolbarApplication;
    QAction *m_actionToolbarLotteries;
    QAction *m_actionToolbarView;

    QMdiArea *m_documentArea;

    Document *createDocument();
    QMdiSubWindow *findDocument(const QString &documentName) const;

    bool loadDocument(const QString &documentName);
};

#endif // MAIN_WINDOW_H
