#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

static const char *GOLD = "#FFD700";
static const char *DARK = "#1a1a1a";
static const char *FONT = "SimHei";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_view(nullptr)
    , m_scene(nullptr)
    , m_finalScoreLabel(nullptr)
    , m_pixelArt(nullptr)
{
    setWindowTitle("吃播大作战 — 良子");
    setFixedSize(820, 700);
    setStyleSheet(QString("background-color: %1;").arg(DARK));

    setupUI();
}

void MainWindow::setupUI()
{
    auto *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // 标题
    auto *titleLabel = new QLabel("吃播大作战");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        QString("color: %1; font-size: 28px; font-weight: bold; "
                "font-family: '%2'; padding: 5px;").arg(GOLD, FONT)
    );
    mainLayout->addWidget(titleLabel);

    // 堆栈容器
    m_stack = new QStackedWidget();
    m_stack->setFixedSize(800, 620);
    m_stack->setStyleSheet(QString("background-color: %1;").arg(DARK));

    // === 页面 0: 开始界面 ===
    auto *startPage = new QWidget();
    auto *startLayout = new QVBoxLayout(startPage);
    startLayout->setAlignment(Qt::AlignCenter);

    auto *subtitleLabel = new QLabel("良子的吃播挑战");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet(
        QString("color: %1; font-size: 32px; font-weight: bold; "
                "font-family: '%2';").arg(GOLD, FONT)
    );
    startLayout->addWidget(subtitleLabel);

    auto *descLabel = new QLabel(
        "← →  或  A D  控制良子移动\n"
        "接住美食加分，躲避蔬菜和减肥药！\n"
        "累计 250 分即可获胜！"
    );
    descLabel->setAlignment(Qt::AlignCenter);
    descLabel->setStyleSheet(
        "color: #ccc; font-size: 14px; font-family: 'SimHei'; "
        "padding: 20px; line-height: 1.8;"
    );
    startLayout->addWidget(descLabel);

    auto *startBtn = new QPushButton("开 始 游 戏");
    startBtn->setFixedSize(200, 50);
    startBtn->setStyleSheet(
        QString("QPushButton { background-color: %1; color: %2; "
                "font-size: 20px; font-weight: bold; font-family: '%3'; "
                "border-radius: 8px; }"
                "QPushButton:hover { background-color: #FFC107; }"
                ).arg(GOLD, DARK, FONT)
    );
    startLayout->addWidget(startBtn, 0, Qt::AlignCenter);
    connect(startBtn, &QPushButton::clicked, this, &MainWindow::onStartClicked);

    m_stack->addWidget(startPage);  // index 0

    // === 页面 1: 游戏视图 ===
    m_scene = new GameScene(this);
    m_view = new QGraphicsView(m_scene);
    m_view->setFixedSize(800, 600);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setStyleSheet(QString("border: 2px solid %1;").arg(GOLD));
    m_view->setRenderHint(QPainter::Antialiasing);
    connect(m_scene, &GameScene::gameEnded, this, &MainWindow::onGameEnded);

    auto *gamePage = new QWidget();
    auto *gameLayout = new QVBoxLayout(gamePage);
    gameLayout->setContentsMargins(0, 0, 0, 0);
    gameLayout->addWidget(m_view);
    m_stack->addWidget(gamePage);  // index 1

    // === 页面 2: 结束界面 ===
    auto *resultPage = new QWidget();
    auto *resultLayout = new QVBoxLayout(resultPage);
    resultLayout->setAlignment(Qt::AlignCenter);

    m_finalScoreLabel = new QLabel();
    m_finalScoreLabel->setAlignment(Qt::AlignCenter);
    resultLayout->addWidget(m_finalScoreLabel);

    m_pixelArt = new PixelArt();
    resultLayout->addWidget(m_pixelArt, 0, Qt::AlignCenter);

    auto *restartBtn = new QPushButton("重新开始");
    restartBtn->setFixedSize(160, 45);
    restartBtn->setStyleSheet(
        QString("QPushButton { background-color: %1; color: %2; "
                "font-size: 18px; font-weight: bold; font-family: '%3'; "
                "border-radius: 8px; }"
                "QPushButton:hover { background-color: #FFC107; }"
                ).arg(GOLD, DARK, FONT)
    );
    resultLayout->addWidget(restartBtn, 0, Qt::AlignCenter);
    connect(restartBtn, &QPushButton::clicked, this, &MainWindow::onStartClicked);

    auto *quitBtn = new QPushButton("退出游戏");
    quitBtn->setFixedSize(160, 40);
    quitBtn->setStyleSheet(
        "QPushButton { background-color: #555; color: #ccc; "
        "font-size: 16px; font-family: 'SimHei'; border-radius: 8px; }"
        "QPushButton:hover { background-color: #777; }"
    );
    resultLayout->addWidget(quitBtn, 0, Qt::AlignCenter);
    connect(quitBtn, &QPushButton::clicked, this, &QMainWindow::close);

    m_stack->addWidget(resultPage);  // index 2

    mainLayout->addWidget(m_stack);

    // 默认显示开始界面
    m_stack->setCurrentIndex(0);
}

void MainWindow::onStartClicked()
{
    beginGame();
}

void MainWindow::beginGame()
{
    m_stack->setCurrentIndex(1);
    m_scene->startGame();
    m_view->setFocus();
}

void MainWindow::onGameEnded(bool victory, int score)
{
    m_scene->stopGame();
    m_stack->setCurrentIndex(2);

    m_pixelArt->setVisible(victory);

    if (victory) {
        m_finalScoreLabel->setText(
            QString("<div style='text-align:center;'>"
                    "<span style='color:%1; font-size:36px; font-weight:bold;'>"
                    "良子吃饱了！</span><br>"
                    "<span style='color:#ccc; font-size:20px;'>"
                    "最终得分: %2</span></div>").arg(GOLD).arg(score)
        );
    } else {
        m_finalScoreLabel->setText(
            QString("<div style='text-align:center;'>"
                    "<span style='color:#ff5555; font-size:36px; font-weight:bold;'>"
                    "游戏结束</span><br>"
                    "<span style='color:#ccc; font-size:20px;'>"
                    "最终得分: %1</span></div>").arg(score)
        );
    }
}
