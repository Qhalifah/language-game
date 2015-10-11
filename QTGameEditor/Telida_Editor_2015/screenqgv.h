#ifndef SCREENQGV_H
#define SCREENQGV_H

#include <QGraphicsView>
#include <QString>
#include <QGraphicsScene>
#include <QString>
#include <QMouseEvent>
#include <vector>
using std::vector;
#include "myrect.h"
#include <QPixmap>
#include <QComboBox>
#include <memory>
using std::unique_ptr;
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <string>
using std::string;
#include "DBMap.h"
#include "activities.h"
#include "activity.h"
#include "badge.h"

class ScreenQGV : public QGraphicsView
{
    Q_OBJECT
public:
    enum EditorType {PAIRACT = 0x1, MATCHACT = 0x2,
                     ACTIVITY = PAIRACT | MATCHACT,
                     MAP, SCENE,
                     NONE};
    explicit ScreenQGV(QWidget *parent = 0);
    ~ScreenQGV();
    void loadBGI(QString filepath);
    QString getBGIFilepath(){return m_filepath;}
    vector<unique_ptr<MyRect>> * rectItems(){return &m_markers;}
    void setName(QString name){m_name =  name;}

    void updateAspectRatio();
    void setAspectRatio(int w, int h);
    QSizeF aspectRatio();

    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void setWidthRatio(QSpinBox * x){m_widthRatio = x;}
    void setHeightRatio(QSpinBox * x){m_heightRatio = x;}

    void deleteBGI();

    void changeOrder(int val);

    void setEditorType(EditorType edType){m_edType = edType;}
    EditorType editorType(){return m_edType;}
    void setId(unsigned id){m_id = id;}
    unsigned id(){return m_id;}
    void setDictSetId(unsigned id){m_dictSetId = id;}
    unsigned dictSetId(){return m_dictSetId;}
    void setisAdding(bool isAdding){m_isAdding = isAdding;}
    bool isAdding(){return m_isAdding;}

    void addImage(QString & filepath, float x, float y, float width, float height);
    void addText(QString text, float x, float y, float width, float height);
    void addHotspot(float x, float y, float w, float h);
    void addBox(float x, float y, float w, float h);

    MyRect * addRect();

    void deleteRect();

    void hideEverything();

    void reset();

    void mouseReleaseEvent(QMouseEvent * event);
    void toggleLines(bool checked, ScreenItemType type);

    void setChoices(size_t x){m_choices = x;}
    void setRounds(size_t x){m_rounds = x;}
    void setMaxScore(size_t x){m_maxScore = x;}
    void setRewardImage(wstring x){m_reward_image = x;}
    void setRewardSoundFile(wstring y){m_reward_MusicItem.file = y;}
    void setRewardSoundVolume(float y){m_reward_MusicItem.volume = y;}
    void setRewardSoundPitch(float y){m_reward_MusicItem.pitch = y;}
    size_t choices(){return m_choices;}
    size_t rounds(){return m_rounds;}
    size_t maxScore(){return m_maxScore;}
    wstring getRewardImage(){return m_reward_image;}
    wstring getRewardSoundFile(){return m_reward_MusicItem.file;}
    float getRewardSoundVolume(){return m_reward_MusicItem.volume;}
    float getRewardSoundPitch(){return m_reward_MusicItem.pitch;}

    void setRewardBadgeId(unsigned id){m_rewardBadgeId = id;}
    unsigned rewardBadgeId(){return m_rewardBadgeId;}
    void incrementActs();
    void decrementActs();
    unsigned numActivities(){return m_numActivities;}

protected:
    QGraphicsRectItem * m_background;
    bool m_markerSelected;
    vector<unique_ptr<MyRect>> m_markers;
    qreal m_oldW; // For resizing
    qreal m_oldH; // For resizing

    QString m_name; // Hotspot, delete?
    QString m_filepath; // Background image, rename to m_BGFilepath

    QSpinBox * m_widthRatio;
    QSpinBox * m_heightRatio;

    EditorType m_edType;
    QPixmap m_bgPixmap;

    unsigned m_id;
    unsigned m_dictSetId;
    bool m_isAdding;

    bool m_showHotspotLines;
    bool m_showTextLines;
    bool m_showImgLines;
    bool m_showBoxLines;

    size_t m_choices; // Activity
    size_t m_rounds; // Activity
    size_t m_maxScore; // Activity
    wstring m_reward_image = L"None"; // Activity
    MusicItem m_reward_MusicItem; // Activity

    unsigned m_rewardBadgeId;
    unsigned m_numActivities;
    // Every ScreenQGV has a show this widget/whatever function,
};

#endif // SCREENQGV_H
