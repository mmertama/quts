/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */


#include "subsystem.h"
#include "objectfinder.h"
#include "qutsengine.h"
#include "utils.h"


using namespace Quts;

Subsystem::Subsystem(QObject* parent, QutsEngine& engine): QObject(parent),
    m_engine(engine) {
}

bool Subsystem::isActive() const {return false;}
void Subsystem::clear() {}
void Subsystem::tick() {}
void Subsystem::requestResume() {}
void Subsystem::close() {}
void Subsystem::create(const QStringList& creationParams) {
    Q_UNUSED(creationParams);
}
void Subsystem::init(const QStringList& creationParams) {
    Q_UNUSED(creationParams);
}

