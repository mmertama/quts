/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#ifndef GUISIGNAL_H
#define GUISIGNAL_H

#include <functional>

#include "signalreceiver.h"
#include "index.h"

namespace Quts {

class GuiSignal : public QObject {
    Q_OBJECT
public:
    GuiSignal(QObject* parent, SignalReceiver& stack, const QString& name, std::function<bool (GuiSignal* ptr)> function);
    ~GuiSignal();
    void replace(const QString& after, const QString& before) {name.replace(after, before);}
public slots:
    void slotFunction(const QVariant& var);
    void slotFunction(const QString& var);
    void slotFunction(int var);
    void slotFunction(qreal var);
    void slotFunction(const QVariantMap& map);
    void slotFunction(const QVariantList& map);
    void slotFunction();
private:
    friend class QtSystem;
    SignalReceiver& m_receiver;
    const std::function<bool (GuiSignal* ptr)> function;
    QMetaObject::Connection connection;
    QString routineName;
    QString name;
    Index pendingLine;
    QStringList values;
};

}

#endif // GUISIGNAL_H
