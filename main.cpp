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
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDir>
#include <QTranslator>
#include <QLibraryInfo>


QStringList findTranslations()
{
    QDir dir(":/translations");
    QStringList fileNames = dir.entryList(QStringList("*"), QDir::Files, QDir::Name);
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
    QString usage;
    usage += QCoreApplication::instance()->arguments().constFirst();
    usage += QLatin1Char(' ') + QStringLiteral("--language");
    usage += QLatin1Char(' ') + QCoreApplication::translate("main", "[Language code]");

    printf("%s\n\n", qPrintable(QCoreApplication::translate("main", "Usage: %1").arg(usage)));
    printf("%s\n", qPrintable(QCoreApplication::translate("main", "Languages:")));

    const QStringList translations = findTranslations();
    for (const QString &translation : translations)
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

    QCommandLineOption languageListOption(QStringLiteral("language-list"), QCoreApplication::translate("main", "Lists available application languages."));
    QCommandLineOption languageOption(QStringLiteral("language"), QCoreApplication::translate("main", "Adjusts application language."), QStringLiteral("language"));

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main", "%1 - A visualization tool for lottery data").arg(app.applicationName()));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(languageListOption);
    parser.addOption(languageOption);
    parser.process(app);

    if (parser.isSet(languageListOption))
        return showLanguageList();

    //
    // Translations

    const QString &language = parser.value(languageOption);
    QLocale locale = !language.isEmpty() ? QLocale(language) : QLocale::system();

    QTranslator translator;
    if (translator.load(locale, QStringLiteral(":/translations/")))
        app.installTranslator(&translator);

    QTranslator translatorQtBase;
    if (translatorQtBase.load(locale, QStringLiteral("qtbase_"), QString(), QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        app.installTranslator(&translatorQtBase);


    MainWindow window;
    window.show();

    return app.exec();
}
