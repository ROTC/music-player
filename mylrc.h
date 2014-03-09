#ifndef MYLRC_H
#define MYLRC_H

#include <QTimer>
#include <QLabel>

class MyLrc : public QLabel
{
    Q_OBJECT
public:
    explicit MyLrc(QWidget *parent = 0);
    void start_lrc_mask(qint64 intervaltime);
    void stop_lrc_mask();
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

signals:

public slots:

private slots:
    void timeout();

private:
    static QLinearGradient linear_gradient_;
    static QLinearGradient mask_linear_gradient_;

    static QFont font_;

    QTimer *timer_;
    qreal lrc_mask_width_;
    qreal lrc_mask_width_interval_;//各期遮罩宽度间隔
    QPoint drag_position_;

};

#endif // MYLRC_H
