#include "../include/cell.hpp"

bool Cell::is_alive()
{
        return alive;
}

void Cell::set_alive(bool s)
{
        alive = s;
}
