#include "backend.h"

#include <iostream>
#include <qobjectdefs.h>
#include <qprocess.h>

Backend::Backend(QObject *parent) : QObject(parent) {}

Q_INVOKABLE void Backend::backup() { std::cout << "Backup" << std::endl; }

Q_INVOKABLE void Backend::refresh() {
    std::cout << "Refresh" << std::endl;

    auto *process = new QProcess(this);
    process->startCommand("restic snapshots");
    QObject::connect(process, &QProcess::readyReadStandardOutput, this,
                     &Backend::readyRefresh);
}

Q_INVOKABLE void Backend::mount(const QString &snapshotId) {
    std::cout << "Mount " << snapshotId.toStdString() << std::endl;
}

Q_INVOKABLE void Backend::restore(const QString &snapshotId) {
    std::cout << "Restore " << snapshotId.toStdString() << std::endl;
}

void Backend::readyRefresh() {
    auto *process = dynamic_cast<QProcess *>(sender());
    auto buf = process->readAllStandardOutput();
    std::cout << buf.toStdString() << std::endl;
}
