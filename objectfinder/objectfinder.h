/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#ifndef OBJECTFINDER_H
#define OBJECTFINDER_H

#include <QList>
#include <QObject>
#include <QScopedPointer>
#include <QString>

#ifdef OBJECT_FINDER_EXPORT
# define OBJECT_FINDER_EXPORT_API Q_DECL_EXPORT
#else
# define OBJECT_FINDER_EXPORT_API Q_DECL_IMPORT
#endif

namespace Quts {

class ObjectFinderPrivate;

class OBJECT_FINDER_EXPORT_API ObjectFinder {
public:
    ObjectFinder();
    virtual ~ObjectFinder();
    QList<QObject*> findByName(const QString& name) const;
    QList<QObject*> findById(const QString& id, const QString& context = "") const;
private:
    QScopedPointer<ObjectFinderPrivate> m_p;
};
}

#endif // OBJECTFINDER_H
