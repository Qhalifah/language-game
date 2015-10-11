#include "chooseacttype.h"
#include "ui_chooseacttype.h"
#include "screenqgv.h"

ChooseActType::ChooseActType(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseActType),
    m_closedByOkButton(false)
{
    ui->setupUi(this);
    if(ui->pairingRadBtn->isChecked())
        m_edtype = ScreenQGV::PAIRACT;
    else
        m_edtype = ScreenQGV::MATCHACT;
}

ChooseActType::~ChooseActType()
{
    delete ui;
}

ScreenQGV::EditorType ChooseActType::getEditorType()
{
    this->exec();
    return m_edtype;
}

void ChooseActType::on_matchingRadBtn_toggled(bool checked)
{
    if(checked)
    {
        m_edtype = ScreenQGV::MATCHACT;
    }
}

void ChooseActType::on_pairingRadBtn_toggled(bool checked)
{
    if(checked)
    {
        m_edtype = ScreenQGV::PAIRACT;
    }
}

void ChooseActType::on_cancel_clicked()
{
    m_edtype = ScreenQGV::NONE;
    this->close();
}

void ChooseActType::closeEvent(QCloseEvent *event)
{
    if(!m_closedByOkButton)
        m_edtype = ScreenQGV::NONE;
    this->close();
}

void ChooseActType::on_ok_clicked()
{
    m_closedByOkButton = true;
    this->close();
}
