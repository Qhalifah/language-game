#include "myrect.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QMouseEvent>
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPen>
#include <QToolTip>
#include <QGraphicsWidget>
#include "handleitem.h"
#include <fstream>
using std::ofstream;
#include <QProcess>
#include <QFileInfo>
#include <QDebug>
#include "mainwindow.h"

class MainWindow;

QProcess * MyRect::m_soundApp = NULL;
bool MyRect::m_isPressed = false;
bool MyRect::m_hasReleased = false;

MyRect::MyRect(QGraphicsWidget *parent):
    QGraphicsRectItem(parent),
    m_otherSelect(false),
    m_type(HOTSPOT),
    m_color(QColor(255, 255, 255, 255)),
    m_hoverText(""),
    m_showLines(true),
    m_hasSound(false),
    m_mouseDownEngage(false), m_mouseDownSelect(false),
    m_mouseUpEngage(false), m_mouseUpSelect(false),
    m_hoverEngage(false), m_hoverSelect(false),
    m_id(0)
{
    setFlags(ItemIsMovable
                  |ItemIsSelectable
                  |ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);

    HandleItem * topleft = new HandleItem(HandleItem::TopLeft, this, &m_handleItems);
    topleft->setCursor(Qt::CursorShape::SizeAllCursor);
    m_handleItems.append(topleft);
    HandleItem * top = new HandleItem(HandleItem::Top, this, &m_handleItems);
    top->setCursor(Qt::CursorShape::SizeVerCursor);
    m_handleItems.append(top);
    HandleItem * topright =
            new HandleItem(HandleItem::TopRight, this, &m_handleItems);
    topright->setCursor(Qt::CursorShape::SizeAllCursor);
    m_handleItems.append(topright);

    HandleItem * right = new HandleItem(HandleItem::Right, this, &m_handleItems);
    right->setCursor(Qt::CursorShape::SizeHorCursor);
    m_handleItems.append(right);

    HandleItem * bottom = new HandleItem(HandleItem::Bottom, this, &m_handleItems);
    bottom->setCursor(Qt::CursorShape::SizeVerCursor);
    m_handleItems.append(bottom);

    HandleItem * bottomleft =
            new HandleItem(HandleItem::BottomLeft, this, &m_handleItems);
    bottomleft->setCursor(Qt::CursorShape::SizeAllCursor);
    m_handleItems.append(bottomleft);
    HandleItem * left = new HandleItem(HandleItem::Left, this, &m_handleItems);
    left->setCursor(Qt::CursorShape::SizeHorCursor);
    m_handleItems.append(left);

    HandleItem * bottomright =
            new HandleItem(HandleItem::BottomRight, this, &m_handleItems);
    bottomright->setCursor(Qt::CursorShape::SizeAllCursor);
    m_handleItems.append(bottomright);
}

void MyRect::setHotspotFilepath(QString filepath)
{
    m_hotspotFilepath = filepath;
    m_type = HOTSPOT;
}

QRectF MyRect::boundingRect() const
{
    return this->rect();
}

void MyRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_mouseDownSelect)
        m_otherSelect = !m_otherSelect;
    m_isPressed = true;
    m_hasReleased = false;
    QGraphicsRectItem::mousePressEvent(event);
    this->update(this->rect());
}

void MyRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_hasSound)
    {
        while(!save());
        m_soundApp->putChar('c');
    }
    if(m_mouseUpSelect && !m_mouseDownSelect)
        m_otherSelect = !m_otherSelect;
    m_isPressed = false;
    m_hasReleased = true;
    this->setSelected(true);
    m_selectedRect = this;
    QGraphicsRectItem::mouseReleaseEvent(event);
    this->update(this->rect());
}

void MyRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
{
    if(m_showLines)
    {
        QRectF rect = boundingRect();
        QPen wpen(Qt::white);
        wpen.setWidth(3);
        QPen ypen(Qt::yellow);
        ypen.setWidth(3);
        QPen bpen(Qt::black);
        bpen.setWidth(1);
        QPen rpen(Qt::red);
        rpen.setWidth(1);

        if(this->isSelected())
        {
            painter->setPen(ypen);
            painter->drawRect(rect);
            if(m_isPressed)
            {
                painter->setPen(rpen);
                painter->drawRect(rect);
            }
            else
            {
                painter->setPen(bpen);
                painter->drawRect(rect);
            }
        }
        else
        {
            painter->setPen(wpen);
            painter->drawRect(rect);
            painter->setPen(bpen);
            painter->drawRect(rect);
        }
    }
    qreal w = this->rect().width();
    qreal h = this->rect().height();
    if(w == 0)
        w = 1;
    if(h == 0)
        h = 1;

    bool engage = false;
    painter->setPen(QPen(m_color));

    if(m_mouseDownEngage && m_isPressed)
    {
        engage = true;
    }
    else if(m_mouseUpEngage && m_hasReleased)
    {
        engage = true;
    }
    else if(m_hoverEngage && isUnderMouse())
    {
        engage = true;
    }

    if(m_type == ScreenItemType::IMAGE)
    {
        if(engage)
            painter->setOpacity(.5);
        painter->drawPixmap(1, 1, w-1, h-1, m_image);
        painter->setOpacity(255);
    }
    else if(m_type == ScreenItemType::TEXT)
    {
        if(engage)
            painter->setOpacity(.5);
        //painter->fillRect(QRectF(1, 1, w-1, h-1), m_color);
        painter->setPen(QPen(m_color));
        painter->drawText(this->rect(), m_text, QTextOption(Qt::AlignHCenter |
                                                            Qt::AlignVCenter));
        painter->setOpacity(255);
    }
    else if(m_type == ScreenItemType::HOTSPOT)
    {
        if(engage)
        {
            painter->setBrush(QBrush(m_color));
            painter->drawEllipse(this->rect());
        }
    }
    else if(m_type == ScreenItemType::BOX)
    {
        painter->setBrush(QBrush(m_color));
        painter->drawRect(2, 2, this->rect().width()-4, this->rect().height()-4);
    }
    if(m_otherSelect && (m_mouseDownSelect || m_mouseUpSelect || m_hoverSelect))
    {
        QPen penSelect(m_color);
        if(m_type != ScreenItemType::HOTSPOT)
            penSelect.setColor(QColor(20, 200, 10, 255));

        penSelect.setWidth(5);
        painter->setPen(penSelect);
        painter->setBrush(QBrush(Qt::NoBrush));
        painter->drawRect(4, 4, this->rect().width()-8, this->rect().height()-8);
    }
}

void MyRect::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if(m_hoverSelect)
        m_otherSelect = true;
    QPoint point(event->screenPos().x(), event->screenPos().y());
    QRect rect(point.x()-4, point.y()-4, 8, 8);
    QToolTip::showText(point, m_hoverText,
                       event->widget(), rect);
    for(auto h : m_handleItems)
    {
        h->setVisible(true);
    }
    update(this->rect());
}

void MyRect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if(m_hoverSelect)
        m_otherSelect = false;
    QPoint point(event->screenPos().x(), event->screenPos().y());
    QRect rect(point.x()-1, point.y()-1, 2, 2);
    QToolTip::showText(point, m_hoverText,
                       event->widget(), rect);
    for(auto h : m_handleItems)
    {
        h->setVisible(false);
    }
    update(this->rect());
}

void MyRect::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QPoint point(event->screenPos().x(), event->screenPos().y());
    QRect rect(point.x()-4, point.y()-4, 8, 8);
    QToolTip::showText(point, m_hoverText,
                       event->widget(), rect);
}

QVariant MyRect::itemChange(QGraphicsItem::GraphicsItemChange change,
                            const QVariant &value)
{
    if (change == ItemPositionChange && scene()) {
        prepareGeometryChange();
        // value is the new position.
        QPointF newPos = value.toPointF();
        qreal w = rect().width();
        qreal h = rect().height();
        if(newPos.x() < 0)
        {
            newPos.setX(0);
        }
        if(newPos.y() < 0)
        {
            newPos.setY(0);
        }
        if(newPos.x()+w > scene()->width()-3)
        {
            newPos.setX(scene()->width()-w-3);
        }
        if(newPos.y()+h > scene()->height()-3)
        {
            newPos.setY(scene()->height()-h-3);
        }
        return newPos;
    }
    return QGraphicsItem::itemChange(change, value);
}

void MyRect::updateHandleItemPositions()
{
    m_handleItems.first()->updateHandleItem();
}

void MyRect::mySetRect(qreal x, qreal y, qreal w, qreal h)
{
    MyRect::setRect(x, y, w, h);
}

void MyRect::setRect(const QRectF &rect)
{
    QGraphicsRectItem::setRect(rect);
    updateHandleItemPositions();
}

void MyRect::setRect(qreal x, qreal y, qreal w, qreal h)
{
    QGraphicsRectItem::setRect(x, y, w, h);
    updateHandleItemPositions();
}
#include <qdebug.h>
// Once set as an image, it stays an image.
void MyRect::setImage(QString filepath)
{
    m_type = IMAGE;
    m_image = QPixmap(filepath);
    setImgFilepath(filepath);
}

void MyRect::setSound(QString filepath)
{
    m_sndFilepath = filepath;

}

void MyRect::setText(QString text)
{
    m_type = TEXT;
    m_text = text;
}

void MyRect::setColor(QColor color)
{
    m_color = color;
}

void MyRect::setHoverText(QString text)
{
    m_hoverText = text;
}

bool MyRect::save()
{
    ofstream ofile(".\\ComSnd.txt");
    if(!ofile.is_open())
        return false;
    int playSound = true;
    ofile << playSound;
    ofile << "\n";
    ofile << m_volume;
    ofile << "\n";
    ofile << m_pitch;
    ofile << "\n";
    ofile << m_sndFilepath.toStdString();
    ofile.close();
    return true;
}

QString MyRect::actFileName()
{
    if(!this->id())
        return QString("");
    QString filename;
    if(this->gameType() == GameType::PAIR)
    {
        filename = "p";
    }
    else if(this->gameType() == GameType::MATCHING)
    {
        filename = "m";
    }
    filename += QString::number(this->id());
    return filename;
}

bool MyRect::addReqBadge(unsigned id)
{
    if(m_reqBadges.contains(id))
        return false;
    m_reqBadges.insert(id);
    return true;
}

bool MyRect::removeBadge()
{
    return m_reqBadges.remove(m_currBadgeId);
}

QStringList MyRect::reqBadgeList()
{
    QStringList list;
    for(auto itr = m_reqBadges.begin(); itr != m_reqBadges.end(); ++itr)
    {
        list.append(MainWindow::m_badges->badgeName(*itr));
    }
    list.sort(Qt::CaseInsensitive);
    return list;
}

MyRect * MyRect::m_selectedRect = NULL;
