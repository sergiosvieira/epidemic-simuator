#include "person.h"
#include <QTimer>
#include <random>
#include <QDebug>
#include <QBrush>
#include <QGraphicsScene>
#include <QPainter>

static std::random_device rdev{};
static std::mt19937 eng{rdev()};
static qreal percent_change_direction = 65.;
static qreal max_speed = 2.;
static qreal percent_death = 2.;
static qreal percent_infect = 10.;

qreal random_min_max(uint16_t min, uint16_t max)
{
    std::uniform_real_distribution<double> urd(min, max);
    return urd(eng);
}

void Person::init()
{
    this->setBrush(Qt::white);
    this->speedX = random_min_max(1, max_speed);
    this->speedY = random_min_max(1, max_speed);
    this->setRect(0, 0, 5, 5);
    this->setPos(random_min_max(0, 640 - this->rect().width()),
                 random_min_max(0, 480 - this->rect().height()));
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(30/1000);
    if (random_min_max(0, 100) < percent_infect) this->type = Type::Infected;
    this->recovery_time = random_min_max(7000, 15000);
}

void Person::moveX()
{

    if (random_min_max(0, 100) > percent_change_direction) this->speedX *= -1.;
    if (this->x() + this->rect().width() + this->speedX > 640)
    {
        this->setX(640. - this->rect().width());
        this->speedX *= -1.;
        this->speedX += random_min_max(1, max_speed);
    } else if (this->x() < 0)
    {
        this->setX(0.);
        this->speedX *= -1.;
        this->speedX += random_min_max(1, max_speed);
    }
    if (speedX > max_speed) speedX = random_min_max(1, max_speed);
}

void Person::moveY()
{
    if (random_min_max(1, 100) > percent_change_direction) this->speedY *= -1.;
    if (this->y() + this->rect().height() + this->speedY > 480)
    {
        this->setY(480. - this->rect().height());
        this->speedY *= -1.;
        this->speedX += random_min_max(1, max_speed);
    } else if (this->y() < 0.)
    {
        this->setY(0.);
        this->speedY *= -1.;
        this->speedX += random_min_max(1, max_speed);
    }
    if (speedY > 5) speedY = random_min_max(1, max_speed);
}

Person::Person(QObject *parent) : QObject(parent)
{
    static int id = 0;
    this->obj_id = ++id;
    this->init();
}

void Person::infected()
{
    this->type = Type::Infected;
}

void Person::cured()
{
    this->type = Type::Cured;
}

void Person::dead()
{
    this->type = Type::Dead;
}

Person::Type Person::getType()
{
    return this->type;
}

void Person::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (this->type == Type::Healthy)
    {
        painter->setBrush(Qt::white);
    }
    else if (this->type == Type::Infected)
    {
        painter->setBrush(Qt::red);
    }
    else if (this->type == Type::Cured)
    {
        painter->setBrush(Qt::gray);
    }
    else if (this->type == Type::Dead)
    {
        painter->setBrush(Qt::black);
    }
    //painter->drawEllipse(-10, -20, 20, 40);
    painter->drawRect(0, 0, 3, 3);
}

void Person::move()
{
    if (this->type != Type::Dead)
    {
        if (this->start != nullptr)
        {

            if (this->start->elapsed() > this->recovery_time
                    && this->type == Type::Infected)
            {
//                qDebug() << "id:" << this->obj_id
//                         << "elapsed: " << this->start->elapsed();
                this->checkHealthy();
            }
            else if (this->type != Type::Cured)
            {
//                qInfo() << "id:" << this->obj_id
//                         << "elapsed: " << this->start->elapsed()
//                         << "recovery: " << this->recovery_time;
            }

        }
        this->moveX();
        this->setX(this->x() + this->speedX);
        this->moveY();
        this->setY(this->y() + this->speedY);
        if (this->scene())
        {
            QList<QGraphicsItem *> collided = scene()->collidingItems(this);
            for (QGraphicsItem* item: collided)
            {
                if (item != this)
                {
                    if (random_min_max(0, 100) < percent_infect
                            && this->type == Type::Infected)
                    {
                        Person* p = dynamic_cast<Person*>(item);
                        if (p->type == Type::Healthy)
                        {
                            p->infected();
                            this->start = new QTime();
                            this->start->start();
//                            QTimer *timer = new QTimer();
//                            connect(timer, SIGNAL(timeout()), p, SLOT(checkHealthy()));
//                            timer->setSingleShot(true);
//                            timer->start(static_cast<int>(random_min_max(7000, 15000)));
                        }
                    }
                }
            }
        }
    }
}

void Person::checkHealthy()
{
    if (this->type != Type::Dead)
    {
        qreal p = random_min_max(0, 100);
        if (p < percent_death
                && this->type == Type::Infected)
        {
            this->dead();
        }
        else this->cured();
    }
}
