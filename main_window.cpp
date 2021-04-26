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

#include "main_window.h"

#include <QApplication>
#include <QDebug>
#include <QMenuBar>
#include <QScreen>
#include <QSettings>

#include "about_dialog.h"
#include "colophon_dialog.h"
#include "preferences_dialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_documentArea(new QMdiArea)
{
    setWindowIcon(QIcon(QStringLiteral(":/icons/apps/512/lotrio.svg")));

    m_preferences.loadSettings();

    createLotteries();

    createActions();
    createMenus();
    createToolBars();

    loadSettings();

    updateActions();
    updateActionFullScreen();
    updateActionTabPositionLotteries();
    updateActionTabPositionSheets(m_preferences.defaultTabPositionSheets());
    updateMenus();

    // Central widget
    m_documentArea->setViewMode(QMdiArea::TabbedView);
    m_documentArea->setTabsMovable(true);
    m_documentArea->setTabsClosable(true);
    setCentralWidget(m_documentArea);
    connect(m_documentArea, &QMdiArea::subWindowActivated, this, &MainWindow::onDocumentWindowActivated);
}

MainWindow::~MainWindow()
{
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if (true) {
        // Store application properties and preferences
        saveSettings();
        m_preferences.saveSettings();

        event->accept();
    }
    else {
        event->ignore();
    }
}


void MainWindow::loadSettings()
{
    QSettings settings;

    // Application properties: Geometry
    const auto geometry = m_preferences.restoreApplicationGeometry() ? settings.value(QStringLiteral("Application/Geometry"), QByteArray()).toByteArray() : QByteArray();
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    }
    else {
        const auto availableGeometry = screen()->availableGeometry();
        resize(availableGeometry.width() * 2/3, availableGeometry.height() * 2/3);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    }

    // Application properties: State
    const auto state = m_preferences.restoreApplicationState() ? settings.value(QStringLiteral("Application/State"), QByteArray()).toByteArray() : QByteArray();
    if (!state.isEmpty()) {
        restoreState(state);
    }
    else {
        m_toolbarApplication->setVisible(true);
        m_toolbarLotteries->setVisible(true);
        m_toolbarTools->setVisible(true);
        m_toolbarView->setVisible(false);
        m_toolbarHelp->setVisible(false);
    }
}


void MainWindow::saveSettings()
{
    QSettings settings;

    // Application properties: Geometry
    const auto geometry = m_preferences.restoreApplicationGeometry() ? saveGeometry() : QByteArray();
    settings.setValue(QStringLiteral("Application/Geometry"), geometry);

    // Application properties: State
    const auto state = m_preferences.restoreApplicationState() ? saveState() : QByteArray();
    settings.setValue(QStringLiteral("Application/State"), state);
}


void MainWindow::createLotteries()
{
    m_listLotteries[QStringLiteral("eurojackpot")] = QStringList() << QStringLiteral("euEurojackpot") << tr("Eurojackpot") << tr("Eurojackpot is a transnational European lottery");
    m_listLotteries[QStringLiteral("euromillions")] = QStringList() << QStringLiteral("euEuroMillions") << tr("EuroMillions") << tr("EuroMillions is a transnational European lottery");
    m_listLotteries[QStringLiteral("vikinglotto")] = QStringList() << QStringLiteral("euVikinglotto") << tr("Vikinglotto") << tr("Vikinglotto is a transnational European lottery");
}


void MainWindow::createActions()
{
    //
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
    m_actionPreferences->setToolTip(tr("Customize the appearance and behavior of the application"));
    connect(m_actionPreferences, &QAction::triggered, this, &MainWindow::onActionPreferencesTriggered);

    m_actionQuit = new QAction(tr("Quit"), this);
    m_actionQuit->setObjectName(QStringLiteral("actionQuit"));
    m_actionQuit->setIcon(QIcon::fromTheme(QStringLiteral("application-exit"), QIcon(QStringLiteral(":/icons/actions/16/application-exit.svg"))));
    m_actionQuit->setShortcut(QKeySequence::Quit);
    m_actionQuit->setToolTip(tr("Quit the application"));
    connect(m_actionQuit, &QAction::triggered, this, &MainWindow::close);

    //
    // Actions: Lotteries

    QMapIterator<QString, QStringList> it(m_listLotteries);
    while (it.hasNext()) {
        it.next();

        auto *actionLottery = new QAction(it.value()[1], this);
        actionLottery->setObjectName(QStringLiteral("actionLottery_%1").arg(it.value()[0]));
        actionLottery->setIconText(it.value()[1]);
        actionLottery->setCheckable(true);
        actionLottery->setToolTip(it.value()[2]);
        actionLottery->setData(QStringLiteral("%1/%2").arg(it.key(), it.value()[1]));
        connect(actionLottery, &QAction::toggled, [=](bool checked) { onActionLotteriesToggled(checked, actionLottery->data().toString()); });

        m_actionLotteries << actionLottery;
    }

    m_actionClose = new QAction(tr("Close"), this);
    m_actionClose->setObjectName(QStringLiteral("actionClose"));
    m_actionClose->setIcon(QIcon::fromTheme(QStringLiteral("document-close"), QIcon(QStringLiteral(":/icons/actions/16/document-close.svg"))));
    m_actionClose->setShortcut(QKeySequence::Close);
    m_actionClose->setToolTip(tr("Close lottery"));
    connect(m_actionClose, &QAction::triggered, this, &MainWindow::onActionCloseTriggered);

    m_actionCloseOther = new QAction(tr("Close Other"), this);
    m_actionCloseOther->setObjectName(QStringLiteral("actionCloseOther"));
    m_actionCloseOther->setToolTip(tr("Close all other lotteries"));
    connect(m_actionCloseOther, &QAction::triggered, this, &MainWindow::onActionCloseOtherTriggered);

    m_actionCloseAll = new QAction(tr("Close All"), this);
    m_actionCloseAll->setObjectName(QStringLiteral("actionCloseAll"));
    m_actionCloseAll->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_W));
    m_actionCloseAll->setToolTip(tr("Close all lotteries"));
    connect(m_actionCloseAll, &QAction::triggered, this, &MainWindow::onActionCloseAllTriggered);

    //
    // Actions: View

    m_actionFullScreen = new QAction(this);
    m_actionFullScreen->setObjectName(QStringLiteral("actionFullScreen"));
    m_actionFullScreen->setIconText(tr("Full Screen"));
    m_actionFullScreen->setCheckable(true);
    m_actionFullScreen->setShortcuts(QList<QKeySequence>() << QKeySequence(Qt::Key_F11) << QKeySequence::FullScreen);
    connect(m_actionFullScreen, &QAction::triggered, this, &MainWindow::onActionFullScreenTriggered);

    auto *actionTabPositionLotteriesTop = new QAction(tr("Top"), this);
    actionTabPositionLotteriesTop->setObjectName(QStringLiteral("actionTabPositionLotteriesTop"));
    actionTabPositionLotteriesTop->setCheckable(true);
    actionTabPositionLotteriesTop->setToolTip(tr("The lottery tabs are displayed above the pages"));
    actionTabPositionLotteriesTop->setData((int) QTabWidget::North);

    auto *actionTabPositionLotteriesBottom = new QAction(tr("Bottom"), this);
    actionTabPositionLotteriesBottom->setObjectName(QStringLiteral("actionTabPositionLotteriesBottom"));
    actionTabPositionLotteriesBottom->setCheckable(true);
    actionTabPositionLotteriesBottom->setToolTip(tr("The lottery tabs are displayed below the pages"));
    actionTabPositionLotteriesBottom->setData((int) QTabWidget::South);

    m_actionTabPositionLotteries = new QActionGroup(this);
    m_actionTabPositionLotteries->setObjectName(QStringLiteral("actionTabPositionLotteries"));
    m_actionTabPositionLotteries->addAction(actionTabPositionLotteriesTop);
    m_actionTabPositionLotteries->addAction(actionTabPositionLotteriesBottom);
    connect(m_actionTabPositionLotteries, &QActionGroup::triggered, this, &MainWindow::onActionTabPositionLotteriesTriggered);

    auto *actionTabPositionSheetsTop = new QAction(tr("Top"), this);
    actionTabPositionSheetsTop->setObjectName(QStringLiteral("actionTabPositionSheetsTop"));
    actionTabPositionSheetsTop->setCheckable(true);
    actionTabPositionSheetsTop->setToolTip(tr("The sheet tabs are displayed above the pages"));
    actionTabPositionSheetsTop->setData((int) QTabWidget::North);

    auto *actionTabPositionSheetsBottom = new QAction(tr("Bottom"), this);
    actionTabPositionSheetsBottom->setObjectName(QStringLiteral("actionTabPositionSheetsBottom"));
    actionTabPositionSheetsBottom->setCheckable(true);
    actionTabPositionSheetsBottom->setToolTip(tr("The sheet tabs are displayed below the pages"));
    actionTabPositionSheetsBottom->setData((int) QTabWidget::South);

    m_actionTabPositionSheets = new QActionGroup(this);
    m_actionTabPositionSheets->setObjectName(QStringLiteral("actionTabPositionSheets"));
    m_actionTabPositionSheets->addAction(actionTabPositionSheetsTop);
    m_actionTabPositionSheets->addAction(actionTabPositionSheetsBottom);
    connect(m_actionTabPositionSheets, &QActionGroup::triggered, this, &MainWindow::onActionTabPositionSheetsTriggered);

    m_actionToolbarApplication = new QAction(tr("Show Application Toolbar"), this);
    m_actionToolbarApplication->setObjectName(QStringLiteral("actionToolbarApplication"));
    m_actionToolbarApplication->setCheckable(true);
    m_actionToolbarApplication->setToolTip(tr("Display the Application toolbar"));
    connect(m_actionToolbarApplication, &QAction::toggled, [=](bool checked) { m_toolbarApplication->setVisible(checked); });

    m_actionToolbarLotteries = new QAction(tr("Show Lotteries Toolbar"), this);
    m_actionToolbarLotteries->setObjectName(QStringLiteral("actionToolbarLotteries"));
    m_actionToolbarLotteries->setCheckable(true);
    m_actionToolbarLotteries->setToolTip(tr("Display the Lotteries toolbar"));
    connect(m_actionToolbarLotteries, &QAction::toggled, [=](bool checked) { m_toolbarLotteries->setVisible(checked); });

    m_actionToolbarTools = new QAction(tr("Show Tools Toolbar"), this);
    m_actionToolbarTools->setObjectName(QStringLiteral("actionToolbarTools"));
    m_actionToolbarTools->setCheckable(true);
    m_actionToolbarTools->setToolTip(tr("Display the Tools toolbar"));
    connect(m_actionToolbarTools, &QAction::toggled, [=](bool checked) { m_toolbarTools->setVisible(checked); });

    m_actionToolbarView = new QAction(tr("Show View Toolbar"), this);
    m_actionToolbarView->setObjectName(QStringLiteral("actionToolbarView"));
    m_actionToolbarView->setCheckable(true);
    m_actionToolbarView->setToolTip(tr("Display the View toolbar"));
    connect(m_actionToolbarView, &QAction::toggled, [=](bool checked) { m_toolbarView->setVisible(checked); });

    m_actionToolbarHelp = new QAction(tr("Show Help Toolbar"), this);
    m_actionToolbarHelp->setObjectName(QStringLiteral("actionToolbarHelp"));
    m_actionToolbarHelp->setCheckable(true);
    m_actionToolbarHelp->setToolTip(tr("Display the Help toolbar"));
    connect(m_actionToolbarHelp, &QAction::toggled, [=](bool checked) { m_toolbarHelp->setVisible(checked); });

    //
    // Actions: Help

    m_actionKeyboardShortcuts = new QAction(tr("Keyboard Shortcuts"), this);
    m_actionKeyboardShortcuts->setObjectName(QStringLiteral("actionKeyboardShortcuts"));
    m_actionKeyboardShortcuts->setIcon(QIcon::fromTheme(QStringLiteral("help-keyboard-shortcuts"), QIcon(QStringLiteral(":/icons/actions/16/help-keyboard-shortcuts.svg"))));
    m_actionKeyboardShortcuts->setIconText(tr("Shortcuts"));
    m_actionKeyboardShortcuts->setToolTip(tr("List of all keyboard shortcuts"));
    connect(m_actionKeyboardShortcuts, &QAction::triggered, this, &MainWindow::onActionKeyboardShortcutsTriggered);
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

    // Menu: Lotteries
    auto *menuLotteries = menuBar()->addMenu(tr("Lotteries"));
    menuLotteries->setObjectName(QStringLiteral("menuLotteries"));
    menuLotteries->addActions(m_actionLotteries);
    menuLotteries->addSeparator();
    menuLotteries->addAction(m_actionClose);
    menuLotteries->addAction(m_actionCloseOther);
    menuLotteries->addAction(m_actionCloseAll);

    // Menu: Tools
    auto *menuTools = menuBar()->addMenu(tr("Tools"));
    menuTools->setObjectName(QStringLiteral("menuTools"));

    //
    // Menu: View

    auto *menuLotteryTabs = new QMenu(tr("Lottery Tabs…"), this);
    menuLotteryTabs->setObjectName(QStringLiteral("menuLotteryTabs"));
    menuLotteryTabs->addActions(m_actionTabPositionLotteries->actions());

    m_menuSheetTabs = new QMenu(tr("Sheet Tabs…"), this);
    m_menuSheetTabs->setObjectName(QStringLiteral("menuSheetTabs"));
    m_menuSheetTabs->addActions(m_actionTabPositionSheets->actions());

    auto *menuView = menuBar()->addMenu(tr("View"));
    menuView->setObjectName(QStringLiteral("menuView"));
    menuView->addAction(m_actionFullScreen);
    menuView->addSeparator();
    menuView->addMenu(menuLotteryTabs);
    menuView->addMenu(m_menuSheetTabs);
    menuView->addSeparator();
    menuView->addAction(m_actionToolbarApplication);
    menuView->addAction(m_actionToolbarLotteries);
    menuView->addAction(m_actionToolbarTools);
    menuView->addAction(m_actionToolbarView);
    menuView->addAction(m_actionToolbarHelp);

    // Menu: Help
    auto *menuHelp = menuBar()->addMenu(tr("Help"));
    menuHelp->setObjectName(QStringLiteral("menuHelp"));
    menuHelp->addAction(m_actionKeyboardShortcuts);
}


void MainWindow::createToolBars()
{
    // Toolbar: Application
    m_toolbarApplication = addToolBar(tr("Application Toolbar"));
    m_toolbarApplication->setObjectName(QStringLiteral("toolbarApplication"));
    m_toolbarApplication->addAction(m_actionAbout);
    m_toolbarApplication->addAction(m_actionPreferences);
    m_toolbarApplication->addSeparator();
    m_toolbarApplication->addAction(m_actionQuit);
    connect(m_toolbarApplication, &QToolBar::visibilityChanged, [=](bool visible) { m_actionToolbarApplication->setChecked(visible); });

    // Toolbar: Lotteries
    m_toolbarLotteries = addToolBar(tr("Lotteries Toolbar"));
    m_toolbarLotteries->setObjectName(QStringLiteral("toolbarLotteries"));
    m_toolbarLotteries->addActions(m_actionLotteries);
    connect(m_toolbarLotteries, &QToolBar::visibilityChanged, [=](bool visible) { m_actionToolbarLotteries->setChecked(visible); });

    // Toolbar: Tools
    m_toolbarTools = addToolBar(tr("Tools Toolbar"));
    m_toolbarTools->setObjectName(QStringLiteral("toolbarTools"));
    connect(m_toolbarTools, &QToolBar::visibilityChanged, [=](bool visible) { m_actionToolbarTools->setChecked(visible); });

    // Toolbar: View
    m_toolbarView = addToolBar(tr("View Toolbar"));
    m_toolbarView->setObjectName(QStringLiteral("toolbarView"));
    m_toolbarView->addAction(m_actionFullScreen);
    connect(m_toolbarView, &QToolBar::visibilityChanged, [=](bool visible) { m_actionToolbarView->setChecked(visible); });

    // Toolbar: Help
    m_toolbarHelp = addToolBar(tr("Help Toolbar"));
    m_toolbarHelp->setObjectName(QStringLiteral("toolbarHelp"));
    m_toolbarHelp->addAction(m_actionKeyboardShortcuts);
    connect(m_toolbarHelp, &QToolBar::visibilityChanged, [=](bool visible) { m_actionToolbarHelp->setChecked(visible); });
}


void MainWindow::updateActions(const int subWindowCount)
{
    const bool hasDocument = subWindowCount >= 1;
    const bool hasDocuments = subWindowCount >= 2;

    // Actions: Lotteries
    m_actionClose->setEnabled(hasDocument);
    m_actionCloseOther->setEnabled(hasDocuments);
    m_actionCloseAll->setEnabled(hasDocument);
}


void MainWindow::updateActionFullScreen()
{
    if (!isFullScreen()) {
        m_actionFullScreen->setText(tr("Full Screen Mode"));
        m_actionFullScreen->setIcon(QIcon::fromTheme(QStringLiteral("view-fullscreen"), QIcon(QStringLiteral(":/icons/actions/16/view-fullscreen.svg"))));
        m_actionFullScreen->setChecked(false);
        m_actionFullScreen->setToolTip(tr("Display the window in full screen"));
    }
    else {
        m_actionFullScreen->setText(tr("Exit Full Screen Mode"));
        m_actionFullScreen->setIcon(QIcon::fromTheme(QStringLiteral("view-restore"), QIcon(QStringLiteral(":/icons/actions/16/view-restore.svg"))));
        m_actionFullScreen->setChecked(true);
        m_actionFullScreen->setToolTip(tr("Exit the full screen mode"));
    }
}


void MainWindow::updateActionTabPositionLotteries()
{
    const QList<QAction *> actions = m_actionTabPositionLotteries->actions();
    for (auto *action : actions) {
        if (action->data() == m_preferences.defaultTabPositionLotteries()) {
            action->setChecked(true);
            break;
        }
    }
}


void MainWindow::updateActionTabPositionSheets(const QTabWidget::TabPosition tabPosition)
{
    const QList<QAction *> actions = m_actionTabPositionSheets->actions();
    for (auto *action : actions) {
        if (action->data() == tabPosition) {
            action->setChecked(true);
            break;
        }
    }
}


void MainWindow::updateMenus(const int subWindowCount)
{
    const bool hasDocument = subWindowCount >= 1;

    // Menu: View
    m_menuSheetTabs->setEnabled(hasDocument);
}


void MainWindow::updateTitleBar()
{
    QString title;

    if (auto *document = activeDocument())
        title = document->documentTitle();

    setWindowTitle(title);
}


void MainWindow::onActionAboutTriggered()
{
    AboutDialog dialog(this);
    dialog.exec();
}


void MainWindow::onActionColophonTriggered()
{
    ColophonDialog dialog(this);
    dialog.exec();
}


void MainWindow::onActionPreferencesTriggered()
{
    PreferencesDialog dialog(this);
    dialog.setPreferences(m_preferences);
    dialog.exec();

    m_preferences = dialog.preferences();
}


void MainWindow::onActionLotteriesToggled(bool checked, const QString &lottery)
{
    if (checked)
        openDocument(lottery);
    else
        closeDocument(lottery);
}


void MainWindow::onActionCloseTriggered()
{
    m_documentArea->closeActiveSubWindow();
}


void MainWindow::onActionCloseOtherTriggered()
{
    const QList<QMdiSubWindow *> subWindows = m_documentArea->subWindowList();
    for (auto *subWindow : subWindows) {
        if (subWindow != m_documentArea->activeSubWindow())
            subWindow->close();
    }
}


void MainWindow::onActionCloseAllTriggered()
{
    m_documentArea->closeAllSubWindows();
}


void MainWindow::onActionFullScreenTriggered()
{
    if (!isFullScreen())
        setWindowState(windowState() | Qt::WindowFullScreen);
    else
        setWindowState(windowState() & ~Qt::WindowFullScreen);

    updateActionFullScreen();
}


void MainWindow::onActionTabPositionLotteriesTriggered(const QAction *actionTabPositionLotteries)
{
    auto tabPosition = static_cast<QTabWidget::TabPosition> (actionTabPositionLotteries->data().toInt());

    m_documentArea->setTabPosition(tabPosition);
}


void MainWindow::onActionTabPositionSheetsTriggered(const QAction *actionTabPositionSheets)
{
    auto tabPosition = static_cast<QTabWidget::TabPosition> (actionTabPositionSheets->data().toInt());

    if (auto *document = activeDocument())
        document->setDocumentTabPosition(tabPosition);
}


void MainWindow::onActionKeyboardShortcutsTriggered()
{
    if (!m_keyboardShortcutsDialog)
        m_keyboardShortcutsDialog = new KeyboardShortcutsDialog(this);

    m_keyboardShortcutsDialog->show();
    m_keyboardShortcutsDialog->raise();
    m_keyboardShortcutsDialog->activateWindow();
}


void MainWindow::onDocumentWindowActivated(const QMdiSubWindow *subWindow)
{
    // Update the application window
    updateActions(m_documentArea->subWindowList().count());
    updateMenus(m_documentArea->subWindowList().count());
    updateTitleBar();

    if (!subWindow)
        return;

    auto *document = qobject_cast<Document *>(subWindow->widget());

    updateActionTabPositionSheets(document->documentTabPosition());
}


void MainWindow::onDocumentAboutToClose(const QString &canonicalName)
{
    // Workaround to show subwindows always maximized
    const QList<QMdiSubWindow *> subWindows = m_documentArea->subWindowList();
    for (auto *subWindow : subWindows) {
        if (!subWindow->isMaximized())
            subWindow->showMaximized();
    }

    // Update menu items without the emitter
    updateActions(m_documentArea->subWindowList().count() - 1);
    updateMenus(m_documentArea->subWindowList().count() - 1);

    // Disable the Lottery action
    const QList<QAction *> actionLotteries = m_actionLotteries;
    for (auto *actionLottery : actionLotteries) {
        if (actionLottery->data().toString() == canonicalName) {
            actionLottery->setChecked(false);
            return;
        }
    }
}


Document *MainWindow::createDocument()
{
    auto *document = new Document;
    document->setPreferences(m_preferences);
    connect(document, &Document::aboutToClose, this, &MainWindow::onDocumentAboutToClose);

    auto *subWindow = m_documentArea->addSubWindow(document);
    subWindow->setWindowIcon(QIcon());
    subWindow->showMaximized();

    return document;
}


QMdiSubWindow *MainWindow::findDocumentWindow(const QString &canonicalName) const
{
    const QList<QMdiSubWindow *> subWindows = m_documentArea->subWindowList();
    for (auto *subWindow : subWindows) {

        auto *document = qobject_cast<Document *>(subWindow->widget());
        if (document->canonicalName() == canonicalName)
            return subWindow;
    }

    return nullptr;
}


Document *MainWindow::activeDocument() const
{
    if (auto *subWindow = m_documentArea->activeSubWindow())
        return qobject_cast<Document *>(subWindow->widget());

    return nullptr;
}


bool MainWindow::openDocument(const QString &canonicalName)
{
    if (auto *subWindow = findDocumentWindow(canonicalName)) {
        // Given document is already loaded; activate the subwindow
        m_documentArea->setActiveSubWindow(subWindow);
        return true;
    }

    return loadDocument(canonicalName);
}


bool MainWindow::loadDocument(const QString &canonicalName)
{
    auto *document = createDocument();

    const bool succeeded = document->load(canonicalName);
    if (succeeded) {
        document->updateDocumentTitle();
        document->show();

        // Update the application window
        updateActions(m_documentArea->subWindowList().count());
        updateActionTabPositionSheets(document->documentTabPosition());
        updateMenus(m_documentArea->subWindowList().count());
        updateTitleBar();
    }
    else {
        document->close();
    }

    return succeeded;
}


bool MainWindow::closeDocument(const QString &canonicalName)
{
    bool succeeded{ false };

    if (auto *subWindow = findDocumentWindow(canonicalName))
        succeeded = subWindow->close();

    return succeeded;
}
