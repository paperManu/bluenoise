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
#include <iostream>
#include <random>

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
     * \param kMax Maximum iteration count
     * \param efunc Error function
     * \param nfunc Neighbour generation function
     */
    Annealer(size_t kMax, const ErrorFunc& eFunc, const NeighbourFunc& nfunc) : _kMax(kMax), _errorFunc(eFunc), _neighbourFunc(nfunc) {}

    /**
     * Apply simulated annealing from the given initial state
     * \param initialState Initial state
     * \return Return the optimized state
     */
    T cook(const T& initialState);

  private:
    size_t _kMax{1000};
    double _eMax{1e-3};
    ErrorFunc _errorFunc{};
    NeighbourFunc _neighbourFunc{};

    double iterToTemp(size_t iter) const { return static_cast<double>(_kMax - iter); }
};

/*************/
template <class T>
T Annealer<T>::cook(const T& initialState)
{
    auto currentState = initialState;
    auto bestState = initialState;

    auto currentError = _errorFunc(currentState);
    auto bestError = _errorFunc(bestState);

    std::random_device rdevice;
    std::mt19937 rgen(rdevice());
    std::uniform_real_distribution<float> rdist(0.f, 1.f);

    for (size_t k = 0; k < _kMax && currentError > _eMax; ++k)
    {
        auto newState = _neighbourFunc(currentState);
        auto newError = _errorFunc(newState);

        std::cout << "Current best error: " << bestError << " " << std::exp((currentError - newError) / iterToTemp(k)) << "\n";
        if (newError < currentError || rdist(rgen) < std::exp((currentError - newError) / iterToTemp(k)))
        {
            currentState = newState;
            currentError = newError;
        }

        if (currentError < bestError)
        {
            bestState = currentState;
            bestError = currentError;
        }
    }

    return bestState;
}

} // namespace bluenoise
