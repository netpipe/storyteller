#include "editnodedialog.hpp"
#include "ui_editnodedialog.h"
#include <qdebug.h>

EditNodeDialog::EditNodeDialog(QWidget *parent, MindmapNode *node, QVector<int> ids) :
    QDialog(parent),
    ui(new Ui::EditNodeDialog)
{
    ui->setupUi(this);
    setWindowTitle("Content of Node");
    currentNode = node;
    ui->title_edit->setText(node->getContent());
    ui->yes_list->setDragEnabled(true);
    ui->yes_list->setAcceptDrops(true);
    ui->yes_list->setDragDropMode(QAbstractItemView::InternalMove);
    ui->yes_list->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    for (const auto& item: node->getYesList()) {
        QListWidgetItem *listItem = new QListWidgetItem(item);
        listItem->setFlags(listItem->flags() | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
        ui->yes_list->addItem(listItem);
    }
    ui->no_list->setDragEnabled(true);
    ui->no_list->setAcceptDrops(true);
    ui->no_list->setDragDropMode(QAbstractItemView::InternalMove);
    ui->no_list->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    for (const auto& item: node->getNoList()) {
        QListWidgetItem *listItem = new QListWidgetItem(item);
        listItem->setFlags(listItem->flags() | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
        ui->no_list->addItem(listItem);
    }
    ui->maybe_list->setDragEnabled(true);
    ui->maybe_list->setAcceptDrops(true);
    ui->maybe_list->setDragDropMode(QAbstractItemView::InternalMove);
    ui->maybe_list->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    for (const auto& item: node->getMaybeList()) {
        QListWidgetItem *listItem = new QListWidgetItem(item);
        listItem->setFlags(listItem->flags() | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
        ui->maybe_list->addItem(listItem);
    }
    ui->description_edit->setText(node->getDescription());
    ui->node_number_combo_box->addItem(QString::number(node->getNodeId()));
    for (int i = 0; i < ids.length(); i++) if (ids[i] != node->getNodeId()) ui->node_number_combo_box->addItem(QString::number(ids[i]));
}

EditNodeDialog::~EditNodeDialog()
{
    delete ui;
}

void EditNodeDialog::on_yes_add_clicked()
{
    auto newItem = QInputDialog::getText(this, "Content", "Set content of the list item", QLineEdit::Normal);
    QListWidgetItem *listItem = new QListWidgetItem(newItem);
    listItem->setFlags(listItem->flags() | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
    ui->yes_list->addItem(listItem);
}


void EditNodeDialog::on_yes_remove_clicked()
{
    if (ui->yes_list->currentItem()) delete ui->yes_list->currentItem();
}


void EditNodeDialog::on_no_add_clicked()
{
    auto newItem = QInputDialog::getText(this, "Content", "Set content of the list item", QLineEdit::Normal);
    QListWidgetItem *listItem = new QListWidgetItem(newItem);
    listItem->setFlags(listItem->flags() | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
    ui->no_list->addItem(listItem);
}


void EditNodeDialog::on_no_remove_clicked()
{
    if (ui->no_list->currentItem()) delete ui->no_list->currentItem();
}


void EditNodeDialog::on_maybe_add_clicked()
{
    auto newItem = QInputDialog::getText(this, "Content", "Set content of the list item", QLineEdit::Normal);
    QListWidgetItem *listItem = new QListWidgetItem(newItem);
    listItem->setFlags(listItem->flags() | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
    ui->maybe_list->addItem(listItem);
}


void EditNodeDialog::on_maybe_remove_clicked()
{
    if (ui->maybe_list->currentItem()) delete ui->maybe_list->currentItem();
}


void EditNodeDialog::on_buttonBox_accepted()
{
//    qDebug() << "accepted";
    QStringList temp;
    currentNode->setContent(ui->title_edit->text());
    currentNode->setDescription(ui->description_edit->toPlainText());
    temp.clear();
    for (int i = 0; i < ui->yes_list->count(); i++) temp << ui->yes_list->item(i)->text();
    currentNode->setYesList(temp);
    temp.clear();
    for (int i = 0; i < ui->no_list->count(); i++) temp << ui->no_list->item(i)->text();
    currentNode->setNoList(temp);
    temp.clear();
    for (int i = 0; i < ui->maybe_list->count(); i++) temp << ui->maybe_list->item(i)->text();
    currentNode->setMaybeList(temp);
    const int updateID = ui->node_number_combo_box->currentText().toInt();
    if (updateID != currentNode->getNodeId()) emit nodeIDChanged(currentNode->getNodeId(), updateID);
}


void EditNodeDialog::on_buttonBox_rejected()
{
//    qDebug() << "rejected";
}

void EditNodeDialog::changeto(int to,int to2){


}

void EditNodeDialog::changefrom(int from,int from2){


}
