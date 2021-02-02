/****************************************************************************
**
** JIWOONG PARK
** Video Annotation Player for fMRI research
** Created: May 12, 2020
** Last Modified: May 12, 2020
**
****************************************************************************/

#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QMediaPlayer>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QAbstractButton;
class QSlider;
class QLabel;
class QUrl;
class QLineEdit;
QT_END_NAMESPACE

class VideoPlayer : public QWidget
{
    Q_OBJECT
public:
    VideoPlayer(QWidget *parent = nullptr);
    ~VideoPlayer();

    void setUrl(const QUrl &url);

public slots:
    void openFile();
    void play();
    void replay();

private slots:
    void mediaStateChanged(QMediaPlayer::State state);
    void positionChanged(qint64 position);
    void timeCalculator();
    void durationChanged(qint64 duration);
    void setPosition(int position);
    void handleError();

private:
    QMediaPlayer* m_mediaPlayer;
    QAbstractButton *m_playButton;
    QAbstractButton *m_replayButton;
    QSlider *m_positionSlider;
    QLabel *m_errorLabel;
    QLineEdit *m_sceneNum;
};

#endif
