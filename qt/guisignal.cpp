/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#include "guisignal.h"


using namespace Quts;

GuiSignal::GuiSignal(QObject* parent, SignalReceiver& receiver, const QString& aname, std::function<bool (GuiSignal* ptr)> afunction) :
    QObject(parent), m_receiver(receiver), function(afunction), name(aname.trimmed()) {}

GuiSignal::~GuiSignal() {
}


#define SLOTFUNCTION(type) \
    void GuiSignal::slotFunction(const type& value) {  \
        m_receiver.pushc(value); \
        m_receiver.expired(this); \
    }

SLOTFUNCTION(QString)
SLOTFUNCTION(QVariantMap)
SLOTFUNCTION(QVariantList)
SLOTFUNCTION(QVariant)

void GuiSignal::slotFunction(int value) {
    m_receiver.pushc(value);
    m_receiver.expired(this);
}

void GuiSignal::slotFunction(qreal value) {
    m_receiver.pushc(value);
    m_receiver.expired(this);
}

void GuiSignal::slotFunction() {
    m_receiver.expired(this);
}
