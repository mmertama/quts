/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "subsystem.h"
#include "qutsdll.h"

namespace Quts {

class Graphics;

class QUTS_EXPORT_API GraphicsSystem : public Subsystem {
    Q_OBJECT
public:
    GraphicsSystem(QObject* parent, QutsEngine& engine);
    void inject();
    QString sysName() const;
    bool isValid() const;
    void create(const QStringList& creationParams);
    void init(const QStringList& creationParams);
    ~GraphicsSystem();
    GUFU(background)
    GUFU(resize)
    GUFU(line)
    GUFU(show)
    GUFU(penColor)
private:
    bool numValue(const QString& p, qreal& r);
private:
    QScopedPointer<Graphics> m_graphics;
};

}


#endif
