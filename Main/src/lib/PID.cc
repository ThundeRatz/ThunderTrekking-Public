#include <cmath>
#include <iostream>

#include "PID.hh"

namespace Trekking {
        PID::PID(double new_p, double new_i, double new_d) {
            p = new_p;
            i = new_i;
            d = new_d;
            integral = 0;
            ultima_amostra = 0;
        }

        double PID::update(double erro) {
            int amostra_antiga = ultima_amostra;
            integral += erro;
            ultima_amostra = erro;
            return - erro * p - integral * i + (amostra_antiga - erro) * d;
        }
}
