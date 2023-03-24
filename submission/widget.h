#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPoint>
#include <QEvent>
#include <QVector>

const int SIZE=8; //格子数
const int WIDTH=50; //格子宽度
const int X=40,Y=80; //初始位置

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    bool dfsAir(int x, int y);
    int judge();
    void whitewin();
    void blackwin();
    void KillYourself();
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    void DrawChessBoard(); //画棋盘
    void DrawItems(); //画棋子
    void DrawItemWithMouse(); //画鼠标跟随的棋子

public slots:
    void aboutGame();
    void saveGame();
    void closeGame();

private slots:

    void on_giveupButton_clicked();

private:
    Ui::Widget *ui;

};

#endif // WIDGET_H
