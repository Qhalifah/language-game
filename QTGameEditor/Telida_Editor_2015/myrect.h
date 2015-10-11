#ifndef MYRECT_H
#define MYRECT_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneDragDropEvent>
#include "handleitem.h"
#include <QRect>
#include <QPixmap>
#include <QProcess>
#include "screen.h"
#include <QFileInfo>
#include "scene.h"
#include <QSet>

class MyRect : public QGraphicsRectItem
{
public:
    static QProcess * m_soundApp;
    static void setReleased(bool hasReleased){m_hasReleased = hasReleased;}

    bool m_otherSelect;
    static MyRect * m_selectedRect;

    explicit MyRect(QGraphicsWidget * parent = 0);
    void setHotspotFilepath(QString filepath);
    QString hotspotFilepath(){return m_hotspotFilepath;}
    QString hotspotBaseName(){return QFileInfo(m_hotspotFilepath).baseName();}
    QString sndBaseName(){return QFileInfo(m_sndFilepath).baseName();}
    QString imgBaseName(){return QFileInfo(m_imgFilepath).baseName();}
    void setSndFilepath(QString filepath){m_sndFilepath = filepath;}
    void setImgFilepath(QString filepath){m_imgFilepath = filepath;}
    QString sndFilepath(){return m_sndFilepath;}
    QString imgFilepath(){return m_imgFilepath;}

    void mySetRect(qreal x, qreal y, qreal w, qreal h);
    QString getText(){return m_text;}
    QColor getColor(){return m_color;}
    QString getHoverText(){return m_hoverText;}
    void setType(ScreenItemType type){m_type = type;}
    void setRect(const QRectF & rect);
    void setRect(qreal x, qreal y, qreal w, qreal h);
    void setImage(QString filepath);
    void setSound(QString filepath);
    void setText(QString text);
    void setColor(QColor color);
    void setHoverText(QString text);
    ScreenItemType getType(){return m_type;}
    void toggleLines(bool checked){m_showLines = checked;}

    void setVol(float vol){m_hasSound = true; m_volume = vol;}
    void setPitch(float pitch){m_hasSound = true; m_pitch = pitch;}
    float volume(){return m_volume;}
    float pitch(){return m_pitch;}
    bool hasSound(){return m_hasSound;}
    void setHasSound(bool hasSound){m_hasSound = hasSound;}
    bool save();

    void setMouseDownEngage(bool checked){m_mouseDownEngage = checked; update();}
    void setMouseDownSelect(bool checked){m_mouseDownSelect = checked; update();}
    void setMouseUpEngage(bool checked){m_mouseUpEngage = checked; update();}
    void setMouseUpSelect(bool checked){m_mouseUpSelect = checked; update();}
    void setHoverEngage(bool checked){m_hoverEngage = checked; update();}
    void setHoverSelect(bool checked){m_hoverSelect = checked; update();}
    bool mouseDownEngage(){return m_mouseDownEngage;}
    bool mouseDownSelect(){return m_mouseDownSelect;}
    bool mouseUpEngage(){return m_mouseUpEngage;}
    bool mouseUpSelect(){return m_mouseUpSelect;}
    bool hoverEngage(){return m_hoverEngage;}
    bool hoverSelect(){return m_hoverSelect;}

    void setId(unsigned id){m_id = id;}
    unsigned id(){return m_id;}
    void setGameType(GameType type){m_gameType = type;}
    GameType gameType(){return m_gameType;}
    QString actFileName();
    void setCurrBadgeId(unsigned id){m_currBadgeId = id;}
    unsigned currBadgeId(){return m_currBadgeId;}
    bool addReqBadge(unsigned id);
    bool removeBadge();
    QStringList reqBadgeList();
    QSet<unsigned> reqBadges(){return m_reqBadges;}
    void setActPieceFilepath(QString filepath){m_actPieceFilepath = filepath; cout << "m_actPieceFilepath: " << filepath.toStdString() << endl;}
    QString actPieceFilepath(){return m_actPieceFilepath;}
protected:
    QRectF boundingRect() const;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent * event);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void updateHandleItemPositions();



private:

    static bool m_isPressed;
    static bool m_hasReleased;

    QList<HandleItem *> m_handleItems;

    ScreenItemType m_type;
    QPixmap m_image;
    QString m_text;
    QString m_hotspotFilepath;
    QColor m_color; // For text/hotspot
    QString m_hoverText;
    QString m_sndFilepath;
    QString m_imgFilepath;
    bool m_showLines;

    bool m_hasSound;
    float m_volume;
    float m_pitch;

    bool m_mouseDownEngage;
    bool m_mouseDownSelect;
    bool m_mouseUpEngage;
    bool m_mouseUpSelect;
    bool m_hoverEngage;
    bool m_hoverSelect;

    unsigned m_id;
    GameType m_gameType;

    QSet<unsigned> m_reqBadges;
    unsigned m_currBadgeId;

    QString m_actPieceFilepath;
};

#endif // MYRECT_H
