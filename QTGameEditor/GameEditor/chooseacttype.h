#ifndef CHOOSEACTTYPE_H
#define CHOOSEACTTYPE_H

#include <QDialog>
#include "screenqgv.h"

namespace Ui {
class ChooseActType;
}

class ChooseActType : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseActType(QWidget *parent = 0);
    ~ChooseActType();
    ScreenQGV::EditorType getEditorType();
private slots:
    void on_matchingRadBtn_toggled(bool checked);

    void on_pairingRadBtn_toggled(bool checked);

    void on_cancel_clicked();

    void closeEvent(QCloseEvent *event);

    void on_ok_clicked();

private:
    Ui::ChooseActType *ui;
    ScreenQGV::EditorType m_edtype;
    bool m_closedByOkButton;
};

#endif // CHOOSEACTTYPE_H
