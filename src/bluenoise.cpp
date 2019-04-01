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

#include <iostream>
#include <random>

#include "./annealer.h"
#include "./pattern.h"

using namespace bluenoise;

int main(int, char**)
{
    const size_t size = 16;
    const size_t dims = 1;
    const double tmax = 10000.0;

    using Pat = Pattern<size, dims>;
    using Ann = Annealer<Pat>;

    Ann::ErrorFunc errorFunc = [](const Pat& pattern) -> double { return pattern.getEnergy(); };
    Ann::NeighbourFunc neighbourFunc = [](const Pat& pattern) -> Pat
    {
        static std::random_device rdevice;
        static std::mt19937 rgen(rdevice());
        static std::uniform_int_distribution<size_t> rdist(0, size - 1);

        auto otherPattern = pattern;
        auto xi = rdist(rgen);
        auto xj = rdist(rgen);
        auto yi = rdist(rgen);
        auto yj = rdist(rgen);

        otherPattern(xi, yi) = pattern(xj, yj);
        otherPattern(xj, yj) = pattern(xi, yi);

        return otherPattern;
    };

    Ann annealer(tmax, errorFunc, neighbourFunc);
    Pat initialPattern;
    auto finalPattern = annealer.cook(initialPattern);

    // Save the image to disk
    initialPattern.saveToFile("whitenoise.png");
    initialPattern.saveFourier("bluenoise_fourier.png");
    finalPattern.saveToFile("bluenoise.png");
    finalPattern.saveFourier("whitenoise_fourier.png");

    return 0;
}
