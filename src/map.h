#ifndef MAP_H_
#define MAP_H_

#include <memory>
#include <vector>
#include "backdrop.h"
#include "units.h"

class Rectangle;
class Graphics;
class Sprite;

class Map {
    public:

        enum class TileType{
            AIR_TILE,
            WALL_TILE
        };

        struct CollisionTile{
            CollisionTile(units::Tile row, units::Tile col, TileType tile_type) :
            row{row}, col{col}, tile_type{tile_type}{}

            TileType tile_type;
            units::Tile row;
            units::Tile col;
        };

        void drawBackground(Graphics & graphics) const;
        void draw(Graphics& graphics) const;

        std::vector<CollisionTile> getCollidingTiles(const Rectangle& rectangle) const;
        
        static Map* createTestMap(Graphics& graphics);
        
    private:
        struct Tile {
            Tile(TileType tile_type = TileType::AIR_TILE, 
                    std::shared_ptr<Sprite> sprite = nullptr) :
                    tile_type{tile_type}, sprite{sprite}{} 
            TileType tile_type;
            std::shared_ptr<Sprite> sprite;
        };
        std::vector<std::vector<std::shared_ptr<Sprite>>> background_tiles_;
        std::vector<std::vector<Tile>> tiles_;
        std::unique_ptr<Backdrop> backdrop_;
};

#endif // MAP_H_