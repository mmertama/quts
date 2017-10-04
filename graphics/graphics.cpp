/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#include "graphics.h"

using namespace Quts;

Graphics::Graphics(GraphicsSystem* sys) : m_sys(sys) {
    m_update.setSingleShot(true);
    m_update.setInterval(0);
    QObject::connect(&m_update, &QTimer::timeout, this, &Graphics::renderNow);
}

void Graphics::create() {
    m_backing.reset(new QBackingStore(this));
    QRasterWindow::create();
    setGeometry(100, 100, 640, 480);
    m_visuals.clear();
}

void Graphics::exposeEvent(QExposeEvent* event) {
    Q_UNUSED(event);
    if(isExposed()) {
        renderNow();
    }
}

void Graphics::setBackground(const QColor& color){
    m_backgroundColor = color;
}

void Graphics::resizeEvent(QResizeEvent* resizeEvent) {
    if(m_backing) {
        m_backing->resize(resizeEvent->size());
        if(isExposed()) {
            renderNow();
        }
    }
}


Graphics::~Graphics() {
    for(auto v : m_visuals) {
        v.reset();// delete v;
    }
}

void Graphics::clear(){
    m_visuals.clear();
    redraw();
}

void Graphics::renderNow() {
    if(!isExposed() || !m_backing || m_visuals.isEmpty()) {
        return;
    }

    currentBrush = QBrush();
    currentPen = QPen();

    QRect rect(0, 0, width(), height());
    m_backing->beginPaint(rect);

    QPaintDevice* device = m_backing->paintDevice();

    QPainter painter(device);
    painter.setRenderHint(QPainter::Antialiasing);
    QBrush brush(m_backgroundColor);
    painter.setBackground(brush);

    painter.fillRect(rect, brush);
    painter.setPen(currentPen);
    painter.setBrush(currentBrush);
    painter.eraseRect(rect);
    for(const auto& v : m_visuals) {
        v->draw(painter);
    }

    m_backing->endPaint();
    m_backing->flush(rect);

    m_visuals.clear();
}

void Graphics::redraw() {
    if(!m_update.isActive()) {
        m_update.start();
    }
}

void Pen::draw(QPainter& painter) {
    painter.setPen(*this);
}



