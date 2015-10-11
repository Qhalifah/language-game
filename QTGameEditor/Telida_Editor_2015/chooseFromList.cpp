#include "choosefromlist.h"
#include "ui_choosefromlist.h"

chooseFromList::chooseFromList(QStringList list, QStringList & delList,
                               QString comboName, Action action, QWidget *parent):
    QDialog(parent),
    m_closedByAction(false),
    ui(new Ui::chooseFromList),
    m_selection(QString("")),
    m_list(list),
    m_deletedList(&delList)
{
    ui->setupUi(this);
    m_deletedList->clear();
    m_list.sort(Qt::CaseInsensitive);
    ui->select->clear();
    ui->select->addItem(comboName);
    ui->select->insertItems(1, m_list);
    if(action == Action::EDIT)
        ui->action->setText("Edit");
    else if(action == Action::ADD)
    {
        ui->del->setHidden(true);
        ui->action->setText("Add");
    }
}

QString chooseFromList::getSelection()
{
    this->exec();
    return m_selection;
}

chooseFromList::~chooseFromList()
{
    delete ui;
}

void chooseFromList::closeEvent(QCloseEvent *event)
{
    if(!m_closedByAction)
        m_selection.clear();
    this->close();
}

void chooseFromList::on_cancel_clicked()
{
    m_selection.clear();
    this->close();
}

void chooseFromList::on_action_clicked()
{
    if(ui->select->currentIndex() == 0)
        return;
    m_closedByAction = true;
    m_selection = ui->select->currentText();
    this->close();
}

void chooseFromList::on_del_clicked()
{
    if(ui->select->currentIndex() == 0)
        return;
    unsigned sceneID = MainWindow::m_scenes->id(ui->select->currentText());

    string name = "WorldMap";
    Map m(name, NULL);
    m.load();
    m.removeScene(sceneID);
    m.save();

    m_list.removeOne(ui->select->currentText());
    m_deletedList->append(ui->select->currentText());
    ui->select->removeItem(ui->select->currentIndex());
    QFile::remove(QString::fromStdString(".//db//scenes//" + std::to_string(sceneID) + ".scn"));
}
