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

#include "lottery_document.h"

#include <QFileInfo>
#include <QVBoxLayout>

#include "lottery_page_about.h"
#include "lottery_page_draws.h"
#include "lottery_page_plays.h"


LotteryDocument::LotteryDocument(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    // Content
    m_tabBox = new QTabWidget;
    m_tabBox->setTabPosition(QTabWidget::South);

    // Main layout
    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_tabBox);
}


void LotteryDocument::setPreferences(const Preferences &preferences)
{
    m_preferences = preferences;
}


void LotteryDocument::setCanonicalName(const QString &canonicalName)
{
    m_canonicalName = canonicalName;
}


QString LotteryDocument::canonicalName() const
{
    return m_canonicalName;
}


QString LotteryDocument::documentTitle() const
{
    return windowTitle();
}


void LotteryDocument::updateDocumentTitle()
{
    const auto fileName = !m_canonicalName.isEmpty() ? QFileInfo(m_canonicalName).fileName() : tr("Untitled");

    setWindowTitle(fileName);
}


void LotteryDocument::setTabPosition(const QTabWidget::TabPosition tabPosition)
{
    m_tabBox->setTabPosition(tabPosition);
}


QTabWidget::TabPosition LotteryDocument::tabPosition() const
{
    return m_tabBox->tabPosition();
}


void LotteryDocument::closeEvent(QCloseEvent *event)
{
    if (true) {
        // Document will be closed
        emit aboutToClose(m_canonicalName);

        event->accept();
    } else {
        event->ignore();
    }
}


bool LotteryDocument::load(const QString &canonicalName)
{
    setCanonicalName(canonicalName);

    // Content
    auto *pageAbout = new LotteryPageAbout(canonicalName);
    auto *pageDraws = new LotteryPageDraws(canonicalName);
    auto *pagePlays = new LotteryPagePlays(canonicalName);

    m_tabBox->addTab(pageAbout, pageAbout->title());
    m_tabBox->addTab(pageDraws, pageDraws->title());
    m_tabBox->addTab(pagePlays, pagePlays->title());

    return true;
}
