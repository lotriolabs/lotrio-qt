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

#include "about_dialog.h"
#include "colophon_dialog.h"
#include "preferences_dialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_documentArea(new QMdiArea)
{
    setWindowIcon(QIcon(QStringLiteral(":/icons/apps/512/lotrio.svg")));

    readSettings();

    createLotteries();

    createActions();
    createMenus();
    createToolBars();

    // Application properties
    setApplicationState(m_applicationState);
    setApplicationGeometry(m_applicationGeometry);

    updateActionFullScreen();

    // Central widget
    m_documentArea->setViewMode(QMdiArea::TabbedView);
    m_documentArea->setTabsMovable(true);
    m_documentArea->setTabsClosable(true);
    setCentralWidget(m_documentArea);
    connect(m_documentArea, &QMdiArea::subWindowActivated, this, &MainWindow::onDocumentActivated);
}

MainWindow::~MainWindow()
{
}


void MainWindow::setApplicationState(const QByteArray &state)
{
    if (!state.isEmpty()) {
        restoreState(state);
    }
    else {
        m_toolbarApplication->setVisible(true);
        m_toolbarLotteries->setVisible(true);
        m_toolbarView->setVisible(false);
    }
}


QByteArray MainWindow::applicationState() const
{
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


QByteArray MainWindow::applicationGeometry() const
{
    return saveGeometry();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if (true) {
        // Application properties
        m_applicationState = m_preferences.restoreApplicationState() ? applicationState() : QByteArray();
        m_applicationGeometry = m_preferences.restoreApplicationGeometry() ? applicationGeometry() : QByteArray();

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

    // Preferences
    m_preferences.load(settings);

    // Application and dialog properties
    m_applicationState = m_preferences.restoreApplicationState() ? settings.value(QStringLiteral("Application/State"), QByteArray()).toByteArray() : QByteArray();
    m_applicationGeometry = m_preferences.restoreApplicationGeometry() ? settings.value(QStringLiteral("Application/Geometry"), QByteArray()).toByteArray() : QByteArray();
    m_aboutDialogGeometry = settings.value(QStringLiteral("AboutDialog/Geometry"), QByteArray()).toByteArray();
    m_colophonDialogGeometry = settings.value(QStringLiteral("ColophonDialog/Geometry"), QByteArray()).toByteArray();
    m_preferencesDialogGeometry = settings.value(QStringLiteral("PreferencesDialog/Geometry"), QByteArray()).toByteArray();
}


void MainWindow::writeSettings()
{
    QSettings settings;

    // Preferences
    m_preferences.save(settings);

    // Application and dialog properties
    settings.setValue(QStringLiteral("Application/State"), m_applicationState);
    settings.setValue(QStringLiteral("Application/Geometry"), m_applicationGeometry);
    settings.setValue(QStringLiteral("AboutDialog/Geometry"), m_aboutDialogGeometry);
    settings.setValue(QStringLiteral("ColophonDialog/Geometry"), m_colophonDialogGeometry);
    settings.setValue(QStringLiteral("PreferencesDialog/Geometry"), m_preferencesDialogGeometry);
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

    // Actions: Lotteries
    QMapIterator<QString, QStringList> it(m_listLotteries);
    while (it.hasNext()) {
        it.next();

        auto *lottery = new QAction(it.value()[1], this);
        lottery->setObjectName(QStringLiteral("actionLottery_%1").arg(it.value()[0]));
        lottery->setIconText(it.value()[1]);
        lottery->setCheckable(true);
        lottery->setToolTip(it.value()[2]);
        lottery->setData(it.key());
        connect(lottery, &QAction::toggled, [=](bool checked) { onActionLotteriesToggled(lottery->data().toString(), checked); });

        m_actionLotteries << lottery;
    }

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

    // Menu: View
    auto *menuView = menuBar()->addMenu(tr("View"));
    menuView->setObjectName(QStringLiteral("menuView"));
    menuView->addAction(m_actionFullScreen);
    menuView->addSeparator();
    menuView->addAction(m_actionToolbarApplication);
    menuView->addAction(m_actionToolbarLotteries);
    menuView->addAction(m_actionToolbarView);
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
}


void MainWindow::updateActionFullScreen()
{
    if (!isFullScreen()) {
        m_actionFullScreen->setText(tr("Full Screen Mode"));
        m_actionFullScreen->setIcon(QIcon::fromTheme(QStringLiteral("view-fullscreen"), QIcon(QStringLiteral(":/icons/actions/16/view-fullscreen.svg"))));
        m_actionFullScreen->setChecked(false);
        m_actionFullScreen->setToolTip(tr("Display the window in full screen [%1]").arg(m_actionFullScreen->shortcut().toString(QKeySequence::NativeText)));
    }
    else {
        m_actionFullScreen->setText(tr("Exit Full Screen Mode"));
        m_actionFullScreen->setIcon(QIcon::fromTheme(QStringLiteral("view-restore"), QIcon(QStringLiteral(":/icons/actions/16/view-restore.svg"))));
        m_actionFullScreen->setChecked(true);
        m_actionFullScreen->setToolTip(tr("Exit the full screen mode [%1]").arg(m_actionFullScreen->shortcut().toString(QKeySequence::NativeText)));
    }
}


void MainWindow::onActionAboutTriggered()
{
    const auto geometry = m_preferences.restoreDialogGeometry() ? m_aboutDialogGeometry : QByteArray();

    AboutDialog dialog(this);
    dialog.setDialogGeometry(geometry);
    dialog.exec();

    m_aboutDialogGeometry = m_preferences.restoreDialogGeometry() ? dialog.dialogGeometry() : QByteArray();
}


void MainWindow::onActionColophonTriggered()
{
    const auto geometry = m_preferences.restoreDialogGeometry() ? m_colophonDialogGeometry : QByteArray();

    ColophonDialog dialog(this);
    dialog.setDialogGeometry(geometry);
    dialog.exec();

    m_colophonDialogGeometry = m_preferences.restoreDialogGeometry() ? dialog.dialogGeometry() : QByteArray();
}


void MainWindow::onActionPreferencesTriggered()
{
    const auto geometry = m_preferences.restoreDialogGeometry() ? m_preferencesDialogGeometry : QByteArray();

    PreferencesDialog dialog(this);
    dialog.setDialogGeometry(geometry);
    dialog.setPreferences(m_preferences);
    dialog.exec();

    m_preferences = dialog.preferences();
    m_preferencesDialogGeometry = m_preferences.restoreDialogGeometry() ? dialog.dialogGeometry() : QByteArray();
}


void MainWindow::onActionLotteriesToggled(const QString &lottery, bool checked)
{
    if (checked)
        openDocument(lottery);
    else
        closeDocument(lottery);
}


void MainWindow::onActionFullScreenTriggered()
{
    if (!isFullScreen())
        setWindowState(windowState() | Qt::WindowFullScreen);
    else
        setWindowState(windowState() & ~Qt::WindowFullScreen);

    updateActionFullScreen();
}


void MainWindow::onDocumentActivated()
{

}


void MainWindow::onDocumentClosed(const QString &documentName)
{
    foreach (QAction *actionLottery, m_actionLotteries) {

        if (actionLottery->objectName() == QStringLiteral("actionLottery_%1").arg(m_listLotteries[documentName][0])) {
            actionLottery->setChecked(false);
            break;
        }
    }
}


Document *MainWindow::createDocument()
{
    auto *document = new Document;
    connect(document, &Document::documentClosed, [=]() { onDocumentClosed(document->name()); });

    m_documentArea->addSubWindow(document);

    return document;
}


QMdiSubWindow *MainWindow::findDocument(const QString &documentName) const
{
    const QList<QMdiSubWindow *> windows = m_documentArea->subWindowList();
    for (auto *window : windows) {

        auto *document = qobject_cast<Document *>(window->widget());
        if (document->name() == documentName)
            return window;
    }

    return nullptr;
}


bool MainWindow::openDocument(const QString &documentName)
{
    // Checks whether the given document is already open.
    if (auto *window = findDocument(documentName)) {
        m_documentArea->setActiveSubWindow(window);
        return true;
    }

    return loadDocument(documentName);
}


bool MainWindow::loadDocument(const QString &documentName)
{
    auto *document = createDocument();

    const bool succeeded = document->load(documentName);
    if (succeeded) {
        document->setWindowTitle(m_listLotteries[documentName][1]);
        document->show();
    }
    else {
        document->close();
    }

    return succeeded;
}


bool MainWindow::closeDocument(const QString &documentName)
{
    bool succeeded = false;

    if (auto *window = findDocument(documentName))
        succeeded = window->close();

    return succeeded;
}
