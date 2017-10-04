/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include "subsystem.h"
#include "qutsdll.h"

namespace Beeper {
class BeepPlayer;
}

namespace Quts {

class QUTS_EXPORT_API AudioSubsystem : public Subsystem {
    Q_OBJECT
public:
    static const QString NAME;
    QString sysName() const;
    AudioSubsystem(QObject* parent, QutsEngine& engine);
    ~AudioSubsystem();
private:
    void inject();
    void create(const QStringList& creationParams);
    bool isValid() const {return m_beepPlayer;}
    void close();
    bool isActive() const;
    bool beep(const QStringList& val);
    void clear();
private:
    bool wait(const QStringList& params);
private:
    QScopedPointer<Beeper::BeepPlayer> m_beepPlayer;
};

}

#endif // AUDIOSYSTEM_H
