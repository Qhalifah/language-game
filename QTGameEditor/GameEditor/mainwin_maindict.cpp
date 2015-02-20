#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QStringList>
#include <QFileDialog>
#include "maindictionary.h"
#include "dictionarysets.h"

// Main Dictionary *****************************************************

void MainWindow::on_addWord_clicked()
{
    m_currWord.name = ui->dinWord->text();
    if(m_currWord.name.isEmpty())
    {
        ui->wordErrorMsg->setText(QString("The Dinak'i word\ncan't be empty."));
        return;
    }
    if(m_currWord.img.isEmpty())
    {
        ui->wordErrorMsg->setText(QString("There must be an image\nassociated with the word."));
        return;
    }
    if(m_currWord.girlSnd.isEmpty())
    {
        ui->wordErrorMsg->setText(QString("There must be a female voice\npronunciation associated with the word."));
        return;
    }
    if(m_currWord.boySnd.isEmpty())
    {
        ui->wordErrorMsg->setText(QString("There must be a male voice\npronunciation associated with the word."));
        return;
    }
    if(!m_mainDict->add(m_currWord))
    {
        ui->wordErrorMsg->setText(QString("That word is already in the dictionary."));
        return;
    }
    m_mainDictModel.setStringList(m_mainDict->list());
    on_clearWord_clicked();
}

void MainWindow::on_mainDictView_clicked(const QModelIndex &index)
{
    QString dinName = m_mainDictModel.stringList().at(index.row());
    m_currId = m_mainDict->id(dinName);

    m_currWord = m_mainDict->word(m_currId);
    ui->dinWord->setText(m_currWord.name);
    ui->wordImgName->setText(QFileInfo(m_currWord.img).baseName());
    ui->femVoiceName->setText(QFileInfo(m_currWord.girlSnd).baseName());
    ui->maleVoiceName->setText(QFileInfo(m_currWord.boySnd).baseName());
    m_wordPixmap.load(m_currWord.img);
    ui->wordImg->setPixmap(m_wordPixmap.scaled(50, 50,
                                               Qt::KeepAspectRatioByExpanding,
                                               Qt::FastTransformation));

    ui->editWordWidg->setHidden(false);
    ui->addWordWidg->setHidden(true);
}

void MainWindow::on_addImg_word_clicked()
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
                                             FileManager::WORDIMG);
    m_currWord.img = newFile;
    ui->wordImgName->setText(QFileInfo(newFile).baseName());
    m_wordPixmap.load(newFile);
    ui->wordImg->setPixmap(m_wordPixmap.scaled(50, 50,
                                               Qt::KeepAspectRatioByExpanding,
                                               Qt::FastTransformation));
}

void MainWindow::on_addFemVoice_clicked()
{
    QString selfilter = tr("All files (*.wav)");
    QString filepath = QFileDialog::getOpenFileName(
            this,
            QString("Select Sound"),
            QString(""),
            tr("All files (*.wav)" ),
            &selfilter
    );
    if(filepath.isEmpty())
        return;
    QString newFile = m_fileManager.copyFile(filepath,
                                             FileManager::FEMSND);
    m_currWord.girlSnd = newFile;
    ui->femVoiceName->setText(QFileInfo(newFile).baseName());
}

void MainWindow::on_addMaleVoice_clicked()
{
    QString selfilter = tr("All files (*.wav)");
    QString filepath = QFileDialog::getOpenFileName(
            this,
            QString("Select Sound"),
            QString(""),
            tr("All files (*.wav)" ),
            &selfilter
    );
    if(filepath.isEmpty())
        return;
    QString newFile = m_fileManager.copyFile(filepath,
                                             FileManager::MALESND);
    m_currWord.boySnd = newFile;
    ui->maleVoiceName->setText(QFileInfo(newFile).baseName());
}

void MainWindow::on_playFemVoice_clicked()
{
    if(m_currWord.girlSnd.isEmpty())
        return;
    ofstream ofile(".\\ComSnd.txt");
    if(!ofile.is_open())
       return;
    int playSound = true;
    ofile << playSound;
    ofile << "\n";
    ofile << 100;
    ofile << "\n";
    ofile << 100;
    ofile << "\n";
    ofile << m_currWord.girlSnd.toStdString();
    ofile.close();
    m_soundApp.putChar('c');
}

void MainWindow::on_playMaleVoice_clicked()
{
    if(m_currWord.boySnd.isEmpty())
        return;
    ofstream ofile(".\\ComSnd.txt");
    if(!ofile.is_open())
       return;
    int playSound = true;
    ofile << playSound;
    ofile << "\n";
    ofile << 100;
    ofile << "\n";
    ofile << 100;
    ofile << "\n";
    ofile << m_currWord.boySnd.toStdString();
    ofile.close();
    m_soundApp.putChar('c');
}

void MainWindow::on_clearWord_clicked()
{
    ui->dinWord->clear();
    ui->wordImgName->clear();
    ui->femVoiceName->clear();
    ui->maleVoiceName->clear();
    ui->wordErrorMsg->clear();
    m_wordPixmap = QPixmap(50, 50);
    m_wordPixmap.fill(Qt::white);
    m_currWord.clear();
    m_currId = 0;
    ui->wordImg->setPixmap(m_wordPixmap);
}

void MainWindow::on_submitWord_clicked()
{
    m_currWord.name = ui->dinWord->text();
    if(m_currWord.name.isEmpty())
    {
        ui->wordErrorMsg->setText("The Dinak'i word\ncan't be empty.");
        return;
    }
    if(!m_mainDict->edit(m_currId, m_currWord))
    {
        ui->wordErrorMsg->setText("That word is already\nin the dictionary.");
        return;
    }
    m_mainDictModel.setStringList(m_mainDict->list());
    on_clearWord_clicked();
    ui->editWordWidg->setHidden(true);
    ui->addWordWidg->setHidden(false);
}

void MainWindow::on_delWord_clicked()
{
    m_mainDict->remove(m_currId);
    m_mainDictModel.setStringList(m_mainDict->list());
    on_clearWord_clicked();
    ui->editWordWidg->setHidden(true);
    ui->addWordWidg->setHidden(false);
}

void MainWindow::on_addDashed_L_clicked()
{
    ui->dinWord->setText(ui->dinWord->text() + QString("Ł"));
    ui->dinWord->setFocus();
}

void MainWindow::on_addDashed_l_clicked()
{
    ui->dinWord->setText(ui->dinWord->text() + QString("ł"));
    ui->dinWord->setFocus();
}
