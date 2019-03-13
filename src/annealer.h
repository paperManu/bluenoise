/*
 * Copyright (C) 2019 Emmanuel Durand
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Splash is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <functional>

namespace bluenoise
{

/**
 * Annealer, a simulated-annealing optimizing class
 */
template <class T>
class Annealer
{
  public:
    using ErrorFunc = std::function<double(const T&)>;
    using NeighbourFunc = std::function<T(const T&)>;

  public:
    /**
     * Constructor
     * \param tmax Maximum/initial temperature
     * \param efunc Error function
     * \param nfunc Neighbour generation function
     */
    Annealer(double tmax, const ErrorFunc& eFunc, const NeighbourFunc& nfunc) : _temperature(tmax), _errorFunc(eFunc), _neighbourFunc(nfunc) {}

    /**
     * Apply simulated annealing from the given initial state
     * \param initialState Initial state
     * \return Return the optimized state
     */
    T cook(const T& initialState);

  private:
    double _temperature{0.f};
    ErrorFunc _errorFunc{};
    NeighbourFunc _neighbourFunc{};
};

/*************/
template <class T>
T Annealer<T>::cook(const T& initialState)
{
    auto currentState = initialState;
    auto error = _errorFunc(currentState);
}

} // namespace bluenoise
