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

// http://kalman.sourceforge.net
// https://en.wikipedia.org/wiki/Kalman_filter
// http://www.diva-portal.org/smash/get/diva2:743775/FULLTEXT01.pdf

#include <cmath>

#include "TrekkingKF.hh"

namespace Trekking {
	TrekkingKF::TrekkingKF() {
		// n = 2;
		// l = 1; // 0
		// m = 4; // 4

		// dt = 0.1;
		// sigma_a = 1.0; // Ver aqui
		// sigma_z = 1.0; // E aqui

		// estado: posição, velocidade (x).
		// entrada: acelerômetro (u).
		// ruído do processo (w).
		// medida: velocidade ou posição (z).
		// ruído da medida (v).
		setDim(2, 1, 1, 1, 1);

		Vector x(2);
		Matrix P(2, 2);
		x(0) = 0;
		x(1) = 0;
		P(0, 0) = 0;
		P(0, 1) = 0;
		P(1, 0) = 0;
		P(1, 1) = 0;
		init(x, P);
	}

	void TrekkingKF::update_accelerometer(double acceleration, double delta) {
		TrekkingKF::Vector u(1);
		u(0) = acceleration;
		timeUpdateStep(u);
		dt = delta;
	}

	void TrekkingKF::update_position(double position) {
		position_update = true;
		TrekkingKF::Vector z(1);
		z = position;
		measureUpdateStep(z);
	}

	void TrekkingKF::update_speed(double speed) {
		position_update = false;
		TrekkingKF::Vector z(1);
		z = speed;
		measureUpdateStep(z);
	}

	/// del(f) / del(x).
	void TrekkingKF::makeBaseA() {
		A(0, 0) = 1;
		A(0,1) = dt;

		A(1, 0) = 0.0;
		A(1, 1) = 1.0;
	}

	/// del(f) / del(x).
	void TrekkingKF::makeA() {
		A(0, 1) = dt;
	}

	/// f = A*x + B*u + W*w
	/// B = M(2, 1)
	void TrekkingKF::makeB() {
		// [1 ; 0] para GPS, [0 ; 1] para encoder
		if (position_update) {
			B(0, 0) = 1.;
			B(0, 1) = 0.;
		} else {
			B(0, 0) = 0.;
			B(0, 1) = 1.;

		}
	}

	void TrekkingKF::makeH() {
		// [1 ; 0] para GPS, [0 ; 1] para encoder
		if (position_update) {
			H(0, 0) = 1.;
			H(0, 1) = 0.;
		} else {
			H(0, 0) = 0.;
			H(0, 1) = 1.;
		}
	}

	/// Cov. ruído do processo.
	void TrekkingKF::makeBaseQ() {
		Q(0, 0) = 1.;
		Q(0, 1) = 0.;

		Q(1, 0) = 0.;
		Q(1, 1) = 1.;
	}

	/// Cov. ruído da medida.
	void TrekkingKF::makeBaseR() {
		// TODO Fazer aqui
		R(0, 0) = 1.;
		R(0, 1) = 0.;

		R(1, 0) = 0.;
		R(1, 1) = 1.;
	}

	// del(h) / del(v)
	void TrekkingKF::makeBaseV() {
		V(0, 0) = 1.;
	}

	/// del(f) / del(w)
	void TrekkingKF::makeBaseW() {
		W(0, 0) = 0.;
		W(0, 1) = 1.;
	}
}
