#include "widget.h"
#include "ui_widget.h"

#include <QPainter>
#include <QLabel>
#include <QPoint>
#include <QEvent>
#include <QMessageBox>
#include <QString>
#include <QDebug>
#include <QMenuBar>
#include <QMenu>
#include <QPushButton>

//问题：鼠标坐标，messagebox的标题栏图标，背景图片的缩放, pushButton风格的设置

bool ChessColor=true; //棋色,true为黑色，false为白色
int ChessX=-1,ChessY=-1; //落棋位置
int ChessSituation[9][9]={{0}}; //-1为白棋，1为黑棋
int DX[4] = {-1, 0, 1, 0};
int DY[4] = {0, -1, 0, 1};
int air_visited[9][9]={{0}}; //判断是否访问过
int value=0; //-1白棋win，1黑棋win

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //开启鼠标监测
    setMouseTracking(true);
    //创建菜单栏
    QMenuBar *menubar;
    menubar=new QMenuBar(this);
    menubar->setStyleSheet("font: 24px 'Arial Rounded MT Bold'");
    menubar->setGeometry(0,0,this->width(),40);
    QAction *saveAction = menubar->addAction("Save");
    QAction *closeAction = menubar->addAction("Close");
    QAction *aboutAction= menubar->addAction("About");
    menubar->show();
    //菜单栏动作
    connect(saveAction,&QAction::triggered,this,&Widget::saveGame);
    connect(closeAction,&QAction::triggered,this,&Widget::closeGame);
    connect(aboutAction,&QAction::triggered,this,&Widget::aboutGame);
    //设置pushButton样式
    ui->giveupButton->setStyleSheet("font:8px,background-color:rgb(0,0,100),color:blue");
    ui->retryButton->setStyleSheet("font:8px,background-color:rgb(0,0,100),color:blue");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::DrawChessBoard()
{
    QPainter *paint;
    paint = new QPainter;
    paint->begin(this);
    paint->setPen(QPen(Qt::black,4,Qt::SolidLine));
    //画横线
    for(int i=0;i<SIZE+1;i++)
    {
        paint->drawLine(X, Y+WIDTH*i, X+WIDTH*SIZE, Y+WIDTH*i);
    }
    //画竖线
    for(int i=0;i<SIZE+1;i++)
    {
        paint->drawLine(X+WIDTH*i, Y, X+WIDTH*i, Y+WIDTH*SIZE);
    }
}

void Widget::DrawItems()
{
    QPainter paint(this);
    paint.setPen(QPen(QColor(Qt::transparent)));
    if(ChessX != -1 && ChessY != -1)
    {
        if(ChessColor)
             paint.setBrush(QBrush(Qt::black,Qt::SolidPattern));//毛刷：颜色，实图案
        else
             paint.setBrush(QBrush(Qt::white,Qt::SolidPattern));//毛刷：颜色，实图案
        paint.drawEllipse(ChessX, ChessY, WIDTH/2, WIDTH/2);
    }
}

void Widget::DrawItemWithMouse()
{
    QPainter paint(this);
    paint.setPen(QPen(QColor(Qt::transparent)));
    if(ChessColor)
        paint.setBrush(Qt::black);
    else
        paint.setBrush(Qt::white);
    paint.drawEllipse(mapFromGlobal(QCursor::pos()),WIDTH/2,WIDTH/2);
}

void Widget::aboutGame()
{
    QMessageBox AboutBox;
    AboutBox.setStyleSheet("font: 18px 'Arial Rounded MT Bold'");
    AboutBox.setText("Author:XuYaowen,WangKunxiang,FangXiangnan\n"
                     "Powered by Qt Creator\n"
                     "Github:'https://github.com/Xuyaowen123/TuringNoGo'\n");
    AboutBox.exec();
}

void Widget::saveGame()
{

}

void Widget::closeGame()
{
    QWidget::close();
}

bool Widget::dfsAir(int x, int y)
{
    air_visited[x][y]=1;
    bool flag=false;
    for(int dir=0;dir<4;dir++) //周围棋子
    {
        int dx=x+DX[dir], dy=y+DY[dir];
        if(dx>=0&&dx<=8&&dy>=0&&dy<=8)
        {
            if(ChessSituation[dx][dy]==0)
                flag=true;
            if(ChessSituation[x][y]==ChessSituation[dx][dy]&&air_visited[dx][dy]==0)
                if(dfsAir(dx,dy))
                    flag=true;
        }
    }
    return flag;
}

int Widget::judge()
{
    for(int i=0;i<9;i++)
        for(int j=0;j<9;j++)
        {
            if(ChessSituation[i][j]==0) continue;
            if(dfsAir(i,j)==false)
            {
                if(ChessSituation[i][j]==-1)
                    return -1; //白棋win
                else if(ChessSituation[i][j]==1)
                    return 1; //黑棋win
            }
        }
    return 0; //继续下棋
}

void Widget::whitewin()
{
   QMessageBox WhiteWinBox(QMessageBox::Icon::Information,"Game Over","<font size='26' color='black'>White Win !!!</font>"
                           ,QMessageBox::StandardButton::Ok);
   WhiteWinBox.exec();

}

void Widget::blackwin()
{
    QMessageBox BlackWinBox(QMessageBox::Icon::Information,"Game Over","<font size='26' color='black'>Black Win !!!</font>"
                            ,QMessageBox::StandardButton::Ok);
    BlackWinBox.exec();
}

void Widget::KillYourself()
{
    QMessageBox KillBox(QMessageBox::Icon::Warning,"Game Over","<font size='26' color='red'>You Kill Yourself !!!</font>"
                ,QMessageBox::StandardButton::Ok);
    KillBox.exec();
}


void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿
    // 绘制棋盘背景
    QRectF target(X, Y, WIDTH*SIZE, WIDTH*SIZE);
    QRectF source(X, Y, WIDTH*SIZE, WIDTH*SIZE);
    QImage image(":/new/prefix1/xiaohui.jpg");
    painter.drawImage(target, image, source);
    //画棋盘
    DrawChessBoard();
    //画棋子
    DrawItems();
    //画鼠标光标
    DrawItemWithMouse();

    update();
}

void Widget::mousePressEvent(QMouseEvent *)
{
    /*if(event->button()==Qt::LeftButton)
        {
            qDebug()<<event->x()<<" "<<event->y();
        }*/
    /*int x=QCursor::pos().x();
    int y=QCursor::pos().y();*/
    int x = QCursor::pos().x();//鼠标点击处横坐标
    int y = QCursor::pos().y();//鼠标点击处纵坐标
    /*QWidget *action = QApplication::widgetAt(mouse_x, mouse_y);//获取鼠标点击处的控件
    QPoint GlobalPoint(action->mapToGlobal(QPoint(0, 0)));//获取该控件在窗体中的坐标
    int x = GlobalPoint.x();//该控件在窗体中左上角横坐标
    int y = GlobalPoint.y();//该控件在窗体中左上角纵坐标
    x-=mouse_x,y-=mouse_y;*/
    qDebug()<<x<<y;
    int ptX,ptY;
    if(x<X||x>X+SIZE*WIDTH||y<Y||y>Y+SIZE*WIDTH) //判断边界合法
        return;
    if((x-X)%WIDTH>WIDTH/2)
        ChessX=((x-X)/WIDTH+1)*WIDTH+X, ptX=(x-X)/WIDTH+1;
    else
        ChessX=((x-X)/WIDTH)*WIDTH+X, ptX=(x-X)/WIDTH;
    if((y-Y)%WIDTH>WIDTH/2)
        ChessY=((y-Y)/WIDTH+1)*WIDTH+Y, ptY=(y-Y)/WIDTH+1;
    else
        ChessY=((y-Y)/WIDTH)*WIDTH+Y, ptY=(y-Y)/WIDTH;
    if(ChessSituation[ptX][ptY]) //判断是否有棋子
        return;
    else if(ChessColor) ChessSituation[ptX][ptY]=1;
    else ChessSituation[ptX][ptY]=-1;

    if(dfsAir(ptX,ptY)==false)  //判断自杀
        KillYourself();
    else value=judge(); //判断胜负
    if(value==-1)
        whitewin();
    else if(value==1)
        blackwin();
    else
        update();
    ChessColor=!ChessColor; //换颜色
}



void Widget::on_giveupButton_clicked()
{
    /* QMessageBox GiveupBox;

     GiveupBox.setStandardButtons(QMessageBox::StandardButton::Ok);
     GiveupBox.setIcon(QMessageBox::Icon::Information);
     GiveupBox.setStyleSheet("font: 24px 'Arial Rounded MT Bold");
     GiveupBox.setText("You lose !");
     GiveupBox.exec();*/

     QMessageBox GiveupBox(QMessageBox::Icon::Information,"Game Over","<font size='30' color='red'>You Lose !!!</font>",
                 QMessageBox::StandardButton::Retry);
     GiveupBox.setWindowIcon(QIcon(":/new/prefix1/xiaohui.jpg"));
     GiveupBox.addButton(QMessageBox::Button::Close);
     connect(,)

     //GiveupBox.setBaseSize(600,600); 没用的函数
     GiveupBox.exec();
}

