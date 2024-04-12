#pragma once

#include "snapshot.h"
#include <QObject>
#include <qobjectdefs.h>
#include <qprocess.h>

class Backend : public QObject {
    Q_OBJECT

  private:
    QProcess *process;

  public:
    explicit Backend(QObject *parent = nullptr);
    Q_INVOKABLE void backup();
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void mount(const QString &);
    Q_INVOKABLE void restore(const QString &);

    // restic process finished
    void handleResticOutput(int, QProcess::ExitStatus);

  signals:
    void snapshotModelUpdated(QList<Snapshot> snapshots);
};
