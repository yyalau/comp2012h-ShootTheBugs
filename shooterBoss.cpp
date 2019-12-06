#include "shooterBoss.h"

ShooterBoss::ShooterBoss(int hp, int dx, int dy, int shoot_freq, bool shoot,
                         int size_x, int size_y, int move_freq, int coll_freq) :
     ShooterBase("Boss", hp, dx, dy, shoot_freq, shoot, size_x, size_y, move_freq, coll_freq)
{
    //TODO: use another sprite
    QPixmap enemyimage(":/image/images/computer.png");
    setPixmap(enemyimage.scaled(size_x, size_y, Qt::KeepAspectRatio));
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
    setTransformOriginPoint(boundingRect().width()/2,boundingRect().height()/2);
    setScale(1.5);

    move_timer= new QTimer();
    connect(move_timer, SIGNAL(timeout()), this, SLOT(move())); //connect the timer and move slot

    coll_timer= new QTimer();
    connect(coll_timer, SIGNAL(timeout()), this, SLOT(collision())); //connect the timer and collision slot

    shoot_timer= new QTimer();
    connect(shoot_timer, SIGNAL(timeout()), this, SLOT(shoot())); //connect the timer and bullet slot

    //start the timers
    unpause();
}

ShooterBoss::~ShooterBoss()
{
    if (health_bar != nullptr) delete health_bar;
    if (pattern_name != nullptr) delete pattern_name;
    //TODO: add delete for timers? i forgot if overriden destructor calls base destructor
}

inline void ShooterBoss::set_shoot_freq(int shoot_freq)
{
    this->shoot_freq= shoot_freq;
    shoot_timer->start(shoot_freq);
}

void ShooterBoss::set_phase(BossPhase phase)
{
    this->phase = phase;
    switch (phase)
    {
        case Entrance:
        case Dialogue:
            //do nothing
            return;
        case PhasePre1:
            set_shoot_freq(4*MIN_FREQ);
            break;
        case Phase1:
            set_shoot_freq(12*MIN_FREQ);
            break;
        case PhasePre2:
            break;
        case Phase2:
            break;
        case Phase3:
            break;
    }
}

void ShooterBoss::show_health()
{
    health_bar = new PopUpDialogue(scene(), "", PopUpDialogue::BossHealth);

    //for debug
    health->setPos(SCREEN_LENGTH, 40);
    scene()->addItem(health);
    health->setDefaultTextColor(Qt::green);
}

void ShooterBoss::start_bossfight()
{
    if (phase != Dialogue) return; //wrong phase to call
    set_phase(PhasePre1);
}

void ShooterBoss::move()
{
    switch (phase)
    {
        case Entrance:
            setPos(x()+dx, y()+dy);
            if (y() >= 0) set_phase(Dialogue);
            break;
        case Dialogue:
        {
        //TODO: #define 250, 0 or something, basically boss's initial position (top center)
            double x_diff = 250 - pos().x();
            double y_diff = 0 - pos().y();
            if (x_diff*x_diff + y_diff*y_diff < 25)
            {
                dx = dy = 0;
                setPos(250, 0);
                if (boss_to_next_phase)
                {
                    for (int i=0; i<4; ++i)
                    {
                        if (health->get_health() == PHASE_HEALTH[i])
                        {
                            set_phase(static_cast<BossPhase>(i));
                            boss_to_next_phase = false;
                            return;
                        }
                    }
                }
            }
            else
            {
                dx = ((x_diff > 0) ? 1 : -1) *
                        static_cast<int>(cos(atan(abs(y_diff/x_diff)))*5);
                dy = ((y_diff > 0) ? 1 : -1) *
                        static_cast<int>(sin(atan(abs(y_diff/x_diff)))*5);
                setPos(x()+dx, y()+dy);
            }

            break;
        }
        case Phase1:
        case Phase2:
        case Phase3:
            //dont move in these phases
            break;
        case PhasePre1:
        {
            static int x_dir = 1;
            dy = 0;
            dx = x_dir * 3;
            if (x()+dx > 350) x_dir = -1;
            else if (x()+dx < 150) x_dir = 1;
            setPos(x()+dx, y());
            break;
        }
        case PhasePre2:
            //TODO
            break;
    }
}

void ShooterBoss::collision()
{
    if (phase == Entrance || phase == Dialogue) return;

    QList<QGraphicsItem*> colliding_items= scene()->collidingItems(this);

    for(int i=0; i<colliding_items.size(); ++i){
        if (typeid(*(colliding_items[i]))==typeid (BulletPlayer))
        {
            //delete the other bullet
            REMOVE_ENTITY(colliding_items[i])

            //decrease own health
            health->decrease_health();

            //TODO: change 1000 to max_hp
            //TODO: show health for individual phases?
            health_bar->set_width(static_cast<int>(static_cast<double>(health->get_health())/1000 * SCREEN_LENGTH));

            //TODO: do something when health reaches checkpoint, activate next phase
            if (phase == Phase3) continue;
            //phase should be from -1 to 2
            if (health->get_health() == PHASE_HEALTH[phase+1]) // use <= if decrease_health is > 1
            {
                switch (phase)
                {
                    case PhasePre1: //show Phase1's pattern name
                        pattern_name = new PopUpDialogue(scene(), "IndexOutOfBoundException", PopUpDialogue::Dialogue);
                        QTimer::singleShot(1000, this, SLOT(remove_pattern_name()));
                        break;
                    case PhasePre2: //show Phase2's pattern name
                        break;
                    case Phase2:    //show Phase3's pattern name
                        break;
                    default:
                        break;
                }

                set_phase(Dialogue);
                return;
            }

            //TODO: when health reach 0
        }
    }
}

void ShooterBoss::shoot()
{
    if (!is_shooting) return;

    switch (phase)
    {
        case Entrance:
        case Dialogue:
            //do nothing
            return;
        case PhasePre1:
        {
            int bullet_dx = rand()%20 - rand()%20;
            int bullet_dy = 10;
            BulletEnemy::BulletType bullet_type = BulletEnemy::Normal;
            shoot_bullet(new BulletEnemy(bullet_dx, bullet_dy, bullet_type));
            break;
        }
        case Phase1:
        {
            static double angle = 0;
            static int dir = 1;

            int bullet_dx = static_cast<int>(sin(angle)*12);
            int bullet_dy = static_cast<int>(cos(angle)*12);
            BulletEnemy::BulletType bullet_type = BulletEnemy::OutOfBound;

            if (angle > 1.1) dir = -1;  //PI/2 = 1.57
            else if (angle < 0.1) dir = 1;
            angle += dir * 0.08;

            shoot_bullet(new BulletEnemy(bullet_dx, bullet_dy, bullet_type));
            shoot_bullet(new BulletEnemy(-bullet_dx, bullet_dy, bullet_type));
            break;
        }
        case PhasePre2:
            break;
        case Phase2:
            break;
        case Phase3:
            break;
    }

}

void ShooterBoss::remove_pattern_name()
{
    delete pattern_name;
    boss_to_next_phase = true; //set flag to true for move() to switch to next phase
}
