#pragma once

#include "snapshot.h"

#include <QtCore>

class SnapshotModel : public QAbstractListModel {
    Q_OBJECT

  public:
    enum RoleNames {
        TimeRole = Qt::UserRole,
        ParentRole,
        TreeRole,
        PathsRole,
        HostnameRole,
        UsernameRole,
        UidRole,
        GidRole,
        ExcludesRole,
        TagsRole,
        ProgramVersionRole,
        IdRole,
        ShortIdRole,
    };

    explicit SnapshotModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

  private:
    QList<Snapshot> snapshots;
    QHash<int, QByteArray> mRoleNames;

  public slots:
    void updateSnapshotModel(QList<Snapshot> snapshots);
};

Q_DECLARE_METATYPE(SnapshotModel);
