#include "gameEvent.h"

GameEvent::GameEvent(QGraphicsScene* parent_scene, ShooterPlayer* shooter, QString instructions) :
    parent_scene(parent_scene), shooter(shooter)
{

    event_timer= new QTimer();
    connect(event_timer, SIGNAL(timeout()), this, SLOT(increment_time())); //connect the timer and time increase slot

    connect(this, SIGNAL(time_reached(int)), this, SLOT(trigger_event(int))); //when time is reached, trigger game events

    //start game dialogue
    dialogue = new PopUpDialogue(parent_scene, instructions, NO_DURATION, PopUpDialogue::FullScreen);

    //for pausing/unpausing the game
    connect(shooter, SIGNAL(pause_all()), this, SLOT(pause_game()));
    connect(shooter, SIGNAL(unpause_all()), this, SLOT(unpause_game()));

    //powerup clearfield/ restart
    connect(shooter, SIGNAL(clear_field(bool)), this, SLOT(trigger_clear_field(bool)));

    //for handling game over
    connect(shooter, SIGNAL(player_dead()), this, SLOT(trigger_game_over()));

}

GameEvent::~GameEvent()
{
    if (dialogue != nullptr) delete dialogue;
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
    if ((game_timer % 50) == 0) //means every second
    {

        switch (game_timer/50-1) {
        case 3: time_reached(0); break;         //Game starts at 3s,
        case 8: time_reached(1); break;         //for first 5 waves of enemy, each lasts for 5s,
        case 13: time_reached(2); break;        //after that, each lasts for 10s
        case 18: time_reached(3); break;
        case 23: time_reached(4); break;
        case 33: time_reached(5); break;
        case 43: time_reached(6); break;
        case 53: time_reached(7); break;
        case 63: time_reached(9); break;

        }

        //emit time_reached((game_timer/200)-1 % 5);
        //emit time_reached(6);
    }
    if ((game_timer % 300)==0)
    {
        emit time_reached(8);
    }
}

void GameEvent::trigger_event(int event_id)
{
    switch (event_id)
    {
        //TODO: create macro or template for enemy creation
        //if macro put it in define? shooterBoss also needs it
        case 0:
        {
            QList<QGraphicsItem*> itemsInScreen=parent_scene->items(0,0,800,600,Qt::IntersectsItemShape,Qt::AscendingOrder);
            for(int i=0; i<itemsInScreen.size();++i){
               if (typeid(*(itemsInScreen[i]))==typeid(ShooterEnemy)){
                   parent_scene->removeItem(itemsInScreen[i]);
                   delete itemsInScreen[i];
               }
            }
             ShooterEnemy* enemy = spawn_enemy(ShooterEnemy::GotoTarget, ShooterEnemy::Random, 5, 0 ,0, 0, 0, DEFAULT_SHOOT_FREQ*4);
             ShooterEnemy* enemy2 = spawn_enemy(ShooterEnemy::GotoTarget, ShooterEnemy::Random, 5, 0 ,0, 700, 0, DEFAULT_SHOOT_FREQ*4);
             enemy->set_targetPos(250, 150);
             enemy2->set_targetPos(450, 150);
             break;
        }
        case 1:
        {
            QList<QGraphicsItem*> itemsInScreen=parent_scene->items(0,0,800,600,Qt::IntersectsItemShape,Qt::AscendingOrder);
            for(int i=0; i<itemsInScreen.size();++i){
               if (typeid(*(itemsInScreen[i]))==typeid(ShooterEnemy)){
                   parent_scene->removeItem(itemsInScreen[i]);
                   delete itemsInScreen[i];
               }
            }

             spawn_enemy(ShooterEnemy::Linear, ShooterEnemy::Random, 2, 0, 2,100+rand()%200, 0);

             spawn_enemy(ShooterEnemy::Linear, ShooterEnemy::Random, 2, 0, 2,400+rand()%200, 0);
            break;
        }
        case 2:
        {
            //ShooterEnemy* enemy = new ShooterEnemy(ShooterEnemy::BorderBounce, ShooterEnemy::Random, 5, -10, 0);
            //enemy->setPos(800-ENEMY_SIZE, 50);
            QList<QGraphicsItem*> itemsInScreen=parent_scene->items(0,0,800,600,Qt::IntersectsItemShape,Qt::AscendingOrder);
            for(int i=0; i<itemsInScreen.size();++i){
               if (typeid(*(itemsInScreen[i]))==typeid(ShooterEnemy)){
                   parent_scene->removeItem(itemsInScreen[i]);
                   delete itemsInScreen[i];
               }
            }
            spawn_enemy(ShooterEnemy::BorderBounce, ShooterEnemy::Forward, 5, -4, 0, 800-ENEMY_SIZE, 50,DEFAULT_SHOOT_FREQ*3);
            spawn_enemy(ShooterEnemy::BorderBounce, ShooterEnemy::Forward, 5, 3, 0, 0, 120, DEFAULT_SHOOT_FREQ*3);
            spawn_enemy(ShooterEnemy::BorderBounce, ShooterEnemy::Forward, 5, -4, 0, 800-ENEMY_SIZE, 200,DEFAULT_SHOOT_FREQ*3);
            break;
        }
        case 3:
        {
            QList<QGraphicsItem*> itemsInScreen=parent_scene->items(0,0,800,600,Qt::IntersectsItemShape,Qt::AscendingOrder);
            for(int i=0; i<itemsInScreen.size();++i){
               if (typeid(*(itemsInScreen[i]))==typeid(ShooterEnemy)){
                   parent_scene->removeItem(itemsInScreen[i]);
                   delete itemsInScreen[i];
               }
            }
            QPainterPath ellipse;
            ellipse.addEllipse(100,50,600,200);
            ShooterEnemy* enemy = spawn_enemy(ShooterEnemy::Circular, ShooterEnemy::Forward, 5, 0, 0,699,157);
            ShooterEnemy* enemy2 = spawn_enemy(ShooterEnemy::Circular, ShooterEnemy::Forward, 5, 0, 0,254,237);
            ShooterEnemy* enemy3 = spawn_enemy(ShooterEnemy::Circular, ShooterEnemy::Forward, 5, 0, 0,238,66);
            //qDebug()<<ellipse.pointAtPercent(0.01).toPoint()<<ellipse.pointAtPercent(0.33).toPoint()<<ellipse.pointAtPercent(0.66).toPoint();
            break;
        }
        case 4:
        {
            QList<QGraphicsItem*> itemsInScreen=parent_scene->items(0,0,800,600,Qt::IntersectsItemShape,Qt::AscendingOrder);
            for(int i=0; i<itemsInScreen.size();++i){
               if (typeid(*(itemsInScreen[i]))==typeid(ShooterEnemy)){
                   parent_scene->removeItem(itemsInScreen[i]);
                   delete itemsInScreen[i];
               }
            }
            ShooterEnemy* enemy[6];
            for(int i=0; i<5;++i){
                enemy[i]=spawn_enemy(ShooterEnemy::GotoTarget, ShooterEnemy::Random, 2,0,0,160*i,0,DEFAULT_SHOOT_FREQ*6);
                enemy[i]->set_targetPos(160*i+80,150+(i>2?(4-i)*50:i*50));
            }
            ShooterEnemy* enemy2=spawn_enemy(ShooterEnemy::GotoTarget, ShooterEnemy::Random, 3,0,0,450,0,DEFAULT_SHOOT_FREQ*4);
            enemy2->set_targetPos(400,70);
            break;
        }
        case 5:
        {
            QList<QGraphicsItem*> itemsInScreen=parent_scene->items(0,0,800,600,Qt::IntersectsItemShape,Qt::AscendingOrder);
            for(int i=0; i<itemsInScreen.size();++i){
               if (typeid(*(itemsInScreen[i]))==typeid(ShooterEnemy)){
                   parent_scene->removeItem(itemsInScreen[i]);
                   delete itemsInScreen[i];
               }
            }
            spawn_enemy(ShooterEnemy::BorderBounce,ShooterEnemy::Double, 5,5,2,0,50,DEFAULT_SHOOT_FREQ*4);
            spawn_enemy(ShooterEnemy::BorderBounce,ShooterEnemy::Double, 5,-5,2,800-ENEMY_SIZE,50,DEFAULT_SHOOT_FREQ*4);
            spawn_enemy(ShooterEnemy::BorderBounce,ShooterEnemy::Double, 5,5,-2,0,300,DEFAULT_SHOOT_FREQ*4);
            spawn_enemy(ShooterEnemy::BorderBounce,ShooterEnemy::Double, 5,-5,-2,800-ENEMY_SIZE,300,DEFAULT_SHOOT_FREQ*4);
            break;
        }
        case 6:
        {
            QList<QGraphicsItem*> itemsInScreen=parent_scene->items(0,0,800,600,Qt::IntersectsItemShape,Qt::AscendingOrder);
            for(int i=0; i<itemsInScreen.size();++i){
               if (typeid(*(itemsInScreen[i]))==typeid(ShooterEnemy)){
                   parent_scene->removeItem(itemsInScreen[i]);
                   delete itemsInScreen[i];
               }
            }
            ShooterEnemy* enemy[4];

            enemy[0]=spawn_enemy(ShooterEnemy::GotoTarget, ShooterEnemy::AimAtPlayer, 3,0,0,50,0,DEFAULT_SHOOT_FREQ*4);
            enemy[1]=spawn_enemy(ShooterEnemy::GotoTarget, ShooterEnemy::AimAtPlayer, 3,0,0,750,0,DEFAULT_SHOOT_FREQ*4);
            enemy[2]=spawn_enemy(ShooterEnemy::GotoTarget, ShooterEnemy::AimAtPlayer, 3,0,0,150,0,DEFAULT_SHOOT_FREQ*4);
            enemy[3]=spawn_enemy(ShooterEnemy::GotoTarget, ShooterEnemy::AimAtPlayer, 3,0,0,650,0,DEFAULT_SHOOT_FREQ*4);
            enemy[0]->set_targetPos(100,100);
            enemy[1]->set_targetPos(700, 100);
            enemy[2]->set_targetPos(300,150);
            enemy[3]->set_targetPos(500,150);
            break;
        }
        case 7:
        {
            QList<QGraphicsItem*> itemsInScreen=parent_scene->items(0,0,800,600,Qt::IntersectsItemShape,Qt::AscendingOrder);
            for(int i=0; i<itemsInScreen.size();++i){
               if (typeid(*(itemsInScreen[i]))==typeid(ShooterEnemy)){
                   parent_scene->removeItem(itemsInScreen[i]);
                   delete itemsInScreen[i];
               }
            }
            ShooterEnemy* enemy[3];
            enemy[0]=spawn_enemy(ShooterEnemy::Wave,ShooterEnemy::Forward,5,0,1,0,200,DEFAULT_SHOOT_FREQ*3);
            enemy[0]->set_targetPos(0,120);
            enemy[1]=spawn_enemy(ShooterEnemy::Wave,ShooterEnemy::Forward,5,0,1,800-ENEMY_SIZE,400,DEFAULT_SHOOT_FREQ*3);
            enemy[1]->set_targetPos(750,120);
            enemy[2]=spawn_enemy(ShooterEnemy::Wave,ShooterEnemy::Forward,3,10,0,400,300,DEFAULT_SHOOT_FREQ*3);
            enemy[2]->set_targetPos(0,250);
           break;
        }
        case 8:
        {
            BulletPowerUp* bullet_powerup=new BulletPowerUp(rand()%10+1, rand()%10+1);
            bullet_powerup->setPos(rand()%GAMEAREA_LENGTH/2,0);
            parent_scene->addItem(bullet_powerup);
            break;
        }
        case 9:
        {
            ShooterBoss* boss = new ShooterBoss();
            boss->setPos(250, -200);
            parent_scene->addItem(boss);
            boss->show_health();
            //boss emits a signal at phase 3 to player
            connect(boss, SIGNAL(start_phase3()), shooter, SLOT(begin_phase3()));
            break;
        }
        default:
            break;
        }
}

ShooterEnemy* GameEvent::spawn_enemy(ShooterEnemy::EnemyPathingType pathing_type, ShooterEnemy::EnemyShootingType shooting_type, int hp, int dx, int dy,int initX, int initY, int shoot_freq){
    ShooterEnemy* enemy = new ShooterEnemy(pathing_type, shooting_type, hp, dx, dy, shoot_freq);
    enemy->setPos(initX, initY);
    parent_scene->addItem(enemy);
    parent_scene->addItem(enemy->get_health_var());
    return enemy;
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
    QList<QGraphicsItem*> scene_items = parent_scene->items(); //pause all items

    for(int i=0; i<scene_items.size(); ++i){
        if (try_pause<BulletEnemy>(scene_items[i])) continue;
        if (try_pause<BulletPlayer>(scene_items[i])) continue;
        if (try_pause<BulletPowerUp>(scene_items[i])) continue;
        if (try_pause<ShooterEnemy>(scene_items[i])) continue;
        if (try_pause<ShooterPlayer>(scene_items[i])) continue;
        if (try_pause<ShooterBoss>(scene_items[i])) continue;
    }
    event_timer->stop();

    //pause screen
    //TODO: make it more interesting
    if(!shooter->get_health_var()->is_dead())
    {
        dialogue = new PopUpDialogue(parent_scene, "\t Press P to continue", NO_DURATION, PopUpDialogue::GameArea);
    }
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
    if (dialogue != nullptr) delete dialogue;

    QList<QGraphicsItem*> scene_items = parent_scene->items(); //unpause all items

    for(int i=0; i<scene_items.size(); ++i){
        if (try_unpause<BulletEnemy>(scene_items[i])) continue;
        if (try_unpause<BulletPlayer>(scene_items[i])) continue;
        if (try_unpause<BulletPowerUp>(scene_items[i])) continue;
        if (try_unpause<ShooterEnemy>(scene_items[i])) continue;
        if (try_unpause<ShooterPlayer>(scene_items[i])) continue;
        if (try_unpause<ShooterBoss>(scene_items[i])) continue;
    }
    event_timer->start(MIN_FREQ);
}

void GameEvent::trigger_clear_field(bool restart)
{
    //TODO: DELETE THE BOSS LMAO I FORGOT
    if (restart)
    {
        game_timer=0;
        shooter->get_health_var()->reset_health();
        shooter->setPos(START_POS_X, START_POS_Y);
    }

    QList<QGraphicsItem*> scene_items = parent_scene->items();
    for(int i=0; i<scene_items.size(); i++)
    {
        if (typeid(*(scene_items[i]))==typeid (BulletEnemy)||typeid(*(scene_items[i]))==typeid (ShooterEnemy))
        {
            parent_scene->removeItem(scene_items[i]);
            delete scene_items[i];
        }

        if (restart&& typeid(*(scene_items[i]))==typeid (BulletPowerUp)){
            parent_scene->removeItem(scene_items[i]);
            delete scene_items[i];
        }
    }
}

//gameover part
void GameEvent::trigger_game_over()
{
    pause_game(); //TODO: if we want to do like death animation or something, add it under here
    dialogue = new PopUpDialogue(parent_scene, "\t YOU LOSE! RUNTIME ERROR!!", NO_DURATION, PopUpDialogue::GameArea);
}

