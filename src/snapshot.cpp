#include "snapshot.h"

#include <qjsonarray.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>

QString Snapshot::time() const { return mTime; }
QString Snapshot::parent() const { return mParent; }
QString Snapshot::tree() const { return mTree; }
QStringList Snapshot::paths() const { return mPaths; }
QString Snapshot::hostname() const { return mHostname; }
QString Snapshot::username() const { return mUsername; }
int Snapshot::uid() const { return mUid; }
int Snapshot::gid() const { return mGid; }
QStringList Snapshot::excludes() const { return mExcludes; }
QStringList Snapshot::tags() const { return mTags; }
QString Snapshot::programVersion() const { return mProgramVersion; }
QString Snapshot::id() const { return mId; }
QString Snapshot::shortId() const { return mShortId; }

Snapshot Snapshot::fromJson(const QJsonObject &json) {
    Snapshot result;

    if (auto v = json["time"]; v.isString()) {
        result.mTime = v.toString();
    }

    if (auto v = json["parent"]; v.isString()) {
        result.mParent = v.toString();
    }

    if (auto v = json["tree"]; v.isString()) {
        result.mTree = v.toString();
    }

    if (auto v = json["paths"]; v.isArray()) {
        result.mPaths = v.toVariant().toStringList();
    }

    if (auto v = json["hostname"]; v.isString()) {
        result.mHostname = v.toString();
    }

    if (auto v = json["username"]; v.isString()) {
        result.mUsername = v.toString();
    }

    if (auto v = json["excludes"]; v.isArray()) {
        result.mExcludes = v.toVariant().toStringList();
    }

    if (auto v = json["tags"]; v.isArray()) {
        result.mTags = v.toVariant().toStringList();
    }

    if (auto v = json["program_version"]; v.isString()) {
        result.mProgramVersion = v.toString();
    }

    if (auto v = json["id"]; v.isString()) {
        result.mId = v.toString();
    }

    if (auto v = json["short_id"]; v.isString()) {
        result.mShortId = v.toString();
    }

    return result;
}

QDebug operator<<(QDebug dbg, const Snapshot &snapshot) {
    dbg.nospace() << "===\n"
                  << "Time: " << snapshot.mTime << "\n"
                  << "Parent: " << snapshot.mParent << "\n"
                  << "Tree: " << snapshot.mTree << "\n"
                  << "Paths: " << snapshot.mPaths << "\n"
                  << "Hostname: " << snapshot.mHostname << "\n"
                  << "Username: " << snapshot.mUsername << "\n"
                  << "Excludes: " << snapshot.mExcludes << "\n"
                  << "Tags: " << snapshot.mTags << "\n"
                  << "ProgramVersion: " << snapshot.mProgramVersion << "\n"
                  << "Id: " << snapshot.mId << "\n"
                  << "ShortId: " << snapshot.mShortId << "\n"
                  << "===\n";
    return dbg;
}
