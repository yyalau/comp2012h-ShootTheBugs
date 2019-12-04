#include "bulletEnemy.h"
#include "bulletPlayer.h"

BulletPlayer::BulletPlayer(int dx, int dy, int size_x, int size_y, int move_freq/*, int coll_freq*/):
    BulletBase(dx, dy, size_x, size_y, move_freq/*, coll_freq*/)
{
    QPixmap bulletimage(":/image/images/firebullet.png");
    setPixmap(bulletimage.scaled(size_x, size_y, Qt::KeepAspectRatio));
    setRotation(-90);
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
    setTransformOriginPoint(boundingRect().width()/2,boundingRect().height()/2);
    setScale(1.2);

//    coll_timer= new QTimer();

//    connect timer and collision slot
//    connect(coll_timer, SIGNAL(timeout()), this, SLOT(collision()));

    //start the timers
    unpause();
}

//void BulletPlayer::pause()
//{
//    move_timer->stop();
//    //coll_timer->stop();
//}

//void BulletPlayer::unpause()
//{
//    move_timer->start(move_freq);
//    //coll_timer->start(coll_freq);
//}

//void BulletPlayer::collision()
//{
//    //collision with other bullets
//    QList<QGraphicsItem*> colliding_items= scene()->collidingItems(this);

//    for(int i=0; i<colliding_items.size(); ++i){
//        if (typeid(*(colliding_items[i]))==typeid (BulletPowerUp))
//        {
//            //delete the other bullet //TODO: remove?
//            REMOVE_ENTITY(colliding_items[i])

//            //delete yourself
//            REMOVE_ENTITY(this)

//            return;
//        }
//    }
//}
