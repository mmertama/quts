/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#ifndef TIMESYSTEM_H
#define TIMESYSTEM_H


#include "subsystem.h"
#include "utils.h"
#include "qutsdll.h"

namespace Quts {

class QUTS_EXPORT_API TimeSystem : public LoopingSubsystem<int> {
    Q_OBJECT
public:
    static const QString NAME;
    QString sysName() const;
    TimeSystem(QObject* parent, QutsEngine& engine);
    void inject();
    void tick();
    void clear();
private:
    GUFU(sleep)
    GUFU(get)
    GUFU(delta)
    GUFU(repeater)
    GUFU(repeat)
private:
    int m_sleepTicks = -1;
    QHash<QString, qlonglong> m_times;
};
}

#endif // TIMESYSTEM_H
