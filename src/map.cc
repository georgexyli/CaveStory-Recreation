#include "map.h"
#include "game.h"
#include "sprite.h"
#include "rectangle.h"

namespace{
const std::string kCaveFile{"content/PrtCave.pbm"};
const std::string kBackdropFile{"content/bkBlue.pbm"};
}

Map* Map::createTestMap(Graphics& graphics){
    Map* map = new Map{};
    map -> backdrop_ = std::make_unique<FixedBackdrop>(kBackdropFile, graphics);
    const units::Tile num_rows{15};
    const units::Tile num_cols{20};

    map -> background_tiles_ = std::vector<std::vector<std::shared_ptr<Sprite>>>(num_rows,
            std::vector<std::shared_ptr<Sprite>>(num_cols, nullptr));

    map -> tiles_ = std::vector<std::vector<Tile>>(num_rows,
            std::vector<Tile>(num_cols, Tile()));

    auto sprite = std::make_shared<Sprite>(
            graphics,
            kCaveFile, 
            units::tileToPixel(1), 
            units::tileToPixel(0), 
            units::tileToPixel(1),
            units::tileToPixel(1)); 
    
    const units::Tile row {11};
    Tile tile{TileType::WALL_TILE, sprite};
    for (units::Tile col{}; col < num_cols; ++col){
        map -> tiles_[row][col] = tile;
    }
    map -> tiles_[10][5] = tile;
    map -> tiles_[9][4] = tile;
    map -> tiles_[8][3] = tile;
    map -> tiles_[10][3] = tile;
    map -> tiles_[7][2] = tile;

    auto chain_top = std::make_shared<Sprite>(
            graphics,
            kCaveFile, 
            units::tileToPixel(11), 
            units::tileToPixel(2), 
            units::tileToPixel(1), 
            units::tileToPixel(1)); 

    auto chain_middle = std::make_shared<Sprite>(
            graphics,
            kCaveFile, 
            units::tileToPixel(12), 
            units::tileToPixel(2), 
            units::tileToPixel(1), 
            units::tileToPixel(1)); 


    auto chain_bottom = std::make_shared<Sprite>(
            graphics,
            kCaveFile, 
            units::tileToPixel(13), 
            units::tileToPixel(2), 
            units::tileToPixel(1), 
            units::tileToPixel(1)); 

    map -> background_tiles_[8][2] = chain_top;
    map -> background_tiles_[9][2] = chain_middle;
    map -> background_tiles_[10][2] = chain_bottom;
    return map;
}

std::vector<Map::CollisionTile> Map::getCollidingTiles(const Rectangle& rectangle) const {
    const units::Tile first_row = units::gameToTile(rectangle.top());
    const units::Tile last_row = units::gameToTile(rectangle.bottom());
    const units::Tile first_col = units::gameToTile(rectangle.left());
    const units::Tile last_col = units::gameToTile(rectangle.right());

    std::vector<CollisionTile> collisionTiles;

    for (units::Tile row{first_row}; row <= last_row; ++row){
        for (units::Tile col{first_col}; col <= last_col; ++col){
            collisionTiles.emplace_back(row, col, tiles_[row][col].tile_type);
        }
    }
    return collisionTiles;
}

void Map::update(units::MS elapsed_time_ms) {
    for (auto& row : tiles_){
        for (auto& tile : row){
            if (tile.sprite) {
                tile.sprite -> update(elapsed_time_ms);
            }
        }
    }
}

void Map::draw(Graphics& graphics) const {
    for (units::Tile row{}; row < tiles_.size(); ++row){
        for (units::Tile col{}; col < tiles_[row].size(); ++col){
            if (tiles_[row][col].sprite){
                tiles_[row][col].sprite -> draw(graphics, units::tileToGame(col), units::tileToGame(row));
            }
        }
    }
}

void Map::drawBackground(Graphics& graphics) const{
    backdrop_ -> draw(graphics);
    for (units::Tile row{}; row < background_tiles_.size(); ++row){
        for (units::Tile col{}; col < background_tiles_[row].size(); ++col){
            if (background_tiles_[row][col]){
                background_tiles_[row][col] -> draw(graphics, units::tileToGame(col), units::tileToGame(row));
            }
        }
    }
}