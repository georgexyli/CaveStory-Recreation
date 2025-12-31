#ifndef UNITS_H_
#define UNITS_H_

#include<cmath>
#include <chrono>
#include <numbers>
#include "config.h"

namespace units{

using HP = int;

using Game = float; // Float for extra precision, intrinsic units of position
using Pixel = int; // Integer for discrete units. Pixel units can be +/-
using Tile = unsigned int; // Also discrete, but non-negative
using Frame = unsigned int; // Discrete non-negative
using Degrees = float; 
using Radians = float;

using TimePoint = std::chrono::steady_clock::time_point; // Timepoint for measuring start and end of time intervals
using MS = std::chrono::milliseconds; // For measuring elapsed time (difference between two timepoints)
using FPS = unsigned int; // frames oer second (Hz or 1 / second)

using Velocity = float; // Game / MS 
using Accelaration = float; //Game / MS / MS
using AngularVelocity = float; // Degree / MS

namespace{
inline constexpr Game kTileSize{32.0f};
inline constexpr double kPi{std::numbers::pi};
}

inline constexpr Pixel gameToPixel(Game game){
    return config::getGraphicsQuality() == config::GraphicsQuality::HIGH_QUALITY ?
            Pixel(round(game)) :
            Pixel(round(game / 2));
}

inline constexpr Tile gameToTile(Game game){
    return Tile(game / kTileSize);
}

inline constexpr Game tileToGame(Tile tile){
    return Game(tile * kTileSize);
}

inline constexpr Pixel tileToPixel(Tile tile){
    return gameToPixel(tileToGame(tile));
}

inline double degreesToRadians(Degrees degrees){
    return degrees * kPi / 180.0f;
}

inline constexpr Game kHalfTile{tileToGame(1) / 2.0f};

};

#endif // UNITS_H_
