#include "gameEvent.h"

GameEvent::GameEvent(QGraphicsScene* parentScene, ShooterPlayer* shooter) :
    parentScene(parentScene), shooter(shooter)
{

    event_timer= new QTimer();
    connect(event_timer, SIGNAL(timeout()), this, SLOT(increment_time())); //connect the timer and time increase slot
    event_timer->start(MIN_FREQ);

    connect(this, SIGNAL(time_reached(int)), this, SLOT(trigger_event(int))); //when time is reached, trigger game events

    //for pausing/unpausing the game
    connect(shooter, SIGNAL(pause_all()), this, SLOT(pause_game()));
    connect(shooter, SIGNAL(unpause_all()), this, SLOT(unpause_game()));

    //for handling game over
    connect(shooter, SIGNAL(player_dead()), this, SLOT(trigger_game_over()));

}

GameEvent::~GameEvent()
{
    clear_popup_screen();
    delete event_timer;
}

int GameEvent::get_time()
{
    return game_timer;
}

void GameEvent::increment_time()
{
    ++game_timer;
    //game_timer/50 = seconds that have passed in-game
    if ((game_timer % 200) == 0) //means every 4 seconds
    {
        emit time_reached(game_timer % 3);
    }
}

void GameEvent::trigger_event(int event_id)
{
    switch (event_id)
    {
        //TODO: create macro or template for enemy creation
        case 0:
        {
            ShooterEnemy* enemy = new ShooterEnemy(ShooterEnemy::Linear, ShooterEnemy::Random, 2, 0, 3);
            enemy->setPos(400 + rand()%300, 0);
            parentScene->addItem(enemy);
            break;
        }
        case 1:
        {
            ShooterEnemy* enemy = new ShooterEnemy(ShooterEnemy::Linear, ShooterEnemy::AimAtPlayer, 3, 4, 0);
            enemy->set_player(shooter);
            enemy->setPos(0, 50);
            parentScene->addItem(enemy);
            break;
        }
        case 2:
        {
            ShooterEnemy* enemy = new ShooterEnemy(ShooterEnemy::BorderBounce, ShooterEnemy::Random, 5, -10, 1);
            enemy->setPos(800-ENEMY_SIZE, 50);
            parentScene->addItem(enemy);
            break;
        }
        case 3:
        {
            ShooterEnemy* enemy = new ShooterEnemy(ShooterEnemy::Linear, ShooterEnemy::AimAtPlayer, 2, 0, 0,ENTITY_SIZE,ENTITY_SIZE,DEFAULT_FREQ,DEFAULT_FREQ,DEFAULT_SHOOT_FREQ*2);
            enemy->setPos(100, 100);
            enemy->set_player(shooter);
            parentScene->addItem(enemy);
            ShooterEnemy* enemy2 = new ShooterEnemy(ShooterEnemy::Linear, ShooterEnemy::AimAtPlayer, 2, 0, 0,ENTITY_SIZE,ENTITY_SIZE,DEFAULT_FREQ,DEFAULT_FREQ,DEFAULT_SHOOT_FREQ*2);
            enemy2->setPos(500, 100);
            enemy2->set_player(shooter);
            parentScene->addItem(enemy2);
            break;
        }
        default:
            break;
        }
}

//helper template
template <typename T>
bool try_pause(QGraphicsItem* item)
{
    if (typeid(*item) == typeid(T))
    {
        (dynamic_cast<T*>(item))->pause();
        return true;
    }
    return false;
}

void GameEvent::pause_game()
{
    QList<QGraphicsItem*> scene_items = parentScene->items(); //pause all items

    for(int i=0; i<scene_items.size(); ++i){
        if (try_pause<BulletEnemy>(scene_items[i])) continue;
        if (try_pause<BulletPlayer>(scene_items[i])) continue;
        if (try_pause<ShooterEnemy>(scene_items[i])) continue;
        if (try_pause<ShooterPlayer>(scene_items[i])) continue;
    }
    event_timer->stop();

    if(!shooter->get_health_var()->is_dead()) display_popup("\t Press P to continue", Qt::gray);
}

//helper template
template <typename T>
bool try_unpause(QGraphicsItem* item)
{
    if (typeid(*item) == typeid(T))
    {
        (dynamic_cast<T*>(item))->unpause();
        return true;
    }
    return false;
}
void GameEvent::unpause_game()
{
    clear_popup_screen();

    QList<QGraphicsItem*> scene_items = parentScene->items(); //pause all items

    for(int i=0; i<scene_items.size(); ++i){
        if (try_unpause<BulletEnemy>(scene_items[i])) continue;
        if (try_unpause<BulletPlayer>(scene_items[i])) continue;
        if (try_unpause<ShooterEnemy>(scene_items[i])) continue;
        if (try_unpause<ShooterPlayer>(scene_items[i])) continue;
    }
    event_timer->start(MIN_FREQ);
}

//gameover part

void GameEvent::trigger_game_over()
{
    pause_game(); //TODO: if we want to do like death animation or something, add it under here
    display_popup("\t YOU LOSE! RUNTIME ERROR!!", Qt::red, 0.7);
}


void GameEvent::display_popup(QString message, QColor color, double opacity, int x, int y, int width, int height)
{
    //popup a new scene
    popup_scene = new QGraphicsRectItem(x,y,width,height);
    QBrush brush;
    popup_scene->setBrush(color);
    popup_scene->setOpacity(opacity);
    parentScene->addItem(popup_scene);

    popup_text = new QGraphicsTextItem(message);
    QFont Font("Times", 16);
    popup_text->setFont(Font);

    popup_text->setPos(0,SCREEN_HEIGHT/2);
    parentScene->addItem(popup_text);
}

void GameEvent::clear_popup_screen()
{
    parentScene->removeItem(popup_scene);
    delete popup_scene;
    parentScene->removeItem(popup_text);
    delete popup_text;
}

