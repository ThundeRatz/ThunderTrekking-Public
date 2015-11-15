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

#include <gtest/gtest.h>

#include <cmath>

#include <Compass.hh>

class CompassTest  : public ::testing::Test {
protected:

	class CompassStub : public Trekking::Compass {
	public:
		virtual double read_compass() {
			return 1.;
		}
	};

	CompassStub compass;

	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST_F(CompassTest, angle) {
	EXPECT_EQ(0., compass.angle(1., 0.));
	EXPECT_EQ(M_PI, compass.angle(-1., 0.));
	EXPECT_EQ(-M_PI, compass.angle(-1., -0.));

	EXPECT_EQ(M_PI_2, compass.angle(0., 1.));
	EXPECT_EQ(-M_PI_2, compass.angle(0., -1.));

	EXPECT_EQ(M_PI_4, compass.angle(1., 1.));
	EXPECT_EQ(-M_PI_4, compass.angle(1., -1.));
	EXPECT_EQ(3 * M_PI_4, compass.angle(-1., 1.));
	EXPECT_EQ(-3 * M_PI_4, compass.angle(-1., -1.));
}

TEST_F(CompassTest, heading) {
	EXPECT_EQ(0., compass.heading(0., 1.));
	EXPECT_EQ(M_PI, compass.heading(0., -1.));
	EXPECT_EQ(-M_PI_2, compass.heading(-1., 0.));
	EXPECT_EQ(M_PI_2, compass.heading(1., 0.));

	EXPECT_EQ(M_PI_4, compass.heading(1., 1.));
	EXPECT_EQ(3 * M_PI_4, compass.heading(1., -1.));
	EXPECT_EQ(-M_PI_4, compass.heading(-1., 1.));
	EXPECT_EQ(-3 * M_PI_4, compass.heading(-1., -1.));
}

TEST_F(CompassTest, diff) {
	EXPECT_EQ(0., compass.diff(1.));
	EXPECT_EQ(1., compass.diff(0.));
	EXPECT_EQ(-1., compass.diff(2.));
	EXPECT_EQ(0., compass.diff(1. + 2 * M_PI));
}
