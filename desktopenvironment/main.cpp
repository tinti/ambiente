#include <qapplication.h>
#include <QGraphicsView>

#include "windowsystem/windowsystem.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QGraphicsScene *scene = new QGraphicsScene;
    scene->setSceneRect(0, 0, 800, 600);

    QGraphicsView *view = new QGraphicsView(scene);
    view->show();

    new WindowSystem(scene);

    return app.exec();
}
