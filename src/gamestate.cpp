#include "gamestate.h"

GameState::GameState(QObject *parent)
    : QObject(parent)
    , m_score(0)
    , m_lives(MAX_LIVES)
    , m_comboMianzi(0)
    , m_comboBanmian(0)
{
}

int GameState::score() const { return m_score; }
int GameState::lives() const { return m_lives; }
int GameState::comboMianzi() const { return m_comboMianzi; }
int GameState::comboBanmian() const { return m_comboBanmian; }
bool GameState::isGameOver() const { return m_lives <= 0; }
bool GameState::isVictory() const { return m_score >= WIN_SCORE; }

void GameState::addScore(int points)
{
    m_score += points;
    if (m_score < 0) m_score = 0;
    emit scoreChanged(m_score);

    if (m_score >= WIN_SCORE)
        emit gameWon();
}

void GameState::loseLife()
{
    m_lives--;
    emit livesChanged(m_lives);
    if (m_lives <= 0)
        emit gameLost();
}

void GameState::resetCombo(const QString &foodType)
{
    if (foodType == "mianzi")
        m_comboMianzi = 0;
    else if (foodType == "banmian")
        m_comboBanmian = 0;
}

void GameState::incrementCombo(const QString &foodType)
{
    if (foodType == "mianzi") {
        m_comboMianzi++;
        m_comboBanmian = 0;
        if (m_comboMianzi >= 3) {
            m_comboMianzi = 0;
            emit comboText("我爱吃焖子！");
        }
    } else if (foodType == "banmian") {
        m_comboBanmian++;
        m_comboMianzi = 0;
        if (m_comboBanmian >= 3) {
            m_comboBanmian = 0;
            emit comboText("板面！");
        }
    } else {
        m_comboMianzi = 0;
        m_comboBanmian = 0;
    }
}

void GameState::reset()
{
    m_score = 0;
    m_lives = MAX_LIVES;
    m_comboMianzi = 0;
    m_comboBanmian = 0;
}
