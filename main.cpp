#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QtEvents>
#include <QDebug>
#include <QOpenGLWidget>
#include "person.h"
#include "view.h"

static qint32 population = 100;

int main(int argc, char *argv[])
{    
    QApplication app(argc, argv);
    QGraphicsScene* scene = new QGraphicsScene();
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    for (int i = 0; i < population; ++i)
    {
        Person* person = new Person();
        scene->addItem(person);
    }
    View* view = new View(scene);
    view->show();
    return app.exec();
}
