// Copyright 2016 Arnaud Becheler    <Arnaud.Becheler@egce.cnrs-gif.fr>

/***********************************************************************                                                                         *
* This program is free software; you can redistribute it and/or modify *
* it under the terms of the GNU General Public License as published by *
* the Free Software Foundation; either version 2 of the License, or    *
* (at your option) any later version.                                  *
*                                                                      *
***************************************************************************/

#ifndef __OCCUPANCY_SPECTRUM_ALGORITHM_H_INCLUDED__
#define __OCCUPANCY_SPECTRUM_ALGORITHM_H_INCLUDED__

#include <vector>
#include <utility>

namespace quetzal{

namespace coalescence{

namespace occupancy_spectrum{

  /*!
   * \ingroup coal_spectrum
   * \brief Generative algorithm for the occupancy spectra resulting from
   *        throwing n balls in m urns.
   * \section Example
   * \snippet coalescence/occupancy_spectrum/test/Generator/Generator_test.cpp Example
   * \section Output
   * \include coalescence/occupancy_spectrum/test/Generator/Generator_test.output
   */
struct Generator{
  /** Type of the occupancy spectrums generated by the algorithm */
  using occupancy_spectrum_type = std::vector<unsigned int>;

  /**
	  * \brief Generative algorithm
	  * \param n number of balls to throw
    * \param m number of urns
    * \param op unary operation function object that will be applied to handle
                generated spectra. The signature
                of the function should be equivalent to the following:
                `Ret fun(Generator::occupancy_spectrum_type && s);`
    * \section Example
		* \snippet coalescence/occupancy_spectrum/test/Generator/Generator_test.cpp Example
		* \section Output
		* \include coalescence/occupancy_spectrum/test/Generator/Generator_test.output
		*/
  template<class UnaryOperation>
  static void generate(unsigned int n, unsigned int m, UnaryOperation op){
    occupancy_spectrum_type M_j(n+1);
    f(n, m, n, M_j, op);
  }

private:

  // j_max = nombre de balles dans l'urne la plus remplie.
  template<class Callback>
  static void f(unsigned int n, unsigned int m, unsigned int j_max, occupancy_spectrum_type& v, Callback callback){

    if(m == 0 && n == 0){
      // probleme resolu
      callback(std::move(v));
      return;
    }

    if (m != 0){

      if (n == 0) {
        auto copy = v;
        copy[0] = m; // ici n = 0 et m = 0, donc probleme resolu
        callback(std::move(copy));
        return;

      } else {
        if( j_max > 0){
          // vertical descent
          for(unsigned int i = n/j_max; i >= 1; --i){

            auto copy = v;
            copy[j_max] = i;

            if(m >= i){
              unsigned int balls_left = n - i*j_max;
              if(balls_left < j_max){
                f( balls_left, m-i, balls_left, copy, callback);
              }else {
                f( balls_left, m-i, j_max-1, copy, callback);
              }
            }
          }
        }
      }
    }

    // horizontal descent
    if( j_max == 0 && n > 0 ){
      // unsolvable
      return;
    }

    auto copy = v;
    copy[j_max] = 0;
    f(n, m, j_max-1, copy, callback);

  }

};

} // namespace occupancy_spectrum
} // namespace coalescence
} // namespace quetzal

#endif
