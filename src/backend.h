#pragma once

#include "snapshot.h"
#include <QObject>
#include <qobjectdefs.h>
#include <qprocess.h>
#include <qtmetamacros.h>

class Backend : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool loading READ loading WRITE setLoading NOTIFY loadingChanged)

  private:
    bool mLoading;
    QProcess *process;

  public:
    explicit Backend(QObject *parent = nullptr);

    bool loading() const;
    void setLoading(const bool loading);
    Q_SIGNAL void loadingChanged();

    Q_INVOKABLE void backup();
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void mount(const QString &);
    Q_INVOKABLE void restore(const QString &);

    // restic process finished
    void handleResticOutput(int, QProcess::ExitStatus);

  signals:
    void snapshotModelUpdated(QList<Snapshot> snapshots);
};
