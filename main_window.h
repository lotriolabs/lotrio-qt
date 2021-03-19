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

    bool openDocument(const QString &canonicalName);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onActionAboutTriggered();
    void onActionColophonTriggered();
    void onActionPreferencesTriggered();

    void onActionLotteriesToggled(const QString &lottery, bool checked);
    void onActionCloseTriggered();
    void onActionCloseOtherTriggered();
    void onActionCloseAllTriggered();

    void onActionFullScreenTriggered();

    void onActionKeyboardShortcutsTriggered();

    void onDocumentWindowActivated(const QMdiSubWindow *window);
    void onDocumentAboutToClose(const QString &canonicalName);

private:
    Preferences m_preferences;

    void loadSettings();
    void saveSettings();

    QMap<QString, QStringList> m_listLotteries;

    void createLotteries();

    QToolBar *m_toolbarApplication;
    QToolBar *m_toolbarLotteries;
    QToolBar *m_toolbarView;
    QToolBar *m_toolbarHelp;

    void createActions();
    void createMenus();
    void createToolBars();

    void updateActions(const int windowCount = 0);
    void updateActionFullScreen();
    void updateTitleBar();

    QAction *m_actionAbout;
    QAction *m_actionColophon;
    QAction *m_actionPreferences;
    QAction *m_actionQuit;

    QList<QAction *> m_actionLotteries;
    QAction *m_actionClose;
    QAction *m_actionCloseOther;
    QAction *m_actionCloseAll;

    QAction *m_actionFullScreen;
    QAction *m_actionToolbarApplication;
    QAction *m_actionToolbarLotteries;
    QAction *m_actionToolbarView;
    QAction *m_actionToolbarHelp;

    QAction *m_actionKeyboardShortcuts;

    QMdiArea *m_documentArea;

    Document *createDocument();
    QMdiSubWindow *findDocumentWindow(const QString &canonicalName) const;
    Document *activeDocument() const;
    bool loadDocument(const QString &canonicalName);
    bool closeDocument(const QString &canonicalName);
};

#endif // MAIN_WINDOW_H
