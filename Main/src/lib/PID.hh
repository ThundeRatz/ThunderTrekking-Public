
namespace Trekking {
    class PID{
        public:
            PID(double new_p, double new_i, double new_d);
            double update(double erro);


        private:
            double integral, p, i, d, ultima_amostra;
        };
}
