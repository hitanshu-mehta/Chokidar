/********************************************************************************
** Form generated from reading UI file 'ui_mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QFrame *summary_frame;
    QHBoxLayout *horizontalLayout_5;
    QFrame *packet_frame;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QLabel *label_4;
    QFrame *frame;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QProgressBar *progressBar;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_6;
    QLabel *label_7;
    QPushButton *pushButton;
    QFrame *filter_frame;
    QHBoxLayout *horizontalLayout_8;
    QPlainTextEdit *plainTextEdit;
    QPushButton *pushButton_2;
    QFrame *graph_frame;
    QFrame *current_frame;
    QHBoxLayout *horizontalLayout;
    QFrame *frame_6;
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_8;
    QLCDNumber *lcdNumber;
    QLabel *label_9;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_10;
    QLCDNumber *lcdNumber_2;
    QLabel *label_11;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_12;
    QLCDNumber *lcdNumber_3;
    QLabel *label_13;
    QFrame *frame_7;
    QFrame *statistic_frame;
    QHBoxLayout *horizontalLayout_2;
    QFrame *frame_8;
    QFrame *frame_9;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        summary_frame = new QFrame(centralwidget);
        summary_frame->setObjectName(QString::fromUtf8("summary_frame"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(summary_frame->sizePolicy().hasHeightForWidth());
        summary_frame->setSizePolicy(sizePolicy1);
        summary_frame->setFrameShape(QFrame::StyledPanel);
        summary_frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_5 = new QHBoxLayout(summary_frame);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        packet_frame = new QFrame(summary_frame);
        packet_frame->setObjectName(QString::fromUtf8("packet_frame"));
        packet_frame->setFrameShape(QFrame::StyledPanel);
        packet_frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(packet_frame);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label = new QLabel(packet_frame);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_3->addWidget(label);

        label_2 = new QLabel(packet_frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_3 = new QLabel(packet_frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_4->addWidget(label_3);

        label_4 = new QLabel(packet_frame);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_4->addWidget(label_4);


        verticalLayout_2->addLayout(horizontalLayout_4);


        horizontalLayout_5->addWidget(packet_frame);

        frame = new QFrame(summary_frame);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_5 = new QLabel(frame);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_6->addWidget(label_5);

        progressBar = new QProgressBar(frame);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy2);
        progressBar->setValue(0);

        horizontalLayout_6->addWidget(progressBar);


        verticalLayout_3->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_6 = new QLabel(frame);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        sizePolicy.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy);

        horizontalLayout_7->addWidget(label_6);

        label_7 = new QLabel(frame);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(1);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy3);

        horizontalLayout_7->addWidget(label_7);


        verticalLayout_3->addLayout(horizontalLayout_7);


        horizontalLayout_5->addWidget(frame);

        pushButton = new QPushButton(summary_frame);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QSizePolicy sizePolicy4(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy4);
        pushButton->setCursor(QCursor(Qt::ArrowCursor));
        pushButton->setCheckable(true);

        horizontalLayout_5->addWidget(pushButton);


        verticalLayout->addWidget(summary_frame);

        filter_frame = new QFrame(centralwidget);
        filter_frame->setObjectName(QString::fromUtf8("filter_frame"));
        sizePolicy1.setHeightForWidth(filter_frame->sizePolicy().hasHeightForWidth());
        filter_frame->setSizePolicy(sizePolicy1);
        filter_frame->setFrameShape(QFrame::StyledPanel);
        filter_frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_8 = new QHBoxLayout(filter_frame);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        plainTextEdit = new QPlainTextEdit(filter_frame);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(plainTextEdit->sizePolicy().hasHeightForWidth());
        plainTextEdit->setSizePolicy(sizePolicy5);
        plainTextEdit->setBackgroundVisible(false);

        horizontalLayout_8->addWidget(plainTextEdit);

        pushButton_2 = new QPushButton(filter_frame);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout_8->addWidget(pushButton_2);


        verticalLayout->addWidget(filter_frame);

        graph_frame = new QFrame(centralwidget);
        graph_frame->setObjectName(QString::fromUtf8("graph_frame"));
        QSizePolicy sizePolicy6(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(6);
        sizePolicy6.setHeightForWidth(graph_frame->sizePolicy().hasHeightForWidth());
        graph_frame->setSizePolicy(sizePolicy6);
        graph_frame->setFrameShape(QFrame::StyledPanel);
        graph_frame->setFrameShadow(QFrame::Raised);

        verticalLayout->addWidget(graph_frame);

        current_frame = new QFrame(centralwidget);
        current_frame->setObjectName(QString::fromUtf8("current_frame"));
        QSizePolicy sizePolicy7(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(5);
        sizePolicy7.setHeightForWidth(current_frame->sizePolicy().hasHeightForWidth());
        current_frame->setSizePolicy(sizePolicy7);
        current_frame->setFrameShape(QFrame::StyledPanel);
        current_frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(current_frame);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        frame_6 = new QFrame(current_frame);
        frame_6->setObjectName(QString::fromUtf8("frame_6"));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        horizontalLayout_9 = new QHBoxLayout(frame_6);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_8 = new QLabel(frame_6);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setWordWrap(true);

        verticalLayout_4->addWidget(label_8);

        lcdNumber = new QLCDNumber(frame_6);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setDigitCount(5);
        lcdNumber->setProperty("intValue", QVariant(0));

        verticalLayout_4->addWidget(lcdNumber);

        label_9 = new QLabel(frame_6);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        verticalLayout_4->addWidget(label_9);


        horizontalLayout_9->addLayout(verticalLayout_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label_10 = new QLabel(frame_6);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        verticalLayout_5->addWidget(label_10);

        lcdNumber_2 = new QLCDNumber(frame_6);
        lcdNumber_2->setObjectName(QString::fromUtf8("lcdNumber_2"));
        lcdNumber_2->setDigitCount(5);
        lcdNumber_2->setProperty("intValue", QVariant(0));

        verticalLayout_5->addWidget(lcdNumber_2);

        label_11 = new QLabel(frame_6);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        verticalLayout_5->addWidget(label_11);


        horizontalLayout_9->addLayout(verticalLayout_5);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label_12 = new QLabel(frame_6);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        verticalLayout_6->addWidget(label_12);

        lcdNumber_3 = new QLCDNumber(frame_6);
        lcdNumber_3->setObjectName(QString::fromUtf8("lcdNumber_3"));
        lcdNumber_3->setDigitCount(5);
        lcdNumber_3->setProperty("intValue", QVariant(0));

        verticalLayout_6->addWidget(lcdNumber_3);

        label_13 = new QLabel(frame_6);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        verticalLayout_6->addWidget(label_13);


        horizontalLayout_9->addLayout(verticalLayout_6);


        horizontalLayout->addWidget(frame_6);

        frame_7 = new QFrame(current_frame);
        frame_7->setObjectName(QString::fromUtf8("frame_7"));
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);

        horizontalLayout->addWidget(frame_7);


        verticalLayout->addWidget(current_frame);

        statistic_frame = new QFrame(centralwidget);
        statistic_frame->setObjectName(QString::fromUtf8("statistic_frame"));
        sizePolicy7.setHeightForWidth(statistic_frame->sizePolicy().hasHeightForWidth());
        statistic_frame->setSizePolicy(sizePolicy7);
        statistic_frame->setFrameShape(QFrame::StyledPanel);
        statistic_frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(statistic_frame);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        frame_8 = new QFrame(statistic_frame);
        frame_8->setObjectName(QString::fromUtf8("frame_8"));
        frame_8->setFrameShape(QFrame::StyledPanel);
        frame_8->setFrameShadow(QFrame::Raised);

        horizontalLayout_2->addWidget(frame_8);

        frame_9 = new QFrame(statistic_frame);
        frame_9->setObjectName(QString::fromUtf8("frame_9"));
        frame_9->setFrameShape(QFrame::StyledPanel);
        frame_9->setFrameShadow(QFrame::Raised);

        horizontalLayout_2->addWidget(frame_9);


        verticalLayout->addWidget(statistic_frame);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 38));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionQuit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "IDS", nullptr));
        actionQuit->setText(QCoreApplication::translate("MainWindow", "Quit", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Packet Captured:", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Packet Discared :", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Buffer usage:", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Filter state:", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "All packets captured", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Capture", nullptr));
        plainTextEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Enter the filter expression here...", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "done", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Network Utilization", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "MBits/s", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Packets", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "Packet/s", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "S1", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "unit", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
