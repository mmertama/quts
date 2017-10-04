/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#ifndef STDSUBSYSTEM_H
#define STDSUBSYSTEM_H

#include <QStack>
#include <QVariantList>

#include "subsystem.h"
#include "utils.h"
#include "qutsdll.h"


namespace Quts {

template <class T>
class QUTS_EXPORT_API EnumerableSystem : public IterableSubsystem<T> {
public:
    EnumerableSystem<T>(QObject* parent, QutsEngine& engine);
    void inject();
private:
    GUFU(len)
};

class QUTS_EXPORT_API ListSystem : public EnumerableSystem<QVariantList> {
    Q_OBJECT
public:
    QString sysName() const;
    ListSystem(QObject* parent, QutsEngine& engine);
    void inject();
private:
    void substring(const QVariant& vval, const QVariant& fromv, const QVariant& lenv);
    bool listAndIndex(const QStringList& values, QVariantList& list, int& index);
/*   void clear();
private:
    struct IInfo {
        Index ipos;
        int index;
        Closure closure;
        QVariantList thisList;
    };
    QStack<IInfo> m_iterators; */
private:
    GUFU(add)
    GUFU(_new)
    GUFU(at)
    GUFU(remove)
};

class QUTS_EXPORT_API StringSystem : public EnumerableSystem<QString> {
    Q_OBJECT
public:
    QString sysName() const;
    StringSystem(QObject* parent, QutsEngine& engine);
    void inject();
private:
    void substring(const QVariant& vval, const QVariant& fromv, const QVariant& lenv);
    bool stringAndIndex(const QStringList& values, QString& list, int& index);
private:
    GUFU(add)
    GUFU(substr)
    GUFU(match)
    GUFU(list)
    GUFU(at)
    GUFU(remove)
};
}

#endif // STDSUBSYSTEM_H
