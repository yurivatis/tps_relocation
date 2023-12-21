#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QCommandLineParser>
#include <optional>
#include <QDebug>
#include <QCommandLineOption>
#include "SQLite.h"
#include "MainWindow.h"


struct CommandLineParseResult
{
    enum class Status {
        Import,
        DisplayPeople,
        DisplayRooms,
        Error,
        Ok
    };
    Status statusCode = Status::DisplayPeople;
    std::optional<QString> errorString = std::nullopt;
};


CommandLineParseResult parseCommandLine(QCommandLineParser &parser)
{
    using Status = CommandLineParseResult::Status;

    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    const QCommandLineOption importOption({"i", "import"}, "Import from csv to database.", "file");
    parser.addOption(importOption);
    const QCommandLineOption displayPeopleOption({"p", "people"}, "Display people");
    parser.addOption(displayPeopleOption);
    const QCommandLineOption displayRoomsOption({"r", "rooms"}, "Display rooms");
    parser.addOption(displayRoomsOption);

    if (!parser.parse(QCoreApplication::arguments()))
        return { Status::Error, parser.errorText() };

    if (parser.isSet(importOption)) {
        SqlInterface *sqlInteface = SqlInterface::getInstance();
        sqlInteface->import(parser.value(importOption));
        return { Status::Import };
    }

    if (parser.isSet(displayPeopleOption))
        return { Status::DisplayPeople };

    if (parser.isSet(displayRoomsOption))
        return { Status::DisplayRooms };

    return { Status::Ok };
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("tps_reloaction");
    QApplication::setApplicationVersion("1.0");
    QCommandLineParser parser;
    using Status = CommandLineParseResult::Status;
    CommandLineParseResult parseResult = parseCommandLine(parser);
    switch (parseResult.statusCode) {
        case Status::Error:
            qDebug("No option set, exiting...");
            return -1;
        case Status::Import:
            break;
        case Status::DisplayPeople:
            break;
        case Status::DisplayRooms:
            break;
        default:
        break;
    }

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "tps_relocation_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }
    MainWindow *mw = new MainWindow(&app);
    mw->show();
    return app.exec();
}
