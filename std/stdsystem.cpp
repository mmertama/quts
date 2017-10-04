/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#include "stdsystem.h"
#include "constants.h"
#include "variantutils.h"
#include "quts_doc.h"


using namespace Quts;

SUBSYSTEM(LIST) {
    return new ListSystem((QObject*)parent,  * (QutsEngine*) engine);
}

SUBSYSTEM(STRING) {
    return new StringSystem((QObject*)parent,  * (QutsEngine*) engine);
}

QString ListSystem::sysName() const {
    return LIST_NAME;
}

QString StringSystem::sysName() const {
    return  STRING_NAME;
}

template <class T>
EnumerableSystem<T>::EnumerableSystem(QObject* parent, QutsEngine& engine) : IterableSubsystem<T>(parent, engine) {
}

ListSystem::ListSystem(QObject* parent, QutsEngine& engine) : EnumerableSystem(parent, engine) {
}

StringSystem::StringSystem(QObject* parent, QutsEngine& engine) : EnumerableSystem(parent, engine) {
}

template <class T>
void EnumerableSystem<T>::inject() {
    GUFF(EnumerableSystem, len);
}

void ListSystem::inject() {
    GUFF(ListSystem, add);
    GUFF(ListSystem, _new);
    GUFF(ListSystem, at);
    GUFF(ListSystem, remove);
    GUFF_ALIAS(new, _new);
    GUFF(ListSystem, iterate);
    return EnumerableSystem::inject();
}

void StringSystem::inject() {
    GUFF(StringSystem, add);
    GUFF(StringSystem, substr);
    GUFF(StringSystem, match);
    GUFF(StringSystem, list);
    GUFF(StringSystem, at);
    GUFF(StringSystem, remove);
    GUFF(StringSystem, iterate);
    return EnumerableSystem::inject();
}


/**DOC
 * N ADD
 * S LIST
 * Add items
 * R result
 **/
bool ListSystem::add(const QStringList& values) {
    MIN_ARG(1);
    bool ok;
    const auto v0 = m_engine.popc(&ok);
    if(!ok) { return false; }

    if(!v0.canConvert(QVariant::List)) {
        syntaxError(INVALID_ARGUMENT);
        return false;
    }

    auto list = v0.toList();

    for(const auto& vv : values) {
        bool ok;
        list.append(getValue(vv, &ok));
        if(!ok) {
            return syntaxError(INVALID_ARGUMENT, quoted(vv));
        }
    }
    m_engine.pushc(list);
    return true;
}

/**DOC
 * N ADD
 * S STRING
 * concanate items
 * R result
 **/
bool StringSystem::add(const QStringList& values) {
    MIN_ARG(1);
    bool ok;
    const auto v0 = m_engine.popc(&ok);
    if(!ok) { return false; }

    if(!v0.canConvert(QVariant::String)) {
        syntaxError(INVALID_ARGUMENT);
        return false;
    }

    auto str = v0.toString();

    for(const auto& vv : values) {
        bool ok;
        const auto s = getValue(vv, &ok);
        if(!ok || !s.canConvert(QVariant::String)) {
            return syntaxError(INVALID_ARGUMENT, quoted(vv));
        }
        str.append(s.toString());
    }
    m_engine.pushc(str);
    return true;
}


/**DOC
 * N MATCH
 * a regular expression
 * S STRING
 * B regexp <string>
 * P value to match, if omitted, a value is taken from stack
 * R true or false
 **/
bool StringSystem::match(const QStringList& values) {
    MIN_ARG(1)
    const auto rexp = getValue(values[0]).toString();
    const QRegularExpression rex(rexp);
    if(!rex.isValid()) {
        syntaxError(INVALID_ARGUMENT, quoted(rexp));
        return false;
    }
    bool ok = true;
    const auto str = (values.length() > 1 ?
                      m_engine.getValue(values[1]) : m_engine.popc(&ok)).toString();
    if(!ok) { return false; }
    if(str.length() > 0) {
        m_engine.pushc(rex.match(str).hasMatch() ? TRUE_ : FALSE_);
        return true;
    } else {
        return syntaxError(INVALID_ARGUMENT);
    }
}

void StringSystem::substring(const QVariant& vval,
                             const QVariant& fromv,
                             const QVariant& lenv) {
    if(!vval.canConvert(QVariant::String)) {
        syntaxError(INVALID_ARGUMENT);
        return;
    }

    int left = 0;
    int len = -1;
    if(!num(fromv, left) || !num(lenv, len)) {
        syntaxError(INVALID_ARGUMENT);
        return;
    }

    auto str = vval.toString();

    const auto mid = str.mid(left, len);
    m_engine.pushc(mid);
}

/**DOC
 * N SUBSTR
 * Get a substring
 *
 **/
bool StringSystem::substr(const QStringList& values) {
    MIN_ARG(1)
    const auto v = getValue(values[0]);
    bool ok;
    v.toInt(&ok);
    if(ok) {
        const auto str = m_engine.popc(&ok);
        if(!ok) { return false; }
        const auto lenv = values.length() > 1 ? getValue(values[1]) : -1;
        if(lenv.isNull()) { return false; }
        substring(str,
                  v,
                  lenv);
    } else {
        ok = true;
        const auto fromv = values.length() > 1 ? getValue(values[1]) : m_engine.popc(&ok);
        if(!ok || fromv.isNull()) { return false; }
        const auto lenv = values.length() > 2 ? getValue(values[2]) : -1;
        if(lenv.isNull()) { return false; }
        substring(v,
                  fromv,
                  lenv);

    }
    return true;
}


/**DOC
 * N LEN
 * S String
 * Length of string on stack
 **/

/**DOC
 * N LEN
 * S List
 * Length of list on stack
 **/

template <class T>
bool EnumerableSystem<T>::len(const QStringList& values) {
    bool ok = true;
    const auto v = values.length() > 0 ? this->getValue(values[0]) : this->m_engine.popc(&ok);
    if(!ok) {
        return this->syntaxError(INVALID_ARGUMENT);
    }
    const auto len = toLen(v);
    if(len < 0) {
        return this->syntaxError(INVALID_ARGUMENT);
    }
    this->m_engine.pushc(len);
    return true;
}


/**DOC
 * N iterate
 * S List
 **/

/*
void ListSystem::clear() {
    m_iterators.clear();
}

bool ListSystem::iterate(const QStringList& values) {
    MIN_ARG(1);
    if(m_iterators.isEmpty() || m_iterators.top().ipos != m_engine.lineId()) {
        bool ok = true;
        const auto v = values.length() > 1 ? getValue(values[0]) : m_engine.popc(&ok);
        if(!ok) {
            return false;
        }
        const auto it = getValue(values[values.length() > 1 ? 1 : 0], &ok, QutsEngine::CALLPARAM);
        if(!ok) {
            return false;
        }

        Closure c;
        if(it.type() == QVariant::String) {
            c.first = Quts::makeGlobalName(it.toString(), m_engine.current());
        } else {
            if(!it.canConvert<Closure>()) {
                return syntaxError(INVALID_ARGUMENT);
            }
            c = it.value<Closure>();
        }
        if(!v.canConvert(QVariant::List)) {
            return syntaxError(INVALID_ARGUMENT);
        }
        m_iterators.push({m_engine.lineId(), 0, c, v.toList()});

    }

    auto& top = m_iterators.top();
    if(m_engine.lineId() == top.ipos) {
        if(top.index < top.thisList.length()) {
            if(!m_engine.callLambda(top.closure,
        {top.thisList[top.index]},
            top.index < top.thisList.length() - 1,
            [this](const bool stepValue) {
                if(!stepValue) {
                    m_iterators.pop();
                }
            })) {
                m_iterators.pop();
                return false;
            }
            ++top.index;
        }
    }
    return true;
}
*/

/**DOC
 * N LIST
 * S LIST
 **/
bool ListSystem::_new(const QStringList& values) {
    QVariantList list;
    for(const auto& v : values) {
        list.append(getValue(v));
    }
    m_engine.pushc(list);
    return true;
}

/**DOC
 * N LIST
 * S STRING
 **/
bool StringSystem::list(const QStringList& values) {
    MIN_ARG(1);
    bool ok;
    QVariantList list;
    const auto v0 = m_engine.getValue(values[0], &ok);
    if(!ok || !v0.canConvert(QVariant::String)) {
        return syntaxError(INVALID_ARGUMENT, Quts::quoted(v0.toString()));
    }
    const auto str = v0.toString();
    const auto r = values.length() > 1 ? getValue(values[1], &ok).toString() : " ";

    if(!ok) {
        return syntaxError(INVALID_ARGUMENT, quoted(values[1]));    //false only if 1
    }

    const auto splitted = str.split(r);
    m_engine.pushc(splitted);
    return true;
}


bool ListSystem::listAndIndex(const QStringList& values, QVariantList& list, int& index) {
    const auto v = values.length() == 1 ? m_engine.popc() : getValue(values[0]);
    if(!v.canConvert(QVariant::List)) {
        syntaxError(INVALID_ARGUMENT, quoted(values[0]));
        return false;
    }
    list = v.toList();
    bool ok;
    const auto vi = values.length() == 1 ?  getValue(values[0], &ok) : getValue(values[1], &ok);
    if(!ok || !num(vi, index)) {
        syntaxError(INVALID_ARGUMENT, quoted(values.last()));
        return false;
    }
    return true;
}

bool ListSystem::at(const QStringList& values) {
    MIN_ARG(1)
    QVariantList list;
    int index;
    if(!listAndIndex(values, list, index)) {
        return false;
    }
    if(index < 0 || index >= list.length()) {
        syntaxError(INVALID_INDEX, quoted(QString::number(index)));
    } else {
        m_engine.pushc(list.at(index));
    }
    return true;
}

bool ListSystem::remove(const QStringList& values) {
    MIN_ARG(1)
    QVariantList list;
    int index;
    if(!listAndIndex(values, list, index)) {
        return false;
    }
    list.removeAt(index);
    m_engine.pushc(list);
    return true;
}

bool StringSystem::stringAndIndex(const QStringList& values, QString& list, int& index) {
    const auto v = values.length() == 1 ? m_engine.popc() : getValue(values[0]);
    if(!v.canConvert(QVariant::String)) {
        syntaxError(INVALID_ARGUMENT, quoted(values[0]));
        return false;
    }
    list = v.toString();
    bool ok;
    const auto vi = values.length() == 1 ?  getValue(values[0], &ok) : getValue(values[1], &ok);
    if(!ok || !num(vi, index)) {
        syntaxError(INVALID_ARGUMENT, quoted(values.last()));
        return false;
    }
    return true;
}

bool StringSystem::at(const QStringList& values) {
    MIN_ARG(1)
    QString str;
    int index;
    if(!stringAndIndex(values, str, index)) {
        return false;
    }
    if(index < 0 || index >= str.length()) {
        syntaxError(INVALID_INDEX, quoted(QString::number(index)));
    } else {
        m_engine.pushc(str.at(index));
    }
    return true;
}

bool StringSystem::remove(const QStringList& values) {
    MIN_ARG(1)
    QString str;
    int index;
    if(!stringAndIndex(values, str, index)) {
        return false;
    }
    m_engine.pushc(str.left(index) + str.mid(index + 1));
    return true;
}
