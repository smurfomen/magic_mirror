#include "plotter.h"
#include <barrel_plotter.h>
Plotter * Plotter::factory(QString param) {
    return new BarrelPlotter(param);
}

