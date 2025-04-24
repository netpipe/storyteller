#ifndef EDITNODEDIALOG_HPP
#define EDITNODEDIALOG_HPP

#include <QDialog>
#include <QInputDialog>
#include <QVector>
#include "mindmapnode.hpp"

namespace Ui {
class EditNodeDialog;
}

class EditNodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditNodeDialog(QWidget *parent = nullptr, MindmapNode *node = nullptr, QVector<int> ids = QVector<int>());
    ~EditNodeDialog();
    MindmapNode *currentNode;

private slots:
    void on_yes_add_clicked();

    void on_yes_remove_clicked();

    void on_no_add_clicked();

    void on_no_remove_clicked();

    void on_maybe_add_clicked();

    void on_maybe_remove_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void changeto(int to,int to2);

    void changefrom(int from,int from2);
private:
    Ui::EditNodeDialog *ui;

signals:
    void nodeIDChanged(int _old, int _new);
};

#endif // EDITNODEDIALOG_HPP
