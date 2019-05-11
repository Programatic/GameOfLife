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
#include <algorithm>

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
                        b->set_coordinates(i, j);
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
        if (b->is_alive()) {
                b->set_state(false);
                remove_element_from_alive(b);
        } else {
                b->set_state(true);
                add_element_to_alive(b);
        }
}

void GameWindow::remove_element_from_alive(Cell* b)
{
        std::vector<Cell*>::iterator position = std::find(curr_alive.begin(), curr_alive.end(), b);
        if (position != curr_alive.end()) // == myVector.end() means the element was not found
                curr_alive.erase(position);
}

void GameWindow::add_element_to_alive(Cell *b)
{
        if (!(std::find(curr_alive.begin(), curr_alive.end(), b) != curr_alive.end())) {
                curr_alive.push_back(b);
        }
}

void GameWindow::next()
{
        //dump_vec(curr_alive);
        std::vector<Cell*> nchk(curr_alive);
        for(std::vector<Cell*>::iterator it = curr_alive.begin(), end = curr_alive.end(); it != end; ++it) {
                std::vector<Cell*> neigh = get_neighbors(*it);
                for (std::vector<Cell*>::iterator i = neigh.begin(), end = neigh.end(); i != end; ++i) {
                        nchk.push_back(*i);
                }
        }
        sort(nchk.begin(), nchk.end());
        nchk.erase(unique(nchk.begin(), nchk.end()), nchk.end());

        updates = nchk;

        for (std::vector<Cell*>::iterator it = nchk.begin(), end = nchk.end(); it != end; ++it)  {
                int n_alive = number_neighbors(*it); //NOTE: J is width and i is actually height

                if (n_alive == 0 || n_alive == 1) {
                        (*it)->set_next_state(false);
                        remove_element_from_alive(*it);
                }
                if (n_alive >= 4) {
                        (*it)->set_next_state(false);
                        remove_element_from_alive(*it);
                }
                if (n_alive == 3) {
                        (*it)->set_next_state(true);
                        add_element_to_alive(*it);
                }
                if (n_alive == 2 && (*it)->is_alive()) {
                        (*it)->set_next_state(true);
                        add_element_to_alive(*it);
                }

                //std::cout << (*it)->get_i() << " " << (*it)->get_j() << " " << n_alive << "\n";
        }
              
        update_states();
}

void GameWindow::update_states()
{
        for (std::vector<Cell*>::iterator it = updates.begin(), end = updates.end(); it != end; ++it) {
                (*it)->load_next_state();
        }
}

int GameWindow::number_neighbors(Cell *b)
{
        std::vector<Cell*> n = get_neighbors(b);
        int n_alive = 0;

        for (std::vector<Cell*>::iterator it = n.begin(), end = n.end(); it != end; ++it) {
                n_alive += (*it)->is_alive();
        }
        return n_alive;
}

std::vector<Cell*> GameWindow::get_neighbors(Cell *b)
{
        int i = b->get_i();
        int j = b->get_j();

        std::vector<Cell*> n;

        if (i == 0 && j == 0) {
                        n.push_back(map[i + 1][j]);
                        n.push_back(map[i+1][j+1]);
                        n.push_back(map[i][j+1]);
                } else if (i == width - 1 && j == height - 1) {
                        n.push_back(map[i-1][j]);
                        n.push_back(map[i-1][j-1]);
                        n.push_back(map[i][j-1]);
                } else if (i == width - 1 && j == 0) {
                        n.push_back(map[i-1][j]);
                        n.push_back(map[i-1][j+1]);
                        n.push_back(map[i][j+1]);
                } else if (i == 0 && j == height - 1) {
                        n.push_back(map[i][j-1]);
                        n.push_back(map[i+1][j-1]);
                        n.push_back(map[i+1][j]);
                } else {
                        if (i == 0) {
                                n.push_back(map[i+1][j]);
                                n.push_back(map[i+1][j+1]);
                                n.push_back(map[i+1][j-1]);
                                n.push_back(map[i][j-1]);
                                n.push_back(map[i][j+1]);
                        } else if (i == width - 1) {
                                n.push_back(map[i-1][j]);
                                n.push_back(map[i-1][j+1]);
                                n.push_back(map[i-1][j-1]);
                                n.push_back(map[i][j-1]);
                                n.push_back(map[i][j+1]);
                        } else if (j == height - 1) {
                                n.push_back(map[i][j-1]);
                                n.push_back(map[i-1][j-1]);
                                n.push_back(map[i+1][j-1]);
                                n.push_back(map[i+1][j]);
                                n.push_back(map[i-1][j]);
                        } else if (j == 0) {
                                n.push_back(map[i][j+1]);
                                n.push_back(map[i-1][j+1]);
                                n.push_back(map[i+1][j+1]);
                                n.push_back(map[i+1][j]);
                                n.push_back(map[i-1][j]);
                        } else {
                                n.push_back(map[i][j-1]);
                                n.push_back(map[i-1][j-1]);
                                n.push_back(map[i+1][j-1]);
                                n.push_back(map[i+1][j]);
                                n.push_back(map[i-1][j]);
                                n.push_back(map[i][j+1]);
                                n.push_back(map[i-1][j+1]);
                                n.push_back(map[i+1][j+1]);
                        }

                }

        return n;
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
        for (std::vector<Cell*>::iterator it = curr_alive.begin(), end = curr_alive.end(); it != end; ++it) {
                (*it)->reset();
        }
        curr_alive.clear();
}

void GameWindow::dump_vec(std::vector<Cell*> v)
{
        for (std::vector<Cell*>::iterator it = v.begin(), end = v.end(); it != end; ++it)
                std::cout << (*it)->get_i() << " " << (*it)->get_j() << " " << (*it)->is_alive() << "\n";
}
