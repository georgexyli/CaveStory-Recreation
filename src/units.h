#ifndef UNITS_H_
#define UNITS_H_

#include<cmath>

namespace units{

using Game = float; // Float for extra precision, intrinsic units of position
using Pixel = int; // Integer for discrete units. Pixel units can be +/-
using Tile = unsigned int; // Also discrete, but non-negative
using Frame = unsigned int; // Discrete non-negative
using Degrees = float; 
using Radians = float;

using MS = unsigned int; // Discrete Milliseconds to match SDL
using FPS = unsigned int; // frames oer second (Hz or 1 / second)

using Velocity = float; // Game / MS 
using Accelaration = float; //Game / MS / MS
using AngularVelocity = float; // Degree / MS

namespace{
const Game kTileSize{32.0f};
const double kPi = atan(1) * 4;
}

inline Pixel gameToPixel(Game game){
    return Pixel(round(game / 2));
}

inline Tile gameToTile(Game game){
    return Tile(game / kTileSize);
}

inline Game tileToGame(Tile tile){
    return Game(tile * kTileSize);
}

inline Pixel tileToPixel(Tile tile){
    return gameToPixel(tileToGame(tile));
}

inline double degreesToRadians(Degrees degrees){
    return degrees * kPi / 180.0f;
}

const Game kHalfTile{tileToGame(1) / 2.0f};

};

#endif // UNITS_H_