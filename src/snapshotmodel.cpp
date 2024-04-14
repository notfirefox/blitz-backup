#include "snapshotmodel.h"

#include "snapshot.h"

#include <qabstractitemmodel.h>
#include <qlogging.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qvariant.h>
#include <utility>

SnapshotModel::SnapshotModel(QObject *parent) : QAbstractListModel(parent) {
    this->mRoleNames[TimeRole] = "time";
    this->mRoleNames[ParentRole] = "parent";
    this->mRoleNames[TreeRole] = "tree";
    this->mRoleNames[PathsRole] = "paths";
    this->mRoleNames[HostnameRole] = "hostname";
    this->mRoleNames[UsernameRole] = "username";
    this->mRoleNames[UidRole] = "uid";
    this->mRoleNames[GidRole] = "gid";
    this->mRoleNames[ExcludesRole] = "excludes";
    this->mRoleNames[TagsRole] = "tags";
    this->mRoleNames[ProgramVersionRole] = "programVersion";
    this->mRoleNames[IdRole] = "id";
    this->mRoleNames[ShortIdRole] = "shortId";

    this->snapshots = QList<Snapshot>();
}

int SnapshotModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return static_cast<int>(snapshots.count());
}

QVariant SnapshotModel::data(const QModelIndex &index, int role) const {
    int row = index.row();

    if (row < 0 || row >= snapshots.count()) {
        qDebug() << row << " out of bounds\n";
        return {};
    }

    auto snapshot = snapshots.at(row);
    switch (role) {
    case TimeRole:
        return snapshot.time();
    case ParentRole:
        return snapshot.parent();
    case TreeRole:
        return snapshot.tree();
    case PathsRole:
        return snapshot.paths();
    case HostnameRole:
        return snapshot.hostname();
    case UsernameRole:
        return snapshot.username();
    case UidRole:
        return snapshot.uid();
    case GidRole:
        return snapshot.gid();
    case ExcludesRole:
        return snapshot.excludes();
    case TagsRole:
        return snapshot.tags();
    case ProgramVersionRole:
        return snapshot.programVersion();
    case IdRole:
        return snapshot.id();
    case ShortIdRole:
        return snapshot.shortId();
    default:
        return {};
    }
}

QHash<int, QByteArray> SnapshotModel::roleNames() const { return mRoleNames; }

void SnapshotModel::updateSnapshotModel(QList<Snapshot> snapshots) {
    this->snapshots = std::move(snapshots);
    emit layoutChanged();
}
