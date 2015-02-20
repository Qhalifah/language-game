#ifndef CHOOSEFROMLIST_H
#define CHOOSEFROMLIST_H

#include <QDialog>
#include <QMap>
#include <QString>
#include "screenqgv.h"

namespace Ui {
class chooseFromList;
}

class chooseFromList : public QDialog
{
    Q_OBJECT

public:
    enum Action {EDIT, ADD};
    chooseFromList(QStringList list, QStringList &delList,
                   QString comboName, Action action = EDIT, QWidget *parent = 0);
    QString getSelection();
    ~chooseFromList();

private slots:

    void closeEvent(QCloseEvent *event);

    void on_cancel_clicked();

    void on_action_clicked();

    void on_del_clicked();

private:
    bool m_closedByAction;
    Ui::chooseFromList *ui;
    QString m_selection;
    QStringList m_list;
    QStringList * m_deletedList;
};

#endif // CHOOSEFROMLIST_H
