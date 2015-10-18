#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QDebug>

// Badges add/edit

void MainWindow::on_badgesView_clicked(const QModelIndex &index)
{
    QString badgeName = m_badgesModel.stringList().at(index.row());
    m_original_badgeName = badgeName;
    m_currId = m_badges->id(badgeName);

    m_currBadge = m_badges->badge(m_currId);
    ui->badgeName->setText(m_currBadge.name);
    ui->badgeImgName->setText(QFileInfo(m_currBadge.img).baseName());
    m_badgePixmap.load(m_currBadge.img);
    ui->badgeImg->setPixmap(m_badgePixmap.scaled(50, 50,
                                               Qt::KeepAspectRatioByExpanding,
                                               Qt::FastTransformation));
    ui->editBadgeWidg->setHidden(false);
    ui->addBadgeWidg->setHidden(true);
}

void MainWindow::on_addDashed_L_2_clicked()
{
    ui->badgeName->setText(ui->badgeName->text() + QString("Ł"));
    ui->badgeName->setFocus();
}

void MainWindow::on_addDashed_l_2_clicked()
{
    ui->badgeName->setText(ui->badgeName->text() + QString("ł"));
    ui->badgeName->setFocus();
}

void MainWindow::on_addImgForBadge_clicked()
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
    QString newFile = m_fileManager.copyFile(filepath,
                                             FileManager::IMG);
    m_currBadge.img = newFile;
    ui->badgeImgName->setText(QFileInfo(newFile).baseName());
    m_badgePixmap.load(newFile);
    ui->badgeImg->setPixmap(m_badgePixmap.scaled(50, 50,
                                               Qt::KeepAspectRatioByExpanding,
                                               Qt::FastTransformation));
}

void MainWindow::on_clearBadge_clicked()
{
    ui->badgeName->clear();
    ui->badgeImgName->clear();
    ui->badgeErr->clear();
    m_badgePixmap = QPixmap(50, 50);
    m_badgePixmap.fill(Qt::white);
    m_currBadge.clear();
    m_currId = 0;
    ui->badgeImg->setPixmap(m_badgePixmap);
}

void MainWindow::on_addBadge_clicked()
{
    m_currBadge.name = ui->badgeName->text();
    if(m_currBadge.name.isEmpty())
    {
        ui->badgeErr->setText(QString("The badge name\ncan't be empty."));
        return;
    }
    if(m_currBadge.img.isEmpty())
    {
        ui->badgeErr->setText(QString("There must be an image\nassociated with the badge."));
        return;
    }
    if(!m_badges->add(m_currBadge))
    {
        ui->badgeErr->setText(QString("That badge name\nis already taken."));
        return;
    }
    m_badgesModel.setStringList(m_badges->list());
    on_clearBadge_clicked();
}

void MainWindow::on_delBadge_clicked()
{
    // Delete Badge from the Map
    Map m("WorldMap", NULL);
    m.load();
    m.removeBadgeFromScenePrereqs(m_currId, m_badges->badgeName(m_currId).toStdWString());
    m.save();

    // Delete Badge from Scenes and their Activity objects
    QDir scnFiles(QString::fromStdString(m_DocumentsPath+"//Dinaki Adventures//db//scenes"));
    scnFiles.setNameFilters(QStringList()<<"*.scn");
    QStringList sceneFileNameList = scnFiles.entryList();
    for(auto ii : sceneFileNameList)
    {
        string name = ii.section(".",0,0).toStdString();
        cout << "scnFileName: " << name << endl;
        Scene scn(name, NULL);
        cout << "Load the scene" << endl;
        scn.load();
        if(scn.m_badge.m_name == m_badges->badgeName(m_currId).toStdWString())
        {
            scn.removeBadgeFromSelfAndActivityObjects();
            scn.save();
        }
    }
    m_mainDictModel.setStringList(m_mainDict->list());

    m_badges->remove(m_currId);
    m_badgesModel.setStringList(m_badges->list());
    on_clearBadge_clicked();
    ui->editBadgeWidg->setHidden(true);
    ui->addBadgeWidg->setHidden(false);
}

void MainWindow::on_submitBadge_clicked()
{
    cout << "old Badge Name: " << m_original_badgeName.toStdString() << endl;
    m_currBadge.name = ui->badgeName->text();
    cout << "current Badge Name: " << m_currBadge.name.toStdString() << endl;
    if(m_currBadge.name.isEmpty())
    {
        ui->badgeErr->setText("The badge name\ncan't be empty.");
        return;
    }
    if(!m_badges->edit(m_currId, m_currBadge))
    {
        ui->badgeErr->setText("That badge name\nis already taken.");
        return;
    }
    m_badgesModel.setStringList(m_badges->list());

    Map m("WorldMap", NULL);
    m.load();
    m.changePrereqBadgeName(m_original_badgeName.toStdWString(), m_currBadge.name.toStdWString());
    m.save();

    // Change Badge in Scenes and their Activity objects
    QDir scnFiles(QString::fromStdString(m_DocumentsPath+"//Dinaki Adventures//db//scenes"));
    scnFiles.setNameFilters(QStringList()<<"*.scn");
    QStringList sceneFileNameList = scnFiles.entryList();
    for(auto ii : sceneFileNameList)
    {
        string name = ii.section(".",0,0).toStdString();
        cout << "scnFileName: " << name << endl;
        Scene scn(name, NULL);
        cout << "Load the scene" << endl;
        scn.load();
        if(scn.m_badge.m_name == m_original_badgeName.toStdWString())
        {
            scn.updateBadgeInSelfAndActivityObjects(m_original_badgeName.toStdWString(), m_currBadge.name.toStdWString(), m_currBadge.img.toStdWString());
            scn.save();
        }
    }

    on_clearBadge_clicked();
    ui->editBadgeWidg->setHidden(true);
    ui->addBadgeWidg->setHidden(false);
}
