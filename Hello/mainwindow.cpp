#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QPushButton>
#include <QTextStream>
#include <QLineEdit>
#include <QDialog>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //初始化文件为未保存状态
    isUntitled = true;
    //初始化文件名为"未命名.txt"
    curFile = "未命名.txt";
    //初始化窗口标题为文件名
    setWindowTitle(curFile);

    //创建新的动作
    QAction *action_Exit = new QAction(tr("&退出(&X)"),this); //创建新的动作
    //添加图标
    QIcon icon(":/myImages/images/actions/application-exit.png");
    action_Exit->setIcon(icon);
    //设置快捷键
    action_Exit->setShortcut(QKeySequence("Ctrl+x"));
    //在文件菜单中设置新的打开动作
    ui->menu_f->addAction(action_Exit);
    action_Exit->setStatusTip("退出程序");

    //查找动作
    QAction *action_Find = new QAction("&查找(&F)",this);
    QIcon findicon(":/myImages/images/actions/filefind.png");
    action_Find->setIcon(findicon);
    action_Find->setShortcut(QKeySequence("Ctrl+f"));
    ui->menu_D->addAction(action_Find);
    ui->mainToolBar->addAction(action_Find);
    action_Find->setStatusTip("查找文件");

    //版本说明
    QAction *action_state = new QAction("&版本说明",this);
    QIcon stateicon(":/myImages/images/actions/gtk-help.png");
    action_state->setIcon(stateicon);
    ui->menu_V->addAction(action_state);
    action_state->setStatusTip("版本说明");


    connect(action_Exit,SIGNAL(triggered(bool)),this,SLOT(action_Exit_triggered()));
    connect(action_state,SIGNAL(triggered(bool)),this,SLOT(action_state_triggered()));
    connect(action_Find,SIGNAL(triggered(bool)),this,SLOT(action_Find_triggered()));

    findDlg = new QDialog(this);  //新建一个窗口
    findDlg->setWindowTitle(QString("查找")); //设置窗口标题
    findLineEdit = new QLineEdit(findDlg);      //添加行编辑
    QPushButton *btn = new QPushButton(tr("查找下一个"),findDlg); //添加按钮
    QVBoxLayout *layout = new QVBoxLayout(findDlg);     //新建布局
    //将控件放入布局中
    layout->addWidget(findLineEdit);
    layout->addWidget(btn);
    connect(btn,SIGNAL(clicked(bool)),this,SLOT(showFindText()));


//    ui->statusBar->showMessage(tr("欢迎来到vip记事本"));   //设置临时消息
    statusLabel = new QLabel;
    statusLabel->setMinimumSize(150,20); //设置标签最小大小
    statusLabel->setFrameShape(QFrame::WinPanel);   //设置标签形状
    statusLabel->setFrameShadow(QFrame::Sunken);    //设置标签阴影
    ui->statusBar->addWidget(statusLabel);  //添加状态栏
    statusLabel->setText("欢迎您VVVIP");

    QLabel *permanent = new QLabel(this);
    permanent->setFrameStyle(QFrame::Box | QFrame::Sunken);
    permanent->setText("<a href=\"http://441895469@qq.com\">441895469@qq.com</a>"); //www.yafeilinux.com
    permanent->setTextFormat(Qt::RichText);
    permanent->setOpenExternalLinks(true);
    ui->statusBar->addPermanentWidget(permanent);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newFile()
{
    if(maybeSave())
    {
        isUntitled = true;
        curFile = "未命名.txt";
        setWindowTitle(curFile);
        ui->textEdit->clear();
        //设置控件可视
        ui->textEdit->setVisible(true);
    }
}

bool MainWindow::maybeSave()
{
    // 如果文档被更改了
    if(ui->textEdit->document()->isModified())
    {
        //自定义警告对话框
        QMessageBox box;
        box.setWindowTitle("警告");
        box.setIcon(QMessageBox::Warning);
        box.setText(curFile+" 尚未保存，是否保存?");
        QPushButton *yesBtn = box.addButton("是(&Y)",QMessageBox::YesRole);

        box.addButton("否(&N)",QMessageBox::NoRole);
        QPushButton *canceBut = box.addButton("取消",QMessageBox::RejectRole);

        box.exec();
        if(box.clickedButton() == yesBtn)
            return save();
        else if(box.clickedButton() == canceBut)
            return false;

    }
    return true;
}

bool MainWindow::save()
{
    if(isUntitled)
    {
        return savaAs();
    }
    else
    {
        return saveFile(curFile);
    }
}


bool MainWindow::savaAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,"另存为",curFile);

    if(fileName.isEmpty()) return false;
    return saveFile(fileName);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);

    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        //%1和%2分别对应后面arg两个参数
        QMessageBox::warning(this,tr("多文档编辑器"),tr("无法写入文件 %1：/n %2")
                             .arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);

    //鼠标指针变为等待状态
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << ui->textEdit->toPlainText();
    //鼠标指针恢复原来的状态
    QApplication::restoreOverrideCursor();
    isUntitled = false;
    //获得文件的标准路径
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);
    return true;
}

bool MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,tr("多文档编辑器"),tr("无法读取文件 %1:\n%2.")
                             .arg(fileName).arg(file.errorString()));
        return false; //只读方式打开文件，出错则提示，并返回false

    }
    QTextStream in(&file); //新建文本对象流
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //读取文件的全部文本内容，并添加到编辑器中
    ui->textEdit->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    //设置当前文件
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);
    return true;

}

void MainWindow::on_action_New_triggered()
{
    newFile();
}

void MainWindow::on_action_Save_triggered()
{
    save();
}

void MainWindow::on_action_SaveAs_triggered()
{
    savaAs();
}

void MainWindow::on_action_Open_triggered()
{
    if(maybeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this);

        //如果文件名不为空，则加载文件
        if(!fileName.isEmpty())
        {
            loadFile(fileName);
            ui->textEdit->setVisible(true);
        }
    }
}

void MainWindow::on_action_Close_triggered()
{
    if(maybeSave())
    {
        ui->textEdit->setVisible(false);
    }
}

void MainWindow::action_Find_triggered()
{
    findDlg->show();
}

void MainWindow::action_Exit_triggered()
{
    //先执行关闭操作看，再退出程序
    //qApp是指向应用程序的全局指针
    on_action_Close_triggered();
    qApp->quit();
}

void MainWindow::action_state_triggered()
{

}


void MainWindow::on_action_Undo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_action_Cut_triggered()
{
    ui->textEdit->cut();
}



void MainWindow::on_action_Copy_triggered()
{
    ui->textEdit->copy();
}


void MainWindow::on_action_Paste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::showFindText()
{
    QString buf = findLineEdit->text();
    if(!ui->textEdit->find(buf,QTextDocument::FindBackward))
    {
        QMessageBox::warning(this,tr("查找"),buf+" 未找到");
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //如果maybeSave()函数返回true,则关闭程序
    if(maybeSave())
    {
        event->accept();
    }
    else  //否则忽略该事件
    {
        event->ignore();
    }
}
