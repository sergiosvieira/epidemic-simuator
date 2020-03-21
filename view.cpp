#include "view.h"
#include <QGraphicsItem>
#include "person.h"

View::View(QGraphicsScene *scene, QWidget *widget):
    QGraphicsView(scene, widget)
{
//    setViewport(new QOpenGLWidget);
    setFixedSize(640, 480);
    setWindowTitle("Pandemic");
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setSceneRect(0, 0, 640, 480);
    fitInView(0, 0, 640, 480, Qt::KeepAspectRatio);
}

void View::paintEvent(QPaintEvent *event)
{
    QList<QGraphicsItem*> items = scene()->items();
    int infected_ = 0; int healthy_ = 0; int cured_ = 0; int dead_ = 0;
    for (QGraphicsItem* item: items)
    {
        Person* p = dynamic_cast<Person*>(item);
        if (p->getType() == Person::Type::Infected) ++infected_;
        else if (p->getType() == Person::Type::Healthy) ++healthy_;
        else if (p->getType() == Person::Type::Cured) ++cured_;
        else if (p->getType() == Person::Type::Dead) ++dead_;
    }
    QGraphicsView::paintEvent(event);
    QPainter painter(viewport());
    painter.setPen(Qt::blue);
    painter.setFont(QFont("Arial", 20));
    painter.drawText(10, 25, QString("Saudáveis: %1").arg(healthy_));
    painter.drawText(10, 50, QString("Infectados: %1").arg(infected_));
    painter.drawText(10, 75, QString("Curados: %1").arg(cured_));
    painter.drawText(10, 100, QString("Mortos: %1").arg(dead_));
}
