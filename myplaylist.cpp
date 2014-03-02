#include "myplaylist.h"
#include <QContextMenuEvent>
#include <QMenu>


MyPlaylist::MyPlaylist(QWidget *parent) :
    QTableWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowTitle(tr("播放列表"));
    //设置为一个独立的窗口，且只有一个关闭按钮
    setWindowFlags(Qt::Window | Qt::WindowTitleHint);
    setFixedSize(200,200);
    setRowCount(0);//初始的行数为0
    setColumnCount(2);//初始的列数为1

    //设置第一个标签
    QStringList list;
    list << tr("歌曲") << tr("格式") ;
    setHorizontalHeaderLabels(list);

    setSelectionMode(QAbstractItemView::SingleSelection);//设置只能选择单行
    setSelectionBehavior(QAbstractItemView::SelectRows);

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
