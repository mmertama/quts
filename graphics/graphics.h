/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QPaintEvent>
#include <QPainter>
#include <QRasterWindow>
#include <QVector>
#include <QBackingStore>
#include <QTimer>


namespace Quts {

class GraphicsSystem;

class Visual {
public:
    virtual void draw(QPainter& painter) = 0;
    virtual ~Visual() = default;
};

class Line : public QVector<QPointF>, public Visual {
    void draw(QPainter& painter) {
        painter.drawPolyline(*this);
    }
};

class Pen : public QPen, public Visual {
public:
    Pen(const QPen& p) : QPen(p){}
    void draw(QPainter& painter);
};


class Graphics : public QRasterWindow {
public:
    Graphics(GraphicsSystem* sys);
    ~Graphics();
    void append(Visual* v) {m_visuals.append(QSharedPointer<Visual>(v));}
    void setBackground(const QColor& color);
    void create();
    void redraw();
    bool isReady() const {return m_backing;}
    void clear();
public:
    QPen currentPen;
    QBrush currentBrush;
private:
    void exposeEvent(QExposeEvent* event);
    void resizeEvent(QResizeEvent* resizeEvent);
private slots:
    void renderNow();
private:
    QVector<QSharedPointer<Visual>> m_visuals;
    GraphicsSystem* m_sys = nullptr;
    QColor m_backgroundColor = Qt::white;
    QScopedPointer<QBackingStore> m_backing;
    QTimer m_update;
};

}

#endif // GRAPHICS_H
