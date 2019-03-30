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

#include <array>
#include <cassert>
#include <random>
#include <type_traits>

namespace bluenoise
{

/**
 * Noise pattern, basically an image
 */
template <size_t size, size_t dims = 1>
class Pattern
{
  public:
    class ConstView;
    class View
    {
        friend ConstView;

      public:
        View(float* ptr)
        {
            for (size_t i = 0; i < dims; ++i)
                _data[i] = &ptr[i];
        }

        float& operator[](size_t index) { return *_data[index]; }

        View& operator=(const View& rhs)
        {
            if (this == &rhs)
                return *this;

            for (size_t i = 0; i < dims; ++i)
                *_data[i] = *rhs._data[i];

            return *this;
        }
        View& operator=(const ConstView& rhs)
        {
            for (size_t i = 0; i < dims; ++i)
                *_data[i] = *rhs._data[i];

            return *this;
        }

      protected:
        std::array<float*, dims> _data;
    };

    class ConstView
    {
        friend View;

      public:
        ConstView(const float* ptr)
        {
            for (size_t i = 0; i < dims; ++i)
                _data[i] = &ptr[i];
        }

        const float& operator[](size_t index) { return *_data[index]; }

      protected:
        std::array<const float*, dims> _data;
    };

  public:
    /**
     * Constructor
     */
    Pattern()
    {
        assert(size != 0);
        assert(dims != 0);

        std::random_device rdevice;
        std::mt19937 rgen(rdevice());
        std::uniform_real_distribution<float> rdist(0.f, 1.f);
        for (size_t i = 0; i < _count; ++i)
            _data[i] = rdist(rgen);
    }

    View operator()(const size_t x, const size_t y)
    {
        assert(x < size);
        assert(y < size);
        View value(&_data[(y * size + x) * dims]);
        return value;
    }
    ConstView operator()(const size_t x, const size_t y) const
    {
        assert(x < size);
        assert(y < size);
        ConstView value(&_data[(y * size + x) * dims]);
        return value;
    }

    /**
     * Get the pattern's energy as detailed in "Blue-noise Dithered Sampling", Iliyan et a.
     * \param pattern Pattern to compute the energy from
     * \param sigma_i Sigma_i, per the article
     * \param sigma_s Sigma_s, per the article
     * \return Return the energy
     */
    float getEnergy(const float sigma_i = 2.1f, const float sigma_s = 1.f) const
    {
        const float sqSigma_i = sigma_i * sigma_i;
        const float sqSigma_s = sigma_s * sigma_s;

        float energy = 0.f;
        for (size_t yi = 0; yi < size; ++yi)
        {
            for (size_t xi = 0; xi < size; ++xi)
            {
                const float* ps = &_data[(yi * size + xi) * dims];
                for (size_t yj = 0; yj < size; ++yj)
                {
                    for (size_t xj = 0; xj < size; ++xj)
                    {
                        if (xi == xj && yi == yj)
                            continue;

                        const float* qs = &_data[(yj * size + xj) * dims];
                        float sqValueDist = 0.f;
                        for (size_t c = 0; c < dims; ++c)
                            sqValueDist += pow(ps[c] - qs[c], 2.f);

                        float localEnergy = -static_cast<float>(std::pow(xi - xj, 2) + std::pow(yi - yj, 2)) / sqSigma_i;
                        localEnergy -= std::pow(sqValueDist, static_cast<float>(dims) / 2.0) / sqSigma_s;
                        energy += std::exp(localEnergy);
                    }
                }
            }
        }

        return energy;
    }

  private:
    static constexpr size_t _count = size * size * dims;
    std::array<float, _count> _data{};
};

} // namespace bluenoise
