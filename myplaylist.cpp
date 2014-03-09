#include "myplaylist.h"
#include <QContextMenuEvent>
#include <QMenu>
#include <QHeaderView>

MyPlaylist::MyPlaylist(QWidget *parent) :
    QTableWidget(parent)
{
    //setAttribute(Qt::WA_TranslucentBackground);
    setWindowTitle(tr("播放列表"));
    //设置为一个独立的窗口，且只有一个关闭按钮
    //setWindowFlags(Qt::Window | Qt::WindowTitleHint);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);//去掉窗体边框,不在任务栏显示图标
    setFixedSize(400,400);
    setRowCount(0);//初始的行数为0
    setColumnCount(4);//初始的列数为1

    //设置第一个标签
    QStringList list;
    list << tr("序号") << tr("歌手") << tr("歌曲") << tr("专辑") ;
    setHorizontalHeaderLabels(list);


    horizontalHeader()->resizeSection(0,30);
    horizontalHeader()->resizeSection(1,80);
    horizontalHeader()->resizeSection(2,150);
    horizontalHeader()->resizeSection(3,140);
    setSelectionMode(QAbstractItemView::SingleSelection);//设置只能选择单行
    setSelectionBehavior(QAbstractItemView::SelectRows);
    this ->verticalHeader() ->setVisible(false);
    this ->horizontalHeader() ->setVisible(false);
    this ->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置内容不可编辑
    //this ->setItemDelegate(new NoFocusDelegate());//去掉选中单元格的虚线
    //this ->horizontalHeader() ->setHighlightSections(false);//点击表时不对表头行光亮（获取焦点）
    setShowGrid(false);//设置不显示网格
}


void MyPlaylist::clear_play_list()
{
    while(rowCount())
        removeRow(0);
    emit play_list_clean();//删除完后，发送清空成功信号
}


void MyPlaylist::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(tr("清空列表"), this, SLOT(clear_play_list()));//可以直接在这里指定槽函数
    menu.exec(event->globalPos());//返回鼠标指针的全局位置
}


void MyPlaylist::closeEvent(QCloseEvent *event)
{
    if(isVisible()) {
        hide();
        event->ignore();//清零接收标志
    }
}
