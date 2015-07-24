#pragma once

#include <kalman/kfilter.hpp>

namespace Trekking {
	class TrekkingKF : public Kalman::KFilter<double, 0, false, false, true> {
		public:
			TrekkingKF();

		protected:
			void makeBaseA();
			void makeBaseB();
			void makeBaseH();
			void makeBaseQ();
			void makeR();
			void makeBaseW();
			void makeBaseV();

			// void makeProcess() {} // Nao tem no KFilter,
			// void makeMeasure() {} // so no EKfilter

		private:
			double dt;      //!< Tempo entre duas medidas
			double sigma_a; //!< Desvio padrão da aceleração (w)
			double sigma_z; //!< Desvio padrão do ruido das medidas (v)

			//! \name Tamanhos dos vetores
			int n;          //!< Estado, Ruído do Processo
			int l;          //!< Entrada
			int m;          //!< Medida, Ruído da Medida
	};
}
