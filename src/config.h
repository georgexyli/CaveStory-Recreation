#ifndef CONFIG_H_
#define CONFIG_H_

namespace config{
enum class GraphicsQuality{
    HIGH_QUALITY,
    ORIGINAL_QUALITY
};

inline constexpr GraphicsQuality getGraphicsQuality(){
    return GraphicsQuality::ORIGINAL_QUALITY;
}
};

#endif