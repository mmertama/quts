#include <QDateTime>

#include "timesystem.h"
#include "utils.h"
#include "quts_doc.h"

#define GUFX(x) GUFF(TimeSystem,x)

using namespace Quts;

SUBSYSTEM(TIME) {
    return new TimeSystem((QObject*)parent,  * (QutsEngine*) engine);
}

QString TimeSystem::sysName() const {
    return TIME_NAME;
}

TimeSystem::TimeSystem(QObject* parent, QutsEngine& engine) : LoopingSubsystem (parent, engine) {
}

void TimeSystem::inject() {
    GUFX(sleep);
    GUFX(get);
    GUFX(delta);
    GUFX(repeater);
    GUFX(repeat);
}

void TimeSystem::tick() {
    if(m_sleepTicks >= 0) {
        --m_sleepTicks;
    }
}

void TimeSystem::clear() {
    m_sleepTicks = -1;
    LoopingSubsystem::clear();

}

/**DOC
 * Sleep milliseconds
 * S TIME
 * N SLEEP
 * B number
 * P milliseconds to sleep
 **/
bool TimeSystem::sleep(const QStringList& values) {
    MIN_ARG(1)
    if(m_sleepTicks < 0) {
        bool ok;
        qreal v = m_engine.parameter<qreal>(values, 0, &ok);
        qreal interval = qMax(static_cast<qreal>(m_engine.interval()), 0.0);
        if(ok) {
            m_sleepTicks = interval > 0 ? v / interval : v;
        } else {
            return false;
        }
    }
    if(m_sleepTicks > 0) {
        m_engine.setPending();
    }
    return true;
}

/**DOC
 * S TIME
 * N get
 **/
bool TimeSystem::get(const QStringList& values) {
    Q_UNUSED(values);
    m_engine.pushc(QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
    return true;
}

/**DOC
 * S TIME
 * N delta
 **/
bool TimeSystem::delta(const QStringList& values) {
    bool ok;
    const auto str = m_engine.parameter<QString>(values, 0, &ok);

    const auto value = QDateTime::fromString(str, Qt::ISODate);

    if(!value.isValid()) {
        syntaxError(INVALID_ARGUMENT, quoted(str));
        return false;
    }

    const auto delta = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch()
                       - value.toMSecsSinceEpoch();
    m_engine.pushc(delta);
    return true;
}

/**DOC
 * S TIME
 * N repeater
 **/
bool TimeSystem::repeater(const QStringList& values) {
    MIN_ARG(1);
    auto limit = std::numeric_limits<int>::max();
    if(values.length() > 2){
        if(!m_engine.numValue(values[2], limit))
            return syntaxError(INVALID_ARGUMENT, values[2]);
    }
    bool ok = true;
    const QVariantList params = map<QVariant, QString>(values.mid(3), [this, &ok](const QString& s){
        return ok ? getValue(s, &ok) : QVariant();});
    if(!ok)
        return syntaxError(INVALID_ARGUMENT);
    return loop(values, [this, limit, params](IInfo& top){
        if(top.index < 0){
            top.index = top.data / m_engine.interval();
            if(!m_engine.callLambda(top.closure, params, top.index < limit,
                                    [this](bool iterated){
                                    if(!iterated)
                                        m_iterators.pop();
                                })){
                return false;
            }
        }
        top.index--;
        return true;
    });
}

/**
  * S TIME
  * N repeat
**/

bool TimeSystem::repeat(const QStringList& values){
    if(values.length() > 0){
        int limit;
        if(!m_engine.numValue(values[0], limit))
            return syntaxError(INVALID_ARGUMENT, values[0]);
        const auto key = asSysKey("repeatKey", qHash(m_engine.lineId()), m_iterators.length());
        if(m_engine.contains(key)){
            const auto v = m_engine[key].toInt();
            if(v < 0){
               LoopingSubsystem::clear();
               return true;
            } else {
                m_engine[key] = v - 1;
            }
        } else {
            const auto t = limit / m_engine.interval();
            m_engine[key] = t;
        }
    }

    if(!m_iterators.isEmpty()){
        m_engine.setPending();
        if(m_iterators.length() > 1) {
            const auto topItem = m_iterators.pop();
            m_iterators.prepend(topItem);
        }
        return run();
    }
    return true;
}
