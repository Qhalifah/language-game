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
    QString selfilter = tr("All files (*.png)");
    QString filepath = QFileDialog::getOpenFileName(
            this,
            QString("Select Image"),
            QString(""),
            tr("All files (*.png)" ),
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
    m_badges->remove(m_currId);
    m_badgesModel.setStringList(m_badges->list());
    on_clearBadge_clicked();
    ui->editBadgeWidg->setHidden(true);
    ui->addBadgeWidg->setHidden(false);
}

void MainWindow::on_submitBadge_clicked()
{
    m_currBadge.name = ui->badgeName->text();
    if(m_currBadge.name.isEmpty())
    {
        ui->badgeErr->setText("The badge name\ncan't be empty.");
        return;
    }
    if(!m_badges->edit(m_currId, m_currBadge))
    {
        ui->badgeErr->setText("That badge name\nis already take.");
        return;
    }
    m_badgesModel.setStringList(m_badges->list());
    on_clearBadge_clicked();
    ui->editBadgeWidg->setHidden(true);
    ui->addBadgeWidg->setHidden(false);
}
