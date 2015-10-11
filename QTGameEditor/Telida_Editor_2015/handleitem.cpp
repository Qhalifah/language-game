#include "handleitem.h"
#include <QGraphicsRectItem>
#include <QBrush>
#include <QRect>
#include <QRectF>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>

HandleItem::HandleItem(int positionFlags, QGraphicsRectItem *parent,
                       QList<HandleItem *> * handleItems):
    QGraphicsRectItem(-4, -4, 8, 8, parent),
    m_positionFlags(positionFlags), m_parent(parent),
    m_handleItems(handleItems)
{
    setVisible(false);
    setBrush(QBrush(Qt::lightGray));
}

int HandleItem::positionFlags() const
{
    return m_positionFlags;
}

void HandleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_pressed = true;
}

void HandleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_pressed = false;
}

void HandleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_pressed)
    {
        QPointF newPos = mapFromParent(restrictPosition(mapToParent(event->pos())));
        QPointF posChange = QPointF(0, 0);
        qreal wChange = m_parent->rect().width();
        qreal hChange = m_parent->rect().height();

        switch (m_positionFlags)
        {

            case TopLeft:
                posChange = -rect().topLeft()+newPos;
                wChange += rect().left()-newPos.x();
                hChange += rect().top()-newPos.y();
                break;
            case Top:
                posChange.setY(-rect().top()+newPos.y());
                hChange += rect().top()-newPos.y();
                break;
            case TopRight:
                posChange.setY(-rect().top()+newPos.y());
                wChange += -rect().right()+newPos.x();
                hChange += rect().top()-newPos.y();
                break;
            case Right:
                wChange += -rect().right()+newPos.x();
                break;
            case BottomRight:
                wChange += -rect().right()+newPos.x();
                hChange += -rect().bottom()+newPos.y();
                break;
            case Bottom:
                hChange += -rect().bottom()+newPos.y();
                break;
            case BottomLeft:
                posChange.setX(-rect().left()+newPos.x());
                wChange += rect().left()-newPos.x();
                hChange += -rect().bottom()+newPos.y();
                break;
            case Left:
                posChange.setX(-rect().left()+newPos.x());
                wChange += rect().left()-newPos.x();
                break;
        }
        m_parent->setRect(0, 0, wChange, hChange);
        m_parent->setPos(m_parent->pos()+posChange);

        updateHandleItem();
    }
}

void HandleItem::updateHandleItem()
{
    foreach(HandleItem * item, *m_handleItems)
    {
        switch (item->positionFlags())
        {
            case TopLeft:
                item->setPos(m_parent->rect().topLeft());
                break;
            case Top:
                item->setPos(m_parent->rect().left() + m_parent->rect().width()/2,
                             m_parent->rect().top());
                break;
            case TopRight:
                item->setPos(m_parent->rect().topRight());
                break;
            case Right:
                item->setPos(m_parent->rect().right(),
                             m_parent->rect().top() + m_parent->rect().height()/2);
                break;
            case BottomRight:
                item->setPos(m_parent->rect().bottomRight());
                break;
            case Bottom:
                item->setPos(m_parent->rect().left() + m_parent->rect().width()/2,
                             m_parent->rect().bottom());
                break;
            case BottomLeft:
                item->setPos(m_parent->rect().bottomLeft());
                break;
            case Left:
                item->setPos(m_parent->rect().left(),
                             m_parent->rect().top() + m_parent->rect().height()/2);
                break;
        }
    }
}

QPointF HandleItem::restrictPosition(const QPointF & pos)
{
    QPointF newPos = pos;
    if (m_positionFlags & Top && newPos.y() > m_parent->rect().bottom()-4)
        newPos.setY(m_parent->rect().bottom()-4);
    else if (m_positionFlags & Bottom && newPos.y() < m_parent->rect().top()+4)
        newPos.setY(m_parent->rect().top()+4);

    if (m_positionFlags & Left && newPos.x() > m_parent->rect().right()-4)
        newPos.setX(m_parent->rect().right()-4);
    else if (m_positionFlags & Right && newPos.x() < m_parent->rect().left()+4)
        newPos.setX(m_parent->rect().left()+4);

    // Restrict point within scene
    QPointF newPosScn = mapToScene(newPos);
    QPointF zero = mapFromScene(0, 0);
    QPointF corner = mapFromScene(scene()->width(), scene()->height());
    if(newPosScn.x() < -4)
        newPos.setX(zero.x()-4);
    if(newPosScn.y() < -4)
        newPos.setY(zero.y()-4);
    if(m_positionFlags & Right &&
            mapToScene(mapFromParent(newPos)).x() > scene()->width()+1)
        newPos.setX(mapToParent(corner).x()+1);
    if(m_positionFlags & Bottom &&
            mapToScene(mapFromParent(newPos)).y() > scene()->height()+1)
        newPos.setY(mapToParent(corner).y()+1);
    return newPos;
}
