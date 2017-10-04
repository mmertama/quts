/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#include "scope.h"

using namespace Quts;

QVariantList Scope::dumpMem() const {
    QVariantList map;
    for(const auto& k : m_values.keys()) {
        const auto v = m_values[k];
        if(!v.canConvert(QVariant::String)) {
            map.append(QString("%1:<%2>").arg(k).arg(v.typeName()));
        }
        map.append(QString("%1:%2").arg(k).arg(v.toString()));
    }
    return map;
}

QVariantList Scope::dumpData() const {
    QVariantList map;
    for(const auto& k : m_data.keys()) {
        const auto v = m_data[k];
        if(!v.canConvert(QVariant::String)) {
            map.append(QString("%1:<%2>").arg(k).arg(v.typeName()));
        }
        map.append(QString("%1:%2").arg(k).arg(v.toString()));
    }
    return map;
}

