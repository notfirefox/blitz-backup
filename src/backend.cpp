#include "backend.h"

#include "config.h"

#include <algorithm>
#include <iostream>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qobjectdefs.h>
#include <qprocess.h>
#include <qtenvironmentvariables.h>

Backend::Backend(QObject *parent)
    : QObject(parent), process(new QProcess(this)) {
    QObject::connect(this->process, &QProcess::finished, this,
                     &Backend::handleResticOutput);
}

void ensure_environment() {
    auto *config = Config::self();
    qputenv("RESTIC_REPOSITORY", config->repository().toStdString());
    qputenv("RESTIC_PASSWORD", config->password().toStdString());
}

Q_INVOKABLE void Backend::backup() { std::cout << "Backup" << std::endl; }

Q_INVOKABLE void Backend::refresh() {
    std::cout << "Refresh" << std::endl;
    if (process->state() == QProcess::NotRunning) {
        ensure_environment();
        this->process->startCommand("restic snapshots --json");
    }
}

Q_INVOKABLE void Backend::mount(const QString &snapshotId) {
    qDebug() << "Mount " << snapshotId << "\n";
}

Q_INVOKABLE void Backend::restore(const QString &snapshotId) {
    qDebug() << "Restore " << snapshotId << "\n";
}

void Backend::handleResticOutput(int exitCode,
                                 QProcess::ExitStatus exitStatus) {
    if (exitStatus == QProcess::NormalExit) {
        auto buf = this->process->readAllStandardOutput();
        auto jsonDoc = QJsonDocument::fromJson(buf);
        if (jsonDoc.isNull() || !jsonDoc.isArray()) {
            return;
        }
        auto jsonArray = jsonDoc.array();

        QList<Snapshot> snapshots;
        for (const auto &v : jsonArray.toVariantList()) {
            auto snapshot = Snapshot::fromJson(v.toJsonObject());
            snapshots.append(snapshot);
        }
        std::reverse(snapshots.begin(), snapshots.end());

        emit snapshotModelUpdated(snapshots);
    } else {
        auto buf = this->process->readAllStandardError();
        std::cout << "Process exited with code: " << exitCode << std::endl;
        std::cout << "Stderr: " << buf.toStdString() << std::endl;
        std::cout << "EOF" << std::endl;
    }
}
