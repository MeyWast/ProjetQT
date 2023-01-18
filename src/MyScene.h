#ifndef CPP_QT_TPMINIPROJET_MYSCENE_H
#define CPP_QT_TPMINIPROJET_MYSCENE_H
#include <QGraphicsScene>
#include <QPixmap>
#include <QTimer>
#include <QPainter>
#include <QRectF>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QVector>
#include <QRectF>
#include <QGraphicsScene>
#include <QPixmap>
#include <QTimer>
#include <QPainter>
#include "Items.h"
#include <QTimer>
#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTextStream>
#include "LabeledTextField.h"
#include <QDialog>

class MyScene : public QGraphicsScene {
    Q_OBJECT
private :
    QGraphicsTextItem* chronometre;
    QPixmap background;

    QVBoxLayout* mainLayout;

    QTimer* minuteur;
    QTimer* timer;
    QTimer* moustique;

    Meteorite* Moustique;

    QGraphicsPixmapItem * CuteGirl;
    QGraphicsPixmapItem * Crabe;
    QGraphicsPixmapItem * Bloc;
    QGraphicsPixmapItem * Bloc1;
    QGraphicsPixmapItem * Bloc2;
    QGraphicsPixmapItem * Bloc3;

    QLabel * success;
    QLabel * echec;
    LabeledTextField * pseudo;
    QString stringpseudo;
    QString bestscore;
    QString bestpseudo;
    bool reussi = false;
    bool nonreussi = false;

    QVector<MurGaucheItems*> pileMurGaucheItems;
    QVector<MurDroitItems*> pileMurDroitItems;
    QVector<SolItems*> pileSolItems;
    QVector<PiqueItems*> pilePiqueItems;
    QVector<PlafondItems*> pilePlafondItems;
    QVector<CheckpointItems*> pileCheckpointItems;
    QVector<FinishItems*> pileFinishItems;
    QVector<ArgentItems*> pileArgentItems;
    QVector<Meteorite*> pileMeteorite;

    QString etat = "vivant";
    QString deplacement;
    bool collisionGauche = false;
    bool collisionSol = false;
    bool collisionDroite = false;
    bool collisionPlafond = false;
    bool collisionFin = false;
    bool collisionCheckpoint = false;
    bool DeplacementDroite = false;
    bool DeplacementGauche = false;
    bool DeplacementHaut =false;
    float DeplacementX=0;
    float DeplacementY=0;
    int PositionXCheckpoint;
    int PositionYCheckpoint;
    int chrono=0;
    int health=0;
    int score=0;
    float DeplacementCrabe=0;
    int multiplicateurCrabe=1;
    int disparition = 0;
    int disparition1 = 0;
    int disparition2 = 0;
    int disparition3 = 0;
    bool bloc = true;
    bool bloc1 = true;
    bool bloc2 = true;
    bool bloc3 = true;
    int pixmapdroite = 1;
    int pixmapgauche = 1;
    int pixmapjump = 1;
    bool CuteGirlFaceDroite = true;

public:
    MyScene(QObject* parent = nullptr);
    virtual ~MyScene();
    void keyPressEvent(QKeyEvent* event);
    void getObstacle(int positionX, int positionY, int tailleX, int tailleY);
    void getWallJumpDroit(int PositionX, int PositionY, int tailleX, int tailleY);
    void getWallJumpGauche(int PositionX, int PositionY, int tailleX, int tailleY);
    void CreateDecor();

    void verificationMort();
    void verificationCollision();
    void verificationDeplacement();

    void drawBackground(QPainter* painter, const QRectF& rect);
    void createMenu();
    void readLeaderboard();

    int getScore(){return score;}
    QGraphicsPixmapItem * getCuteGirl(){return CuteGirl;}



protected:
    void keyReleaseEvent(QKeyEvent* event);

public slots :
    void update();
    void temps();
    void spawn();
    void quit();
    void setpseudo();
    void save();
    void newtry();

};


#endif //CPP_QT_TPMINIPROJET_MYSCENE_H
