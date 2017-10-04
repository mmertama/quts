/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#ifndef SUBSYSTEMDATA_H
#define SUBSYSTEMDATA_H

#include <QSet>
#include <QString>
#include "constants.h"

namespace Quts {

class Subsystem;

class SubsystemData {
public:
    SubsystemData();
    SubsystemData(Subsystem* system, const QString& name, const QString& libName, const QStringList& params);
    void append(const QString& function) {m_functions.insert(function);}
    const Subsystem* operator->() const {return m_sys;}
    Subsystem* operator->() {return m_sys;}
    Subsystem* data() {return m_sys;}
    const Subsystem* data() const {return m_sys;}
    QString functionName(const QString& key) const;
    QString name() const {return m_name;}
    bool setName(const QString& name) {if(name != INVALID_NAME && m_name != INVALID_NAME) return false; m_name = name; return true;}
    QString libraryName() const;
    bool create();
private:
    Subsystem* m_sys = nullptr;
    QSet<QString> m_functions;
    QString m_name = INVALID_NAME;
    QString m_libraryName;
    QStringList m_params;
};
}

#endif // SUBSYSTEMDATA_H
