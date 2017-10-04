/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#ifndef OBJECTFINDER_PRIVATE_H
#define OBJECTFINDER_PRIVATE_H

#include <functional>

#include <QSet>
#include <QObject>
#include <QMutex>
#include <QThread>


class QQmlContextData;


namespace Quts {


class ObjectData {
public:
    enum {
        CACHED = 0x1
    };
    int flags;
    QQmlContextData* context;
    QString id;
};

class ObjectFinderPrivate {

public:
    ObjectFinderPrivate();
    ~ObjectFinderPrivate();
    QList<QObject*> findByName(const QString& name) const;
    QList<QObject*> findById(const QString& id, const QString& context = "") const;
private:
    void append(QObject* object);
    void remove(QObject* object);
private:
    QSet<QObject*> m_objects;
    quintptr m_hooks[2];
    mutable QMutex m_mutex;
};
}

#endif // OBJECTFINDER_PRIVATE_H
