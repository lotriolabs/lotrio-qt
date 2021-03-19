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

    m_preferences.load();

    createLotteries();

    createActions();
    createMenus();
    createToolBars();

    loadSettings();

    updateActions();
    updateActionFullScreen();

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
        saveSettings();
        m_preferences.save();
        event->accept();
    }
    else {
        event->ignore();
    }
}


void MainWindow::loadSettings()
{
    QSettings settings;

    // Application properties
    const auto state = m_preferences.restoreApplicationState() ? settings.value(QStringLiteral("Application/State"), QByteArray()).toByteArray() : QByteArray();
    if (!state.isEmpty()) {
        restoreState(state);
    }
    else {
        m_toolbarApplication->setVisible(true);
        m_toolbarLotteries->setVisible(true);
        m_toolbarView->setVisible(false);
        m_toolbarHelp->setVisible(false);
    }

    const auto geometry = m_preferences.restoreApplicationGeometry() ? settings.value(QStringLiteral("Application/Geometry"), QByteArray()).toByteArray() : QByteArray();
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    }
    else {
        const auto availableGeometry = screen()->availableGeometry();
        resize(availableGeometry.width() * 2/3, availableGeometry.height() * 2/3);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    }
}


void MainWindow::saveSettings()
{
    QSettings settings;

    // Application properties
    const auto state = m_preferences.restoreApplicationState() ? saveState() : QByteArray();
    settings.setValue(QStringLiteral("Application/State"), state);

    const auto geometry = m_preferences.restoreApplicationGeometry() ? saveGeometry() : QByteArray();
    settings.setValue(QStringLiteral("Application/Geometry"), geometry);
}


void MainWindow::createLotteries()
{
    m_listLotteries[QStringLiteral("eurojackpot")] = QStringList() << QStringLiteral("euEurojackpot") << tr("Eurojackpot") << tr("Eurojackpot is a transnational European lottery");
    m_listLotteries[QStringLiteral("euromillions")] = QStringList() << QStringLiteral("euEuroMillions") << tr("EuroMillions") << tr("EuroMillions is a transnational European lottery");
    m_listLotteries[QStringLiteral("vikinglotto")] = QStringList() << QStringLiteral("euVikinglotto") << tr("Vikinglotto") << tr("Vikinglotto is a transnational European lottery");
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
    m_actionPreferences->setToolTip(tr("Customize the appearance and behavior of the application"));
    connect(m_actionPreferences, &QAction::triggered, this, &MainWindow::onActionPreferencesTriggered);

    m_actionQuit = new QAction(tr("Quit"), this);
    m_actionQuit->setObjectName(QStringLiteral("actionQuit"));
    m_actionQuit->setIcon(QIcon::fromTheme(QStringLiteral("application-exit"), QIcon(QStringLiteral(":/icons/actions/16/application-exit.svg"))));
    m_actionQuit->setShortcut(QKeySequence::Quit);
    m_actionQuit->setToolTip(tr("Quit the application"));
    connect(m_actionQuit, &QAction::triggered, this, &MainWindow::close);

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
        connect(actionLottery, &QAction::toggled, [=](bool checked) { onActionLotteriesToggled(actionLottery->data().toString(), checked); });

        m_actionLotteries << actionLottery;
    }

    m_actionClose = new QAction(tr("Close"), this);
    m_actionClose->setObjectName(QStringLiteral("actionClose"));
    m_actionClose->setIcon(QIcon::fromTheme(QStringLiteral("document-close"), QIcon(QStringLiteral(":/icons/actions/16/document-close.svg"))));
    m_actionClose->setShortcut(QKeySequence::Close);
    m_actionClose->setToolTip(tr("Close document"));
    connect(m_actionClose, &QAction::triggered, this, &MainWindow::onActionCloseTriggered);

    m_actionCloseOther = new QAction(tr("Close Other"), this);
    m_actionCloseOther->setObjectName(QStringLiteral("actionCloseOther"));
    m_actionCloseOther->setToolTip(tr("Close all other documents"));
    connect(m_actionCloseOther, &QAction::triggered, this, &MainWindow::onActionCloseOtherTriggered);

    m_actionCloseAll = new QAction(tr("Close All"), this);
    m_actionCloseAll->setObjectName(QStringLiteral("actionCloseAll"));
    m_actionCloseAll->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_W));
    m_actionCloseAll->setToolTip(tr("Close all documents"));
    connect(m_actionCloseAll, &QAction::triggered, this, &MainWindow::onActionCloseAllTriggered);

    // Actions: View
    m_actionFullScreen = new QAction(this);
    m_actionFullScreen->setObjectName(QStringLiteral("actionFullScreen"));
    m_actionFullScreen->setIconText(tr("Full Screen"));
    m_actionFullScreen->setCheckable(true);
    m_actionFullScreen->setShortcuts(QList<QKeySequence>() << QKeySequence(Qt::Key_F11) << QKeySequence::FullScreen);
    connect(m_actionFullScreen, &QAction::triggered, this, &MainWindow::onActionFullScreenTriggered);

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

    // Menu: View
    auto *menuView = menuBar()->addMenu(tr("View"));
    menuView->setObjectName(QStringLiteral("menuView"));
    menuView->addAction(m_actionFullScreen);
    menuView->addSeparator();
    menuView->addAction(m_actionToolbarApplication);
    menuView->addAction(m_actionToolbarLotteries);
    menuView->addAction(m_actionToolbarView);
    menuView->addAction(m_actionToolbarHelp);

    // Menu: Help
    auto *menuHelp = menuBar()->addMenu(tr("Help"));
    menuHelp->setObjectName(QStringLiteral("menuHelp"));
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

    // Toolbar: View
    m_toolbarView = addToolBar(tr("View Toolbar"));
    m_toolbarView->setObjectName(QStringLiteral("toolbarView"));
    m_toolbarView->addAction(m_actionFullScreen);
    connect(m_toolbarView, &QToolBar::visibilityChanged, [=](bool visible) { m_actionToolbarView->setChecked(visible); });

    // Toolbar: Help
    m_toolbarHelp = addToolBar(tr("Help Toolbar"));
    m_toolbarHelp->setObjectName(QStringLiteral("toolbarHelp"));
    connect(m_toolbarHelp, &QToolBar::visibilityChanged, [=](bool visible) { m_actionToolbarHelp->setChecked(visible); });
}


void MainWindow::updateActions(const int windowCount)
{
    bool hasDocument = windowCount >= 1;
    bool hasDocuments = windowCount >= 2;

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


void MainWindow::updateTitleBar()
{
    QString title;

    if (auto *document = activeDocument())
        title = document->documentTitle();

    setWindowTitle(title);
}


void MainWindow::onActionAboutTriggered()
{
    AboutDialog dialog(m_preferences.restoreDialogGeometry(), this);
    dialog.exec();
}


void MainWindow::onActionColophonTriggered()
{
    ColophonDialog dialog(m_preferences.restoreDialogGeometry(), this);
    dialog.exec();
}


void MainWindow::onActionPreferencesTriggered()
{
    PreferencesDialog dialog(m_preferences.restoreDialogGeometry(), this);
    dialog.setPreferences(m_preferences);
    dialog.exec();

    m_preferences = dialog.preferences();
}


void MainWindow::onActionLotteriesToggled(const QString &lottery, bool checked)
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
    const QList<QMdiSubWindow *> windows = m_documentArea->subWindowList();
    for (auto *window : windows)
        if (window != m_documentArea->activeSubWindow())
            window->close();
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


void MainWindow::onDocumentWindowActivated(const QMdiSubWindow *window)
{
    updateActions(m_documentArea->subWindowList().count());
    updateTitleBar();

    if (!window)
        return;


}


void MainWindow::onDocumentAboutToClose(const QString &canonicalName)
{
    // Update menu items; delete the emitter from the list
    updateActions(m_documentArea->subWindowList().count() - 1);

    // Update lottery button
    foreach (auto *actionLottery, m_actionLotteries) {
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

    auto *window = m_documentArea->addSubWindow(document);
    window->setWindowIcon(QIcon());
    window->showMaximized();

    return document;
}


QMdiSubWindow *MainWindow::findDocumentWindow(const QString &canonicalName) const
{
    const QList<QMdiSubWindow *> windows = m_documentArea->subWindowList();
    for (auto *window : windows) {

        auto *document = qobject_cast<Document *>(window->widget());
        if (document->canonicalName() == canonicalName)
            return window;
    }

    return nullptr;
}


Document *MainWindow::activeDocument() const
{
    if (auto *window = m_documentArea->activeSubWindow())
           return qobject_cast<Document *>(window->widget());

    return nullptr;
}


bool MainWindow::openDocument(const QString &canonicalName)
{
    if (auto *window = findDocumentWindow(canonicalName)) {
        // Given document is already open; activate the window
        m_documentArea->setActiveSubWindow(window);
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

        // Update application
        updateActions(m_documentArea->subWindowList().count());
        updateTitleBar();
    }
    else {
        document->close();
    }

    return succeeded;
}


bool MainWindow::closeDocument(const QString &canonicalName)
{
    bool succeeded = false;

    if (auto *window = findDocumentWindow(canonicalName))
        succeeded = window->close();

    return succeeded;
}
