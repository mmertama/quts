/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#ifndef STORESYSTEM_H
#define STORESYSTEM_H

#include "subsystem.h"
#include "utils.h"
#include "qutsdll.h"

class QSettings;

namespace Quts {

class QUTS_EXPORT_API StoreSystem : public Subsystem {
    Q_OBJECT
public:
    StoreSystem(QObject* parent, QutsEngine& engine);
    ~StoreSystem();
    QString sysName() const;
    void inject();
    void create(const QStringList& creationParams);
    bool isValid() const {return m_settings;}
    void close();
    GUFU(read)
    GUFU(write)
    GUFU(contains)
    GUFU(container)
private:
    QScopedPointer<QSettings> m_settings;
};

}

#endif // STORESYSTEM_H
