#ifndef PERSON_H
#define PERSON_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QTime>

class Person : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
protected:
    int obj_id = 0;
    qreal speedX = 1.0;
    qreal speedY = 1.0;
    QTime* start = nullptr;
    qreal recovery_time;
    void init();
    void moveX();
    void moveY();
public:
    enum class Type{Healthy, Infected, Cured, Dead};
    explicit Person(QObject *parent = nullptr);
    void infected();
    void cured();
    void dead();
    Type getType();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
protected:
    Type type = Type::Healthy;
public slots:
    void move();
    void checkHealthy();
};

#endif // PERSON_H
