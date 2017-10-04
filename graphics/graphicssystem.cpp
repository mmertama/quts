/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#include "graphicssystem.h"
#include "graphics.h"
#include "variantutils.h"
#include "constants.h"

using namespace  Quts;


SUBSYSTEM(GRAPHICS) {
    return new GraphicsSystem((QObject*)parent,  * (QutsEngine*) engine);
}

QString GraphicsSystem::sysName() const {
    return GRAPHICS_NAME;
}

GraphicsSystem::GraphicsSystem(QObject* parent, QutsEngine& engine) :
    Subsystem(parent, engine), m_graphics(new Graphics(this)) {
}

void GraphicsSystem::inject() {
    GUFF(GraphicsSystem, background);
    GUFF(GraphicsSystem, resize);
    GUFF(GraphicsSystem, line);
    GUFF(GraphicsSystem, show);
    GUFF(GraphicsSystem, penColor);
}

void GraphicsSystem::create(const QStringList& creationParams) {
    Q_UNUSED(creationParams);
    m_graphics->create();
}

GraphicsSystem::~GraphicsSystem() {
}


bool GraphicsSystem::isValid() const {
    return m_graphics && m_graphics->isReady();
}

bool GraphicsSystem::background(const QStringList& values) {
    MIN_ARG(1);
    bool ok;
    const QColor color(getValue(values[0], &ok).toString());
    if(ok && color.isValid()) {
        m_graphics->setBackground(color);
    } else { return syntaxError(INVALID_ARGUMENT, values[0]); }
    return true;
}

bool GraphicsSystem::resize(const QStringList& values) {
    MIN_ARG(2);
    int width, height = 0;
    if(num(values[0], width) && num(values[1], height)) {
        m_graphics->resize(width, height);
    } else { return false; }
    return true;
}

void GraphicsSystem::init(const QStringList& creationParams){
    if(m_graphics)
        m_graphics->clear();
}


bool GraphicsSystem::show(const QStringList& values) {
    bool ok = true;
    const auto s = values.length() > 0 ? toBool(getValue(values[0], &ok)) : true;
    if(!ok) {
        return syntaxError(INVALID_ARGUMENT, values[0]);
    }
    if(s) {
        if(!m_graphics->isVisible()) {
            m_graphics->show();
        }
        m_graphics->redraw();
    } else {
        m_graphics->hide();
    }
    return true;
}

bool GraphicsSystem::numValue(const QString& p, qreal& r) {
    bool ok = false;
    const auto v = getValue(p, &ok);
    if(v.canConvert<qreal>()){
        r = v.toReal();
        return true;
    }
    syntaxError(INVALID_ARGUMENT, p);
    return false;
}

bool GraphicsSystem::line(const QStringList& values) {
    MIN_ARG(4);
    QScopedPointer<Line> line(new Line());

    for(auto u = 0; u < values.length(); u += 2) {
        qreal x, y = 0;
        if(numValue(values[u], x) && numValue(values[u + 1], y)) {
            line->append(QPointF(x, y));
        } else {
            return false;
        }
    }
    m_graphics->append(line.take());
    return true;
}


bool GraphicsSystem::penColor(const QStringList& values){
    const auto currentPen = m_graphics->currentPen;
    if(values.length() > 0){
        const QColor color(m_engine.getValue(values[0]).toString());
        if(!color.isValid())
                return syntaxError(INVALID_ARGUMENT, values[0]);
        auto pen = new Pen(currentPen);
        pen->setColor(color);
        m_graphics->currentPen = *pen;
        m_graphics->append(pen);
    }
    m_engine.pushc(currentPen.color().name(QColor::HexArgb));
    return  true;
}
