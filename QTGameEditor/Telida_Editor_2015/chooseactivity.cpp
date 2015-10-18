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
    bool actIsPAIR = true;
    // Get the index of the selection
    int actIDBeingDeleted;
    // Get the name of the selection
    string actFileNameBeingDeleted;
    if(ui->pairingRadBtn->isChecked())
    {
        actIDBeingDeleted = MainWindow::m_pairActs->id(ui->select->currentText());
        actFileNameBeingDeleted = "p" + std::to_string(actIDBeingDeleted);
    }
    else
    {
        actIDBeingDeleted = MainWindow::m_matchActs->id(ui->select->currentText());
        actFileNameBeingDeleted = "m" + std::to_string(actIDBeingDeleted);
        actIsPAIR = false;
    }
    // Create a Scene object pointer
    Scene *sceneObjectPtr;
    // Get the list of the Scenes in the Scene folder
    QStringList qStrList;
    QDir dir(".//db//scenes//");
    dir.setNameFilters(QStringList("*.scn"));
    qStrList = dir.entryList();
    // Go through each Scene
    for(int ii=0; ii < qStrList.size(); ++ii)
    {
        // Get the list of ScreenItems
        string sceneFileName = qStrList[ii].toStdString();
        string sceneID = "";
        for(int ii=0; ii<sceneFileName.size(); ++ii)
        {
            if(sceneFileName[ii] == '.')
                break;
            sceneID.append(sceneFileName, ii, 1);
        }
        sceneObjectPtr = new Scene(sceneID);
        sceneObjectPtr->load();
        for(int jj=0; jj < sceneObjectPtr->m_activities.size(); ++jj)
        {
            // Check if the MiniGame has the same GameType
            if((sceneObjectPtr->m_activities[jj].first == GameType::PAIR && actIsPAIR) ||
                    (sceneObjectPtr->m_activities[jj].first == GameType::MATCHING && !actIsPAIR))
            {
                // If the ScreenItem has this Activity, set the GameType to NONE
                string actFileNameFromScene = sceneObjectPtr->m_activities[jj].second;
                // Check if the file name for the Activity is same in the
                if(actFileNameFromScene == actFileNameBeingDeleted)
                {
                    // Delete the Activity data from the m_activities vector
                    sceneObjectPtr->m_activities.erase(sceneObjectPtr->m_activities.begin() + jj);
                    // Go through the m_requirements and remove any entry that has the actIDBeingDeleted
                    for(auto itr = sceneObjectPtr->m_requirements.begin(); itr != sceneObjectPtr->m_requirements.end(); ++itr)
                    {
                        if(itr->second == actFileNameFromScene)
                        {
                            auto iter = sceneObjectPtr->m_requirements.find(itr->first);
                            sceneObjectPtr->m_requirements.erase(itr);
                            break;
                        }
                    }
                    sceneObjectPtr->save();
                    break;
                }
            }
        }
    }
    // Delete the Activity from the main Activities map
    if(actIsPAIR)
    {
        m_pairActs->remove(ui->select->currentText());
    }
    else
    {
        m_matchActs->remove(ui->select->currentText());
    }
    // Delete the Actvity from this window's dropdown menu
    ui->select->removeItem(ui->select->currentIndex());


    // Get the user's Documents folder path
    wchar_t cStr[MAX_PATH];
    SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, DWORD("FOLDERID_Documents"), cStr);
    string DocumentsPath = QString::fromStdWString(cStr).toStdString();

    // Actually delete the Activity's file from the activities folder in the file system
    QFile::remove(QString::fromStdString(DocumentsPath+"//Dinaki Adventures//db//activities//" + actFileNameBeingDeleted + ".act"));
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
