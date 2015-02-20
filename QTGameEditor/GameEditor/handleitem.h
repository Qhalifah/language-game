#ifndef HANDLEITEM_H
#define HANDLEITEM_H

#include <QGraphicsRectItem>

class HandleItem : public QGraphicsRectItem
{
public:
    enum
    {
        Top         = 0x1,
        Bottom      = 0x2,
        Left        = 0x4,
        TopLeft     = Top | Left,
        BottomLeft  = Bottom | Left,
        Right       = 0x8,
        TopRight    = Top | Right,
        BottomRight = Bottom | Right
    };
    HandleItem(int positionFlags, QGraphicsRectItem * parent,
               QList<HandleItem *> *handleItems);
    int positionFlags() const;
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    void updateHandleItem();

protected:

private:
    QPointF restrictPosition(const QPointF & pos);

    int m_positionFlags;
    QGraphicsRectItem * m_parent;
    bool m_pressed;
    QList<HandleItem *> * m_handleItems;
};

#endif // HANDLEITEM_H
