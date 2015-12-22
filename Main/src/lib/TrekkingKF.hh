/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 ThundeRatz

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#pragma once

#include <kalman/kfilter.hpp>

namespace Trekking {
	class TrekkingKF : public Kalman::KFilter<double, 0, false, false, true> {
		public:
			TrekkingKF();
			void update_accelerometer(double acceleration, double delta);
			void update_position(double position);
			void update_speed(double speed);

		protected:
			void makeBaseA();
			void makeA();
			void makeB();
			void makeH();
			void makeBaseQ();
			void makeBaseR();
			void makeBaseW();
			void makeBaseV();

		private:
			double dt;      //!< Tempo entre duas medidas
			// double sigma_a; //!< Desvio padrão da aceleração (w)
			// double sigma_z; //!< Desvio padrão do ruido das medidas (v)

			// //! \name Tamanhos dos vetores
			// int n;          //!< Estado, Ruído do Processo
			// int l;          //!< Entrada
			// int m;          //!< Medida, Ruído da Medida

			bool position_update;
	};

	typedef TrekkingKF::Vector Vetor;
	typedef TrekkingKF::Matrix Matriz;
}
