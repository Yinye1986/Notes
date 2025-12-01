#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QComboBox>
#include <QProcess>
#include <QMessageBox>
#include <QVBoxLayout>

extern QString list;
class MainWindow : public QMainWindow
{
    Q_OBJECT // 必须包含的宏，用于支持 Qt 的信号和槽机制

public:
    explicit MainWindow(QWidget *parent = nullptr); // 构造函数
    ~MainWindow(); // 析构函数


// protected:
//     void resizeEvent(QResizeEvent *event) override; // 声明 resizeEvent
private slots:
    void getList();
    void showPlatforms();
    void showBuildtools();
    void downloadPlatform(); // 用于下载选中的 platform

private:
    QPushButton *getlistButton; // 显示 platforms 列表的按钮
    QPushButton *platformsButton; // 显示 platforms 列表的按钮
    QPushButton *buildtoolsButton; // 显示 platforms 列表的按钮
    QTableWidget *packageTable; // 显示 platforms 列表的文本框
    QComboBox *platformsComboBox; // 选择 platform 的下拉框
    QPushButton *downloadButton; // 下载选中的 platform 的按钮
};

#endif // MAINWINDOW_H
