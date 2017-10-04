/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */


#ifndef SCOPE_H
#define SCOPE_H

#include <functional>
#include "varianthash.h"
#include "index.h"

namespace Quts {

typedef std::function<bool ()> Cleanup;

class Scope : public VariantHash {
public:
    enum  Type {INVALID, FOR, IF, CALL};
    Scope(Type type, const QPair<Index, Index>& scope, const Index& index,
          const QHash<int, QVariant>& data = QHash<int, QVariant>(),
          Cleanup doClean = nullptr) : m_type(type), m_index(index),
        m_cleanup(doClean), m_scope(scope), m_data(data) {
        Q_ASSERT(scope.first.isValid());
        Q_ASSERT(scope.second.isValid());
    }
    Scope() : m_type(INVALID), m_cleanup(nullptr) {}

    const Index& outIndex() const {return m_index;}
    QVariant operator[](int value) const {return m_data[value];}
    QVariant& operator[](int value) {return m_data[value];}
    bool cleanup() const { Q_ASSERT(m_values.keys() == m_values.uniqueKeys()); return (m_cleanup != nullptr) ? m_cleanup() : true;}
    bool contains(int value) const {return m_data.contains(value);}
    QVariant operator[](const QString& key) const {return m_values[key];}
    QVariant& operator[](const QString& key) {return m_values[key];}
    bool contains(const QString& key) const {Q_ASSERT(m_values.keys() == m_values.uniqueKeys()); return m_values.contains(key);}
    void remove(const QString& key) {Q_ASSERT(m_values.keys() == m_values.uniqueKeys()); m_values.remove(key);}
    bool isIn(const Index& index) const {return index >= m_scope.first && index <= m_scope.second;}
    Index begin() const {return m_scope.first;}
    Index end() const {return m_scope.second;}
    Type type() const {return m_type;}
    QVariantList dumpMem() const;
    QVariantList dumpData() const;
private:
    Type m_type;
    Index m_index;
    Cleanup m_cleanup = nullptr;
    QPair<Index, Index> m_scope;
    QHash<int, QVariant> m_data;
    QHash<QString, QVariant> m_values;
};


}


#endif // SCOPE_H
