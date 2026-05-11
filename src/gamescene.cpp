#include "gamescene.h"
#include "fooditem.h"
#include <QKeyEvent>
#include <QFont>
#include <QPen>
#include <QMessageBox>
#include <QRandomGenerator>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent)
    , m_player(nullptr)
    , m_state(nullptr)
    , m_timer(new QTimer(this))
    , m_scoreText(nullptr)
    , m_livesText(nullptr)
    , m_spawnCounter(0)
    , m_baseSpawnInterval(25)
    , m_gameRunning(false)
{
    setSceneRect(0, 0, 800, 600);
    setBackgroundBrush(QColor(30, 30, 35));

    connect(m_timer, &QTimer::timeout, this, &GameScene::onTick);
}

GameScene::~GameScene()
{
    stopGame();
}

void GameScene::startGame()
{
    m_gameRunning = true;

    // 状态
    m_state = new GameState(this);
    connect(m_state, &GameState::gameLost, this, [this]() {
        int finalScore = m_state->score();
        stopGame();
        emit gameEnded(false, finalScore);
    });
    connect(m_state, &GameState::gameWon, this, [this]() {
        int finalScore = m_state->score();
        stopGame();
        emit gameEnded(true, finalScore);
    });
    connect(m_state, &GameState::comboText, this, &GameScene::showComboText);

    // 玩家
    m_player = new Player();
    m_player->setSceneWidth(800);
    addItem(m_player);

    // 分数显示
    m_scoreText = addText("分数: 0");
    m_scoreText->setDefaultTextColor(QColor(255, 215, 0));
    m_scoreText->setFont(QFont("SimHei", 18, QFont::Bold));
    m_scoreText->setPos(10, 10);
    m_scoreText->setZValue(100);

    // 生命显示
    m_livesText = addText("生命: ♥♥♥");
    m_livesText->setDefaultTextColor(QColor(255, 100, 100));
    m_livesText->setFont(QFont("SimHei", 16));
    m_livesText->setPos(650, 10);
    m_livesText->setZValue(100);

    m_spawnCounter = 0;
    m_timer->start(30);
}

void GameScene::stopGame()
{
    m_gameRunning = false;
    m_timer->stop();

    // 清理所有食物
    for (auto *food : m_foodItems) {
        removeItem(food);
        delete food;
    }
    m_foodItems.clear();

    // 清理所有陷阱
    for (auto *trap : m_trapItems) {
        removeItem(trap);
        delete trap;
    }
    m_trapItems.clear();

    // 清理玩家
    if (m_player) {
        removeItem(m_player);
        delete m_player;
        m_player = nullptr;
    }

    // 清理 UI 文本
    if (m_scoreText) {
        removeItem(m_scoreText);
        delete m_scoreText;
        m_scoreText = nullptr;
    }
    if (m_livesText) {
        removeItem(m_livesText);
        delete m_livesText;
        m_livesText = nullptr;
    }

    // 清理连击文字
    clearComboTexts();

    // 清理状态
    if (m_state) {
        delete m_state;
        m_state = nullptr;
    }
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    if (!m_gameRunning || !m_player)
        return;

    switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_A:
        m_player->moveLeft();
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        m_player->moveRight();
        break;
    default:
        QGraphicsScene::keyPressEvent(event);
    }
}

void GameScene::onTick()
{
    if (!m_gameRunning) return;

    m_spawnCounter++;

    // 难度递增：食物掉落间隔随分数缩短
    int interval = m_baseSpawnInterval - m_state->score() / 50;
    if (interval < 8) interval = 8;

    if (m_spawnCounter % interval == 0)
        spawnFood();

    if (m_spawnCounter % 50 == 0)
        spawnTrap();

    moveItems();
    checkCollisions();
    cleanOffscreenItems();
    animateComboTexts();
    updateScoreDisplay();
}

void GameScene::spawnFood()
{
    FoodType types[] = {
        FoodType::PaoMian,
        FoodType::MianZi,
        FoodType::BanMian,
        FoodType::KeLe
    };
    FoodType type = types[QRandomGenerator::global()->bounded(4)];

    auto *food = new FoodItem(type);
    int x = QRandomGenerator::global()->bounded(50, 751);
    food->setPos(x, -20);
    m_foodItems.append(food);
    addItem(food);
}

void GameScene::spawnTrap()
{
    // 75% 概率生成陷阱
    if (QRandomGenerator::global()->bounded(100) > 75)
        return;

    TrapType type = (QRandomGenerator::global()->bounded(2) == 0)
        ? TrapType::Vegetable : TrapType::DietPill;

    auto *trap = new TrapItem(type);
    int x = QRandomGenerator::global()->bounded(50, 751);
    trap->setPos(x, -20);
    m_trapItems.append(trap);
    addItem(trap);
}

void GameScene::moveItems()
{
    // 掉落速度随分数递增
    int speed = 4 + m_state->score() / 30;
    if (speed > 12) speed = 12;

    for (auto *food : m_foodItems) {
        food->setPos(food->x(), food->y() + speed);
    }
    for (auto *trap : m_trapItems) {
        trap->setPos(trap->x(), trap->y() + speed);
    }
}

void GameScene::checkCollisions()
{
    QList<FoodItem *> eaten;
    QList<TrapItem *> hit;

    for (auto *food : m_foodItems) {
        if (food->collidesWithItem(m_player)) {
            m_state->addScore(food->scoreValue());
            m_state->incrementCombo(
                food->foodType() == FoodType::MianZi  ? "mianzi"  :
                food->foodType() == FoodType::BanMian ? "banmian" : ""
            );
            m_player->setFatness(m_state->score());

            if (food->foodType() != FoodType::MianZi &&
                food->foodType() != FoodType::BanMian) {
                m_state->resetCombo("");
            }

            eaten.append(food);
        }
    }

    for (auto *trap : m_trapItems) {
        if (trap->collidesWithItem(m_player)) {
            if (trap->isFatal()) {
                m_state->loseLife();
            } else {
                m_state->addScore(-trap->scorePenalty());
            }
            hit.append(trap);
        }
    }

    for (auto *food : eaten) {
        m_foodItems.removeOne(food);
        removeItem(food);
        delete food;
    }
    for (auto *trap : hit) {
        m_trapItems.removeOne(trap);
        removeItem(trap);
        delete trap;
    }
}

void GameScene::cleanOffscreenItems()
{
    QList<FoodItem *> foodToRemove;
    QList<TrapItem *> trapToRemove;

    for (auto *food : m_foodItems) {
        if (food->y() > 620)
            foodToRemove.append(food);
    }
    for (auto *food : foodToRemove) {
        m_foodItems.removeOne(food);
        removeItem(food);
        delete food;
    }
    for (auto *trap : m_trapItems) {
        if (trap->y() > 620)
            trapToRemove.append(trap);
    }
    for (auto *trap : trapToRemove) {
        m_trapItems.removeOne(trap);
        removeItem(trap);
        delete trap;
    }
}

void GameScene::updateScoreDisplay()
{
    if (m_scoreText && m_state) {
        m_scoreText->setPlainText(QString("分数: %1").arg(m_state->score()));
    }
    if (m_livesText && m_state) {
        QString hearts;
        for (int i = 0; i < m_state->lives(); i++)
            hearts += "♥";
        m_livesText->setPlainText(QString("生命: %1").arg(hearts));
    }
}

void GameScene::showComboText(const QString &text)
{
    auto *txt = addText(text);
    txt->setDefaultTextColor(QColor(255, 215, 0));
    txt->setFont(QFont("SimHei", 28, QFont::Bold));
    txt->setPos(400 - txt->boundingRect().width() / 2, 250);
    txt->setZValue(200);

    ComboText ct;
    ct.item = txt;
    ct.lifetime = 50; // 50帧 ≈ 1.5秒
    m_comboTexts.append(ct);
}

void GameScene::animateComboTexts()
{
    for (int i = m_comboTexts.size() - 1; i >= 0; i--) {
        ComboText &ct = m_comboTexts[i];
        ct.lifetime--;
        ct.item->setPos(ct.item->x(), ct.item->y() - 1.5);

        int alpha = (ct.lifetime > 30) ? 255 : (ct.lifetime * 255 / 30);
        QColor c(255, 215, 0);
        c.setAlpha(alpha);
        ct.item->setDefaultTextColor(c);

        if (ct.lifetime <= 0) {
            removeItem(ct.item);
            delete ct.item;
            m_comboTexts.removeAt(i);
        }
    }
}

void GameScene::clearComboTexts()
{
    for (auto &ct : m_comboTexts) {
        removeItem(ct.item);
        delete ct.item;
    }
    m_comboTexts.clear();
}

void GameScene::showScoreBoard()
{
    // 阶段5完善
}
