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

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

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
     * Get a pointer to the data
     * \return Return a pointer to the data
     */
    float* data() const { return _data.data(); }

    /**
     * Generate the Fourier transform for this pattern, and save it
     * \param filename Path to save the Fourier transform to
     */
    void saveFourier(const std::string& filename) const
    {
        const double pi2 = 2.0 * M_PI;
        const double invS = 1.0 / static_cast<double>(size);
        double angle, cosA;

        // FT along X
        std::array<double, _count> fourierX;
        for (size_t y = 0; y < size; ++y)
        {
            for (size_t x = 0; x < size; ++x)
            {
                for (size_t c = 0; c < dims; ++c)
                {
                    auto index = ((y * size) + x) * dims + c;
                    fourierX[index] = 0.f;

                    for (size_t xx = 0; xx < size; ++xx)
                    {
                        angle = pi2 * x * xx * invS;
                        cosA = cos(angle);
                        fourierX[index] += _data[((y * size) + xx) * dims + c] * cosA;
                    }
                    fourierX[index] *= invS;
                }
            }
        }

        // FT along Y
        std::array<double, _count> fourierY;
        for (size_t x = 0; x < size; ++x)
        {
            for (size_t y = 0; y < size; ++y)
            {
                for (size_t c = 0; c < dims; ++c)
                {
                    auto index = ((y * size) + x) * dims + c;
                    fourierY[index] = 0.f;

                    for (size_t yy = 0; yy < size; ++yy)
                    {
                        angle = pi2 * y * yy * invS;
                        cosA = cos(angle);
                        fourierY[index] += fourierX[((y * size) + yy) * dims + c] * cosA;
                    }
                    fourierY[index] *= invS;
                }
            }
        }

        // Combine both dimensions
        std::array<double, _count> fourier2D;
        double maxValue = 0.f;
        for (size_t y = 0; y < size; ++y)
        {
            for (size_t x = 0; x < size; ++x)
            {
                for (size_t c = 0; c < dims; ++c)
                {
                    auto index = ((y * size) + x) * dims + c;
                    fourier2D[index] = fourierX[((y * size) + x) * dims + c];
                    fourier2D[index] *= fourierY[((y * size) + x) * dims + c];
                    maxValue = std::max(maxValue, fourier2D[index]);
                }
            }
        }

        // Save to file
        std::array<uint8_t, _count> imgData;
        for (size_t y = 0; y < size; ++y)
            for (size_t x = 0; x < size; ++x)
                for (size_t c = 0; c < dims; ++c)
                    imgData[(y * size) + x] = static_cast<uint8_t>(255.0 / maxValue * fourier2D[((y * size) + x) * dims + c]);
        stbi_write_png(filename.c_str(), size, size, 1, imgData.data(), size);
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

    /**
     * Save the pattern to disk, as an 8bpc image
     * \param filename Path to save the pattern to
     * \return Return true if all went well
     */
    bool saveToFile(const std::string& filename)
    {
        std::array<uint8_t, _count> imgData;
        for (size_t y = 0; y < size; ++y)
            for (size_t x = 0; x < size; ++x)
                for (size_t c = 0; c < dims; ++c)
                    imgData[(y * size) + x] = static_cast<uint8_t>(255.f * _data[((y * size) + x) * dims + c]);
        return stbi_write_png(filename.c_str(), size, size, 1, imgData.data(), size);
    }

  private:
    static constexpr size_t _count = size * size * dims;
    std::array<float, _count> _data{};
};

} // namespace bluenoise
