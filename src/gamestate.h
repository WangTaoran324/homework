#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QObject>

class GameState : public QObject
{
    Q_OBJECT

public:
    explicit GameState(QObject *parent = nullptr);

    int score() const;
    int lives() const;
    int comboMianzi() const;
    int comboBanmian() const;
    bool isGameOver() const;
    bool isVictory() const;

    void addScore(int points);
    void loseLife();
    void resetCombo(const QString &foodType);
    void incrementCombo(const QString &foodType);
    void reset();

signals:
    void scoreChanged(int score);
    void livesChanged(int lives);
    void gameLost();
    void gameWon();
    void comboText(const QString &text);

private:
    int m_score;
    int m_lives;
    int m_comboMianzi;   // 连续焖子计数
    int m_comboBanmian;  // 连续板面计数

    static constexpr int WIN_SCORE = 250;
    static constexpr int MAX_LIVES = 3;
};

#endif // GAMESTATE_H
