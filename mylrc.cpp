#include "mylrc.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QMenu>
#include <QPainter>
#include <QMouseEvent>
#include <QContextMenuEvent>

QLinearGradient MyLrc::linear_gradient_;
QLinearGradient MyLrc::mask_linear_gradient_;
QFont MyLrc::font_;

MyLrc::MyLrc(QWidget *parent):
    QLabel(parent),lrc_mask_width_(0),lrc_mask_width_interval_(0)
{
    //去掉窗体边框和总是显示在屏幕最前面,但要和showNormal一起用
    setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint
                              | Qt::FramelessWindowHint);//去掉窗体边框,不在任务栏显示图标
    //setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
    setFixedSize(1000,60);

    //设置歌词位置
    double width = QApplication::desktop() ->width();
    double height = QApplication::desktop() ->height();
    this ->move(QPoint(width / 2 - 800 / 2, height - 90));

    linear_gradient_.setStart(0,10);
    linear_gradient_.setFinalStop(0,40);
    //第一个参数终点坐标，相对于我们上面的区域而言，按照比例进行计算
    linear_gradient_.setColorAt(0.1, QColor(14, 179, 255));
    linear_gradient_.setColorAt(0.5, QColor(114, 232, 255));
    linear_gradient_.setColorAt(0.9, QColor(14, 179, 255));

    mask_linear_gradient_.setStart(0,10);
    mask_linear_gradient_.setFinalStop(0,40);
    mask_linear_gradient_.setColorAt(0.1, QColor(222, 54, 4));
    mask_linear_gradient_.setColorAt(0.5, QColor(255, 72, 16));
    mask_linear_gradient_.setColorAt(0.9, QColor(222, 54, 4));

    // 设置字体
    font_.setFamily("Times New Roman");
    font_.setBold(true);
    font_.setPointSize(30);

    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(timeout()));
}

void MyLrc::timeout()
{
    //每隔一段固定的时间笼罩的长度就增加一点
    lrc_mask_width_ += lrc_mask_width_interval_;
    update();//更新widget，但是并不立即重绘，而是安排一个Paint事件，当返回主循环时由系统来重绘
}

void MyLrc::start_lrc_mask(qint64 intervaltime)
{
    // 这里设置每隔30毫秒更新一次遮罩的宽度，因为如果更新太频繁
    // 会增加CPU占用率，而如果时间间隔太大，则动画效果就不流畅了
    qreal count = intervaltime / 30;
    // 获取遮罩每次需要增加的宽度，这里的800是部件的固定宽度
    lrc_mask_width_interval_ = 1000 / count;
    lrc_mask_width_ = 0;
    timer_->start(30);
}

void MyLrc::stop_lrc_mask()
{
    timer_->stop();
    lrc_mask_width_ = 0;
    update();
}

void MyLrc::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setFont(font_);

    // 先绘制底层文字，作为阴影，这样会使显示效果更加清晰，且更有质感
    painter.setPen(QColor(0, 0, 0, 200));
    painter.drawText(1, 1, 1000, 60, Qt::AlignLeft, text());//左对齐

    // 再在上面绘制渐变文字
    painter.setPen(QPen(linear_gradient_, 0));
    painter.drawText(0, 0, 1000, 60, Qt::AlignLeft, text());

    // 设置歌词遮罩
    painter.setPen(QPen(mask_linear_gradient_, 0));
    painter.drawText(0, 0, lrc_mask_width_, 60, Qt::AlignLeft, text());
}

//左击操作
void MyLrc::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        drag_position_ = event->globalPos() - frameGeometry().topLeft();
}

void MyLrc::mouseMoveEvent(QMouseEvent *event)
{
    //移动鼠标到歌词上时，会显示手型
    //event->buttons()返回鼠标点击的类型，分为左击，中击，右击
    //这里用与操作表示是左击
    if (event->buttons() == Qt::LeftButton) {
        setCursor(Qt::PointingHandCursor);
        //实现移动操作
        move(event->globalPos() - drag_position_);
       }
}

//右击事件
void MyLrc::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(tr("隐藏"), this, SLOT(hide()));
    menu.exec(event->globalPos());//globalPos()为当前鼠标的位置坐标
}
