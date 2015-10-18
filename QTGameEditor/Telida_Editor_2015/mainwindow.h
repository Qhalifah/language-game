#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "backgroundmusic.h"
#include <QProcess>
#include "filemanager.h"
#include <QStringListModel>
#include <QStringList>
#include <QMap>
#include "dbmap.h"
#include "maindictionary.h"
#include "dictionarysets.h"
#include "activities.h"
#include "badges.h"
#include "map.h"
#include <QMediaPlayer>
#include <QURL>
#include <windows.h>
#include <iostream>
#include <shlobj.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static Ui::MainWindow *ui;
    static DBMap * m_scenes;
    static Activities * m_pairActs;
    static Activities * m_matchActs;
    static MainDictionary * m_mainDict;
    static DictionarySets * m_dictSets;
    static Badges * m_badges;

private slots:

    void closeEvent(QCloseEvent *event);

    void on_loadBGI_clicked();

    void on_addBGM_clicked();

    void on_actionExit_triggered();

    void on_playBGM_clicked(bool checked);

    void on_delBGM_clicked();

    void on_selectBGM_activated(const QString &arg1);

    void on_selectBGM_currentIndexChanged(const QString &arg1);

    void on_vol_valueChanged(int value);

    void on_pitch_valueChanged(int value);

    void on_addImg_clicked();

    void on_delBGI_clicked();

    void on_addText_clicked();

    void on_del_clicked();

    void on_editText_clicked();

    void on_textColor_clicked();

    void on_imgLines_toggled(bool checked);

    void on_hotspotLines_toggled(bool checked);

    void on_TextLines_toggled(bool checked);

    void on_addSnd_clicked();

    void on_delSnd_clicked();

    void on_editHLCol_clicked();

    void on_sndVol_valueChanged(int value);

    void on_sndPitch_valueChanged(int value);

    void on_hovLineEdit_textEdited(const QString &arg1);

    void on_order_valueChanged(int arg1);

    void on_mD_engage_chkBox_toggled(bool checked);

    void on_mD_select_chkBox_toggled(bool checked);

    void on_mU_engage_chkBox_toggled(bool checked);

    void on_mU_select_chkBox_toggled(bool checked);

    void on_hov_engage_chkBox_toggled(bool checked);

    void on_hov_select_chkBox_toggled(bool checked);

    void on_widthRatio_valueChanged(int arg1);

    void on_heightRatio_valueChanged(int arg1);

    void on_actionThe_Dictionary_triggered();

    void on_addWord_clicked();

    void on_mainDictView_clicked(const QModelIndex &index);

    void on_addImg_word_clicked();

    void on_addFemVoice_clicked();

    void on_addMaleVoice_clicked();

    void on_playFemVoice_clicked();

    void on_playMaleVoice_clicked();

    void on_clearWord_clicked();

    void on_submitWord_clicked();

    void on_delWord_clicked();

    void on_addDashed_L_clicked();

    void on_addDashed_l_clicked();

    void on_actionA_Dictionary_Set_triggered();

    void on_actionNew_Dictionary_SEt_triggered();

    void on_dictSetDone_clicked();

    void saveDictSet(shared_ptr<Dictionary> &dict);

    void on_dictSetAdd_clicked();

    void on_mainDictToDictSet_clicked();

    void on_actionThe_Map_triggered();

    void on_delDictSetWord_clicked();

    //void on_copyDictSetTo_clicked();

    void on_saveEditor_clicked();

    void on_actionNew_Scene_triggered();

    void on_actionA_Scene_triggered();

    void on_goToScene_activated(const QString &arg1);

    void on_addHotspot_clicked();

    void on_addBox_clicked();

    void on_boxLines_toggled(bool checked);

    void on_actionAn_Activity_triggered();

    void on_actionNew_Activity_triggered();

    void on_addScreen_clicked();

    void on_editScreen_clicked();

    void on_numChoices_valueChanged(int arg1);

    void on_numRounds_valueChanged(int arg1);

    void on_selectDictSet_activated(const QString &arg1);

    void on_goToActivity_activated(const QString &arg1);

    void on_badgesView_clicked(const QModelIndex &index);

    void on_addDashed_L_2_clicked();

    void on_addDashed_l_2_clicked();

    void on_addImgForBadge_clicked();

    void on_clearBadge_clicked();

    void on_addBadge_clicked();

    void on_delBadge_clicked();

    void on_submitBadge_clicked();

    void on_actionThe_Badges_triggered();

    void on_selReqBadge_activated(const QString &arg1);

    void on_addBadgeToScn_clicked();

    void on_delBadgeFromScn_clicked();

    void on_chooseRewardBadge_activated(const QString &arg1);

    void on_setActPieceImg_clicked();

    void on_setActRewardImg_clicked();

    void on_maxScore_valueChanged(int arg1);

    void on_actionAbout_triggered();

public:
    void hideRightHandToolbarItems();

    // Used to get the user's Documents file path
    wchar_t cStr[MAX_PATH];
    std::string m_DocumentsPath;

private:
    void setupScreenQGVWidgets();
    bool remindUserToSave();

    BackgroundMusic m_BGM;
    QProcess m_soundApp;
    FileManager m_fileManager;
    QString m_mapStr;
    QString m_sceneStr;
    QString m_hotspotStr;

    // Dictionary variables
    QStringListModel m_mainDictModel;
    QStringListModel m_dictSetModel;
    string dictSetOriginalName;
    QPixmap m_wordPixmap;
    MainDictionary::Word m_currWord;
    wstring m_originalWordName;
    unsigned m_currId;

    // Badges variables
    QStringListModel m_badgesModel;
    QPixmap m_badgePixmap;
    Badges::Badge m_currBadge;
    QString m_original_badgeName;
};

#endif // MAINWINDOW_H
