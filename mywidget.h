#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QLabel>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "myplaylist.h"
#include "mylrc.h"

QT_BEGIN_NAMESPACE
class QAbstractButton;
class QAbstractSlider;
class QComboBox;
class QSlider;
QT_END_NAMESPACE

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = 0);
    bool isMuted() const;
    ~MyWidget();
signals:
    void play();
    void pause();
    void stop();
    void next();
    void previous();
    void ChangeVolume(int volume);
    void ChangeMuting(bool muting);
public slots:
    void SetState(QMediaPlayer::State state);
    void SetVolume(int volume);
private slots:
    void seek(int seconds);
    void PlayClicked();
    void SetMuted(bool muted);
    void SetPlayListShown();
    void SetLrcShown();
    void BackwardClicked();
    void ForwardClicked();
    void OpenFile();
    void StopClicked();
    void MuteClicked();
    void AddToPlaylist(const QStringList &fileNames);
    void CleanSources();
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void updateSongList(int index);
    void tableClicked(int row);
private:
    void InitPlayer();
    void ConnectActionToPlayer();
    void UpdateDurationInfo(qint64 currentInfo);
    void LoadMusicList(const QString& argument);
    void LoadMusicList(const QStringList& list);
    void ReadFormDB();
    Ui::MyWidget *ui;
    QSystemTrayIcon *tray_icon_;
    QAction *play_action_;
    QAction *stop_action_;
    QAction *skip_backward_action_;
    QAction *skip_forward_action_;
    QLabel *top_label_;
    QLabel *time_label_;
    QAbstractSlider *volume_slider_;
    QSlider *progress_slider_;
    QAbstractButton *mute_button_;
    QMediaPlayer::State player_state_;
    QMediaPlayer *player_;
    QMediaPlaylist *playlist_;
    bool player_muted_;   //muted state of the current media
    qint64 duration;
    MyPlaylist * ui_playlist_;
    MyLrc *lrc_;
};

#endif // MYWIDGET_H
