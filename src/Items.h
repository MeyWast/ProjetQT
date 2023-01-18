//
// Created by alexa on 30/05/2022.
//

#ifndef CPP_QT_TPMINIPROJET_ITEMS_H
#define CPP_QT_TPMINIPROJET_ITEMS_H

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QPainter>

class SolItems : public QGraphicsPixmapItem {
public:

    SolItems(QString img, int x, int y, int w, int h) : QGraphicsPixmapItem(QPixmap(img).scaled(w, h)) {
        this->setPos(x, y);
    }
};

class MurGaucheItems : public QGraphicsPixmapItem {
public:

    MurGaucheItems(QString img, int x, int y, int w, int h) : QGraphicsPixmapItem(QPixmap(img).scaled(w, h)) {
        this->setPos(x, y);
    }
};

class MurDroitItems : public QGraphicsPixmapItem {
public:

    MurDroitItems(QString img, int x, int y, int w, int h) : QGraphicsPixmapItem(QPixmap(img).scaled(w, h)) {
        this->setPos(x, y);
    }
};

class PiqueItems : public QGraphicsPixmapItem {
public:

    PiqueItems(QString img, int x, int y, int w, int h) : QGraphicsPixmapItem(QPixmap(img).scaled(w, h)) {
        this->setPos(x, y);
    }
};

class PlafondItems : public QGraphicsPixmapItem {
public:
    PlafondItems(QString img, int x, int y, int w, int h) : QGraphicsPixmapItem(QPixmap(img).scaled(w, h)) {
        this->setPos(x, y);
    }
};

class CheckpointItems : public QGraphicsPixmapItem {
public:
    CheckpointItems(QString img, int x, int y, int w, int h) : QGraphicsPixmapItem(QPixmap(img).scaled(w, h)){
        this->setPos(x, y);
    }
};

class FinishItems: public QGraphicsPixmapItem {
public:
    FinishItems(QString img, int x, int y, int w, int h) : QGraphicsPixmapItem(QPixmap(img).scaled(w, h)){
        this->setPos(x, y);
    }
};

class Meteorite: public QGraphicsPixmapItem {
public:
    Meteorite(QString img, int x, int y, int w, int h) : QGraphicsPixmapItem(QPixmap(img).scaled(w, h)) {
        this->setPos(x, y);
    }
};

class CrabeItems: public QGraphicsPixmapItem
{
public:
    CrabeItems(QString img, int x, int y, int w, int h) : QGraphicsPixmapItem(QPixmap(img).scaled(w, h))
    {
        this->setPos(x, y);
    }
};

class BlocItems : public QGraphicsPixmapItem
{
public:
    BlocItems(QString img, int x, int y, int w, int h) : QGraphicsPixmapItem(QPixmap(img).scaled(w, h))
    {
        this->setPos(x, y);
    }
};

    class ArgentItems: public QGraphicsPixmapItem
    {
    public:
        ArgentItems(QString img, int x, int y, int w, int h) : QGraphicsPixmapItem(QPixmap(img).scaled(w, h))
        {
            this->setPos(x, y);
        }
    };
#endif //CPP_QT_TPMINIPROJET_ITEMS_H
