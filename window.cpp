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

#include "window.h"

#include <QApplication>
#include <QMenuBar>
#include <QScreen>
#include <QSettings>

#include "about_dialog.h"
#include "colophon_dialog.h"
#include "preferences_dialog.h"


Window::Window(QWidget *parent)
    : QMainWindow(parent)
    , m_windowArea(new WindowArea)
{
    setWindowIcon(QIcon(QStringLiteral(":/icons/apps/512/lotrio.svg")));

    m_preferences.loadSettings();

    m_windowArea->setViewMode(WindowArea::TabbedView);
    m_windowArea->setTabsMovable(true);
    m_windowArea->setTabsClosable(true);
    m_windowArea->setTabPosition(m_preferences.defaultTabbarLotteriesPosition());
    connect(m_windowArea, &WindowArea::subWindowActivated, this, &Window::onDocumentWindowActivated);

    createLotteries();

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    loadSettings();

    updateActionFullScreen();
    updateActionsTabPositionLotteries();

    enableUiElements();

    // Central widget
    setCentralWidget(m_windowArea);
}

Window::~Window()
{
}


void Window::closeEvent(QCloseEvent *event)
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


void Window::loadSettings()
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


void Window::saveSettings()
{
    QSettings settings;

    // Application properties: Geometry
    const auto geometry = m_preferences.restoreApplicationGeometry() ? saveGeometry() : QByteArray();
    settings.setValue(QStringLiteral("Application/Geometry"), geometry);

    // Application properties: State
    const auto state = m_preferences.restoreApplicationState() ? saveState() : QByteArray();
    settings.setValue(QStringLiteral("Application/State"), state);
}


void Window::createLotteries()
{
    m_listLotteries[QStringLiteral("eurojackpot")] = QStringList() << QStringLiteral("euEurojackpot") << tr("Eurojackpot") << tr("Eurojackpot is a transnational European lottery");
    m_listLotteries[QStringLiteral("euromillions")] = QStringList() << QStringLiteral("euEuroMillions") << tr("EuroMillions") << tr("EuroMillions is a transnational European lottery");
    m_listLotteries[QStringLiteral("vikinglotto")] = QStringList() << QStringLiteral("euVikinglotto") << tr("Vikinglotto") << tr("Vikinglotto is a transnational European lottery");
}


void Window::createActions()
{
    //
    // Actions: Application

    m_actionAbout = new QAction(tr("About %1").arg(QApplication::applicationName()), this);
    m_actionAbout->setObjectName(QStringLiteral("actionAbout"));
    m_actionAbout->setIcon(QIcon(QStringLiteral(":/icons/apps/512/lotrio.svg")));
    m_actionAbout->setIconText(tr("About"));
    m_actionAbout->setToolTip(tr("Brief description of the application"));
    connect(m_actionAbout, &QAction::triggered, this, &Window::onActionAboutTriggered);

    m_actionColophon = new QAction(tr("Colophon"), this);
    m_actionColophon->setObjectName(QStringLiteral("actionColophon"));
    m_actionColophon->setToolTip(tr("Lengthy description of the application"));
    connect(m_actionColophon, &QAction::triggered, this, &Window::onActionColophonTriggered);

    m_actionPreferences = new QAction(tr("Preferences…"), this);
    m_actionPreferences->setObjectName(QStringLiteral("actionPreferences"));
    m_actionPreferences->setIcon(QIcon::fromTheme(QStringLiteral("configure"), QIcon(QStringLiteral(":/icons/actions/16/application-configure.svg"))));
    m_actionPreferences->setToolTip(tr("Customize the appearance and behavior of the application"));
    connect(m_actionPreferences, &QAction::triggered, this, &Window::onActionPreferencesTriggered);

    m_actionQuit = new QAction(tr("Quit"), this);
    m_actionQuit->setObjectName(QStringLiteral("actionQuit"));
    m_actionQuit->setIcon(QIcon::fromTheme(QStringLiteral("application-exit"), QIcon(QStringLiteral(":/icons/actions/16/application-exit.svg"))));
    m_actionQuit->setShortcut(QKeySequence::Quit);
    m_actionQuit->setToolTip(tr("Quit the application"));
    connect(m_actionQuit, &QAction::triggered, this, &Window::close);


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
    connect(m_actionClose, &QAction::triggered, m_windowArea, &WindowArea::closeActiveSubWindow);

    m_actionCloseOther = new QAction(tr("Close Other"), this);
    m_actionCloseOther->setObjectName(QStringLiteral("actionCloseOther"));
    m_actionCloseOther->setToolTip(tr("Close all other lotteries"));
    connect(m_actionCloseOther, &QAction::triggered, m_windowArea, &WindowArea::closeOtherSubWindows);

    m_actionCloseAll = new QAction(tr("Close All"), this);
    m_actionCloseAll->setObjectName(QStringLiteral("actionCloseAll"));
    m_actionCloseAll->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_W));
    m_actionCloseAll->setToolTip(tr("Close all lotteries"));
    connect(m_actionCloseAll, &QAction::triggered, m_windowArea, &WindowArea::closeAllSubWindows);


    //
    // Actions: View

    m_actionFullScreen = new QAction(this);
    m_actionFullScreen->setObjectName(QStringLiteral("actionFullScreen"));
    m_actionFullScreen->setIconText(tr("Full Screen"));
    m_actionFullScreen->setCheckable(true);
    m_actionFullScreen->setShortcuts(QList<QKeySequence>() << QKeySequence(Qt::Key_F11) << QKeySequence::FullScreen);
    connect(m_actionFullScreen, &QAction::triggered, this, &Window::onActionFullScreenTriggered);

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

    m_actionStatusbar = new QAction(tr("Show Statusbar"), this);
    m_actionStatusbar->setObjectName(QStringLiteral("actionStatusbar"));
    m_actionStatusbar->setCheckable(true);
    m_actionStatusbar->setChecked(true);
    m_actionStatusbar->setToolTip(tr("Display the statusbar"));
    connect(m_actionStatusbar, &QAction::toggled, [=](bool checked) { m_statusbar->setVisible(checked); });


    //
    // Actions: Help

    m_actionKeyboardShortcuts = new QAction(tr("Keyboard Shortcuts"), this);
    m_actionKeyboardShortcuts->setObjectName(QStringLiteral("actionKeyboardShortcuts"));
    m_actionKeyboardShortcuts->setIcon(QIcon::fromTheme(QStringLiteral("help-keyboard-shortcuts"), QIcon(QStringLiteral(":/icons/actions/16/help-keyboard-shortcuts.svg"))));
    m_actionKeyboardShortcuts->setIconText(tr("Shortcuts"));
    m_actionKeyboardShortcuts->setToolTip(tr("List of all keyboard shortcuts"));
    connect(m_actionKeyboardShortcuts, &QAction::triggered, this, &Window::onActionKeyboardShortcutsTriggered);


    //
    // Tab Position Lotteries

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

    m_actionsTabPositionLotteries = new QActionGroup(this);
    m_actionsTabPositionLotteries->setObjectName(QStringLiteral("actionsTabPositionLotteries"));
    m_actionsTabPositionLotteries->addAction(actionTabPositionLotteriesTop);
    m_actionsTabPositionLotteries->addAction(actionTabPositionLotteriesBottom);
    connect(m_actionsTabPositionLotteries, &QActionGroup::triggered, this, &Window::onActionsTabPositionLotteriesTriggered);


    //
    // Tab Position Sheets

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

    m_actionsTabPositionSheets = new QActionGroup(this);
    m_actionsTabPositionSheets->setObjectName(QStringLiteral("actionsTabPositionSheets"));
    m_actionsTabPositionSheets->addAction(actionTabPositionSheetsTop);
    m_actionsTabPositionSheets->addAction(actionTabPositionSheetsBottom);
    connect(m_actionsTabPositionSheets, &QActionGroup::triggered, this, &Window::onActionsTabPositionSheetsTriggered);
}


void Window::createMenus()
{
    auto *menuLotteryTabs = new QMenu(tr("Show Lottery Tabs…"), this);
    menuLotteryTabs->setObjectName(QStringLiteral("menuLotteryTabs"));
    menuLotteryTabs->addActions(m_actionsTabPositionLotteries->actions());

    m_menuSheetTabs = new QMenu(tr("Show Sheet Tabs…"), this);
    m_menuSheetTabs->setObjectName(QStringLiteral("menuSheetTabs"));
    m_menuSheetTabs->addActions(m_actionsTabPositionSheets->actions());


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

    // Menu: View
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
    menuView->addSeparator();
    menuView->addAction(m_actionStatusbar);

    // Menu: Help
    auto *menuHelp = menuBar()->addMenu(tr("Help"));
    menuHelp->setObjectName(QStringLiteral("menuHelp"));
    menuHelp->addAction(m_actionKeyboardShortcuts);
}


void Window::createToolBars()
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


void Window::createStatusBar()
{
    m_statusbar = statusBar();
}


void Window::updateActionFullScreen()
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


void Window::updateActionsTabPositionLotteries()
{
    const QList<QAction *> actions = m_actionsTabPositionLotteries->actions();
    for (auto *action : actions) {
        if (action->data() == m_preferences.defaultTabbarLotteriesPosition()) {
            action->setChecked(true);
            break;
        }
    }
}


void Window::updateActionsTabPositionSheets(const QTabWidget::TabPosition tabPosition)
{
    const QList<QAction *> actions = m_actionsTabPositionSheets->actions();
    for (auto *action : actions) {
        if (action->data() == tabPosition) {
            action->setChecked(true);
            break;
        }
    }
}


void Window::updateTitleBar()
{
    QString title;

    if (auto *document = activeDocument())
        title = document->documentTitle();

    setWindowTitle(title);
}


void Window::enableUiElements(const int subWindowCount)
{
    const bool hasDocument = subWindowCount >= 1;
    const bool hasDocuments = subWindowCount >= 2;

    // Actions: Lotteries
    m_actionClose->setEnabled(hasDocument);
    m_actionCloseOther->setEnabled(hasDocuments);
    m_actionCloseAll->setEnabled(hasDocument);

    // Menu: View
    m_menuSheetTabs->setEnabled(hasDocument);
}


void Window::onActionAboutTriggered()
{
    AboutDialog dialog(this);
    dialog.exec();
}


void Window::onActionColophonTriggered()
{
    ColophonDialog dialog(this);
    dialog.exec();
}


void Window::onActionPreferencesTriggered()
{
    PreferencesDialog dialog(this);
    dialog.setPreferences(m_preferences);
    dialog.exec();

    m_preferences = dialog.preferences();
}


void Window::onActionLotteriesToggled(bool checked, const QString &lottery)
{
    if (checked)
        openDocument(lottery);
    else
        closeDocument(lottery);
}


void Window::onActionFullScreenTriggered()
{
    if (!isFullScreen())
        setWindowState(windowState() | Qt::WindowFullScreen);
    else
        setWindowState(windowState() & ~Qt::WindowFullScreen);

    updateActionFullScreen();
}


void Window::onActionsTabPositionLotteriesTriggered(const QAction *actionTabPositionLotteries)
{
    auto tabPosition = static_cast<QTabWidget::TabPosition> (actionTabPositionLotteries->data().toInt());

    m_windowArea->setTabPosition(tabPosition);
}


void Window::onActionsTabPositionSheetsTriggered(const QAction *actionTabPositionSheets)
{
    auto tabPosition = static_cast<QTabWidget::TabPosition> (actionTabPositionSheets->data().toInt());

    if (auto *document = activeDocument())
        document->setTabPosition(tabPosition);
}


void Window::onActionKeyboardShortcutsTriggered()
{
    if (!m_keyboardShortcutsDialog)
        m_keyboardShortcutsDialog = new KeyboardShortcutsDialog(this);

    m_keyboardShortcutsDialog->show();
    m_keyboardShortcutsDialog->raise();
    m_keyboardShortcutsDialog->activateWindow();
}


void Window::onDocumentWindowActivated(const QMdiSubWindow *subWindow)
{
    // Update application window and UI elements
    updateTitleBar();
    enableUiElements(m_windowArea->subWindowList().count());

    if (!subWindow)
        return;

    auto *document = qobject_cast<LotteryDocument *>(subWindow->widget());

    updateActionsTabPositionSheets(document->tabPosition());
}


void Window::onDocumentAboutToClose(const QString &canonicalName)
{
    // Workaround to show subwindows always maximized
    const QList<QMdiSubWindow *> subWindows = m_windowArea->subWindowList();
    for (auto *subWindow : subWindows) {
        if (!subWindow->isMaximized())
            subWindow->showMaximized();
    }

    // Update UI elements without the emitter
    enableUiElements(m_windowArea->subWindowList().count() - 1);

    // Disable the Lottery action
    const QList<QAction *> actionLotteries = m_actionLotteries;
    for (auto *actionLottery : actionLotteries) {
        if (actionLottery->data().toString() == canonicalName) {
            actionLottery->setChecked(false);
            return;
        }
    }
}


LotteryDocument *Window::createDocument()
{
    auto *document = new LotteryDocument;
    document->setPreferences(m_preferences);
    document->setTabPosition(m_preferences.defaultTabbarSheetsPosition());
    connect(document, &LotteryDocument::aboutToClose, this, &Window::onDocumentAboutToClose);

    auto *subWindow = m_windowArea->addSubWindow(document);
    subWindow->setWindowIcon(QIcon());
    subWindow->showMaximized();

    return document;
}


QMdiSubWindow *Window::findDocumentWindow(const QString &canonicalName) const
{
    const QList<QMdiSubWindow *> subWindows = m_windowArea->subWindowList();
    for (auto *subWindow : subWindows) {

        auto *document = qobject_cast<LotteryDocument *>(subWindow->widget());
        if (document->canonicalName() == canonicalName)
            return subWindow;
    }

    return nullptr;
}


LotteryDocument *Window::activeDocument() const
{
    if (auto *subWindow = m_windowArea->activeSubWindow())
        return qobject_cast<LotteryDocument *>(subWindow->widget());

    return nullptr;
}


bool Window::openDocument(const QString &canonicalName)
{
    if (auto *subWindow = findDocumentWindow(canonicalName)) {
        // Given document is already loaded; activate the subwindow
        m_windowArea->setActiveSubWindow(subWindow);
        return true;
    }

    return loadDocument(canonicalName);
}


bool Window::loadDocument(const QString &canonicalName)
{
    auto *document = createDocument();

    const bool succeeded = document->load(canonicalName);
    if (succeeded) {
        document->updateDocumentTitle();
        document->show();

        // Update application window
        updateTitleBar();
    }
    else {
        document->close();
    }

    return succeeded;
}


bool Window::closeDocument(const QString &canonicalName)
{
    bool succeeded{ false };

    if (auto *subWindow = findDocumentWindow(canonicalName))
        succeeded = subWindow->close();

    return succeeded;
}
