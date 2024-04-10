#include "backend.h"
#include "restic.h"

#include <iostream>
#include <qobjectdefs.h>
#include <qprocess.h>

Backend::Backend(QObject *parent) : QObject(parent), restic(new Restic(this)) {}

Q_INVOKABLE void Backend::backup() { std::cout << "Backup" << std::endl; }

Q_INVOKABLE void Backend::refresh() {
    std::cout << "Refresh" << std::endl;
    this->restic->snapshots();
}

Q_INVOKABLE void Backend::mount(const QString &snapshotId) {
    std::cout << "Mount " << snapshotId.toStdString() << std::endl;
}

Q_INVOKABLE void Backend::restore(const QString &snapshotId) {
    std::cout << "Restore " << snapshotId.toStdString() << std::endl;
}
