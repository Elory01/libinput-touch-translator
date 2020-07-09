#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "shortcut-dialog.h"

#include <QSettings>
#include <QMessageBox>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_settings = new QSettings(QString("/etc/xdg/ukui/gestures.conf"), QSettings::NativeFormat, this);
    qDebug()<<m_settings->fileName();

    ui->tableWidget->setEnabled(m_settings->isWritable());
    ui->tableWidget_2->setEnabled(m_settings->isWritable());

    if (!m_settings->isWritable()) {
        QMessageBox::information(0, 0, tr("You have not premisson to change shortcut settings. \n"
                                          "Run this application with sudo might help."));
    }

    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(3);

    ui->tableWidget_2->setColumnCount(2);
    ui->tableWidget_2->setRowCount(3);

    auto leftHeader = new QTableWidgetItem(tr("Left"));
    ui->tableWidget->setHorizontalHeaderItem(0, leftHeader);
    auto rightHeader = new QTableWidgetItem(tr("Right"));
    ui->tableWidget->setHorizontalHeaderItem(1, rightHeader);
    auto upHeader = new QTableWidgetItem(tr("Up"));
    ui->tableWidget->setHorizontalHeaderItem(2, upHeader);
    auto downHeader = new QTableWidgetItem(tr("Down"));
    ui->tableWidget->setHorizontalHeaderItem(3, downHeader);

    auto zoomInHeader = new QTableWidgetItem(tr("ZoomIn"));
    ui->tableWidget_2->setHorizontalHeaderItem(0, zoomInHeader);
    auto zoomOutHeader = new QTableWidgetItem(tr("zoomOut"));
    ui->tableWidget_2->setHorizontalHeaderItem(1, zoomOutHeader);

    m_settings->beginGroup("touch screen");
    for (int i = 3; i < 6; i++) {
        // table1
        m_settings->beginGroup("Swipe");
        m_settings->beginReadArray("Finished");
        m_settings->setArrayIndex(i);

        QTableWidgetItem *label = new QTableWidgetItem;
        label->setText(tr("%1 Finger, Finished").arg(i));
        ui->tableWidget->setVerticalHeaderItem(i - 3, label);

        QTableWidgetItem *leftItem = new QTableWidgetItem;
        auto left = m_settings->value("Left").toString();
        leftItem->setText(left);
        ui->tableWidget->setItem(i - 3, 0, leftItem);

        QTableWidgetItem *rightItem = new QTableWidgetItem;
        auto right = m_settings->value("Right").toString();
        rightItem->setText(right);
        ui->tableWidget->setItem(i - 3, 1, rightItem);

        QTableWidgetItem *upItem = new QTableWidgetItem;
        auto up = m_settings->value("Up").toString();
        upItem->setText(up);
        ui->tableWidget->setItem(i - 3, 2, upItem);

        QTableWidgetItem *downItem = new QTableWidgetItem;
        auto down = m_settings->value("Down").toString();
        downItem->setText(down);
        ui->tableWidget->setItem(i - 3, 3, downItem);

        m_settings->endArray();
        m_settings->endGroup();

        // table2
        auto label2 = new QTableWidgetItem(tr("%1 Finger, Finished").arg(i));
        ui->tableWidget_2->setVerticalHeaderItem(i - 3, label2);

        m_settings->beginGroup("Zoom");
        m_settings->beginReadArray("Finished");
        m_settings->setArrayIndex(i);

        auto zoomInItem = new QTableWidgetItem;
        auto zoomIn = m_settings->value("ZoomIn").toString();
        zoomInItem->setText(zoomIn);
        ui->tableWidget_2->setItem(i - 3, 0, zoomInItem);

        auto zoomOutItem = new QTableWidgetItem;
        auto zoomOut = m_settings->value("ZoomOut").toString();
        zoomOutItem->setText(zoomOut);
        ui->tableWidget_2->setItem(i - 3, 1, zoomOutItem);

        m_settings->endArray();
        m_settings->endGroup();
    }
    m_settings->endGroup();

    connect(ui->tableWidget, &QTableWidget::cellClicked, this, [=](int row, int column){
        qDebug()<<row<<column;
        auto item = ui->tableWidget->item(row, column);
        QKeySequence shortcut = QKeySequence(item->text());
        ShortcutDialog dlg(shortcut);
        if (dlg.exec()) {
            auto shortcut = dlg.getKeySequence();
            item->setText(shortcut.toString());

            QString direction;
            switch (column) {
            case 0:
                direction = "Left";
                break;
            case 1:
                direction = "Right";
                break;
            case 2:
                direction = "Up";
                break;
            case 3:
                direction = "Down";
                break;
            default:
                return;
            }

            m_settings->beginGroup("touch screen");
            m_settings->beginGroup("Swipe");

            m_settings->beginWriteArray("Finished");
            m_settings->setArrayIndex(row + 3);
            m_settings->setValue(direction, shortcut);
            m_settings->endArray();

            m_settings->endGroup();
            m_settings->endGroup();

            m_settings->sync();
        }
    });

    connect(ui->tableWidget_2, &QTableWidget::cellClicked, this, [=](int row, int column){
        auto item = ui->tableWidget_2->item(row, column);
        QKeySequence shortcut = QKeySequence(item->text());

        ShortcutDialog dlg(shortcut);
        if (dlg.exec()) {
            auto shortcut = dlg.getKeySequence();
            item->setText(shortcut.toString());

            QString direction;
            switch (column) {
            case 0:
                direction = "ZoomIn";
                break;
            case 1:
                direction = "ZoomOut";
                break;
            default:
                return;
            }

            m_settings->beginGroup("touch screen");
            m_settings->beginGroup("Zoom");

            m_settings->beginWriteArray("Finished");
            m_settings->setArrayIndex(row + 3);
            m_settings->setValue(direction, shortcut);
            m_settings->endArray();

            m_settings->endGroup();
            m_settings->endGroup();

            m_settings->sync();
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
