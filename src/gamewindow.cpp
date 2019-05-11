#include "../include/gamewindow.hpp"
#include <QInputDialog>
#include <QWidget>
#include <QRect>
#include <QGridLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QScreen>
#include <QSize>
#include <iostream>
#include <QStatusBar>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QShortcut>

GameWindow::GameWindow()
{
        create_menu_bar();
        width = QInputDialog::getInt(this, tr("Width"), tr("Please enter game width:"), QLineEdit::Normal);
        height = QInputDialog::getInt(this, tr("Height"), tr("Please enter game height:"), QLineEdit::Normal);
        //width = 50;
        //height = 50;

        statusBar()->setSizeGripEnabled(false);

        QWidget *disp = new QWidget;
        QGridLayout *layout = new QGridLayout;

        QDesktopWidget *desktop = QApplication::desktop();
        QRect geo = desktop->availableGeometry();
        int sheight = geo.height();
        int swidth = geo.width();
        int use = (sheight > swidth) ? swidth : sheight;

        QSize size(use/height, use/height);

        for (int i = 0; i < width; i++) {
                std::vector<Cell*> v;
                for (int j = 0; j < height; j++) {
                        Cell *b = new Cell("", disp);
                        b->setStyleSheet("border-radius: 0px;");
                        b->setFixedSize(size);
                        b->set_palette(b->palette());
                        connect(b, SIGNAL (released()), this, SLOT (button_press()));
                        layout->addWidget(b, i, j, 1, 1);
                        v.push_back(b);
                }
                map.push_back(v);
        }
        layout->setSpacing(0);

        disp->setLayout(layout);
        setCentralWidget(disp);

        show();

        QSize sz(disp->width(), disp->height());
        this->setMinimumSize(sz);
        this->setMaximumSize(sz);
}

void GameWindow::button_press()
{
        Cell *b = qobject_cast<Cell*>(sender());
        b->toggle_alive();
}

void GameWindow::next()
{
        for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                        int n_alive = number_neighbors(i,j); //NOTE: J is width and i is actually height

                        if (n_alive == 0 || n_alive == 1)
                                map[i][j]->set_next_state(false);
                        if (n_alive >= 4)
                                map[i][j]->set_next_state(false);
                        if (n_alive == 3)
                                map[i][j]->set_next_state(true);
                        if (n_alive == 2 && map[i][j]->is_alive())
                                map[i][j]->set_next_state(true);
                }
        }
        update_states();
}

void GameWindow::update_states()
{
        for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                        map[i][j]->load_next_state();
                }
        }
}

int GameWindow::number_neighbors(int i, int j)
{
        int n_alive = 0;
        if (i == 0 && j == 0) {
                        n_alive += map[i + 1][j]->is_alive() +
                                map[i+1][j+1]->is_alive() +
                                map[i][j+1]->is_alive();
                } else if (i == width - 1 && j == height - 1) {
                        n_alive += map[i-1][j]->is_alive() +
                                map[i-1][j-1]->is_alive() +
                                map[i][j-1]->is_alive();
                } else if (i == width - 1 && j == 0) {
                        n_alive += map[i-1][j]->is_alive() +
                                map[i-1][j+1]->is_alive() +
                                map[i][j+1]->is_alive();
                } else if (i == 0 && j == height - 1) {
                        n_alive += map[i][j-1]->is_alive() +
                                map[i+1][j-1]->is_alive() +
                                map[i+1][j]->is_alive();
                } else {
                        if (i == 0) {
                                n_alive += map[i+1][j]->is_alive() +
                                        map[i+1][j+1]->is_alive() +
                                        map[i+1][j-1]->is_alive() +
                                        map[i][j-1]->is_alive() +
                                        map[i][j+1]->is_alive();
                        } else if (i == width - 1) {
                                n_alive += map[i-1][j]->is_alive() +
                                        map[i-1][j+1]->is_alive() +
                                        map[i-1][j-1]->is_alive() +
                                        map[i][j-1]->is_alive() +
                                        map[i][j+1]->is_alive();
                        } else if (j == height - 1) {
                                n_alive += map[i][j-1]->is_alive() +
                                        map[i-1][j-1]->is_alive() +
                                        map[i+1][j-1]->is_alive() +
                                        map[i+1][j]->is_alive() +
                                        map[i-1][j]->is_alive();
                        } else if (j == 0) {
                                n_alive += map[i][j+1]->is_alive() +
                                        map[i-1][j+1]->is_alive() +
                                        map[i+1][j+1]->is_alive() +
                                        map[i+1][j]->is_alive() +
                                        map[i-1][j]->is_alive();
                        } else {
                                n_alive += map[i][j-1]->is_alive() +
                                        map[i-1][j-1]->is_alive() +
                                        map[i+1][j-1]->is_alive() +
                                        map[i+1][j]->is_alive() +
                                        map[i-1][j]->is_alive() +
                                        map[i][j+1]->is_alive() +
                                        map[i-1][j+1]->is_alive() +
                                        map[i+1][j+1]->is_alive();
                        }
                       
                }
        return n_alive;
}

void GameWindow::create_menu_bar()
{
        QMenu *file_menu = menuBar()->addMenu(tr("&Actions"));
        QAction *n_act = new QAction(tr("&Next"), this);
        connect(n_act, &QAction::triggered, this, &GameWindow::next);
        file_menu->addAction(n_act);
        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_N), this, SLOT(next()));

        QAction *r_act = new QAction(tr("Reset"), this);
        connect(r_act, &QAction::triggered, this, &GameWindow::reset);
        file_menu->addAction(r_act);
}

void GameWindow::reset()
{
        for (int i = 0; i < width; i++)
                for (int j = 0; j < height; j++)
                        map[i][j]->reset();
}
