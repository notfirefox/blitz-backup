#include "backend.h"
#include "snapshotmodel.h"

#include <KAboutData>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QUrl>
#include <QtQml>
#include <qjsengine.h>
#include <qjsvalue.h>
#include <qlogging.h>
#include <qqml.h>
#include <qqmlengine.h>

int main(int argc, char *argv[]) {
    // Enable debug logging
    QLoggingCategory::defaultCategory()->setEnabled(QtMsgType::QtDebugMsg,
                                                    true);

    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("blitz-backup");
    QCoreApplication::setOrganizationName(QStringLiteral("notfirefox"));
    QCoreApplication::setOrganizationDomain(
        QStringLiteral("notfirefox.github.io"));
    QCoreApplication::setApplicationName(QStringLiteral("Blitz Backup"));

    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    }

    KAboutData aboutData(QStringLiteral("blitz-backup"),
                         i18nc("@title", "Blitz Backup"), QStringLiteral("1.0"),
                         i18n("Fast & Secure Backups"), KAboutLicense::GPL_V3,
                         i18n("(c) 2024 notfirefox"));
    aboutData.setOrganizationDomain("io.github.notfirefox");
    aboutData.setDesktopFileName("io.github.notfirefox.blitz-backup");

    KAboutData::setApplicationData(aboutData);
    qmlRegisterSingletonType("io.github.notfirefox", 1, 0, "About",
                             [](QQmlEngine *engine, QJSEngine *) -> QJSValue {
                                 return engine->toScriptValue(
                                     KAboutData::applicationData());
                             });

    Backend backend;
    qmlRegisterSingletonInstance<Backend>("io.github.notfirefox", 1, 0,
                                          "Backend", &backend);

    qmlRegisterType<SnapshotModel>("io.github.notfirefox", 1, 0,
                                   "SnapshotModel");
    backend.refresh();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
