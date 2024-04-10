#pragma once

#include "restic.h"

#include <QObject>
#include <qobjectdefs.h>

class Backend : public QObject {
    Q_OBJECT

  private:
    Restic *restic;

  public:
    explicit Backend(QObject *parent = nullptr);
    Q_INVOKABLE void backup();
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void mount(const QString &);
    Q_INVOKABLE void restore(const QString &);

    // void readyRefresh();
};
