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
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDir>
#include <QLibraryInfo>
#include <QTranslator>


const QStringList findTranslations()
{
    QDir dir(":/translations");

    QStringList fileNames = dir.entryList(QDir::Files, QDir::Name);
    for (QString &fileName : fileNames)
        fileName = dir.filePath(fileName);

    return fileNames;
}


QString languageCode(const QString &translation)
{
    return QFileInfo(translation).fileName();
}


QString languageDescription(const QString &translation)
{
    QTranslator translator;
    translator.load(translation);

    QLocale locale(translator.language());
    return QCoreApplication::translate("main", "%1 (%2)").arg(locale.languageToString(locale.language()), locale.nativeLanguageName());
}


int showLanguageList()
{
    QString usage = QCoreApplication::instance()->arguments().constFirst();
    usage += QStringLiteral(" --language <") + QCoreApplication::translate("main", "language code") + QStringLiteral(">");

    printf("%s\n\n", qPrintable(QCoreApplication::translate("main", "Usage: %1").arg(usage)));
    printf("%s\n", qPrintable(QCoreApplication::translate("main", "Languages:")));

    for (const QString &translation : findTranslations())
        printf("  %s  %s\n", qPrintable(languageCode(translation)), qPrintable(languageDescription(translation)));

    return EXIT_SUCCESS;
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName(QStringLiteral("LotrioLabs"));
    app.setOrganizationDomain(QStringLiteral("https://lotriolabs.github.io"));
    app.setApplicationName(QStringLiteral("Lotrio-Qt"));
    app.setApplicationDisplayName(QStringLiteral("Lotrio-Qt"));
    app.setApplicationVersion(QStringLiteral("0.1.0"));


    //
    // Command line

    QCommandLineOption languageListOption(QStringLiteral("language-list"),
        QCoreApplication::translate("main", "Lists available application languages."));

    QCommandLineOption languageOption(QStringLiteral("language"),
        QCoreApplication::translate("main", "Adjusts application language."),
        QCoreApplication::translate("main", "language code"));

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main", "%1 - A visualization tool for lottery data").arg(app.applicationName()));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(languageListOption);
    parser.addOption(languageOption);
    parser.process(app);

    // Command line: Language list
    if (parser.isSet(languageListOption))
        return showLanguageList();

    // Command line: Language
    const QString &language = parser.value(languageOption);


    //
    // Translations

    QLocale locale = !language.isEmpty() ? QLocale(language) : QLocale::system();

    QTranslator translator;
    if (translator.load(locale, QString(), QString(), QStringLiteral(":/translations")))
        app.installTranslator(&translator);

    QTranslator translatorQtBase;
    if (translatorQtBase.load(locale, QStringLiteral("qtbase"), QString("_"), QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        app.installTranslator(&translatorQtBase);


    Window win;
    win.show();

    return app.exec();
}
