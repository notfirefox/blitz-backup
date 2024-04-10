#pragma once

#include <QObject>
#include <qobjectdefs.h>
#include <qprocess.h>

class Restic : public QObject {
    Q_OBJECT

  private:
    QProcess *process;

  public:
    explicit Restic(QObject *parent = nullptr);
    void snapshots();
    void processFinished(int, QProcess::ExitStatus);
};
