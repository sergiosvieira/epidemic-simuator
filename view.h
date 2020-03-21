#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>

class QGraphicsScene;
class QWidget;

class View : public QGraphicsView
{
public:
    explicit View(QGraphicsScene* scene, QWidget* widget = nullptr);
    void paintEvent(QPaintEvent *event) override;
};

#endif // VIEW_H
