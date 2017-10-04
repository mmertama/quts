/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#ifndef UTILS_H
#define UTILS_H

#include <functional>
#include <QHash>
#include <QPair>
#include "constants.h"

#ifdef QUTSLIB_EXPORT
# define QUTSLIB_EXPORT_API Q_DECL_EXPORT
#else
# define QUTSLIB_EXPORT_API Q_DECL_IMPORT
#endif

namespace Quts {

#define GUFU(x) bool x (const QStringList& value);
#define GUFF(c,x) this->m_engine.inject(this, Keyword(#x), std::bind(&c::x, this, std::placeholders::_1))
#define GUFF_ALIAS(x, f) this->m_engine.makeAlias(this, Keyword(#x), Keyword(#f))
#define MIN_ARG(x) if(values.length() < x) {syntaxError(INVALID_ARGUMENT); return true;}
#define PRINT (stream() <<  (contains(PRINTPREFIX_) ? operator[](PRINTPREFIX_).toString() : ""))

class Subsystem;

template <typename O, typename I>
QList<O> map(const QList<I>& container, std::function<O (const I&)> func){
    QList<O> out;
    for(const auto& i : container)
        out.append(func(i));
    return out;
}

class Keyword{
public:
    Keyword() {}
    Keyword(const Keyword& key) : m_key(key.m_key) {}
    Keyword& operator=(const Keyword& key) {m_key = key.m_key; return *this;}
    Keyword(const QString& key) : m_key(key.toUpper()){}
    Keyword& operator=(const QString& key) {m_key = key.toUpper(); return *this;}
    operator QString() const {return m_key;}
    bool operator==(const QString& key) const {return key.toUpper() == m_key;}
    bool operator!=(const QString& key) const {return key.toUpper() != m_key;}
    bool operator<(const QString& key) const {return key.toUpper() < m_key;}
    bool isValid() const {return !m_key.isEmpty();}
private:
    QString m_key;
};


QString QUTSLIB_EXPORT_API quoted(const QString& v);
QString QUTSLIB_EXPORT_API makeGlobalName(const QString& local, const QString& current);
inline QString asSysKey(const QString& owner, int index, int scope){
    return QString("###_sys_%1_%2").arg(owner).arg(index + (65535 * scope));
}


}

#endif // UTILS_H
