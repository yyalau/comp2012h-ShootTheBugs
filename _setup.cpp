#include "_setup.h"

/**
 * TODO: Document this more properly
 */

_SetUp::_SetUp()
{
    //-----------------------------------GameSetting-------------------------------
    //scene
    GameScene = new QGraphicsScene(); //create a scene
    GameScene->setSceneRect(0, 0, SCREEN_LENGTH+INFOBOX_LENGTH, SCREEN_HEIGHT);

    //view
    GameView = new QGraphicsView();
    GameView->setScene(GameScene); // view is scene's boss
    GameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //disable horizontal scroll bar
    GameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //disable vertical scroll bar
    GameView->setFixedSize(SCREEN_LENGTH+INFOBOX_LENGTH, SCREEN_HEIGHT); //set view and scene are of the same size

    //shooter
    shooter= new ShooterPlayer(); //create a rect
    GameScene->addItem(shooter); //put rect into the scene
    shooter->setFlag(QGraphicsItem::ItemIsFocusable); // setfocus1
    shooter->setFocus(); //setfocus2
    shooter->setPos(400, 500);// set the initial position of the shooter
    ShooterEnemy::set_player(shooter);

    //time counter to trigger events
    gameEvent = new GameEvent(GameScene, shooter);

    //------------------------------------------------InfoBoxSetting-------------------------------------------

    //add infoBox into the GameScene
    infoBox = new InfoBox(shooter->get_health_var());
    infoBox->setPos(SCREEN_LENGTH, 0);
    GameScene->addItem(infoBox);
    infoBox->show_info();

    //Layout Setting
    layout = new QHBoxLayout();
    layout->addWidget(GameView);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    window = new QWidget();
    window->setLayout(layout);
    window->show();

}

_SetUp::~_SetUp()
{
    //delete shooter;
    //delete gameEvent;
    //delete infoBox;

    //dont mess up the following order
    //delete GameView;
    //delete layout;
    //delete window;
    //delete GameScene;
}


