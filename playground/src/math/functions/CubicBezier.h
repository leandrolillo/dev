/*
 * CubicBezier.h
 *
 *  Created on: May 6, 2021
 *      Author: leandro
 */

#ifndef SRC_MATH_FUNCTIONS_CUBICBEZIER_H_
#define SRC_MATH_FUNCTIONS_CUBICBEZIER_H_

#include<FunctionRealVectorial.h>
#include<Logger.h>

class CubicBezier: public FunctionRealVectorial {
    private:
        vectorN x0;
        vectorN x1;
        vectorN x2;
        vectorN x3;
        Logger *logger;

    public:
        CubicBezier() {
            logger = Logger::getLogger("CubicBezier");

        }

        void setControlPoints(const vectorN &x0, const vectorN &x1, const vectorN &x2, const vectorN &x3) {
            unsigned int length = x0.getLength();

            if (length <= 0 || x1.getLength() != length || x2.getLength() != length || x3.getLength() != length)
                throw InvalidArgumentException("Control points length should match and be greater than zero");

            this->x0 = x0;
            this->x1 = x1;
            this->x2 = x2;
            this->x3 = x3;
        }

        vectorN evaluate(real t) const {
            if (t < 0 || t > 1.0)
                throw new InvalidArgumentException("t parameter must belong to [0.0, 1.0] interval");

//      vectorN term1 = x0 * pow(1.0 - t, 3.0);
//      logger->debug("term1 = [%s]", term1.toString().c_str());
//      vectorN term2 = x1 * (3.0 * pow(1.0 - t, 2.0) * t);
//      logger->debug("term2 = [%s]", term2.toString().c_str());
//      vectorN term3 = x2 * (3.0 * (1.0 - t) * pow(t, 2.0));
//      logger->debug("term3 = [%s]", term3.toString().c_str());
//      vectorN term4 = x3 * pow (t, 3.0);
//      logger->debug("term4 = [%s]", term4.toString().c_str());
//
//      vectorN result = term1 + term2 + term3 + term4;
//      logger->debug("result = [%s]", result.toString().c_str());

            return x0 * pow(1.0 - t, 3.0) + x1 * (3.0 * pow(1.0 - t, 2.0) * t) + x2 * (3.0 * (1.0 - t) * pow(t, 2.0))
                    + x3 * pow(t, 3.0);
        }

        String toString() const
        {
            return "Cubic Bezier";
        }
    };



#endif /* SRC_MATH_FUNCTIONS_CUBICBEZIER_H_ */
