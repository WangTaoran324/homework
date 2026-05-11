#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsTextItem>
#include <QList>
#include "player.h"
#include "gamestate.h"
#include "fooditem.h"
#include "trapitem.h"

class GameScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GameScene(QObject *parent = nullptr);
    ~GameScene();

    void startGame();
    void stopGame();

signals:
    void gameEnded(bool victory, int score);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onTick();

private:
    void spawnFood();
    void spawnTrap();
    void checkCollisions();
    void showScoreBoard();
    void updateScoreDisplay();
    void moveItems();
    void cleanOffscreenItems();
    void showComboText(const QString &text);
    void animateComboTexts();
    void clearComboTexts();

    Player *m_player;
    GameState *m_state;
    QTimer *m_timer;
    QGraphicsTextItem *m_scoreText;
    QGraphicsTextItem *m_livesText;

    struct ComboText {
        QGraphicsTextItem *item;
        int lifetime; // 剩余帧数
    };
    QList<ComboText> m_comboTexts;

    QList<FoodItem *> m_foodItems;
    QList<TrapItem *> m_trapItems;

    int m_spawnCounter;
    int m_baseSpawnInterval;
    bool m_gameRunning;
};

#endif // GAMESCENE_H
