#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QString>
#include <QGraphicsScene>
#include <QPixmap>
#include <QFileInfo>
#include <QInputDialog>
#include <QColorDialog>
#include <QCheckBox>
#include "FileManager.h"
#include <QStringListModel>
#include <QDebug>
#include <QActionGroup>
#include <QMessageBox>
#include "dbmap.h"
#include "activities.h"
#include <map>
#include <memory>
using std::shared_ptr;
#include "badges.h"

Ui::MainWindow * MainWindow::ui = new Ui::MainWindow;
DBMap * MainWindow::m_scenes = new DBMap(string("scenes"));
Activities * MainWindow::m_pairActs = new Activities(QString("pairActs"));
Activities * MainWindow::m_matchActs = new Activities(QString("matchActs"));
MainDictionary * MainWindow::m_mainDict = new MainDictionary();
DictionarySets * MainWindow::m_dictSets = new DictionarySets();
Badges * MainWindow::m_badges = new Badges();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_mapStr("Add New Scene"),
    m_sceneStr("Add New Activity"),
    m_hotspotStr(m_mapStr)
{
    ui->setupUi(this);
    m_BGM.setComboBox(ui->selectBGM);
    // Main Dictionary
    ui->wordErrorMsg->setStyleSheet(QString("QLabel { background-color : white; color : red; }"));
    ui->mainDictView->setModel(&m_mainDictModel);
    ui->mainDictView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Edit Dictionary Set
    ui->mainDictView2->setModel(&m_mainDictModel);
    ui->dictSetView->setModel(&m_dictSetModel);
    ui->dictSetView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->dictSetName->setValidator(new QRegExpValidator( QRegExp("[ŁłA-Za-z0-9_]+"), this ));

    // Badges
    ui->badgesView->setModel(&m_badgesModel);
    ui->badgesView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->badgeName->setValidator(new QRegExpValidator(QRegExp("[ -~]+")));

    // no Łł because game app only takes string not wstring
    ui->screenName->setValidator(new QRegExpValidator( QRegExp("[A-Za-z0-9_]+"), this ));

    MyRect::m_soundApp = &m_soundApp;
    m_soundApp.start(QString(".\\SFML_SOUND"));
    ui->goToScene->insertItems(1, m_scenes->list());
    ui->graphicsView->setEditorType(ScreenQGV::NONE);
    setupScreenQGVWidgets();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
}

void MainWindow::on_loadBGI_clicked()
{
    QString selfilter = tr("All files (*.jpg *.jpeg *.png *bmp)");
    QString filepath = QFileDialog::getOpenFileName(
            this,
            QString("Select Background Image"),
            QString(""),
            tr("All files (*.jpg *.jpeg *.png *bmp);;JPEG (*.jpg *.jpeg);;PNG (*.png);; BMP (*.bmp)" ),
            &selfilter
    );
    if(filepath.isEmpty())
        return;
    QString newFile = m_fileManager.copyFile(filepath, FileManager::IMG);
    ui->graphicsView->loadBGI(newFile);
}

void MainWindow::on_addBGM_clicked()
{
    QString selfilter = tr("All files (*.ogg)");
    QString filepath = QFileDialog::getOpenFileName(
            this,
            QString("Select Background Music"),
            QString(""),
            tr("All files (*.ogg)" ),
            &selfilter
    );
    if(filepath.isEmpty())
        return;
    QString newFile;
    if(ui->graphicsView->editorType() == ScreenQGV::MAP)
    {
        newFile = m_fileManager.copyFile(filepath, FileManager::MAPBGM);
    }
    else if(ui->graphicsView->editorType() == ScreenQGV::SCENE)
    {
        newFile = m_fileManager.copyFile(filepath, FileManager::SCNBGM);
    }
    else // Activity
    {
        newFile = m_fileManager.copyFile(filepath, FileManager::ACTBGM);
    }
    m_BGM.add(newFile, 50, 50);
}

void MainWindow::on_playBGM_clicked(bool checked)
{
    if(checked)
    {
        ui->playBGM->setText(QString("Stop Music"));
        m_BGM.play();
    }
    else
    {
        ui->playBGM->setText(QString("Play Music"));
        m_BGM.stop();
    }
}

void MainWindow::on_delBGM_clicked()
{
    if(ui->selectBGM->currentIndex() == 0)
    {
        return;
    }
    QString filepath = m_BGM.remove(ui->selectBGM->currentText());
    m_fileManager.deleteFile(filepath);
    ui->selectBGM->removeItem(ui->selectBGM->currentIndex());
    ui->selectBGM->setCurrentIndex(0);
    ui->vol->setEnabled(false);
    ui->pitch->setEnabled(false);
}

void MainWindow::on_selectBGM_activated(const QString &arg1)
{
    if(ui->selectBGM->currentIndex() == 0)
    {
        ui->vol->setEnabled(false);
        ui->pitch->setEnabled(false);
        return;
    }
    ui->vol->setEnabled(true);
    ui->pitch->setEnabled(true);
    ui->vol->setSliderPosition(m_BGM.getVolume(arg1));
    ui->pitch->setSliderPosition(m_BGM.getPitch(arg1));
}

void MainWindow::on_vol_valueChanged(int value)
{
    if(ui->selectBGM->currentIndex() == 0)
        return;
    m_BGM.setVolume(value, ui->selectBGM->currentText());
}

void MainWindow::on_pitch_valueChanged(int value)
{
    if(ui->selectBGM->currentIndex() == 0)
        return;
    m_BGM.setPitch(value, ui->selectBGM->currentText());
}

void MainWindow::on_addImg_clicked()
{
    QString selfilter = tr("All files (*.jpg *.jpeg *.png *bmp)");
    QString filepath = QFileDialog::getOpenFileName(
            this,
            QString("Select Image"),
            QString(""),
            tr("All files (*.jpg *.jpeg *.png *bmp);;JPEG (*.jpg *.jpeg);;PNG (*.png);; BMP (*.bmp)" ),
            &selfilter
    );
    if(filepath.isEmpty())
        return;
    QString newFile = m_fileManager.copyFile(filepath, FileManager::IMG);
    ui->graphicsView->addImage(newFile, 0, 0, 50, 50);
}

void MainWindow::on_delBGI_clicked()
{
    QString filepath = ui->graphicsView->getBGIFilepath();
    ui->graphicsView->deleteBGI();
}

void MainWindow::on_addText_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter text"),
                                         tr("Text:"), QLineEdit::Normal,
                                         QString(""), &ok);
    if(ok && !text.isEmpty())
        ui->graphicsView->addText(text, 0, 0, 100, 50);
}

void MainWindow::on_del_clicked()
{
    ui->graphicsView->deleteRect();
}

void MainWindow::on_editText_clicked()
{
    MyRect * r = MyRect::m_selectedRect;
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter Text"),
                                         tr("Text:"), QLineEdit::Normal,
                                         r->getText(), &ok);
    if(ok && !text.isEmpty())
        r->setText(text);
}

void MainWindow::on_textColor_clicked()
{
    MyRect * r = MyRect::m_selectedRect;
    QColor col = QColorDialog::getColor(r->getColor(), this,
                                        QString("Choose Text Background Color"),
                                        QColorDialog::ShowAlphaChannel);
    if(col.isValid())
        r->setColor(col);
}

void MainWindow::on_imgLines_toggled(bool checked)
{
    ui->graphicsView->toggleLines(checked, ScreenItemType::IMAGE);
}

void MainWindow::on_hotspotLines_toggled(bool checked)
{
    ui->graphicsView->toggleLines(checked, ScreenItemType::HOTSPOT);
}

void MainWindow::on_TextLines_toggled(bool checked)
{
    ui->graphicsView->toggleLines(checked, ScreenItemType::TEXT);
}

void MainWindow::on_addSnd_clicked()
{
    MyRect * r = MyRect::m_selectedRect;
    QString selfilter = tr("All files (*.wav)");
    QString filename = QFileDialog::getOpenFileName(
            this,
            QString("Select Sound"),
            QString(""),
            tr("All files (*.wav)" ),
            &selfilter
    );
    if(filename.isEmpty())
        return;
    QString newFile = m_fileManager.copyFile(filename, FileManager::OTHERSND);
    r->setSndFilepath(newFile);
    r->setVol(50);
    r->setPitch(50);
    ui->addSnd->setHidden(true);
    ui->delSnd->setHidden(false);
    ui->sndVolWidg->setHidden(false);
    ui->sndPitchWidg->setHidden(false);
}

void MainWindow::on_delSnd_clicked()
{
    MyRect * r = MyRect::m_selectedRect;
    r->setHasSound(false);
    ui->addSnd->setHidden(false);
    ui->delSnd->setHidden(true);
    ui->sndVolWidg->setHidden(true);
    ui->sndPitchWidg->setHidden(true);
    m_fileManager.deleteFile(r->sndFilepath());
}

void MainWindow::on_editHLCol_clicked()
{
    MyRect * r = MyRect::m_selectedRect;
    QColor col = QColorDialog::getColor(r->getColor(), this,
                                        QString("Choose Text Background Color"),
                                        QColorDialog::ShowAlphaChannel);
    if(col.isValid())
        r->setColor(col);
}

void MainWindow::on_sndVol_valueChanged(int value)
{
    MyRect * r = MyRect::m_selectedRect;
    r->setVol(value);
}

void MainWindow::on_sndPitch_valueChanged(int value)
{
    MyRect * r = MyRect::m_selectedRect;
    r->setPitch(value);
}

void MainWindow::on_hovLineEdit_textEdited(const QString &arg1)
{
    MyRect * r = MyRect::m_selectedRect;
    r->setHoverText(arg1);
}

void MainWindow::on_order_valueChanged(int arg1)
{
    ui->graphicsView->changeOrder(arg1);
}

void MainWindow::on_mD_engage_chkBox_toggled(bool checked)
{
    MyRect * r = MyRect::m_selectedRect;
    r->setMouseDownEngage(checked);
}

void MainWindow::on_mD_select_chkBox_toggled(bool checked)
{
    MyRect * r = MyRect::m_selectedRect;
    r->setMouseDownSelect(checked);
}

void MainWindow::on_mU_engage_chkBox_toggled(bool checked)
{
    MyRect * r = MyRect::m_selectedRect;
    r->setMouseUpEngage(checked);
}

void MainWindow::on_mU_select_chkBox_toggled(bool checked)
{
    MyRect * r = MyRect::m_selectedRect;
    r->setMouseUpSelect(checked);
}

void MainWindow::on_hov_engage_chkBox_toggled(bool checked)
{
    MyRect * r = MyRect::m_selectedRect;
    r->setHoverEngage(checked);
}

void MainWindow::on_hov_select_chkBox_toggled(bool checked)
{
    MyRect * r = MyRect::m_selectedRect;
    r->setHoverSelect(checked);
}

void MainWindow::on_widthRatio_valueChanged(int arg1)
{
    ui->graphicsView->updateAspectRatio();
}

void MainWindow::on_heightRatio_valueChanged(int arg1)
{
    ui->graphicsView->updateAspectRatio();
}

bool MainWindow::remindUserToSave()
{
    if(ui->graphicsView->editorType() == ScreenQGV::NONE)
        return true;
    QMessageBox msgBox;
    msgBox.setText("This a reminder to save any changes.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Cancel)
        return false;
    else
    {
        ui->playBGM->setText(QString("Play Music"));
        if(ret == QMessageBox::Save)
            on_saveEditor_clicked();
    }
    return true;
}

void MainWindow::on_saveEditor_clicked()
{
    if(ui->graphicsView->editorType() == ScreenQGV::SCENE)
    {
        if(ui->screenName->text().isEmpty())
        {
            ui->screenErr->setText(QString("Name cannot be empty."));
            return;
        }
        if(ui->graphicsView->isAdding())
        {
            ui->graphicsView->setId(m_scenes->add(ui->screenName->text()));
            if(!ui->graphicsView->id())
            {
                ui->screenErr->setText(QString("Name already exists."));
                return;
            }
            ui->graphicsView->setisAdding(false);
        }
        else if(!m_scenes->rename(ui->graphicsView->id(), ui->screenName->text()))
        {
            ui->screenErr->setText(QString("Name already exists."));
            return;
        }
    }
    m_fileManager.saveFiles(*(ui->graphicsView), m_BGM);
}

void MainWindow::on_goToScene_activated(const QString &arg1)
{
    MyRect * r = MyRect::m_selectedRect;
    if(!ui->goToScene->currentIndex())
    {
        r->setId(0);
        ui->badgeWidg->setHidden(true);
        return;
    }
    r->setId(m_scenes->id(arg1));
    ui->badgeWidg->setHidden(false);
}

void MainWindow::on_addHotspot_clicked()
{
    ui->graphicsView->addHotspot(0, 0, 50, 50);
}

void MainWindow::on_addBox_clicked()
{
    ui->graphicsView->addBox(0, 0, 50, 50);
}

void MainWindow::on_boxLines_toggled(bool checked)
{
    ui->graphicsView->toggleLines(checked, ScreenItemType::BOX);
}

void MainWindow::setupScreenQGVWidgets()
{
    ui->scrollArea->setHidden(false);
    ui->screenErr->clear();
    ScreenQGV::EditorType edType = ui->graphicsView->editorType();
    ui->screenName->setReadOnly(false);
    ui->actPieceWidg->setHidden(true);
    if(edType == ScreenQGV::MAP || edType == ScreenQGV::SCENE)
    {
        if(edType == ScreenQGV::MAP)
        {
            ui->graphicsView->setEditorType(ScreenQGV::MAP);
            ui->editorType->setText("Map Editor");
            ui->screenName->setText("WorldMap");
            ui->screenName->setReadOnly(true);
            ui->goToScene->clear();
            ui->goToScene->addItem(QString("Go to Scene"));
            ui->goToScene->insertItems(1, m_scenes->list());

            ui->chooseRewardBadge->setHidden(true);
            ui->badgeLine->setHidden(true);
        }
        else // SCENE
        {
            ui->graphicsView->setEditorType(ScreenQGV::SCENE);
            ui->editorType->setText("Scene Editor");
            ui->goToActivity->clear();
            ui->goToActivity->addItem(QString("Go to Activity"));
            QStringList list = m_pairActs->listWithName(QString("Pairing - "));
            list.append(m_matchActs->listWithName(QString("Matching - ")));
            ui->goToActivity->insertItems(1, list);

            ui->chooseRewardBadge->setHidden(false);
            ui->badgeLine->setHidden(false);
            ui->chooseRewardBadge->clear();
            ui->chooseRewardBadge->addItem(QString("Choose a Reward Badge"));
            ui->chooseRewardBadge->insertItems(1, m_badges->list());
            ui->chooseRewardBadge->setCurrentText(m_badges->badgeName(ui->graphicsView->rewardBadgeId()));
        }
        ui->choicesWidg->setHidden(true);
        ui->roundsWidg->setHidden(true);
        ui->selectDictSet->setHidden(true);
        ui->actLine->setHidden(true);

        ui->addText->setHidden(false);
        ui->TextLines->setHidden(false);
        ui->textLine->setHidden(false);
        ui->addImg->setHidden(false);
        ui->imgLines->setHidden(false);
        ui->imgLine->setHidden(false);
        ui->addHotspot->setHidden(false);
        ui->hotspotLines->setHidden(false);
        ui->hotspotLine->setHidden(false);
        ui->addBox->setHidden(false);
        ui->boxLines->setHidden(false);
        ui->boxLine->setHidden(false);
        ui->saveEditor->setHidden(false);

        ui->addScreen->setHidden(true);
        ui->editScreen->setHidden(true);
    }
    else if(edType == ScreenQGV::PAIRACT || edType == ScreenQGV::MATCHACT)
    {
        ui->choicesWidg->setHidden(false);
        ui->roundsWidg->setHidden(false);
        ui->selectDictSet->setHidden(false);
        ui->actLine->setHidden(false);

        ui->addText->setHidden(true);
        ui->TextLines->setHidden(true);
        ui->textLine->setHidden(true);
        ui->addImg->setHidden(true);
        ui->imgLines->setHidden(true);
        ui->imgLine->setHidden(true);
        ui->addHotspot->setHidden(true);
        ui->hotspotLines->setHidden(true);
        ui->hotspotLine->setHidden(true);
        ui->addBox->setHidden(true);
        ui->boxLines->setHidden(true);
        ui->boxLine->setHidden(true);
        ui->saveEditor->setHidden(true);
        ui->chooseRewardBadge->setHidden(true);
        ui->badgeLine->setHidden(true);

        ui->editorType->setText("Activity Editor");
        ui->selectDictSet->clear();
        ui->selectDictSet->addItem(QString("Choose a Dictionary"));
        ui->selectDictSet->insertItems(1, m_dictSets->dictSetsList());
        ui->selectDictSet->setCurrentText(m_dictSets->dictSetName(ui->graphicsView->dictSetId()));
        ui->numChoices->setValue(ui->graphicsView->choices());
        ui->numRounds->setValue(ui->graphicsView->rounds());
    }
    else // NONE
    {
        ui->scrollArea->setHidden(true);
    }
    ui->stackedWidget->setCurrentWidget(ui->screenEditor);
}

void MainWindow::on_addScreen_clicked()
{
    ui->screenErr->clear();
    if(ui->dictSetName->text().isEmpty())
    {
        ui->screenErr->setText(QString("The activity\nname cannot be empty."));
        return;
    }
    if(!ui->selectDictSet->currentIndex())
    {
        ui->screenErr->setText(QString("The activity\nmust have a dictionary set."));
        return;
    }
    unsigned id;
    Activities::Activity act;
    act.name = ui->screenName->text();
    act.dictSetId = ui->graphicsView->dictSetId();
    if(ui->graphicsView->editorType() == ScreenQGV::PAIRACT)
    {
        id = m_pairActs->add(act);
    }
    else
    {
        id = m_matchActs->add(act);
    }
    if(!id)
    {
        ui->screenErr->setText(QString("That activity\nname already exists."));
        return;
    }
    ui->graphicsView->setId(id);
    m_fileManager.saveFiles(*(ui->graphicsView), m_BGM);
    ui->stackedWidget->setCurrentWidget(ui->mainMenu);
}

void MainWindow::on_editScreen_clicked()
{
    ui->screenErr->clear();
    if(ui->dictSetName->text().isEmpty())
    {
        ui->screenErr->setText(QString("The activity\nname cannot be empty."));
        return;
    }
    if(!ui->selectDictSet->currentIndex())
    {
        ui->screenErr->setText(QString("The activity\nmust have a dictionary set."));
        return;
    }
    unsigned id;
    Activities::Activity act;
    act.name = ui->screenName->text();
    act.dictSetId = ui->graphicsView->dictSetId();
    if(ui->graphicsView->editorType() == ScreenQGV::PAIRACT)
    {
        if(!m_pairActs->edit(ui->graphicsView->id(), act))
        {
            ui->screenErr->setText(QString("That activity\nname already exists."));
        }
    }
    else
    {
        if(!m_matchActs->edit(ui->graphicsView->id(), act))
        {
            ui->screenErr->setText(QString("That activity\nname already exists."));
        }
    }
    m_fileManager.saveFiles(*(ui->graphicsView), m_BGM);
    ui->stackedWidget->setCurrentWidget(ui->mainMenu);
}

void MainWindow::on_numChoices_valueChanged(int arg1)
{
    ui->graphicsView->setChoices(arg1);
}

void MainWindow::on_numRounds_valueChanged(int arg1)
{
    ui->graphicsView->setRounds(arg1);
}

void MainWindow::on_selectDictSet_activated(const QString &arg1)
{
    if(!ui->selectDictSet->currentIndex())
    {
        ui->graphicsView->setDictSetId(0);
        return;
    }
    ui->graphicsView->setDictSetId(m_dictSets->dictSetId(arg1));
}

void MainWindow::on_goToActivity_activated(const QString &arg1)
{
    MyRect * r = MyRect::m_selectedRect;
    if(!ui->goToActivity->currentIndex())
    {
        if(r->id())
            ui->graphicsView->decrementActs();
        r->setId(0);
        ui->actPieceWidg->setHidden(true);
        return;
    }
    QString actType = ui->goToActivity->currentText().section(QChar('-'), 0, 0);
    actType = actType.remove(QChar(' '));
    QString str = ui->goToActivity->currentText().section(QChar('-'), 1, 1);
    ui->actPieceWidg->setHidden(false);
    if(!r->id())
    {
        r->setActPieceFilepath(m_fileManager.errorImgFilepath());
        ui->actPieceImg->setPixmap(QPixmap(r->actPieceFilepath()).scaled(50, 50,
                                                                         Qt::KeepAspectRatioByExpanding,
                                                                         Qt::FastTransformation));
        ui->graphicsView->incrementActs();
    }
    if(actType == QString("Pairing"))
    {
        r->setId(m_pairActs->id(str.remove(0, 1)));
        r->setGameType(GameType::PAIR);
    }
    else if(actType == QString("Matching"))
    {
        r->setId(m_matchActs->id(str.remove(0, 1)));
        r->setGameType(GameType::MATCHING);
    }

}

void MainWindow::on_selReqBadge_activated(const QString &arg1)
{
    if(!ui->selReqBadge->currentIndex())
    {
        MyRect::m_selectedRect->setCurrBadgeId(0);
        return;
    }
    MyRect::m_selectedRect->setCurrBadgeId(m_badges->id(arg1));
}

void MainWindow::on_addBadgeToScn_clicked()
{
    unsigned choice = m_badges->getSelection();
    if(!choice)
        return;
    MyRect::m_selectedRect->addReqBadge(choice);
    QString currSelection = ui->selReqBadge->currentText();
    ui->selReqBadge->clear();
    ui->selReqBadge->addItem(QString("Select a Required Badge"));
    ui->selReqBadge->insertItems(1, MyRect::m_selectedRect->reqBadgeList());
    ui->selReqBadge->setCurrentText(currSelection);
}

void MainWindow::on_delBadgeFromScn_clicked()
{
    MyRect::m_selectedRect->removeBadge();
    ui->selReqBadge->removeItem(ui->selReqBadge->findText(m_badges->badgeName(MyRect::m_selectedRect->currBadgeId())));
}

void MainWindow::on_chooseRewardBadge_activated(const QString &arg1)
{
    ui->graphicsView->setRewardBadgeId(m_badges->id(arg1));
}

void MainWindow::on_setActPieceImg_clicked()
{
    QString selfilter = tr("All files (*.jpg *.jpeg *.png *bmp)");
    QString filepath = QFileDialog::getOpenFileName(
            this,
            QString("Select Image"),
            QString(""),
            tr("All files (*.jpg *.jpeg *.png *bmp);;JPEG (*.jpg *.jpeg);;PNG (*.png);; BMP (*.bmp)" ),
            &selfilter
    );
    if(filepath.isEmpty())
        return;
    QString newFile = m_fileManager.copyFile(filepath, FileManager::IMG);
    MyRect::m_selectedRect->setActPieceFilepath(newFile);
    ui->actPieceImg->setPixmap(QPixmap(MyRect::m_selectedRect->actPieceFilepath()).scaled(50, 50,
                                                                     Qt::KeepAspectRatioByExpanding,
                                                                     Qt::FastTransformation));
}
