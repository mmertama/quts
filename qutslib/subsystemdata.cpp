/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#include "subsystemdata.h"
#include "subsystem.h"

#include <QDebug>

using namespace Quts;

QString SubsystemData::functionName(const QString& key) const {
#ifdef PRINT_A_LOT
    qDebug() << "function name" << key << m_name << m_sys->sysName();
#endif
    if(m_functions.contains(key) && (m_name.isEmpty())) {
        return key;
    }
    const auto pk = key.lastIndexOf(GLOBAL_SEP);
    if(pk >= 0) {
        const auto name = key.left(pk);
        const auto functionName = key.mid(pk + 1);
        if(m_functions.contains(functionName) &&
                //key is get from call, therefore name can be eq to sysname and
                //not need check
                name == m_name) {
            return functionName;
        }
    }
    return QString();
}

QString SubsystemData::libraryName() const {
    return m_libraryName;
}


bool SubsystemData::create() {
    if(!m_sys->isValid()) {
        m_sys->create(m_params);
    } else {
        m_sys->init(m_params);
    }
    return m_sys->isValid();
}

SubsystemData::SubsystemData() {}
SubsystemData::SubsystemData(Subsystem* system, const QString& name, const QString& libName, const QStringList& params)  :
    m_sys(system), m_name(name), m_libraryName(libName), m_params(params) {
    Q_ASSERT(m_name != INVALID_NAME);
}
