// Menu Actions
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "choosefromlist.h"
#include <QStackedWidget>
#include <QString>
#include <QMap>
#include "dictionarysets.h"
#include "maindictionary.h"
#include <QDebug>
#include "activities.h"
#include "chooseactivity.h"
#include "chooseacttype.h"

void MainWindow::on_actionA_Dictionary_Set_triggered()
{
    if(ui->stackedWidget->currentWidget() == ui->screenEditor &&  !remindUserToSave())
    {
        cout << "Cancel returned at widget stuff" << endl;
        return;
    }
    unsigned chosenDictSetId = m_dictSets->getSelection();
    if(!chosenDictSetId)
    {
        cout << "Cancel returned at chosenDictSetId" << endl;
        return;
    }
    m_currId = chosenDictSetId;
    m_dictSetModel.setStringList(m_dictSets->dictSetList(m_currId, m_mainDict));
    m_mainDictModel.setStringList(m_mainDict->list());
    ui->dictSetAdd->setHidden(true);
    ui->dictSetDone->setHidden(false);
    ui->dictSetName->setText(m_dictSets->dictSetName(m_currId));
    dictSetOriginalName = m_dictSets->dictSetName(m_currId).toStdString();
    ui->dictSetErr->clear();
    ui->stackedWidget->setCurrentWidget(ui->subDict);
}

void MainWindow::on_actionExit_triggered()
{
    if(ui->stackedWidget->currentWidget() == ui->screenEditor &&  !remindUserToSave())
        return;
    m_soundApp.kill();
    exit(0);
}


void MainWindow::on_actionThe_Dictionary_triggered()
{
    if(ui->stackedWidget->currentWidget() == ui->screenEditor &&  !remindUserToSave())
        return;
    ui->stackedWidget->setCurrentWidget(ui->mainDictionary);
    ui->editWordWidg->setHidden(true);
    ui->addWordWidg->setHidden(false);
    ui->wordErrorMsg->clear();
    m_wordPixmap = QPixmap(50, 50);
    m_wordPixmap.fill(QColor(Qt::white));
    ui->wordImg->setPixmap(m_wordPixmap);
    m_currWord.clear();
    m_mainDictModel.setStringList(m_mainDict->list());
}

void MainWindow::on_actionNew_Dictionary_SEt_triggered()
{
    if(ui->stackedWidget->currentWidget() == ui->screenEditor &&  !remindUserToSave())
        return;
    ui->dictSetName->setText(QString("Dictionary_Set_Name"));
    ui->dictSetErr->clear();
    ui->dictSetDone->setHidden(true);
    ui->dictSetAdd->setHidden(false);
    QStringList list;
    m_dictSetModel.setStringList(list);
    m_mainDictModel.setStringList(m_mainDict->list());
    ui->stackedWidget->setCurrentWidget(ui->subDict);
}

void MainWindow::on_actionThe_Map_triggered()
{
    if(ui->stackedWidget->currentWidget() == ui->screenEditor &&  !remindUserToSave())
        return;
    ui->graphicsView->reset();
    ui->graphicsView->setEditorType(ScreenQGV::MAP);
    m_fileManager.loadFiles(*(ui->graphicsView), m_BGM);
    setupScreenQGVWidgets();
}

void MainWindow::on_actionNew_Scene_triggered()
{
    if(ui->stackedWidget->currentWidget() == ui->screenEditor &&  !remindUserToSave())
        return;
    ui->graphicsView->reset();
    ui->graphicsView->setEditorType(ScreenQGV::SCENE);
    setupScreenQGVWidgets();
    ui->screenName->setText("Scene_Name");
    ui->graphicsView->setisAdding(true);
}

void MainWindow::on_actionA_Scene_triggered()
{
    if(ui->stackedWidget->currentWidget() == ui->screenEditor &&  !remindUserToSave())
        return;
    QString result = m_scenes->getSelection();
    if(result.isEmpty())
    {
        return;
    }
    ui->graphicsView->reset();
    ui->graphicsView->setEditorType(ScreenQGV::SCENE);
    ui->graphicsView->setisAdding(false);
    ui->graphicsView->setId(m_scenes->id(result));
    m_fileManager.loadFiles(*(ui->graphicsView), m_BGM);
    setupScreenQGVWidgets();
    ui->screenName->setText(m_scenes->name(ui->graphicsView->id()));
}

void MainWindow::on_actionAn_Activity_triggered()
{
    if(ui->stackedWidget->currentWidget() == ui->screenEditor &&  !remindUserToSave())
        return;
    ChooseActivity cA(m_matchActs, m_pairActs);
    QString result = cA.getSelection();
    Activities::Activity act;
    if(result.isEmpty())
    {
        return;
    }
    ui->graphicsView->reset();
    if(cA.getEditorType() == ScreenQGV::PAIRACT)
    {
        ui->graphicsView->setEditorType(ScreenQGV::PAIRACT);
        ui->graphicsView->setId(m_pairActs->id(result));
        act = m_pairActs->act(ui->graphicsView->id());
    }
    else
    {
        ui->graphicsView->setEditorType(ScreenQGV::MATCHACT);
        ui->graphicsView->setId(m_matchActs->id(result));
        act = m_matchActs->act(ui->graphicsView->id());
    }
    ui->screenName->setText(act.name);
    ui->graphicsView->setDictSetId(act.dictSetId);
    m_fileManager.loadFiles(*(ui->graphicsView), m_BGM);
    setupScreenQGVWidgets();
    ui->addScreen->setHidden(true);
    ui->editScreen->setHidden(false);
    ui->graphicsView->setisAdding(false);
    ui->selectDictSet->setCurrentText(m_dictSets->dictSetName(ui->graphicsView->dictSetId()));
}

void MainWindow::on_actionNew_Activity_triggered()
{
    if(ui->stackedWidget->currentWidget() == ui->screenEditor &&  !remindUserToSave())
        return;
    ScreenQGV::EditorType type = ChooseActType().getEditorType();
    if(type == ScreenQGV::NONE)
        return;
    cout << "resetting the reward image" << endl;
    ui->graphicsView->reset();

    if(type == ScreenQGV::PAIRACT)
        ui->graphicsView->setEditorType(ScreenQGV::PAIRACT);
    else
        ui->graphicsView->setEditorType(ScreenQGV::MATCHACT);
    ui->addScreen->setHidden(false);
    ui->editScreen->setHidden(true);
    ui->graphicsView->setisAdding(true);
    setupScreenQGVWidgets();
    ui->screenName->setText("Activity_Name");
    ui->graphicsView->setisAdding(true);
}

void MainWindow::on_actionThe_Badges_triggered()
{
    if(ui->stackedWidget->currentWidget() == ui->screenEditor &&  !remindUserToSave())
        return;
    ui->stackedWidget->setCurrentWidget(ui->badgeEditor);
    ui->editBadgeWidg->setHidden(true);
    ui->addBadgeWidg->setHidden(false);
    ui->badgeErr->clear();
    m_currBadge.clear();
    m_badgesModel.setStringList(m_badges->list());
}

void MainWindow::on_actionAbout_triggered()
{
    if(ui->stackedWidget->currentWidget() == ui->screenEditor &&  !remindUserToSave())
        return;
    ui->stackedWidget->setCurrentWidget(ui->aboutPage);

    QFile file(QString::fromStdString(m_DocumentsPath+"//Dinaki Adventures//editor//AboutPageText.txt"));
    if(!file.open(QIODevice::ReadOnly)) {
        cout << "Failed to open AboutPageText.txt" << endl;
    }
    QTextStream in(&file);
    QString text = "";
    while(!in.atEnd()) {
        text.append(in.readLine());
        text.append("\n");
    }
    file.close();
    ui->aboutPageText->setText(text);
}
