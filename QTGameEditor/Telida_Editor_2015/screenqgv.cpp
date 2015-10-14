#include "screenqgv.h"
#include <QGraphicsPixmapItem>
#include "myrect.h"
#include <QMouseEvent>
#include <memory>
using std::unique_ptr;
#include <QLineEdit>
#include <QFileInfo>
#include <QPixmap>
#include "filemanager.h"
#include "screen.h"
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

class MainWindow;
class Ui_MainWindow;

ScreenQGV::ScreenQGV(QWidget *parent) :
    QGraphicsView(parent),
    m_markerSelected(false),
    m_filepath(""), m_edType(NONE), m_id(0),
    m_showBoxLines(true), m_showHotspotLines(true),
    m_showImgLines(true), m_showTextLines(true),
    m_rewardBadgeId(0)
{
    this->setBackgroundBrush(QBrush(Qt::black));
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QGraphicsScene * scene = new QGraphicsScene(this);
    this->setScene(scene);
    this->scene()->setItemIndexMethod(QGraphicsScene::NoIndex);
    this->setMouseTracking(true);
    m_background = this->scene()->addRect(0, 0, 0, 0, QPen(), QBrush(Qt::white));

    setRewardSoundFile(L"None");
    setRewardSoundVolume(50);
    setRewardSoundPitch(1);
}

ScreenQGV::~ScreenQGV()
{
}

void ScreenQGV::loadBGI(QString filepath)
{
    m_filepath = filepath;
    if(!m_filepath.isEmpty())
    {
        m_bgPixmap = QPixmap(m_filepath);
        m_background->setBrush(QBrush(m_bgPixmap.scaled(this->scene()->width(), this->scene()->height(), Qt::IgnoreAspectRatio, Qt::FastTransformation)));
    }
}

void ScreenQGV::deleteBGI()
{
    m_bgPixmap = QPixmap();
    m_filepath.clear();
    m_background->setBrush(QBrush(Qt::white));
}

void ScreenQGV::updateAspectRatio()
{
    qreal whRatio = (qreal)(MainWindow::ui->widthRatio->value())/(qreal)(MainWindow::ui->heightRatio->value());
    qreal width;
    qreal height;
    if(whRatio > 1)
    {
        width = this->width();
        height = width/whRatio;
        if(height > this->height())
        {
            height = this->height();
            width = height*whRatio;
        }
    }
    else
    {
        height = this->height();
        width = height*whRatio;
        if(width > this->width())
        {
            width = this->width();
            height = width/whRatio;
        }
    }
    this->scene()->setSceneRect(0, 0, width, height);
    m_background->setRect(0, 0, width, height);
    if(!m_filepath.isEmpty())
    {
        m_background->setBrush(QBrush(m_bgPixmap.scaled(width, height,
                Qt::IgnoreAspectRatio, Qt::FastTransformation)));
    }
    else
    {
        m_background->setBrush(QBrush(Qt::white));
    }

    qreal xScale = width/m_oldW;
    qreal yScale = height/m_oldH;

    for(size_t i = 0; i < m_markers.size(); ++i)
    {
        qreal x = m_markers[i]->x();
        qreal y = m_markers[i]->y();
        qreal w = m_markers[i]->rect().width();
        qreal h = m_markers[i]->rect().height();
        m_markers[i]->setRect(0, 0, w*xScale, h*yScale);
        m_markers[i]->setPos(x*xScale, y*yScale);
    }
    m_oldW = width;
    m_oldH = height;
}

void ScreenQGV::resizeEvent(QResizeEvent *event)
{
    updateAspectRatio();
    QGraphicsView::resizeEvent(event);
}

void ScreenQGV::changeOrder(int val)
{
    int index = -1;
    for(size_t i = 0; i < m_markers.size(); ++i)
    {
        if(m_markers[i]->isSelected())
        {
            index = i;
            break;
        }
    }
    if(index >= 0)
    {
        int temp = m_markers[index]->zValue();
        m_markers[index]->setZValue(m_markers[val-1]->zValue());
        m_markers[val-1]->setZValue(temp);
        m_markers[index].swap(m_markers[val-1]);
    }
}

void ScreenQGV::mousePressEvent(QMouseEvent * event)
{
    MyRect::setReleased(false);
    QGraphicsView::mousePressEvent(event);
}

void ScreenQGV::addImage(QString &filepath, float x, float y, float width, float height)
{
    MyRect * rect = addRect();
    rect->setPos(x, y);
    rect->setRect(0, 0, width, height);
    rect->setImage(filepath);
    rect->toggleLines(m_showImgLines);
}

void ScreenQGV::addText(QString text, float x, float y, float width, float height)
{
    MyRect * rect = addRect();
    rect->setPos(x, y);
    rect->setRect(0, 0, width, height);
    rect->setText(text);
    rect->setColor(QColor(0, 0, 0, 255));
    rect->toggleLines(m_showTextLines);
}

void ScreenQGV::addHotspot(float x, float y, float w, float h)
{
    MyRect * rect = addRect();
    rect->setType(ScreenItemType::HOTSPOT);
    rect->setPos(x, y);
    rect->setRect(0, 0, w, h);
    rect->toggleLines(m_showHotspotLines);
}

void ScreenQGV::addBox(float x, float y, float w, float h)
{
    MyRect * rect = addRect();
    rect->setPos(x, y);
    rect->setRect(0, 0, w, h);
    rect->toggleLines(m_showBoxLines);
    rect->setType(ScreenItemType::BOX);
}

MyRect * ScreenQGV::addRect()
{
    m_markers.push_back(unique_ptr<MyRect>(new MyRect()));
    this->scene()->addItem(m_markers.back().get());
    m_markers.back()->setZValue(m_markers.size()-1);
    MainWindow::ui->order->setMaximum(MainWindow::ui->order->maximum()+1);
    MainWindow::ui->order->setMinimum(1);
    return m_markers.back().get();
}

void ScreenQGV::deleteRect()
{
    //m_scene.removeItem(MyRect::m_selectedRect);
    bool changedOrder = false;
    for(auto itr = m_markers.begin(); itr != m_markers.end();)
    {
        if(changedOrder)
        {
            (*itr)->setZValue((*itr)->zValue()-1);
        }
        if((*itr)->isSelected())
        {
            this->scene()->removeItem(itr->get());
            itr = m_markers.erase(itr);
            MyRect::m_selectedRect = NULL;
            MainWindow::ui->order->setMaximum(MainWindow::ui->order->maximum()-1);
            changedOrder = true;
        }
        else
            ++itr;
    }
    hideEverything();
}

void ScreenQGV::hideEverything()
{
    if(MyRect::m_selectedRect)
    {
        MyRect::m_selectedRect->setSelected(false);
    }
    MainWindow::ui->type_name->setHidden(true);
    MainWindow::ui->hoverWidg->setHidden(true);
    MainWindow::ui->editText->setHidden(true);
    MainWindow::ui->textColor->setHidden(true);
    MainWindow::ui->editHLCol->setHidden(true);
    if(m_edType == ScreenQGV::MAP || m_edType == ScreenQGV::SCENE || m_edType == NONE)
        MainWindow::ui->addSnd->setHidden(true);
    MainWindow::ui->delSnd->setHidden(true);
    MainWindow::ui->sndVolWidg->setHidden(true);
    MainWindow::ui->sndPitchWidg->setHidden(true);
    MainWindow::ui->orderWidg->setHidden(true);
    MainWindow::ui->del->setHidden(true);
    MainWindow::ui->behaviorWidg->setHidden(true);
    MainWindow::ui->goToScene->setHidden(true);
    MainWindow::ui->goToActivity->setHidden(true);
    MainWindow::ui->actPieceWidg->setHidden(true);
    MainWindow::ui->badgeWidg->setHidden(true);
    MyRect::m_selectedRect = NULL;
}

void ScreenQGV::reset()
{
    hideEverything();
    for(auto itr = m_markers.begin(); itr != m_markers.end(); ++itr)
    {
        this->scene()->removeItem(itr->get());
    }
    m_markers.clear();
    m_bgPixmap = QPixmap(this->sceneRect().width(), this->sceneRect().height());
    m_bgPixmap.fill(Qt::white);
    m_background->setBrush(QBrush(m_bgPixmap));
    m_markerSelected = false;
    m_id = 0;
    m_dictSetId = 0;
    m_choices = 4;
    m_rounds = 5;
    m_maxScore = 100;
    MainWindow::ui->chooseRewardBadge->setEnabled(false);
}

void ScreenQGV::mouseReleaseEvent(QMouseEvent *event)
{
    hideEverything();

    QGraphicsView::mouseReleaseEvent(event);

    if(MyRect::m_selectedRect)
    {
        MyRect * sel = MyRect::m_selectedRect;
        if(m_edType == ScreenQGV::MAP)
        {
            MainWindow::ui->goToScene->setHidden(false);
            MainWindow::ui->goToScene->clear();
            MainWindow::ui->goToScene->addItem(QString("Go To Scene"));
            MainWindow::ui->goToScene->insertItems(1, MainWindow::m_scenes->list());
            if(!sel->id())
            {
                MainWindow::ui->goToScene->setCurrentIndex(0);
            }
            else
            {
                MainWindow::ui->goToScene->setCurrentIndex(MainWindow::ui->goToScene->findText(MainWindow::m_scenes->name(sel->id())));
                MainWindow::ui->badgeWidg->setHidden(false);
                MainWindow::ui->selReqBadge->setHidden(false);
                MainWindow::ui->selReqBadge->clear();
                MainWindow::ui->selReqBadge->addItem(QString("Select a Required Badge"));
                MainWindow::ui->selReqBadge->insertItems(1, sel->reqBadgeList());
                cout << "sel->reqBadgeList.size(): " << sel->reqBadgeList().size() << endl;
                cout << "sel->reqBadgeList.size(): " << MyRect::m_selectedRect->reqBadgeList().size() << endl;
                if(sel->reqBadgeList().size() > 0)
                {
                    cout << "sel->reqBadgeList()[0]: " << sel->reqBadgeList()[0].toStdString() << endl;
                    MainWindow::ui->selReqBadge->setCurrentIndex(1);
                    sel->setCurrBadgeId(MainWindow::m_badges->id(MainWindow::ui->selReqBadge->currentText()));
                }
                MainWindow::ui->addBadgeToScn->setHidden(false);
                MainWindow::ui->delBadgeFromScn->setHidden(false);
            }
        }
        else if(m_edType == ScreenQGV::SCENE)
        {
            MainWindow::ui->goToActivity->setHidden(false);
            if(!sel->id())
            {
                MainWindow::ui->goToActivity->setCurrentIndex(0);
            }
            else
            {
                // Sets the Piece image from an Activity's file
                // If there is no file, then the GameType is set to NONE
                if(MyRect::m_selectedRect->gameType() != GameType::NONE)
                {
                    // Check if the file exists
                    string actFileName = ".//db//activities//";
                    actFileName.append(MyRect::m_selectedRect->actFileName().toStdString());
                    actFileName.append(".act");
                    QFileInfo checkFile(QString::fromStdString(actFileName));
                    // If the file exists load the Activity data into the MyRect
                    if (checkFile.exists())
                    {
                        MainWindow::ui->actPieceWidg->setHidden(false);
                        MainWindow::ui->actPieceImg->setPixmap(QPixmap(MyRect::m_selectedRect->actPieceFilepath()).scaled(50, 50,
                                                                                   Qt::KeepAspectRatioByExpanding,
                                                                                   Qt::FastTransformation));
                    }
                    else // The file does not exist, set the GameType to NONE
                    {
                        MyRect::m_selectedRect->setGameType(GameType::NONE);
                    }
                }
                QString actName;
                if(sel->gameType() == GameType::PAIR)
                {
                    actName = "Pairing - ";
                    actName += MainWindow::m_pairActs->act(sel->id()).name;
                    MainWindow::ui->goToActivity->setCurrentIndex(MainWindow::ui->goToActivity->findText(actName));
                }
                else if(sel->gameType() == GameType::MATCHING)
                {
                    actName = "Matching - ";
                    actName += MainWindow::m_matchActs->act(sel->id()).name;
                    MainWindow::ui->goToActivity->setCurrentIndex(MainWindow::ui->goToActivity->findText(actName));
                }
            }
        }

        MainWindow::ui->behaviorWidg->setHidden(false);
        MainWindow::ui->mD_engage_chkBox->setChecked(sel->mouseDownEngage());
        MainWindow::ui->mD_select_chkBox->setChecked(sel->mouseDownSelect());
        MainWindow::ui->mU_engage_chkBox->setChecked(sel->mouseUpEngage());
        MainWindow::ui->mU_select_chkBox->setChecked(sel->mouseUpSelect());
        MainWindow::ui->hov_engage_chkBox->setChecked(sel->hoverEngage());
        MainWindow::ui->hov_select_chkBox->setChecked(sel->hoverSelect());
        QString str = QString("");
        if(sel->getType() == ScreenItemType::TEXT)
        {
            str += QString("Text: ");
            str += sel->getText();
        }
        else if(sel->getType() == ScreenItemType::IMAGE)
        {
            str += QString("Image: ");
            str += sel->imgBaseName();
        }
        else if(sel->getType() == ScreenItemType::HOTSPOT)
        {
            str += QString("Hotspot");
        }
        else if(sel->getType() == ScreenItemType::BOX)
        {
            str += QString("Box");
        }
        MainWindow::ui->type_name->setText(str);
        MainWindow::ui->type_name->setHidden(false);
        MainWindow::ui->hoverWidg->setHidden(false);
        MainWindow::ui->orderWidg->setHidden(false);
        MainWindow::ui->del->setHidden(false);
        MainWindow::ui->hovLineEdit->setText(MyRect::m_selectedRect->getHoverText());
        int index = 0;
        for(size_t i = 0; i < m_markers.size(); ++i)
        {
            if(m_markers[i]->isSelected())
            {
                index = i;
                break;
            }
        }
        MainWindow::ui->order->setValue(index+1);
        if(sel->hasSound())
        {
            MainWindow::ui->delSnd->setHidden(false);
            MainWindow::ui->sndVolWidg->setHidden(false);
            MainWindow::ui->sndPitchWidg->setHidden(true);
            MainWindow::ui->sndVol->setValue(sel->volume());
            MainWindow::ui->sndPitch->setValue(1);
        }
        else
        {
            MainWindow::ui->addSnd->setHidden(false);
        }


        if(sel->getType() == TEXT)
        {
            MainWindow::ui->editText->setHidden(false);
            MainWindow::ui->textColor->setHidden(false);
        }
        else if(sel->getType() == IMAGE)
        {

        }
        else // HOTSPOT
        {
            MainWindow::ui->editHLCol->setHidden(false);
        }
    }
}

void ScreenQGV::toggleLines(bool checked, ScreenItemType type)
{
    for(size_t m = 0; m < m_markers.size(); ++m)
    {
        if(m_markers[m]->getType() == type)
            m_markers[m]->toggleLines(checked);
    }
    if(type == ScreenItemType::IMAGE)
        m_showImgLines = checked;
    else if(type == ScreenItemType::TEXT)
        m_showTextLines = checked;
    else if(type == ScreenItemType::HOTSPOT)
        m_showHotspotLines = checked;
    else if(type == ScreenItemType::BOX)
        m_showBoxLines = checked;
    this->scene()->update(0, 0, this->scene()->width(), this->scene()->height());
}
