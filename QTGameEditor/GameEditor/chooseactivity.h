#ifndef CHOOSEACTIVITY_H
#define CHOOSEACTIVITY_H

#include <QDialog>
#include <QMap>
#include <QString>
#include "screenqgv.h"
#include "activities.h"

namespace Ui {
class ChooseActivity;
}

class ChooseActivity : public QDialog
{
    Q_OBJECT
public:
    explicit ChooseActivity(Activities * matchActs,
                            Activities * pairActs,
            QWidget *parent = 0);
    ~ChooseActivity();
    QString getSelection();
    ScreenQGV::EditorType getEditorType();

private slots:

    void on_cancel_clicked();

    void closeEvent(QCloseEvent *event);

    void on_edit_clicked();

    void on_del_clicked();

    void on_matchingRadBtn_toggled(bool checked);

    void on_pairingRadBtn_toggled(bool checked);

private:
    bool m_closeByEdit;
    QString m_selection;
    ScreenQGV::EditorType m_edtype;
    Activities * m_matchActs;
    Activities * m_pairActs;
    Ui::ChooseActivity *ui;
};

#endif // CHOOSEACTIVITY_H
