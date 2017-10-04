/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#ifndef SIGNALRECEIVER_H
#define SIGNALRECEIVER_H

#include <QString>
#include <QVariant>

namespace Quts {

class GuiSignal;


class SignalReceiver {
public:
    virtual void pushc(const QVariant& item) = 0;
    virtual void expired(GuiSignal* sig) = 0;
};
}


#endif // SIGNALRECEIVER_H
