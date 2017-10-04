/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#ifndef QTSYSTEM_H
#define QTSYSTEM_H

#include<QSet>

class QQuickItem;

#include "signalreceiver.h"
#include "subsystem.h"
#include "utils.h"
#include "qutsdll.h"

namespace Quts {
class GuiSignal;
class QutsAPI;

class QUTS_EXPORT_API QtSystem : public Subsystem, public SignalReceiver {
    Q_OBJECT
public:
    QString sysName() const;
    QtSystem(QObject* parent, QutsEngine& engine);
    ~QtSystem();
    void inject();
protected:
    bool isActive() const;
    void clear();
    void tick();
    void requestResume();
private:
    void pushc(const QVariant& string) {m_engine.pushc(string);}
    void expired(GuiSignal* sig);
    void signalHandler(GuiSignal* item, const QString& param);
    void setFocus(const QList<QObject*>& objects, const QStringList& values);
    QMetaObject::Connection connect(GuiSignal* sig);
    bool setWait(const QVariant& value);
    GUFU(click)
    GUFU(write)
    GUFU(slot)
    GUFU(wait)
    GUFU(set)
    GUFU(get)
    GUFU(signal)
    GUFU(child)
    GUFU(parent)
    GUFU(find)
    GUFU(item)
    GUFU(fid)
    GUFU(qutsSignal)
private:
    QSet<GuiSignal*> m_signals;
    QObject* m_focus = nullptr;
    int m_skipWait = 0;
    int m_waitTicks = -1;
    bool m_waitPenging = false;
};
}

#endif // QTSYSTEM_H
