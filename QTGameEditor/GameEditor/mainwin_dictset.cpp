#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QStringList>
#include "maindictionary.h"
#include "dictionarysets.h"
#include <QDebug>
// Dictionary Set Add/Edit

void MainWindow::on_dictSetDone_clicked()
{
    if(ui->dictSetName->text().isEmpty())
    {
        ui->dictSetErr->setText(QString("The dictionary set\nname cannot be empty."));
        return;
    }
    if(!m_dictSets->editDictSet(m_currId, ui->dictSetName->text(),
                               m_dictSetModel.stringList(), m_mainDict))
    {
        ui->dictSetErr->setText(QString("That dictionary set\nname already exists."));
        return;
    }
    ui->stackedWidget->setCurrentWidget(ui->mainMenu);
}

void MainWindow::on_dictSetAdd_clicked()
{
    if(ui->dictSetName->text().isEmpty())
    {
        ui->dictSetErr->setText(QString("The dictionary set\nname cannot be empty."));
        return;
    }
    unsigned id = m_dictSets->addDictSet(ui->dictSetName->text(), m_dictSetModel.stringList(),
                                          m_mainDict);
    if(!id)
    {
        ui->dictSetErr->setText(QString("That dictionary set\nname already exists."));
        return;
    }
    ui->stackedWidget->setCurrentWidget(ui->mainMenu);
}

void MainWindow::on_mainDictToDictSet_clicked()
{
    ui->dictSetErr->clear();
    if(ui->mainDictView2->currentIndex().row() < 0)
        return;

    QString wordName = m_mainDictModel.stringList().at(ui->mainDictView2->currentIndex().row());
    QStringList list = m_dictSetModel.stringList();
    if(list.contains(wordName))
    {
        ui->dictSetErr->setText(QString("That word is already\nin the dictionary set."));
        return;
    }
    list.append(wordName);
    list.sort(Qt::CaseInsensitive);
    m_dictSetModel.setStringList(list);
}

void MainWindow::on_delDictSetWord_clicked()
{
    ui->dictSetErr->clear();
    if(ui->dictSetView->currentIndex().row() < 0)
        return;
    QStringList list = m_dictSetModel.stringList();
    list.removeAt(ui->dictSetView->currentIndex().row());
    m_dictSetModel.setStringList(list);
}
/*
void MainWindow::on_copyDictSetTo_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter Dictionary Name to Copy to."),
                                         tr("Dictionary Name:"), QLineEdit::Normal,
                                         QString(""), &ok);
    if(!ok)
        return;
    if(text.isEmpty())
    {
        ui->dictSetErr->setText(QString("The Dictionary Name\ncannot be empty."));
        return;
    }
    if(m_dictSetData.contains(text))
    {
        ui->dictSetErr->setText(QString("There is already a\ndictionary set with that name."));
        return;
    }
    m_dictSetData[text] = m_dictSetList;
    m_fileManager.saveDictSet(m_dictSetData);
}
*/
