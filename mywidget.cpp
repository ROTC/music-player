#include "mywidget.h"
#include "ui_mywidget.h"
#include "myplaylist.h"
#include <QLabel>
#include <QToolBar>
#include <QVBoxLayout>
#include <QTime>
#include <QMessageBox>
#include <QFileInfo>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMenu>
#include <QCloseEvent>
#include <QSlider>
#include <QToolButton>
#include <QStringList>
#include <QFileDialog>
#include "mtag.h"
#include <QDebug>

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget),
    tray_icon_(0),
    play_action_(0),
    stop_action_(0),
    skip_backward_action_(0),
    skip_forward_action_(0),
    top_label_(0),
    time_label_(0),
    volume_slider_(0),
    progress_slider_(0),
    mute_button_(0),
    player_state_(QMediaPlayer::StoppedState),
    player_(0),
    playlist_(0),
    player_muted_(false)
{
    ui->setupUi(this);
    InitPlayer();
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::InitPlayer()
{
    //media
    player_ = new QMediaPlayer(this);
    // owned by PlaylistModel
    playlist_ = new QMediaPlaylist();
    playlist_->setPlaybackMode(QMediaPlaylist::Loop);
    player_->setPlaylist(playlist_);

    //设置窗口基本属性
    //this->setAttribute(Qt::WA_TranslucentBackground);
    //setWindowFlags(Qt::FramelessWindowHint);//去掉窗体边框
    setWindowTitle(tr("MyPlayer音乐播放器"));
    setWindowIcon(QIcon(":/images/icon.png"));//从资源文件中招图标
    setFixedSize(400,200);

    top_label_ = new QLabel(tr("<a href=\"http://my.oschina.net/u/263978/blog/\">http://my.oschina.net/u/263978/blog/</a>"));
    top_label_->setTextFormat(Qt::RichText);
    top_label_->setOpenExternalLinks(true);//运行点击进入外部链接
    top_label_->setAlignment(Qt::AlignCenter);

    progress_slider_ = new QSlider(Qt::Horizontal, this);//进度条
    progress_slider_->setRange(0,player_->duration()/100);

    //设置显示时间的标签
    QToolBar *widget_bar = new QToolBar(this);
    time_label_ = new QLabel(tr("00:00/00:00"), this);
    time_label_->setToolTip(tr("当前时间/总时间"));
    time_label_->setAlignment(Qt::AlignCenter);
    //QSizePolicy类是描述水平和垂直修改大小策略的一种属性
    time_label_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);//水平方向上尺寸可扩展，垂直方向已固定

    ui_playlist_ = new MyPlaylist(this);
    //播放列表开启控制图标
    QAction *PLAction = new QAction(tr("PL"), this);
    PLAction->setShortcut(QKeySequence("F4"));//设置开启播放列表的快捷键为F4
    PLAction->setToolTip(tr("播放列表(F4)"));
    connect(PLAction,&QAction::triggered,this,&MyWidget::SetPlayListShown);

    //桌面歌词显示开启控制图标
    QAction *LRCAction = new QAction(tr("LRC"), this);
    LRCAction->setShortcut(QKeySequence("F2"));//设置开启桌面歌词的播放列表快捷键为F2
    LRCAction->setToolTip(tr("桌面歌词(F2)"));
    connect(LRCAction,&QAction::triggered,this,&MyWidget::SetLrcShown);

    //将上面2个action和1个widget添加到工具栏，默认的添加方式为水平方向添加
    widget_bar->addAction(PLAction);
    widget_bar->addSeparator();
    widget_bar->addWidget(time_label_);
    widget_bar->addSeparator();
    widget_bar->addAction(LRCAction);
    widget_bar->addSeparator();

    //设置播放动作
    QToolBar *tool_bar = new QToolBar(this);//该构造函数没有写入文字
    play_action_ = new QAction(this);
    play_action_->setIcon(QIcon(":/images/play.png"));
    play_action_->setText(tr("播放(F5)"));
    play_action_->setShortcut(QKeySequence("F5"));//播放的快捷键位F5
    connect(play_action_,&QAction::triggered,this,&MyWidget::PlayClicked);

    //设置停止动作
    stop_action_ = new QAction(this);
    stop_action_->setIcon(QIcon(":/images/stop.png"));
    stop_action_->setText(tr("暂停(F6)"));
    stop_action_->setShortcut(QKeySequence("F6"));
    connect(stop_action_,&QAction::triggered,this,&MyWidget::StopClicked);

    //设置上一首动作
    skip_backward_action_ = new QAction(this);
    skip_backward_action_->setIcon(QIcon(":/images/skipBackward.png"));
    skip_backward_action_->setText(tr("上一首(Ctrl+Left)"));
    skip_backward_action_->setShortcut(QKeySequence("Ctrl+Left"));
    connect(skip_backward_action_,&QAction::triggered,this,&MyWidget::BackwardClicked);

    //设置下一首动作
    skip_forward_action_ = new QAction(this);
    skip_forward_action_->setIcon(QIcon(":/images/skipForward.png"));
    skip_forward_action_->setText(tr("上一首(Ctrl+Right)"));
    skip_forward_action_->setShortcut(QKeySequence("Ctrl+Right"));
    connect(skip_forward_action_,&QAction::triggered,this,&MyWidget::ForwardClicked);

    //设置打开文件动作
    QAction *open_action = new QAction(this);
    open_action->setIcon(QIcon(":/images/open.png"));
    open_action->setText(tr("播放文件(Ctrl+O)"));
    open_action->setShortcut(QKeySequence("Ctrl+O"));
    open_action->setEnabled(true);
    connect(open_action,&QAction::triggered,this,&MyWidget::OpenFile);

    mute_button_ = new QToolButton(this);
    mute_button_->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    connect(mute_button_,SIGNAL(clicked()),this,SLOT(MuteClicked()));

    //音乐控制部件
    volume_slider_ = new QSlider(Qt::Horizontal, this);
    volume_slider_->setRange(0, 100);
    //通过声音条控制player的声音,暂时没实现静音后声音条变为0
    connect(volume_slider_,SIGNAL(sliderMoved(int)),player_,SLOT(setVolume(int)));

    tool_bar->addAction(play_action_);
    tool_bar->addSeparator();
    tool_bar->addAction(stop_action_);
    tool_bar->addSeparator();
    tool_bar->addAction(skip_backward_action_);
    tool_bar->addSeparator();
    tool_bar->addAction(skip_forward_action_);
    tool_bar->addSeparator();
    tool_bar->addWidget(mute_button_);
    tool_bar->addWidget(volume_slider_);
    tool_bar->addSeparator();
    tool_bar->addAction(open_action);

    //创建主界面管理器
    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addWidget(top_label_);
    main_layout->addWidget(progress_slider_);
    main_layout->addWidget(widget_bar);
    main_layout->addWidget(tool_bar);
    setLayout(main_layout);

    //设置初始音量和初始状态
    SetState(player_->state());
    SetVolume(player_->volume());
    SetMuted(player_muted_);
    ConnectActionToPlayer();
}

void MyWidget::ConnectActionToPlayer()
{
    connect(this,SIGNAL(play()),player_,SLOT(play()));

    connect(this,SIGNAL(pause()),player_,SLOT(pause()));

    connect(this,SIGNAL(stop()),player_,SLOT(stop()));

    //handled directly in BackWardClicked()
    //connect(this,SIGNAL(previous()),playlist_,SLOT(previous()));

    connect(this,SIGNAL(next()),playlist_,SLOT(next()));

    //用于实时更新按钮的状态和成员变量player_muted
    connect(player_,SIGNAL(mutedChanged(bool)),this,SLOT(SetMuted(bool)));
    //更新播放器的静音状态
    connect(this,SIGNAL(ChangeMuting(bool)),player_,SLOT(setMuted(bool)));

    //根据player的状态来更新按钮的形式
    connect(player_, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(SetState(QMediaPlayer::State)));

    //进度条的变化
    connect(progress_slider_, SIGNAL(sliderMoved(int)), this, SLOT(seek(int)));

    //歌曲的时间长短导致进度条表示的范围的长短的变化
    connect(player_, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));

    //歌曲的推移导致进度条的前进
    connect(player_, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));

    //
    connect(playlist_, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSongList(int)));

    connect(ui_playlist_, SIGNAL(cellClicked(int,int)), this, SLOT(tableClicked(int)));
}

bool MyWidget::isMuted() const
{
    return player_muted_;
}

void MyWidget::SetVolume(int volume)
{
    if (volume_slider_)
        volume_slider_->setValue(volume);
}

void MyWidget::SetState(QMediaPlayer::State state)
{
    if (state != player_state_) {
        player_state_ = state;
        switch (state) {
        case QMediaPlayer::StoppedState:
            stop_action_->setEnabled(false);
            play_action_->setIcon(QIcon(":/images/play.png"));
            break;
        case QMediaPlayer::PlayingState:
            stop_action_->setEnabled(true);
            play_action_->setIcon(QIcon(":/images/pause.png"));
            break;
        case QMediaPlayer::PausedState:
            stop_action_->setEnabled(true);
            play_action_->setIcon(QIcon(":/images/play.png"));
            break;
        }
    }
}

void MyWidget::PlayClicked()
{
    switch (player_state_) {
    case QMediaPlayer::StoppedState:
    case QMediaPlayer::PausedState:
        emit play();
        break;
    case QMediaPlayer::PlayingState:
        emit pause();
        break;
    }
}

void MyWidget::StopClicked()
{
    emit stop();
}

void MyWidget::BackwardClicked()
{
    // Go to previous track if we are within the first 5 seconds of playback
    // Otherwise, seek to the beginning.
    if(player_->position() <= 5000)
        playlist_->previous();
    else
        player_->setPosition(0);
}

void MyWidget::SetMuted(bool muted)
{
    if (muted != player_muted_) {
        player_muted_ = muted;

        mute_button_->setIcon(style()->standardIcon(muted
                ? QStyle::SP_MediaVolumeMuted
                : QStyle::SP_MediaVolume));
    }
}

void MyWidget::MuteClicked()
{
    emit ChangeMuting(!player_muted_);
}

void MyWidget::ForwardClicked()
{
    emit next();
}

void MyWidget::SetPlayListShown()
{
    if(ui_playlist_->isHidden()) {
        ui_playlist_->move(frameGeometry().bottomLeft());//显示在主界面的下方
        ui_playlist_->show();
    }
    else {
        ui_playlist_->hide();
    }
}

void MyWidget::SetLrcShown()
{

}

void MyWidget::OpenFile()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open Files"));
    AddToPlaylist(fileNames);
}

void MyWidget::AddToPlaylist(const QStringList &fileNames)
{
    foreach (const QString &argument, fileNames) {
        QByteArray ba = argument.toUtf8();
        const char* c_str = ba.constData();
        //qDebug()<<c_str;
        mtag_file_t *file = mtag_file_new(c_str);
        mtag_tag_t *tag = mtag_file_tag(file);
        //QString fileName = argument.split("/").last();
        QString author = mtag_tag_get(tag,"artist");
        QString album = mtag_tag_get(tag,"album");
        QString title = mtag_tag_get(tag,"title");
        int rownum = ui_playlist_->rowCount();
        ui_playlist_->insertRow(rownum);
        ui_playlist_->setItem(rownum, 0, new QTableWidgetItem(author));
        ui_playlist_->setItem(rownum, 1, new QTableWidgetItem(title));
        ui_playlist_->setItem(rownum, 2, new QTableWidgetItem(album));
//        QString fileName = argument.split("/").last();
//        int rownum = ui_playlist_->rowCount();
//        ui_playlist_->insertRow(rownum);
//        ui_playlist_->setItem(rownum, 0, new QTableWidgetItem(fileName.split(".").front()));
//        ui_playlist_->setItem(rownum, 1, new QTableWidgetItem(fileName.split(".").last()));
//       ui_playlist_->setItem(rownum, 2, new QTableWidgetItem(fileName));
        QFileInfo fileInfo(argument);
        if (fileInfo.exists()) {
            QUrl url = QUrl::fromLocalFile(fileInfo.absoluteFilePath());
            if (fileInfo.suffix().toLower() == QLatin1String("m3u")) {
                playlist_->load(url);
            } else
                playlist_->addMedia(url);
        } else {
            //当string给出的不是文件路径，而是其它，如网址时
            QUrl url(argument);
            if (url.isValid()) {
                playlist_->addMedia(url);
            }
        }
    }
}

void MyWidget::seek(int seconds)
{
    player_->setPosition(seconds*1000);
}

void MyWidget::durationChanged(qint64 duration)
{
    this->duration = duration/1000;
    progress_slider_->setMaximum(duration / 1000);
}

void MyWidget::positionChanged(qint64 progress)
{
    if (!progress_slider_->isSliderDown()) {
        progress_slider_->setValue(progress / 1000);
    }
    UpdateDurationInfo(progress / 1000);
}

void MyWidget::UpdateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || duration) {
        QTime currentTime((currentInfo/3600)%60, (currentInfo/60)%60, currentInfo%60, (currentInfo*1000)%1000);
        QTime totalTime((duration/3600)%60, (duration/60)%60, duration%60, (duration*1000)%1000);
        QString format = "mm:ss";
        if (duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    time_label_->setText(tStr);
}

void MyWidget::updateSongList(int index)
{
    ui_playlist_->selectRow(index);
    top_label_->setText(tr("正在播放: %1").arg(ui_playlist_->item(index, 0)->text()));
}

void MyWidget::tableClicked(int row)
{
    playlist_->setCurrentIndex(row);
    player_->play();
}
