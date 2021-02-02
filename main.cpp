/****************************************************************************
**
** JIWOONG PARK
** Video Annotation Player for fMRI research
** Created: May 12, 2020
** Last Modified: May 12, 2020
**
****************************************************************************/

#include "videoplayer.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include <QtCore/QDir>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setApplicationName("Movie annotationPlayer");
    QCoreApplication::setOrganizationName("PECON JWPARK");
    QGuiApplication::setApplicationDisplayName(QCoreApplication::applicationName());
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription("Movie annotationPlayer");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("url", "The URL to open.");
    parser.process(app);

    VideoPlayer player;
    if (!parser.positionalArguments().isEmpty()) {
        const QUrl url =
            QUrl::fromUserInput(parser.positionalArguments().constFirst(),
                                QDir::currentPath(), QUrl::AssumeLocalFile);
        player.setUrl(url);
    }

    const QRect availableGeometry = QApplication::desktop()->availableGeometry(&player);
    player.resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
    player.show();

    return app.exec();
}
