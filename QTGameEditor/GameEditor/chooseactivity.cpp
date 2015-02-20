#include "chooseactivity.h"
#include "ui_chooseactivity.h"

ChooseActivity::ChooseActivity(Activities *matchActs,
                               Activities *pairActs, QWidget *parent) :
    QDialog(parent),
    m_closeByEdit(false),
    ui(new Ui::ChooseActivity),
    m_selection(QString("")),
    m_matchActs(matchActs), m_pairActs(pairActs)
{
    ui->setupUi(this);
    ui->select->clear();
    ui->select->addItem(QString("Choose an Activity"));
    if(ui->pairingRadBtn->isChecked())
    {
        ui->select->insertItems(1, m_pairActs->list());
    }
    else
    {
        ui->select->insertItems(1, m_matchActs->list());
    }
}

QString ChooseActivity::getSelection()
{
    this->exec();
    return m_selection;
}

ScreenQGV::EditorType ChooseActivity::getEditorType()
{
    return m_edtype;
}

ChooseActivity::~ChooseActivity()
{
    delete ui;
}

void ChooseActivity::on_cancel_clicked()
{
    m_selection.clear();
    this->close();
}

void ChooseActivity::closeEvent(QCloseEvent *event)
{
    if(!m_closeByEdit)
        m_selection.clear();
    this->close();
}

void ChooseActivity::on_edit_clicked()
{
    if(ui->select->currentIndex() == 0)
        return;
    m_selection = ui->select->currentText();
    m_closeByEdit = true;
    if(ui->pairingRadBtn->isChecked())
        m_edtype = ScreenQGV::PAIRACT;
    else
        m_edtype = ScreenQGV::MATCHACT;
    this->close();
}

void ChooseActivity::on_del_clicked()
{
    if(ui->select->currentIndex() == 0)
        return;
    if(ui->pairingRadBtn->isChecked())
    {
        m_pairActs->remove(ui->select->currentText());
    }
    else
    {
        m_matchActs->remove(ui->select->currentText());
    }
    ui->select->removeItem(ui->select->currentIndex());
}

void ChooseActivity::on_matchingRadBtn_toggled(bool checked)
{
    if(checked)
    {
        ui->select->clear();
        ui->select->addItem(QString("Choose an Activity"));
        ui->select->insertItems(1, m_matchActs->list());
    }
}

void ChooseActivity::on_pairingRadBtn_toggled(bool checked)
{
    if(checked)
    {
        ui->select->clear();
        ui->select->addItem(QString("Choose an Activity"));
        ui->select->insertItems(1, m_pairActs->list());
    }
}
