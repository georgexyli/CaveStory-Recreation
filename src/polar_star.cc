#include "polar_star.h"
#include "map.h"
#include "particle_system.h"
#include "projectile_star_particle.h"
#include "projectile_wall_particle.h"
#include "sprite.h"
#include "gun_experience_hud.h"

namespace {
template <typename T>
using GunArray = std::array<T, units::kMaxGunLevel>;

const std::string kArmsFile{"Arms"};

const int kPolarStarIndex{2};
const units::Game kGunWidth{3 * units::kHalfTile};
const units::Game kGunHeight{2 * units::kHalfTile};

const units::Game kUpOffset{units::tileToGame(2)};
const units::Game kDownOffset{units::tileToGame(4)};
const units::Game kHorizontalOffset{0};

const units::Game kLeftOffset{0};
const units::Game kRightOffset{units::tileToGame(1)};

const units::Game kNozzleHorizontalY{23};
const units::Game kNozzleHorizontalLeftX{10};
const units::Game kNozzleHorizontalRightX{38};

const units::Game kNozzleUpY{4};
const units::Game kNozzleUpLeftX{27};
const units::Game kNozzleUpRightX{21};

const units::Game kNozzleDownY{28};
const units::Game kNozzleDownLeftX{29};
const units::Game kNozzleDownRightX{19};

const GunArray<units::Tile> kProjectileSourceYs{2, 2, 3};
const GunArray<units::Tile> kHorizontalProjectileSourceXs{8, 10, 8};

const units::Velocity kProjectileSpeed{0.6f};
const GunArray<units::Game> kProjectileMaxOffsets{7 * units::kHalfTile, 
    units::tileToGame(5), 
    units::tileToGame(7)};

const GunArray<units::Game> kProjectileWidths{4.0f, 8.0f, 16.0f};
const units::Game kProjectileHeight{32.0f};

const GunArray<units::HP> kDamages{1, 2, 4};
} // namespace

PolarStar::PolarStar(Graphics &graphics) : 
    current_level_{3}
{ initializeSprites(graphics); }

void PolarStar::updateProjectiles(units::MS elapsed_time, const Map &map,
                                  ParticleTools &particle_tools) {
  if (projectile_a_) {
    if (!projectile_a_->update(elapsed_time, map, particle_tools)) {
      projectile_a_.reset();
    }
  }
  if (projectile_b_) {
    if (!projectile_b_->update(elapsed_time, map, particle_tools)) {
      projectile_b_.reset();
    }
  }
}

void PolarStar::draw(Graphics &graphics, units::Game player_x,
                     units::Game player_y, HorizontalFacing horizontal_facing,
                     VerticalFacing vertical_facing, bool gun_up) {
  units::Game x{gun_x(horizontal_facing, player_x)};
  units::Game y{gun_y(vertical_facing, player_y, gun_up)};

  sprite_map_[std::make_tuple(horizontal_facing, vertical_facing)]->draw(
      graphics, x, y);
  if (projectile_a_)
    projectile_a_->draw(graphics);
  if (projectile_b_)
    projectile_b_->draw(graphics);
}

void PolarStar::drawHUD(Graphics& graphics, GunExperienceHUD& hud){
  hud.draw(graphics, current_level_, 2, 10);
}

units::Game PolarStar::gun_y(VerticalFacing vertical_facing,
                             units::Game player_y, bool gun_up) const {
  units::Game gun_y{player_y};
  if (vertical_facing == VerticalFacing::UP) {
    gun_y -= units::kHalfTile / 2;
  } else if (vertical_facing == VerticalFacing::DOWN) {
    gun_y += units::kHalfTile / 2;
  }
  if (gun_up)
    gun_y -= 2.0f;
  return gun_y;
}

void PolarStar::startFire(units::Game player_x, units::Game player_y,
                          HorizontalFacing horizontal_facing,
                          VerticalFacing vertical_facing, bool gun_up,
                          ParticleTools &particle_tools) {
  if (projectile_a_ && projectile_b_)
    return;
  units::Game bullet_x{gun_x(horizontal_facing, player_x) - units::kHalfTile};
  units::Game bullet_y{gun_y(vertical_facing, player_y, gun_up) -
                       units::kHalfTile};

  switch (vertical_facing) {
  case VerticalFacing::HORIZONTAL:
    bullet_y += kNozzleHorizontalY;
    if (horizontal_facing == HorizontalFacing::LEFT) {
      bullet_x += kNozzleHorizontalLeftX;
    } else {
      bullet_x += kNozzleHorizontalRightX;
    }
    break;
  case VerticalFacing::UP:
    bullet_y += kNozzleUpY;
    if (horizontal_facing == HorizontalFacing::LEFT) {
      bullet_x += kNozzleUpLeftX;
    } else {
      bullet_x += kNozzleUpRightX;
    }
    break;
  case VerticalFacing::DOWN:
    bullet_y += kNozzleDownY;
    if (horizontal_facing == HorizontalFacing::LEFT) {
      bullet_x += kNozzleDownLeftX;
    } else {
      bullet_x += kNozzleDownRightX;
    }
  default:
    break;
  }

  if (!projectile_a_) {
    projectile_a_ = std::make_shared<Projectile>(
        vertical_facing == VerticalFacing::HORIZONTAL ? 
            horizontal_projectiles_[current_level_ - 1] : 
            vertical_projectiles_[current_level_ - 1],
        horizontal_facing, vertical_facing, 
        bullet_x, bullet_y, 
        current_level_,
        particle_tools);
  } else {
    projectile_b_ = std::make_shared<Projectile>(
        vertical_facing == VerticalFacing::HORIZONTAL ? 
            horizontal_projectiles_[current_level_ - 1] : 
            vertical_projectiles_[current_level_ - 1],
        horizontal_facing, vertical_facing, 
        bullet_x, bullet_y, 
        current_level_,
        particle_tools);
  }
}

std::vector<std::shared_ptr<Projectile>> PolarStar::getProjectiles() {
  std::vector<std::shared_ptr<::Projectile>> projectiles;
  if (projectile_a_)
    projectiles.push_back(projectile_a_);
  if (projectile_b_)
    projectiles.push_back(projectile_b_);
  return projectiles;
}

void PolarStar::initializeSprites(Graphics &graphics) {
  for (units::GunLevel gun_level{0}; gun_level < units::kMaxGunLevel; ++gun_level){
    horizontal_projectiles_[gun_level] = std::make_shared<Sprite>(
        graphics, "Bullet", 
        units::tileToPixel(kHorizontalProjectileSourceXs[gun_level]),
        units::tileToPixel(kProjectileSourceYs[gun_level]), 
        units::tileToPixel(1),
        units::tileToPixel(1));
    vertical_projectiles_[gun_level] = std::make_shared<Sprite>(
        graphics, "Bullet", 
        units::tileToPixel(kHorizontalProjectileSourceXs[gun_level] + 1),
        units::tileToPixel(kProjectileSourceYs[gun_level]), 
        units::tileToPixel(1),
        units::tileToPixel(1));
  }

  ENUM_FOREACH(hf, HORIZONTAL_TYPE, HorizontalFacing) {
    ENUM_FOREACH(vf, VERTICAL_TYPE, VerticalFacing) {
      initializeSprite(graphics,
                       std::make_tuple(static_cast<HorizontalFacing>(hf),
                                       static_cast<VerticalFacing>(vf)));
    }
  }
}

void PolarStar::initializeSprite(Graphics &graphics,
                                 const SpriteState &sprite_state) {
  units::Game source_y{sprite_state.horizontal_facing() ==
                               HorizontalFacing::LEFT
                           ? kLeftOffset
                           : kRightOffset};
  switch (sprite_state.vertical_facing()) {
  case VerticalFacing::HORIZONTAL:
    source_y += kHorizontalOffset;
    break;
  case VerticalFacing::UP:
    source_y += kUpOffset;
    break;
  case VerticalFacing::DOWN:
    source_y += kDownOffset;
    break;
  default:
    break;
  }
  sprite_map_.emplace(
      sprite_state,
      std::make_shared<Sprite>(
          graphics, kArmsFile, units::gameToPixel(kPolarStarIndex * kGunWidth),
          units::gameToPixel(source_y), units::gameToPixel(kGunWidth),
          units::gameToPixel(kGunHeight)));
}

PolarStar::Projectile::Projectile(std::shared_ptr<Sprite> sprite,
                                  HorizontalFacing horizontal_direction,
                                  VerticalFacing vertical_direction,
                                  units::Game x, units::Game y,
                                  units::GunLevel gun_level,
                                  ParticleTools &particle_tools)
    : sprite_{sprite}, horizontal_direction_{horizontal_direction},
      vertical_dirction_{vertical_direction}, x_{x}, y_{y}, offset_{0},
      alive_{true}, gun_level_{gun_level} {
  particle_tools.front_system.addNewParticle(
      std::make_shared<ProjectileStarParticle>(particle_tools.graphics, x, y));
}

bool PolarStar::Projectile::update(units::MS elapsed_time, const Map &map,
                                   ParticleTools &particle_tools) {
  offset_ += elapsed_time.count() * kProjectileSpeed;
  std::vector<Map::CollisionTile> colliding_tiles{
      map.getCollidingTiles(collisionRectangle())};
  for (const auto &tile : colliding_tiles) {
    if (tile.tile_type == Map::TileType::WALL_TILE) {
      const Rectangle tile_rectangle{
          units::tileToGame(tile.col), units::tileToGame(tile.row),
          units::tileToGame(1), units::tileToGame(1)};

      units::Game particle_x, particle_y;
      switch (vertical_dirction_) {
      case VerticalFacing::HORIZONTAL:
        if (horizontal_direction_ == HorizontalFacing::LEFT) {
          particle_x = tile_rectangle.right() - units::kHalfTile;
        } else {
          particle_x = tile_rectangle.left() - units::kHalfTile;
        }
        particle_y = getY();
        break;
      case VerticalFacing::UP:
        particle_x = getX();
        particle_y = tile_rectangle.bottom() - units::kHalfTile;
        break;
      case VerticalFacing::DOWN:
        particle_x = getX();
        particle_y = tile_rectangle.top() - units::kHalfTile;
        break;
      default:
        break;
      }
      particle_tools.front_system.addNewParticle(
          std::make_shared<ProjectileWallParticle>(particle_tools.graphics,
                                                   particle_x, particle_y));
      return false;
    }
  }

  if (!alive_) {
    return false;
  } else if (offset_ >= kProjectileMaxOffsets[gun_level_ - 1]) {
    particle_tools.front_system.addNewParticle(
        std::make_shared<ProjectileStarParticle>(particle_tools.graphics,
                                                 getX(), getY()));
    return false;
  }
  return true;
}

Rectangle PolarStar::Projectile::collisionRectangle() const {
  const units::Game width{vertical_dirction_ == VerticalFacing::HORIZONTAL
                              ? units::tileToGame(1)
                              : kProjectileWidths[gun_level_- 1]};
  const units::Game height{vertical_dirction_ != VerticalFacing::HORIZONTAL
                               ? units::tileToGame(1)
                               : kProjectileWidths[gun_level_- 1]};
  return Rectangle{getX() + units::kHalfTile - width / 2,
                   getY() + units::kHalfTile - height / 2, width, height};
}

units::HP PolarStar::Projectile::contactDamage() const{
  return kDamages[gun_level_ - 1];
}


units::Game PolarStar::Projectile::getX() const {
  if (vertical_dirction_ == VerticalFacing::HORIZONTAL) {
    return x_ + (horizontal_direction_ == HorizontalFacing::LEFT ? -offset_
                                                                 : offset_);
  }
  return x_;
}

units::Game PolarStar::Projectile::getY() const {
  units::Game y{y_};
  switch (vertical_dirction_) {
  case VerticalFacing::UP:
    y -= offset_;
    break;
  case VerticalFacing::DOWN:
    y += offset_;
    break;
  default:
    break;
  }
  return y;
}

void PolarStar::Projectile::draw(Graphics &graphics) {
  sprite_->draw(graphics, getX(), getY());
}
