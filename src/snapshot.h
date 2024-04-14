#pragma once

#include <qcontainerfwd.h>
#include <qobject.h>
#include <qtmetamacros.h>

class Snapshot {
    Q_GADGET

  public:
    QString time() const;
    QString parent() const;
    QString tree() const;
    QStringList paths() const;
    QString hostname() const;
    QString username() const;
    int uid() const;
    int gid() const;
    QStringList excludes() const;
    QStringList tags() const;
    QString programVersion() const;
    QString id() const;
    QString shortId() const;

    static Snapshot fromJson(const QJsonObject &json);
    friend QDebug operator<<(QDebug dbg, const Snapshot &snapshot);

  private:
    QString mTime;
    QString mParent;
    QString mTree;
    QStringList mPaths;
    QString mHostname;
    QString mUsername;
    int mUid;
    int mGid;
    QStringList mExcludes;
    QStringList mTags;
    QString mProgramVersion;
    QString mId;
    QString mShortId;
};
