#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QGraphicsView>
#include <QLabel>
#include <QPushButton>
#include "gamescene.h"
#include "pixelart.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onStartClicked();
    void onGameEnded(bool victory, int score);

private:
    void setupUI();
    void beginGame();

    QStackedWidget *m_stack;
    QGraphicsView *m_view;
    GameScene *m_scene;
    QLabel *m_finalScoreLabel;
    PixelArt *m_pixelArt;
};

#endif // MAINWINDOW_H
