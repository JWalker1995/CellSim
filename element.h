#ifndef ELEMENT_H
#define ELEMENT_H

#include <QString>
#include <QChar>
#include <QColor>

class Element
{
public:
    explicit Element(QString name2, QString symbol2, qreal rad2, qreal mass2, int color2);

    QString name;
    QString symbol;
    qreal rad;
    qreal mass;
    int color;
    
signals:
    
public slots:
    
};

#endif // ELEMENT_H
