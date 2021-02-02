/****************************************************************************
**
** JIWOONG PARK
** Video Annotation Player for fMRI research
** Created: May 12, 2020
** Last Modified: May 12, 2020
**
****************************************************************************/

#include "videoplayer.h"

#include <QtWidgets>
#include <QVideoWidget>
#include <iostream>
#include <string>
#include <QMessageBox>

int pos(0);
int previous_pos(0);
int current_pos(0);
int duration(0);
int sceneint(0);
using namespace std;

VideoPlayer::VideoPlayer(QWidget *parent)
    : QWidget(parent)
{
    m_mediaPlayer = new QMediaPlayer(this, QMediaPlayer::VideoSurface);
    QVideoWidget *videoWidget = new QVideoWidget;

    QAbstractButton *openButton = new QPushButton(tr("Open..."));
    connect(openButton, &QAbstractButton::clicked, this, &VideoPlayer::openFile);
    m_mediaPlayer -> setNotifyInterval(100);
    m_playButton = new QPushButton;
    m_playButton->setEnabled(false);
    m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(m_playButton, &QAbstractButton::clicked,
            this, &VideoPlayer::play);

    m_sceneNum = new QLineEdit("1", this);
    m_sceneNum->setMaximumWidth(30);
    m_sceneNum->setFixedWidth(30);
    m_sceneNum->setValidator( new QIntValidator(0, 1000, this) );

    m_replayButton = new QPushButton;
    m_replayButton->setEnabled(true);
    m_replayButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));

    connect(m_replayButton, &QAbstractButton::clicked,
            this, &VideoPlayer::replay);


    m_positionSlider = new QSlider(Qt::Horizontal);
    m_positionSlider->setRange(0, 0);

    //connect(m_positionSlider, &QAbstractSlider::sliderMoved,this, &VideoPlayer::setPosition); //slider click cha

    m_errorLabel = new QLabel;
    m_errorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setContentsMargins(0, 0, 0, 0);
    controlLayout->addWidget(openButton);
    controlLayout->addWidget(m_playButton);
    controlLayout->addWidget(m_sceneNum);
    controlLayout->addWidget(m_replayButton);
    controlLayout->addWidget(m_positionSlider);

    QBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(videoWidget);
    layout->addLayout(controlLayout);
    layout->addWidget(m_errorLabel);

    setLayout(layout);

    m_mediaPlayer->setVideoOutput(videoWidget);
    connect(m_mediaPlayer, &QMediaPlayer::stateChanged,
            this, &VideoPlayer::mediaStateChanged);
    connect(m_mediaPlayer, &QMediaPlayer::positionChanged, this, &VideoPlayer::positionChanged);
    connect(m_mediaPlayer, &QMediaPlayer::positionChanged, this, &VideoPlayer::timeCalculator);
    connect(m_mediaPlayer, &QMediaPlayer::durationChanged, this, &VideoPlayer::durationChanged);
    connect(m_mediaPlayer, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),
            this, &VideoPlayer::handleError);
}

VideoPlayer::~VideoPlayer()
{
}

void VideoPlayer::openFile()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Movie"));
    QStringList supportedMimeTypes = m_mediaPlayer->supportedMimeTypes();
    if (!supportedMimeTypes.isEmpty())
        fileDialog.setMimeTypeFilters(supportedMimeTypes);
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted)
        setUrl(fileDialog.selectedUrls().constFirst());
}

void VideoPlayer::setUrl(const QUrl &url)
{ 
    m_errorLabel->setText(QString());
    setWindowFilePath(url.isLocalFile() ? url.toLocalFile() : QString());
    qInfo() << url;
    m_mediaPlayer->setMedia(url);
    m_playButton->setEnabled(true);
}

void VideoPlayer::play()
{

    switch (m_mediaPlayer->state()) {
    case QMediaPlayer::PlayingState:
        break;
    default:
        ::sceneint = ::sceneint+1;
        QString scene = QString::number(::sceneint);
        m_sceneNum->setText(scene);

        ::previous_pos = m_mediaPlayer->position();
        m_mediaPlayer->play();
        qInfo() << "Play button";
        qInfo() << ::previous_pos;
        break;
    }
}

void VideoPlayer::replay()
{
    switch (m_mediaPlayer->state()) {
    case QMediaPlayer::PlayingState:
        break;
    default:
        ::duration = m_mediaPlayer->duration();
        //::sceneint = (long)((::previous_pos)/2000.0 + 1.5);
        //QString scene = QString::number(::sceneint);
        QString scene = m_sceneNum->text();
        ::sceneint = (long)(scene.toInt()-1);
        if (::sceneint*2000 > duration){
            QMessageBox::information(
                this,
                tr("Annoation player"),
                tr("Invalid integer") );
            ::sceneint = 0;
            QString scene = QString::number(::sceneint);
            m_sceneNum->setText(scene);
        }
        //m_sceneNum->setText(scene);

        m_mediaPlayer->setPosition(::sceneint*2000);
        //::previous_pos = m_mediaPlayer->position();
        ::previous_pos = ::sceneint*2000;
        ::sceneint = ::sceneint+1;
        m_mediaPlayer->play();
        qInfo() << "Replay button";
        qInfo() << ::previous_pos;
        break;
    }
}


void VideoPlayer::mediaStateChanged(QMediaPlayer::State state)
{
    switch(state) {
    case QMediaPlayer::PlayingState:
        m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    default:
        m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
}


void VideoPlayer::timeCalculator()
{
    ::duration = m_mediaPlayer->duration();
    //qInfo() << ::duration/2000.0;
    ::current_pos = m_mediaPlayer->position();
    //m_sceneNum->setText(std::to_string(::scene));
    if (::current_pos>::previous_pos+2000){
        m_mediaPlayer->pause();
//        ::sceneint = (long)((::previous_pos)/2000.0 + 1.5);
//        QString scene = QString::number(::sceneint);
//        m_sceneNum->setText(scene);

        m_mediaPlayer->setPosition((long)((::sceneint)*2000));
        //::sceneint = ::sceneint + 1;
    }
}


void VideoPlayer::positionChanged(qint64 position)
{
    m_positionSlider->setValue(position);
}

void VideoPlayer::durationChanged(qint64 duration)
{
    m_positionSlider->setRange(0, duration);
}

void VideoPlayer::setPosition(int position)
{
    qInfo() << position; // int position in ms
    //m_mediaPlayer->setPosition(position);
    m_mediaPlayer->setPosition((long)(position/2000.0+0.5)*2000);
}

void VideoPlayer::handleError()
{
    m_playButton->setEnabled(false);
    const QString errorString = m_mediaPlayer->errorString();
    QString message = "Error: ";
    if (errorString.isEmpty())
        message += " #" + QString::number(int(m_mediaPlayer->error()));
    else
        message += errorString;
    m_errorLabel->setText(message);
}
