/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#ifndef MATHSYSTEM_H
#define MATHSYSTEM_H

#include "subsystem.h"
#include "constants.h"
#include "qutsdll.h"
#include <qmath.h>
#include <functional>
#include <QHash>
#include <QStack>

namespace Quts {

#define MFTYPE(x) void x()
typedef QHash<QString, std::function<void ()>> Commands;


template<typename T>
class Mathsystem : public Subsystem { //Templated class not allowed be Q_OBJECT
public:
    Mathsystem(QObject* parent, QutsEngine& engine);
    void inject();
protected:
    bool calc(const QStringList& params);
protected:
    MFTYPE(push);
    MFTYPE(pop);
    MFTYPE(sum);
    MFTYPE(dup);
    MFTYPE(drop);
    MFTYPE(over);
    MFTYPE(subt);
    MFTYPE(mul);
    MFTYPE(div);
    MFTYPE(pow);
    MFTYPE(swap);
    MFTYPE(equal);
    MFTYPE(lessThan);
    MFTYPE(lessThanOrEqual);
    MFTYPE(moreThan);
    MFTYPE(moreThanOrEqual);
    MFTYPE(assert);
    T& operator[](int index) {return m_registers[index];}
protected:
    enum {MATH_OK, MATH_UNDERFLOW, MATH_DIV_BY_ZERO, MATH_ARGUMENT};
    inline void pushc(T v) {m_stack.push(v);}
    inline T popc() {
        if(m_stack.count() > 0) {
            return m_stack.pop();
        }
        m_err = MATH_UNDERFLOW;
        return 1;
    }
    int  m_err = MATH_OK;
    Commands m_commands;
private:
    QStack<T> m_stack;
    T m_registers[4]
#ifdef BROKEN_COMPILER
    ;
#else
        = {0, 0, 0, 0};
#endif
};

class QUTS_EXPORT_API RealMath : public Mathsystem<qreal> {
    Q_OBJECT
public:
    static const QString NAME;
    QString sysName() const;
    RealMath(QObject* parent, QutsEngine& engine);
private:
    MFTYPE(sqrt);
    MFTYPE(sin);
    MFTYPE(cos);
    MFTYPE(tan);
    MFTYPE(atan);
    MFTYPE(atan2);
    MFTYPE(acos);
    MFTYPE(asin);
    MFTYPE(exp);
    MFTYPE(ceil);
    MFTYPE(floor);
    MFTYPE(ln);
    MFTYPE(rad);
    MFTYPE(deg);
};

class QUTS_EXPORT_API IntMath : public Mathsystem<int> {
    Q_OBJECT
public:
    static const QString NAME;
    QString sysName() const;
    IntMath(QObject* parent, QutsEngine& engine);
private:
    MFTYPE(remainder);
    MFTYPE(func_and);
    MFTYPE(func_or);
    MFTYPE(func_xor);
    MFTYPE(func_not);
};
}

#include "mathsystem_template.h"

#endif // MATHSYSTEM_H
