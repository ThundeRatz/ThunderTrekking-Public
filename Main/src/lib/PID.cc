#include <math.h>
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
            if (erro > M_PI / 4)
                return - 255 - 255;
            if (erro < -M_PI / 4)
                return 255 + 255;
            integral += erro;
            if (integral > 500)
                integral = 500;
            if (integral < -500)
                integral = -500;
            ultima_amostra = erro;
            return - erro * p - integral * i + (amostra_antiga - erro) * d;
        }
}
