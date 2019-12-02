#ifndef BULLETENEMY_H
#define BULLETENEMY_H

#include <QGraphicsRectItem>
#include <QObject>

#include "bulletBase.h"

/**
 * @brief The BulletEnemy class
 * Class for bullets shot by enemies
 * @include collision(): overrides bulletBase's function (UNUSED)
 */

class BulletEnemy:public BulletBase{

public:
    BulletEnemy(int dx = 0, int dy = 0, int size_x = BULLET_SIZE, int size_y = BULLET_SIZE,
                int move_freq = DEFAULT_FREQ, int coll_freq = DEFAULT_FREQ);
    virtual ~BulletEnemy() override =default;

public slots:
    virtual void collision() override;

};

#endif // BULLETENEMY_H

