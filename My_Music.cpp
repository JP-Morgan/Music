#include "My_Music.h"
#include "PlaybackController.h"
#include <QGraphicsDropShadowEffect>

MyMusic::MyMusic(QWidget *parent)
    : QWidget(parent)
    , m_player(new MusicPlayer(this))
    , m_library(new MusicLibrary(this))
    , m_controller(new PlaybackController(m_player, m_library, this))
{
    ui.setupUi(this);
    initWindowStyle();
    setupConnections();
}

MyMusic::~MyMusic()
{
}

void MyMusic::initWindowStyle()
{
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setColor(QColor(0, 0, 0));
    shadowEffect->setBlurRadius(20);
    this->setGraphicsEffect(shadowEffect);
}

void MyMusic::setupConnections()
{
    connect(ui.closeButton, &QPushButton::clicked, this, &QWidget::close);
    connect(ui.maximizeButton, &QPushButton::clicked, this, [this]() {
        if (isMaximized())
            showNormal();
        else
            showMaximized();
    });

    connect(ui.playPauseButton, &QPushButton::clicked, m_controller, &PlaybackController::togglePlay);
    connect(ui.stopButton, &QPushButton::clicked, m_controller, &PlaybackController::stop);
    connect(ui.previousButton, &QPushButton::clicked, m_controller, &PlaybackController::previous);
    connect(ui.nextButton, &QPushButton::clicked, m_controller, &PlaybackController::next);

    connect(ui.volumeButton, &QPushButton::clicked, this, [this]() {
        m_player->setMuted(!m_player->isMuted());
    });

    connect(m_controller, &PlaybackController::durationChanged, this, &MyMusic::onDurationChanged);
    connect(m_controller, &PlaybackController::positionChanged, this, &MyMusic::onPositionChanged);
    connect(m_controller, &PlaybackController::currentMediaChanged, this, &MyMusic::onMediaInfoChanged);
}

void MyMusic::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        move(event->globalPosition().toPoint() - m_dragPosition);
        return;
    }
}

void MyMusic::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        QPoint headerPos = ui.header->mapTo(this, QPoint(0, 0));
        QRect headerRect(headerPos, ui.header->size());
        if (headerRect.contains(event->pos()))
        {
            m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
            return;
        }
    }
    QWidget::mousePressEvent(event);
}

void MyMusic::onDurationChanged(qint64 duration)
{
    ui.progressBar->setMaximum(static_cast<int>(duration));
}

void MyMusic::onPositionChanged(qint64 position)
{
    ui.progressBar->setValue(static_cast<int>(position));
}

void MyMusic::onMediaInfoChanged(const QString& filePath, const QString& title, const QString& artist)
{
    Q_UNUSED(filePath)
    ui.songTitleLabel->setText(title);
    ui.artistNameLabel->setText(artist);
}
