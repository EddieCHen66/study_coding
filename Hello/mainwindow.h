#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

class QLineEdit;
class QDialog;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void newFile();    //新建文件

    bool maybeSave();  //判断是否需要保存

    bool save();       //保存操作

    bool savaAs();     //另存为操作

    bool saveFile(const QString &fileName);  //保存文件

    bool loadFile(const QString &fileName);

private slots:

    void on_action_New_triggered();

    void on_action_Save_triggered();

    void on_action_SaveAs_triggered();

    void on_action_Open_triggered();

    void on_action_Close_triggered();

    void action_Find_triggered();

    void action_Exit_triggered();

    void action_state_triggered();

    void on_action_Undo_triggered();

    void on_action_Cut_triggered();


    void on_action_Copy_triggered();

    void on_action_Paste_triggered();

    void showFindText();

private:
    Ui::MainWindow *ui;
    //判断文件是否保存
    bool isUntitled;
    //保存当前文件的路径
    QString curFile;

    QLineEdit *findLineEdit;
    QDialog *findDlg;

    QLabel *statusLabel;

protected:
    void closeEvent(QCloseEvent *event);
};


#endif // MAINWINDOW_H
