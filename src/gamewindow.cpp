#include "../include/gamewindow.hpp"
#include <QInputDialog>
#include <QWidget>
#include <QRect>
#include <QGridLayout>
#include <QApplication>
#include <QScreen>
#include <QSize>
#include <iostream>
#include <QWeakPointer>

GameWindow::GameWindow()
{
        //width = QInputDialog::getInt(this, tr("Width"), tr("Please enter game width:"), QLineEdit::Normal);
        //height = QInputDialog::getInt(this, tr("Height"), tr("Please enter game height:"), QLineEdit::Normal);
        width = 50;
        height = 50;

        QSize screen = this->size();
        int swidth = screen.width();
        int sheight = screen.height();
        std::cout << sheight << " " << height;

        QWidget *disp = new QWidget;
        QGridLayout *layout = new QGridLayout;

        for (int i = 0; i < height; i++) {
                std::vector<Cell*> v;
                for (int j = 0; j < width; j++) {
                        Cell *b = new Cell("", disp);
                        de = b->palette();
                        connect(b, SIGNAL (released()), this, SLOT (button_press()));
                        layout->addWidget(b, i, j, 1, 1);
                        v.push_back(b);
                }
                map.push_back(v);
        }
        layout->setSpacing(0);

        disp->setLayout(layout);
        setCentralWidget(disp);
}

void GameWindow::button_press()
{
        Cell *b = qobject_cast<Cell*>(sender());
        QPalette pal = b->palette();
        if (!b->is_alive()) {
                pal.setColor(QPalette::Button, QColor(Qt::red));
                b->setAutoFillBackground(true);
                b->setPalette(pal);
                b->update();
                b->set_alive(true);
        } else {
                b->setAutoFillBackground(false);
                b->setPalette(de);
                b->update();
                b->set_alive(false);
        }
}
