#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFont>
#include <QFontDatabase>
#include <QStringList>

#include "ui/MainWindow.h"

namespace {

QString addFont(const QString &path)
{
    const int id = QFontDatabase::addApplicationFont(path);
    if (id < 0) {
        qWarning() << "Font load failed:" << path;
        return QString();
    }

    const QStringList families = QFontDatabase::applicationFontFamilies(id);
    return families.isEmpty() ? QString() : families.first();
}

QString addFontFromFolder(const QStringList &folders, const QString &fileName)
{
    for (const QString &folder : folders) {
        const QString path = QDir(folder).absoluteFilePath(fileName);
        if (QFileInfo::exists(path)) {
            return addFont(path);
        }
    }

    qWarning() << "Font file not found:" << fileName << folders;
    return QString();
}

void applyGameFonts(QApplication &app)
{
    const QString appDir = QCoreApplication::applicationDirPath();
    const QStringList fontFolders = {
        QDir(appDir).absoluteFilePath("resources/fonts"),
        QDir(appDir).absoluteFilePath("../resources/fonts"),
        QDir::current().absoluteFilePath("resources/fonts")
    };

    const QString sourceHanFamily = addFontFromFolder(fontFolders, QStringLiteral("SourceHanSerifCN-Regular-1.otf"));
    addFontFromFolder(fontFolders, QStringLiteral("SourceHanSerifCN-Bold-2.otf"));

    QString timesFamily = addFontFromFolder(fontFolders, QStringLiteral("Times New Roman.ttf"));
    if (timesFamily.isEmpty()) {
        timesFamily = QStringLiteral("Times New Roman");
    }

    QFont appFont(timesFamily, 12);
    app.setFont(appFont);

    const QString fallbackFamily = sourceHanFamily.isEmpty()
        ? QStringLiteral("Source Han Serif CN")
        : sourceHanFamily;

    app.setStyleSheet(QStringLiteral("* { font-family: \"%1\", \"%2\"; }")
        .arg(timesFamily, fallbackFamily));
}

}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("TheLastKeep");
    QCoreApplication::setApplicationName("TheLastKeep");

    applyGameFonts(app);

    MainWindow window;
    window.show();

    return app.exec();
}