/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#include "mathsystem.h"
#include <qmath.h>

#include "comp_utils.h"
#include "quts_doc.h"

#include <QDebug>

using namespace Quts;


#define RF(n)  {#n, std::bind(&RealMath::n, this)}
#define RF_ALIAS(n, f)  {#n, std::bind(&RealMath::f, this)}

#define IF(n)  {#n, std::bind(&IntMath::n, this)}
#define IF_ALIAS(n, f)  {#n, std::bind(&IntMath::f, this)}

template <typename T>
void join(QHash<QString, T>& to, const QHash<QString, T>& from) {
    for(const auto& key : from.uniqueKeys()) {
        Q_ASSERT(!to.contains(key));
        to.insert(key, from[key]);
    }
}



SUBSYSTEM(FLOAT) {
    return new RealMath((QObject*)parent,  * (QutsEngine*) engine);
}

SUBSYSTEM(INT) {
    return new IntMath((QObject*)parent,  * (QutsEngine*) engine);
}

QString RealMath::sysName() const {return FLOAT_NAME;}
QString IntMath::sysName() const {return INT_NAME;}

RealMath::RealMath(QObject* parent, QutsEngine& engine) :
    Mathsystem(parent, engine) {
#ifdef BROKEN_COMPILER
    const auto commands = c_fromPair<Commands, QPair<QString, std::function<void()>>>({
#else
    Commands commands({
#endif
        RF(sqrt),
        RF(sin),
        RF(cos),
        RF(tan),
        RF(atan),
        RF(atan2),
        RF(acos),
        RF(asin),
        RF(exp),
        RF(ceil),
        RF(floor),
        RF(ln),
        RF(rad),
        RF(deg)
    });
    m_commands.unite(commands);
#ifndef BROKEN_COMPILER
    Q_ASSERT(m_commands.keys() == m_commands.uniqueKeys());
#endif
}

/**DOC
 * N sqrt
 * S FLOAT
 * Squareroot
**/
void RealMath::sqrt() {
    pushc(qSqrt(popc()));
}

/**DOC
 * N sin
 * S FLOAT
**/
void RealMath::sin() {
    pushc(qSin(popc()));
}

/**DOC
 * N cos
 * S FLOAT
**/
void RealMath::cos() {
    pushc(qCos(popc()));
}

/**DOC
 * N tan
 * S FLOAT
**/
void RealMath::tan() {
    pushc(qTan(popc()));
}

/**DOC
 * N atan
 * S FLOAT
**/
void RealMath::atan() {
    pushc(qAtan(popc()));
}

/**DOC
 * N atan2
 * S FLOAT
**/
void RealMath::atan2() {
    const auto y = popc();
    const auto x = popc();
    pushc(qAtan2(x, y));
}

/**DOC
 * N acos
 * S FLOAT
**/
void RealMath::acos() {
    pushc(qAcos(popc()));
}

/**DOC
 * N asin
 * S FLOAT
**/
void RealMath::asin() {
    pushc(qAsin(popc()));
}

/**DOC
 * N exp
 * S FLOAT
**/
void RealMath::exp() {
    pushc(qExp(popc()));
}

/**DOC
 * N ceil
 * S FLOAT
**/
void RealMath::ceil() {
    pushc(qCeil(popc()));
}

/**DOC
 * N floor
 * S FLOAT
**/
void RealMath::floor() {
    pushc(qFloor(popc()));
}

/**DOC
 * N ln
 * S FLOAT
**/
void RealMath::ln() {
    pushc(qLn(popc()));
}


/**DOC
 * N rad
 * S FLOAT
**/
void RealMath::rad(){
    pushc(qDegreesToRadians(popc()));
}

/**DOC
 * N rad
 * S FLOAT
**/
void RealMath::deg(){
    pushc(qRadiansToDegrees(popc()));
}

IntMath::IntMath(QObject* parent, QutsEngine& engine) :
    Mathsystem(parent, engine) {
#ifdef BROKEN_COMPILER
    const auto commands = c_fromPair<Commands, QPair<QString, std::function<void()>>>({
#else
    Commands commands({
#endif
        IF(remainder),
        IF(func_and),
        IF(func_or),
        IF(func_xor),
        IF(func_not),
        IF_ALIAS( and, func_and),
        IF_ALIAS( or, func_or),
        IF_ALIAS(xor, func_xor),
        IF_ALIAS(not, func_not)
    });
    m_commands.unite(commands);
#ifndef BROKEN_COMPILER
    Q_ASSERT(m_commands.keys() == m_commands.uniqueKeys());
#endif
}

/**DOC
 * N remainder
 * S INT
**/
void IntMath::remainder() {
    const auto r = popc();
    const auto d = popc();
    if(r != 0) {
        pushc(d % r);
    } else {
        m_err = MATH_DIV_BY_ZERO;
    }
}

/**DOC
 * Bitwise and
 * N and
 * S INT
**/
void IntMath::func_and() {
    pushc(popc() & popc());
}

/**DOC
 * Bitwise or
 * N or
 * S INT
**/
void IntMath::func_or() {
    pushc(popc() | popc());
}

/**DOC
 * Bitwise xor
 * N xor
 * S INT
**/
void IntMath::func_xor() {
    pushc(popc() ^ popc());
}

/**DOC
 * N not
 * S INT
**/
void IntMath::func_not() {
    pushc(~ popc());
}



