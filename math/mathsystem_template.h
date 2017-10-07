/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#ifndef MATHSYSTEM_TEMPLATE_H
#define MATHSYSTEM_TEMPLATE_H

#include "mathsystem.h"
#include "utils.h"


#define MF(n)  {#n, std::bind(&Mathsystem::n, this)}
#define MF_ALIAS(n, f)  {#n, std::bind(&Mathsystem::f, this)}

#define GUFF_MATH(x) m_engine.inject(this, Keyword(#x), std::bind(&Mathsystem::x, this, std::placeholders::_1))
#define GUFF_ALIAS_MATH(x, f) m_engine.makeAlias(this, Keyword(#x), Keyword(#f))

namespace Quts {


template <typename T>
Mathsystem<T>::Mathsystem(QObject* parent, QutsEngine& engine) : Subsystem(parent, engine),
    m_commands(
#ifdef BROKEN_COMPILER
        c_fromPair<Commands, QPair<QString, std::function<void()>>>(
#endif
{
    MF(push),
    MF(pop),
    MF(sum),
    MF(dup),
    MF(drop),
    MF(over),
    MF(subt),
    MF(mul),
    MF(div),
    MF(pow),
    MF(equal),
    MF(lessThan),
    MF(lessThanOrEqual),
    MF(moreThan),
    MF(moreThanOrEqual),
    MF(swap)
})
#ifdef BROKEN_COMPILER
) {
    memset(m_registers, 0, sizeof(m_registers) * sizeof(int));
#else
{
#endif
}

template <typename T>
void Mathsystem<T>::inject() {
    GUFF_MATH(calc);
}

template <typename T>
bool Mathsystem<T>::calc(const QStringList& params) {
    Q_ASSERT(params.length() > 0);
    for(const auto& cmd : params) {
        bool isNum;
        const auto v = cmd.toDouble(&isNum);
        if(isNum) {
            m_stack.push(v);
        } else {
            if(m_commands.contains(cmd)) {
                m_commands[cmd]();
                if(m_err != MATH_OK) {
                    switch(m_err) {
                    case  MATH_UNDERFLOW:
                        break;
                    case MATH_DIV_BY_ZERO:
                        m_engine.syntaxError(DIVISION_BY_ZERO, cmd);
                        break;
                    case MATH_ARGUMENT:
                        break;
                    default:
                        m_engine.syntaxError(INVALID_ERROR, cmd);
                        break;
                    }
                    return false;
                }
            } else {
                m_engine.syntaxError(INVALID_ARGUMENT, cmd);
                return false;
            }
        }
    }
    return true;
}

/**DOC
* pop a value from application stack into corresponding math stack.
* N push
* S INT
**/
/**DOC
* pop a value from application stack into corresponding math stack.
* N push
* S FLOAT
**/

template<typename T>
void Mathsystem<T>::push() {
    bool ok;
    const auto value = m_engine.popc(&ok);
    if(ok) {
        const auto v = value.toDouble(&ok);
        if(ok) {
            pushc(static_cast<T>(v));
        } else {
            m_engine.syntaxError(INVALID_ARGUMENT, QString("\"%1\"").arg(value.toString()));
            m_err = MATH_ARGUMENT;
        }
    } else {
        m_err = MATH_UNDERFLOW;
    }
}

/**DOC
* pop a value from math stack on application stack
* N pop
* S INT
**/
/**DOC
* pop a value from math stack on application stack
* N pop
* S FLOAT
**/
template<typename T>
void Mathsystem<T>::pop() {
    const auto v = popc();
    m_engine.pushc(QString::number(v));
}

/**DOC
* N sum
* S INT
**/
/**DOC
* N sum
* S FLOAT
**/
template<typename T>
void Mathsystem<T>::sum() {
    pushc(popc() + popc());
}

/**DOC
* duplicate value
* N dup
* S FLOAT
**/
/**DOC
* duplicate value
* N dup
* S INT
**/
template<typename T>
void Mathsystem<T>::dup() {
    if(m_stack.count() <= 0) {
        m_err = MATH_UNDERFLOW;
    } else {
        pushc(m_stack.top());
    }
}

/**DOC
* remove topmost value
* N drop
* S INT
**/
/**DOC
* remove topmost value
* N drop
* S FLOAT
**/
template<typename T>
void Mathsystem<T>::drop() {
    popc();
}

/**DOC
* duplicate value under top on top
* N over
* S INT
**/
/**DOC
* duplicate value under top on top
* N over
* S FLOAT
**/
template<typename T>
void Mathsystem<T>::over() {
    if(m_stack.count() <= 1) {
        m_err = MATH_UNDERFLOW;
    }
    const auto t = m_stack.pop();
    const auto b = m_stack.top();
    m_stack.push(t);
    m_stack.push(b);
}
/**DOC
* N subt
* S FLOAT
* Subtract
**/
/**DOC
* N subt
* S FLOAT
* Subtract
**/
template<typename T>
void Mathsystem<T>::subt() {
    pushc(popc() - popc());
}

/**DOC
* N mul
* S INT
**/
/**DOC
* N mul
* S FLOAT
**/
template<typename T>
void Mathsystem<T>::mul() {
    pushc(popc() * popc());
}

/**DOC
* N div
* S INT
**/
/**DOC
* N div
* S FLOAT
**/
template<typename T>
void Mathsystem<T>::div() {
    const auto r = popc();
    const auto d = popc();
    if(r != 0) {
        m_stack.push(d / r);
    } else {
        m_err = MATH_DIV_BY_ZERO;
    }
}

/**DOC
* N pow
* S INT
**/
/**DOC
* N pow
* S FLOAT
**/
template<typename T>
void Mathsystem<T>::pow() {
    const auto e = popc();
    const auto b = popc();
    pushc(qPow(b, e));
}

/**DOC
* N equal
* S INT
**/
/**DOC
* N equal
* S FLOAT
**/
template<typename T>
void Mathsystem<T>::equal() {
    pushc(popc() == popc());
}

/**DOC
* N lessThan
* S INT
**/
/**DOC
* N lessThan
* S FLOAT
**/
template<typename T>
void Mathsystem<T>::lessThan() {
    pushc(popc() < popc());
}

/**DOC
* N lessThanOrEqual
* S INT
**/
/**DOC
* N lessThanOrEqual
* S FLOAT
**/
template<typename T>
void Mathsystem<T>::lessThanOrEqual() {
    pushc(popc() <= popc());
}

/**DOC
* N moreThan
* S INT
**/
/**DOC
* N moreThan
* S FLOAT
**/
template<typename T>
void Mathsystem<T>::moreThan() {
    pushc(popc() >= popc());
}

/**DOC
* N moreThanOrEqual
* S INT
**/
/**DOC
* N moreThanOrEqual
* S FLOAT
**/
template<typename T>
void Mathsystem<T>::moreThanOrEqual() {
    pushc(popc() >= popc());
}

/**DOC
* N swap
* S INT
**/
/**DOC
* N swap
* S FLOAT
**/
template<typename T>
void Mathsystem<T>::swap() {
    const auto a = popc();
    const auto b = popc();
    pushc(a);
    pushc(b);
}

/**DOC
* Error if two top values are non equal.
* N assert
* S INT
**/
/**DOC
* Error if two top values are non equal.
* N assert
* S FLOAT
**/
template<typename T>

void Mathsystem<T>::assert() {
    if(!equal()) {
        errorStr(ASSERTED);
    }
}
}

#endif // MATHSYSTEM_TEMPLATE_H
