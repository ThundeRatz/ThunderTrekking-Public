// http://kalman.sourceforge.net
// https://en.wikipedia.org/wiki/Kalman_filter
// http://www.diva-portal.org/smash/get/diva2:743775/FULLTEXT01.pdf

#include <cmath>

#include "TrekkingKF.hh"

namespace Trekking {
	TrekkingKF::TrekkingKF() {
		n = 3; // 4
		l = 1; // 0
		m = 4; // 4
		setDim(n, l, n, m, m);
		dt = 0.1;
		// sigma_a = 1.0; // Ver aqui
		// sigma_z = 1.0; // E aqui
	}

	void TrekkingKF::makeBaseA() {
		A(0,0) = 1.0;
		A(0,1) = dt;
		A(0,2) = dt*dt/2;

		A(1,0) = 0.0;
		A(1,1) = 1.0;
		A(1,2) = dt;

		A(2,0) = 0.0;
		A(2,1) = 0.0;
		A(2,2) = 1.0;
	}

	void TrekkingKF::makeBaseB() {
		B(0,0) = 0.0;
	}

	void TrekkingKF::makeBaseH() {
		H(0,0) = 1.0;
		H(0,1) = 0.0;
		H(0,2) = 0.0;

		H(1,0) = 0.0;
		H(1,1) = 1.0;
		H(1,2) = 0.0;

		H(2,0) = 0.0;
		H(2,1) = 1.0;
		H(2,2) = 0.0;

		H(3,0) = 0.0;
		H(3,1) = 0.0;
		H(3,2) = 1.0;
	}

	void TrekkingKF::makeBaseQ() {
		Q(0,0) = 1.0;
		Q(0,1) = 0.0;

		Q(1,0) = 0.0;
		Q(1,1) = 1.0;
	}

	void TrekkingKF::makeBaseR() {
		// TODO Fazer aqui
		R(0,0) = 1.0;
		R(0,1) = 0.0;
		R(0,2) = 0.0;
		R(0,3) = 0.0;

		R(1,0) = 0.0;
		R(1,1) = 1.0;
		R(1,2) = 0.0;
		R(1,3) = 0.0;

		R(2,0) = 0.0;
		R(2,1) = 0.0;
		R(2,2) = 1.0;
		R(2,3) = 0.0;

		R(3,0) = 0.0;
		R(3,1) = 0.0;
		R(3,2) = 0.0;
		R(3,3) = 1.0;
	}

	void TrekkingKF::makeBaseV() {
		V(0,0) = 1.0;
		V(0,1) = 0.0;
		V(0,2) = 0.0;
		V(0,3) = 0.0;

		V(1,0) = 0.0;
		V(1,1) = 1.0;
		V(1,2) = 0.0;
		V(1,3) = 0.0;

		V(2,0) = 0.0;
		V(2,1) = 0.0;
		V(2,2) = 1.0;
		V(2,3) = 0.0;

		V(3,0) = 0.0;
		V(3,1) = 0.0;
		V(3,2) = 0.0;
		V(3,3) = 1.0;
	}

	void TrekkingKF::makeBaseW() {
		W(0,0) = 0.0;
		W(0,1) = 0.0;

		W(1,0) = 0.0;
		W(1,1) = 1.0;

		W(2,0) = 1.0;
		W(2,1) = 0.0;
	}
}
