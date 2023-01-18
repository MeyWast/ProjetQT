#include "MyScene.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QTimer>
#include <QPointF>
#include <QDebug>
#include <iostream>
#include <cstring>
#include <cmath>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include "Items.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QVector>
#include <iostream>
#include <QFont>
#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>
#include <QFormLayout>
#include <cstdlib>
#include <QPushButton>
#include "LabeledTextField.h"
#include <QDialog>
#include <QTextEdit>
#include <iostream>
#include <fstream>
#include <QCoreApplication>
using namespace std;

MyScene::MyScene(QObject* parent) : QGraphicsScene(parent)
{
    //scene
    background.load("../ressources/Décor/Background.png");
    setSceneRect(0, 0, background.width(), background.height());

    //timer pour notre slot update qui gère collisions/deplacement/mort
    timer = new QTimer(this);
    timer->start(50);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    //on crée notre perso et on l'ajoute à la scène
    CuteGirl = new QGraphicsPixmapItem(QPixmap("../ressources/CuteGirl/idle (1).png"));
    (*getCuteGirl()).setPos(10,600);
    addItem(CuteGirl);

    //on crée le timer pour notre slot temps qui gère le chrono/nombre de vie perdues
    chronometre = new QGraphicsTextItem;
    minuteur = new QTimer(this);
    (*chronometre).setDefaultTextColor(Qt::blue);
    (*chronometre).setFont(QFont("times",16));
    connect(minuteur, SIGNAL(timeout()), this, SLOT(temps()));
    minuteur->start(1000);
    addItem(chronometre);

    //on cree le timer pour notre slot qui gère l'apparition de météorite
    moustique = new QTimer();
    connect(moustique,SIGNAL(timeout()),this,SLOT(spawn()));
    moustique->start(150);

    //on génére le décor
    CreateDecor();

};

void MyScene::update() {
    // on update la position de notre timer en fonction de la position de notre perso
    (*chronometre).setPlainText(QString("Temps: ") + QString::QString::number(chrono) + QString(" secondes") + QString(" - Vie Perdues: ") + QString::QString::number(health));
    (*chronometre).setPos((*getCuteGirl()).pos().x()-150, (*getCuteGirl()).pos().y()-200);

    // on update notre système de collision, considéré à false au début
    collisionSol = false;
    collisionGauche = false;
    collisionDroite = false;
    collisionPlafond = false;
    collisionFin = false;

    // on centre la view sur notre perso
    for(QGraphicsView * MyView: (*this).views())
    {
        (*MyView).centerOn(CuteGirl);

    }

    // on vérifie l'état de notre perso et agit en fonction
    verificationMort();

    // on update le perso en fonction des variables qu'on incremente en fonction des touches qu'on appuie
    QPointF pos = (*CuteGirl).pos();
    (*CuteGirl).setPos(pos.rx()+DeplacementX, pos.ry()+DeplacementY);

    // on vérifie si notre perso est en collision et agit en conséquences
    verificationCollision();

    // on vérifie les déplacements et agit en conséquences
    verificationDeplacement();

}

MyScene::~MyScene() {

}

//on agit on fonction des touches relachées, on set nos booléens en fonction qui correspond à notre déplacement
void MyScene::keyReleaseEvent(QKeyEvent *event)
{
    if((*event).key() == Qt::Key_Right){
        DeplacementDroite = false;
    }
    if((*event).key() == Qt::Key_Left){
        DeplacementGauche = false;
    }
}

//on agit on fonction des touches sur lesquelles on appuie, on set nos booléens en fonction qui correspond à notre déplacement
// j'ai mal gérer mes blocs qui disparaissent, je les initialises un par un
void MyScene::keyPressEvent(QKeyEvent *event)
{
    if((*event).key() == Qt::Key_Right)
    {
        DeplacementDroite = true;
    }

    if((*event).key() == Qt::Key_Left)
    {
        DeplacementGauche = true;
    }

    if((*event).key() == Qt::Key_Up)
    {
        for(auto wall : pileSolItems)
        {
            if((*getCuteGirl()).collidesWithItem(wall))
            {
                DeplacementHaut = true;
            }
        }
        if((*getCuteGirl()).collidesWithItem(Bloc))
        {
            DeplacementHaut = true;
        }
        if((*getCuteGirl()).collidesWithItem(Bloc1))
        {
            DeplacementHaut = true;
        }
        if((*getCuteGirl()).collidesWithItem(Bloc2))
        {
            DeplacementHaut = true;
        }

        if((*getCuteGirl()).collidesWithItem(Bloc3))
        {
            DeplacementHaut = true;
        }
    }
}

//fonction qui crée un obstacle en fonction des variables mis en arguments, on a repris la méthode utilisé dans le tp4 avec le systeme de pile
void MyScene::getObstacle(int positionX, int positionY, int tailleX, int tailleY)
{
    if(tailleY <= 30)
    {
        pileSolItems.push_back(new SolItems("../ressources/Décor/Sol.png", positionX, positionY, tailleX, 10));

        pileMurGaucheItems.push_back(new MurGaucheItems("../ressources/Décor/MurGauche.png", positionX, positionY+10, tailleX*0.5, tailleY-20));

        pilePlafondItems.push_back(new PlafondItems("../ressources/Décor/Plafond.png", positionX, positionY+tailleY-10, tailleX, 10));

        pileMurDroitItems.push_back(new MurDroitItems("../ressources/Décor/MurDroit.png", positionX + tailleX*0.5, positionY+10, tailleX*0.5, tailleY-20));
    }
    else{
        pileSolItems.push_back(new SolItems("../ressources/Décor/Sol.png", positionX, positionY, tailleX, 20));

        pileMurGaucheItems.push_back(new MurGaucheItems("../ressources/Décor/MurGauche.png", positionX, positionY+10, tailleX*0.5, tailleY-30));

        pilePlafondItems.push_back(new PlafondItems("../ressources/Décor/Plafond.png", positionX, positionY+tailleY-20, tailleX, 10));

        pileMurDroitItems.push_back(new MurDroitItems("../ressources/Décor/MurDroit.png", positionX + tailleX*0.5, positionY+10, tailleX*0.5, tailleY-30));
    }
}

//slot qui me permet d'augmenter le chrono et le score, mais aussi le spawn/disparition des blocs
void MyScene::temps()
{
    //augmente le chrono si je n'ai pas atteint la fin
    if(!collisionFin)
    {
        chrono++;
        score = chrono + health*30;
    }

    //augmente une variable qui gere la disparition du bloc
    if((*CuteGirl).collidesWithItem(Bloc))
    {
        disparition++;
//        cout << "Collision avec le bloc" << endl;
    }
    else
    {
        // fait respawn le bloc
        if(bloc == false)
        {
            disparition = 0;
            (*Bloc).setOpacity(1);
//            cout << "Le bloc a respawn" << endl;
            bloc = true;
        }
    }

    //même chose que le premier bloc
    if((*CuteGirl).collidesWithItem(Bloc1))
    {
        disparition1++;
    }
    else
    {
        if(bloc1 == false)
        {
            disparition1 = 0;
            (*Bloc1).setOpacity(1);
            cout << "Le bloc1 a respawn" << endl;
            bloc1 = true;
        }
    }

    //même chose que le premier bloc
    if((*CuteGirl).collidesWithItem(Bloc2))
    {
        disparition2++;
    }
    else
    {
        if(bloc2 == false)
        {
            disparition2 = 0;
            (*Bloc2).setOpacity(1);
            cout << "Le bloc2 a respawn" << endl;
            bloc2 = true;
        }
    }

    //même chose que le premier bloc
    if((*CuteGirl).collidesWithItem(Bloc3))
    {
        disparition3++;
    }
    else
    {
        if(bloc3 == false)
        {
            disparition3 = 0;
            (*Bloc3).setOpacity(1);
//            cout << "Le bloc3 a respawn" << endl;
            bloc3 = true;
        }
    }

}

//crée le décor
void MyScene::CreateDecor()
{
    getObstacle(0, 1000, 300, 100);

    getObstacle(400, 940, 60, 30);

    getObstacle(520, 880, 60, 30);

    getObstacle(660, 820, 60, 30);

    getObstacle(820, 760, 1500, 340);

    getObstacle(940, 560, 300, 100);

    getObstacle( 1820, 680, 100, 80);

    getObstacle( 2000, 600, 120, 80);

    getObstacle( 1330, 520, 530, 80);

    getObstacle( 1100, 400, 100, 80);

    getObstacle( 1350, 320, 1000, 80);

    pilePiqueItems.push_back(new PiqueItems("../ressources/Décor/Cactus.png",1650, 280, 15, 40));
    pilePiqueItems.push_back(new PiqueItems("../ressources/Décor/Cactus.png",1670, 280, 15, 40));
    pilePiqueItems.push_back(new PiqueItems("../ressources/Décor/Cactus.png",1690, 280, 15, 40));

    pilePiqueItems.push_back(new PiqueItems("../ressources/Décor/Cactus.png",1850, 280, 15, 40));
    pilePiqueItems.push_back(new PiqueItems("../ressources/Décor/Cactus.png",1870, 280, 15, 40));
    pilePiqueItems.push_back(new PiqueItems("../ressources/Décor/Cactus.png",1890, 280, 15, 40));

    pilePiqueItems.push_back(new PiqueItems("../ressources/Décor/Cactus.png",2050, 280, 15, 40));
    pilePiqueItems.push_back(new PiqueItems("../ressources/Décor/Cactus.png",2070, 280, 15, 40));
    pilePiqueItems.push_back(new PiqueItems("../ressources/Décor/Cactus.png",2090, 280, 15, 40));

    pileCheckpointItems.push_back(new CheckpointItems("../ressources/Décor/Checkpoint.png", 960, 480, 80, 80));
    pileCheckpointItems.push_back(new CheckpointItems("../ressources/Décor/Checkpoint.png", 2300, 240, 80, 80));

    Crabe = new CrabeItems("../ressources/Décor/Crabe.png", 1330, 480, 40, 40);
    addItem(Crabe);

    Bloc= new BlocItems("../ressources/Décor/StoneBlock.png", 2400, 400, 50, 50);
    addItem(Bloc);
    Bloc1 =new BlocItems("../ressources/Décor/StoneBlock.png", 2600, 600, 50, 50);
    addItem(Bloc1);
    Bloc2 = new BlocItems("../ressources/Décor/StoneBlock.png", 2800, 800, 50, 50);
    addItem(Bloc2);

    getObstacle(3000, 1000, 1000, 100);
    Bloc3 = new BlocItems("../ressources/Décor/StoneBlock.png", 4000, 1000, 150, 100);
    addItem(Bloc3);

    getWallJumpDroit(4150,  900, 20, 100);
    getWallJumpDroit(4150,  700, 20, 100);
    getWallJumpDroit(4150,  500, 20, 100);
    getWallJumpDroit(4150,  300, 20, 100);

    pilePlafondItems.push_back(new PlafondItems("../ressources/Décor/Plafond.png", 4010, 750, 40, 20));
    pilePlafondItems.push_back(new PlafondItems("../ressources/Décor/Plafond.png", 4010, 550, 40, 20));
    pilePlafondItems.push_back(new PlafondItems("../ressources/Décor/Plafond.png", 4010, 350, 40, 20));
    pileSolItems.push_back(new SolItems("../ressources/Décor/Bush.png", 4010, 100, 10, 100));

    pileMurDroitItems.push_back(new MurDroitItems("../ressources/Décor/MurDroit.png",4000, 700, 10, 100));
    pileMurDroitItems.push_back(new MurDroitItems("../ressources/Décor/MurDroit.png",4000, 500, 10, 100));
    pileMurDroitItems.push_back(new MurDroitItems("../ressources/Décor/MurDroit.png",4000, 300, 10, 100));
    pileMurDroitItems.push_back(new MurDroitItems("../ressources/Décor/MurDroit.png",4000, 100, 10, 100));

    getWallJumpGauche(4000, 800, 20, 100);
    getWallJumpGauche(4000, 600, 20, 100);
    getWallJumpGauche(4000, 400, 20, 100);
    getWallJumpGauche(4000, 200, 20, 100);
    getWallJumpGauche(4000, 0, 20, 100);

    pilePlafondItems.push_back(new PlafondItems("../ressources/Décor/Plafond.png", 4120, 850, 40, 20));
    pilePlafondItems.push_back(new PlafondItems("../ressources/Décor/Plafond.png", 4120, 650, 40, 20));
    pilePlafondItems.push_back(new PlafondItems("../ressources/Décor/Plafond.png", 4120, 450, 40, 20));
    pilePlafondItems.push_back(new PlafondItems("../ressources/Décor/Plafond.png", 4120, 250, 40, 20));

    pileMurGaucheItems.push_back(new MurGaucheItems("../ressources/Décor/MurGauche.png",4160, 800, 10, 100));
    pileMurGaucheItems.push_back(new MurGaucheItems("../ressources/Décor/MurGauche.png",4160, 600, 10, 100));
    pileMurGaucheItems.push_back(new MurGaucheItems("../ressources/Décor/MurGauche.png",4160, 400, 10, 100));
    pileMurGaucheItems.push_back(new MurGaucheItems("../ressources/Décor/MurGauche.png",4160, 200, 10, 100));

    getObstacle(4150, 100, 250, 100);

    pileFinishItems.push_back(new FinishItems("../ressources/Décor/Fin.png", 4300, 20, 80, 80));

    for (unsigned int i = 0; i < pilePiqueItems.size(); i++) {
        PiqueItems* pique = pilePiqueItems[i];
        addItem(pique);
    }

    for (unsigned int i = 0; i < pileSolItems.size(); i++) {
        SolItems* sol = pileSolItems[i];
        addItem(sol);
    }

    for (unsigned int i = 0; i < pileMurGaucheItems.size(); i++) {
        MurGaucheItems* gauche = pileMurGaucheItems[i];
        addItem(gauche);
    }

    for (unsigned int i = 0;i < pileMurDroitItems.size(); i++)
    {
        MurDroitItems* droite = pileMurDroitItems[i];
        addItem(droite);
    }

    for(unsigned int i = 0; i < pilePlafondItems.size(); i++)
    {
        PlafondItems* plafond = pilePlafondItems[i];
        addItem(plafond);
    }

    for(unsigned int i = 0; i < pileCheckpointItems.size(); i++)
    {
        CheckpointItems* checkpoint = pileCheckpointItems[i];
        addItem(checkpoint);
    }


    for(unsigned int i = 0; i < pileFinishItems.size(); i++)
    {
        FinishItems* fin = pileFinishItems[i];
        addItem(fin);
    }

    for(unsigned int i = 0; i < pileArgentItems.size(); i ++)
    {
        ArgentItems* argent = pileArgentItems[i];
        addItem(argent);
    }
}

//fonction qui vérifie si mon personnage entre en collision, chaque bloc possede une hitbox sol, mur gauche, mur droit, et plafond
void MyScene::verificationCollision()
{
    //verifie collision avec sol
    for (auto wall : pileSolItems)
    {
        if ((*getCuteGirl()).collidesWithItem(wall))
        {
            collisionSol = true;
            // cout << "Collision avec le Sol" << endl;
        }
    }

    //verifie collision avec les items pique
    for (auto wall : pilePiqueItems) {
        if ((*getCuteGirl()).collidesWithItem(wall)) {
            etat = "mort";
//            cout << "Mort par pique" <<endl;
        }
    }

    //verifie collision avec les murs gauche
    for (auto wall : pileMurGaucheItems) {
        if ((*getCuteGirl()).collidesWithItem(wall)) {
            collisionGauche = true;
//            cout << "Collision avec le mur gauche" <<endl;
        }
    }

    //verifie collision avec les murs droit
    for(auto wall : pileMurDroitItems) {
        if((*getCuteGirl()).collidesWithItem(wall)) {
            collisionDroite = true;
//            cout << "Collision avec le mur droit" << endl;
        }
    }

    //verifie collision avec les murs plafonds
    for(auto wall : pilePlafondItems)
    {
        if((*getCuteGirl()).collidesWithItem(wall))
        {
            collisionPlafond = true;
//            cout << "Collision avec le plafond" << endl;
        }
    }

    //verifie collision avec les checkpoint et recupere les coordonnées du dernier checkpoint
    for(auto wall : pileCheckpointItems)
    {
        if((*getCuteGirl()).collidesWithItem(wall))
        {
            collisionCheckpoint = true;
            PositionXCheckpoint = (*CuteGirl).pos().x();
            PositionYCheckpoint = (*CuteGirl).pos().y();
//            cout << "J'ai récupéré un checkpoint" << endl;
        }
    }

    //verifie collision avec le finish et affiche le menu de fin
    for(auto wall : pileFinishItems)
    {
        if((*CuteGirl).collidesWithItem(wall))
        {
            timer->stop();
            createMenu();
            collisionFin = true;
//            cout << getScore() << endl;
//            cout << "Le jeu est fini" << endl;
        }
    }

    //verifie collision avec le crabe
    if((*CuteGirl).collidesWithItem(Crabe))
    {
        etat = "mort";
        cout << "Je suis mort d'un crabe" << endl;
    }

    //verifie collision avec le bloc qui disparait et fait disparaitre le bloc
    if ((*CuteGirl).collidesWithItem(Bloc))
    {
        collisionSol = true;
        if (disparition == 2)
        {
            (*Bloc).setOpacity(0);
            collisionSol = false;
            bloc = false;
        }
    }

    //verifie collision avec le bloc qui disparait et fait disparaitre le bloc
    if ((*CuteGirl).collidesWithItem(Bloc1))
    {
        collisionSol = true;
        if (disparition1 == 2)
        {
            (*Bloc1).setOpacity(0);
            collisionSol = false;
            bloc1 = false;
        }
    }

    //verifie collision avec le bloc qui disparait et fait disparaitre le bloc
    if ((*CuteGirl).collidesWithItem(Bloc2))
    {
        collisionSol = true;
        if (disparition2 == 2)
        {
            (*Bloc2).setOpacity(0);
            collisionSol = false;
            bloc2 = false;
        }
    }

    //verifie collision avec le bloc qui disparait et fait disparaitre le bloc
    if ((*CuteGirl).collidesWithItem(Bloc3))
    {
        collisionSol = true;
        if (disparition3 == 2)
        {
            (*Bloc3).setOpacity(0);
            collisionSol = false;
            bloc3 = false;
        }
    }

    //verifie collision avec les météorites
    for (auto wall : pileMeteorite) {
        if ((*getCuteGirl()).collidesWithItem(wall)) {
            etat = "mort";
            cout << "Mort par meteorite" << endl;
        }
    }

}

// fonction qui vérifie les déplacements
void MyScene::verificationDeplacement()
{
    // vérifie deplacement crabe et update sa position
    (*Crabe).setPos((*Crabe).pos().x()+DeplacementCrabe*0.1*multiplicateurCrabe,(*Crabe).pos().y());

    if(DeplacementCrabe != 100)
    {
        DeplacementCrabe = DeplacementCrabe + 1;
    }
    else{
        multiplicateurCrabe = multiplicateurCrabe*(-1);
        DeplacementCrabe = 0;
    }

    // vérifie deplacement gauche si mur
    if(collisionDroite)
    {
        DeplacementGauche = false;
//        cout << "Je ne peux plus aller à gauche" << endl;
    }

    // vérifie deplacement droit si mur
    if(collisionGauche){
        DeplacementDroite = false;
//        cout << "Je ne peux plus aller à droite" << endl;
    }

    // on implémente la gravite et les animations
    if (!collisionSol) {

        if(pixmapjump < 31)
        {
            if(CuteGirlFaceDroite)
            {
                deplacement = QString("../ressources/CuteGirl/droite/Jump (") + QString::QString::number(pixmapjump) + QString(").png");
            }
            else
            {
                deplacement = QString("../ressources/CuteGirl/gauche/Jump (") + QString::QString::number(pixmapjump) + QString(").png");
            }
            (*CuteGirl).setPixmap(deplacement);
            pixmapjump++;
        }
        else{
            pixmapjump = 1;
        }
        if(!DeplacementHaut)
        {
            DeplacementY = DeplacementY + 0.5;
        }
    }

    // on enleve la gravité et les animations
    if (collisionSol){
        if(!DeplacementHaut)
        {
            DeplacementY=0;
        }

        if(CuteGirlFaceDroite)
        {
            if(pixmapdroite < 21)
            {
                pixmapgauche = 1;
                pixmapdroite++;
                deplacement = QString("../ressources/CuteGirl/droite/Run (") + QString::QString::number(pixmapdroite) + QString(").png");
                (*CuteGirl).setPixmap(deplacement);
                pixmapdroite++;
            }
            else{
                pixmapdroite = 1;
            }
        }
        else
        {
            if(pixmapgauche < 21)
            {
                pixmapdroite = 1;
                pixmapgauche++;
                deplacement = QString("../ressources/CuteGirl/gauche/Run (") + QString::QString::number(pixmapgauche) + QString(").png");
                (*CuteGirl).setPixmap(deplacement);
                pixmapgauche++;
            }
            else{
                pixmapdroite = 1;
            }
        }

    }

    // on implemente le saut
    if(DeplacementHaut){
        if(collisionPlafond)
        {
            DeplacementY=0;
            DeplacementHaut = false;
        }
        else
        {
            DeplacementY = DeplacementY - 5;
            if(DeplacementY < -34){
                DeplacementY = 0;
                DeplacementHaut = false;
            }
        }
    }

    // on implemente le deplacement gauche
    if(DeplacementGauche)
    {
        DeplacementX = -10;
        CuteGirlFaceDroite = false;
    }

    // on implemente le deplacement gauche
    if (DeplacementDroite)
    {
        DeplacementX = 10;
        CuteGirlFaceDroite = true;
    }

    // si pas de deplacement, le perso ne se deplace pas
    if (!DeplacementDroite && !DeplacementGauche)
    {
        DeplacementX = 0;
        pixmapgauche= 1;
        pixmapdroite = 1;
        if(CuteGirlFaceDroite)
        {
            deplacement = "../ressources/CuteGirl/idle (1).png";
        }
        else{
            deplacement = "../ressources/CuteGirl/idle (2).png";
        }
        (*CuteGirl).setPixmap(deplacement);
    }

    // on implemente le deplacement de nos meteorite
    for(unsigned int i = 0; i < pileMeteorite.size(); i++)
    {
        (*pileMeteorite[i]).setPos(pileMeteorite[i]->pos().x(), pileMeteorite[i]->pos().y()+10);
//        if((*pileMeteorite[i]).pos().y() > 1100)
//        {
//            removeItem(pileMeteorite[i]);
//        }
    }

}

void MyScene::drawBackground(QPainter *painter, const QRectF &rect) {
    Q_UNUSED(rect);
    painter->drawPixmap(QRectF(0,0,background.width(), background.height()), background, sceneRect());
}

//crée un wall jump droit
void MyScene::getWallJumpDroit(int PositionX, int PositionY, int tailleX, int tailleY)
{

    pileSolItems.push_back(new SolItems("../ressources/Décor/Bush.png", PositionX, PositionY, 10,tailleY));
    pileMurGaucheItems.push_back(new MurGaucheItems("../ressources/Décor/MurGauche.png",PositionX+10, PositionY,tailleX - 10, tailleY));
}

//crée un wall jump gauche
void MyScene::getWallJumpGauche(int PositionX, int PositionY, int tailleX, int tailleY)
{
    pileSolItems.push_back(new SolItems("../ressources/Décor/Bush.png", PositionX+10, PositionY, 10, tailleY));
    pileMurDroitItems.push_back(new MurDroitItems("../ressources/Décor/MurDroit.png",PositionX, PositionY,tailleX - 10, tailleY));
}

//la fonction fait spawn nos météorites
void MyScene::spawn()
{
    int random = rand() % 950   ;
    Moustique = new Meteorite("../ressources/Décor/Meteorite.png", 3000 + random, 0, 30, 30);
    pileMeteorite.push_back(Moustique);
    addItem(Moustique);
}

// ferme slot qui l'application quand le bouton quitter est cliqué
void MyScene::quit()
{
    QCoreApplication::quit();
}

// slot qui recupere le pseudo
void MyScene::setpseudo()
{
    this->stringpseudo = this->pseudo->text->toPlainText();
}

// recupere le meilleur score et le meilleur pseudo dans notre fichier txt
void MyScene::readLeaderboard()
{
    ifstream file("../Leaderboard/score.txt");
    string temporairepseudo;
    string temporairescore;
    getline(file, temporairepseudo);
    getline(file, temporairescore);
    QString qtemporairepseudo = QString::fromStdString(temporairepseudo);
    QString qtemporairescore = QString::fromStdString(temporairescore);
    bestscore = qtemporairescore;
    bestpseudo = qtemporairepseudo;
//    cout << bestscore << endl;
//    cout << bestpseudo << endl;
}

// crée le menu de fin
void MyScene::createMenu()
{
    QDialog* menu = new QDialog();
    (*menu).setWindowTitle("Victoire");
    mainLayout = new QVBoxLayout();
    QLabel* labelscore = new QLabel();
    QString stringscore;
    QTextStream(&stringscore) << "Score (temps + 30*(Vies Perdues)) :" << score;
    (*labelscore).setText(stringscore);

    QString stringtime;
    auto* labeltime = new QLabel(QString::QString::number(chrono));
    QTextStream(&stringtime) << "Temps :" << labeltime;
    pseudo = new LabeledTextField();
    (*(*pseudo).label).setText("Pseudo : ");
    stringpseudo = (*(*pseudo).text).toPlainText();

    QPushButton* nouvelessai = new QPushButton("Réessayer");
    QPushButton* sauvegarder = new QPushButton("Sauvegarder");
    QPushButton* quitter = new QPushButton("Quitter");
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    (*buttonLayout).addWidget(nouvelessai);
    (*buttonLayout).addWidget(sauvegarder);
    (*buttonLayout).addWidget(quitter);

    (*mainLayout).addWidget(labelscore);
    (*mainLayout).addWidget(labeltime);
    (*mainLayout).addWidget(pseudo);
    (*mainLayout).addLayout(buttonLayout);
    (*menu).setLayout(mainLayout);

    readLeaderboard();
    QLabel* bestscorelabel = new QLabel(bestscore);
    QLabel* bestpseudolabel = new QLabel(bestpseudo);
    (*mainLayout).addWidget(bestscorelabel);
    (*mainLayout).addWidget(bestpseudolabel);

    (*menu).show();

    connect(quitter, SIGNAL(clicked(bool)), this, SLOT(quit()));
    connect(pseudo->text, SIGNAL(textChanged()), this, SLOT(setpseudo()));
    connect(sauvegarder, SIGNAL(clicked(bool)), this, SLOT(save()));
    connect(nouvelessai, SIGNAL(clicked(bool)), this, SLOT(newtry()));

}

void MyScene::save()
{
    if(score < bestscore.toInt())
    {
        ofstream file("../Leaderboard/score.txt");
        string temporairescore = to_string(score);
        string temporairepseudo = stringpseudo.toStdString();
        file << temporairepseudo << "\n";
        file << temporairescore;
        file.close();
        success = new QLabel("Ton score a été enregistré");
        (*mainLayout).addWidget(success);
        bool reussi = true;
    }
    else
    {
        echec = new QLabel("Ton score n'est pas meilleur");
        (*mainLayout).addWidget(echec);
        bool nonreussi = true;
    }
}

void MyScene::newtry()
{
    if(reussi)
    {
        (*mainLayout).removeWidget(success);
        reussi = false;
    }
    if(nonreussi)
    {
        (*mainLayout).removeWidget(echec);
        nonreussi = false;
    }

    timer->start(50);
    (*CuteGirl).setPos(10,600);
    PositionXCheckpoint = 10;
    PositionYCheckpoint = 600;
    chrono = 0;
    health = 0;
}

void MyScene::verificationMort()
{
    if((*CuteGirl).pos().y() > 1100)
    {
        etat = "mort";
//        cout << "Je suis tombé dans le vide" << endl;
    }

    if(etat == "mort"){
        health = health + 1;
        if(collisionCheckpoint)
        {
            (*getCuteGirl()).setPos(PositionXCheckpoint, PositionYCheckpoint);
            etat = "vivant";
//            cout << "Je suis mort, je respawn au checkpoint" << endl;
        }
        else {
            (*getCuteGirl()).setPos(10, 600);
            etat = "vivant";
//            cout << "Je suis mort, je respawn " << endl;
        }
    }
}
