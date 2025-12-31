#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

bool isSafeModeActive() {
    auto pl = PlayLayer::get();
    if (!pl || !pl->m_level) return false;
    return (Mod::get()->getSettingValue<bool>("auto-safe-mode") && pl->m_level->m_stars > 0);
}

class $modify(PlayerObject) {
    void destroyPlayer(PlayerObject* player, GameObject* object) {
        if (Mod::get()->getSettingValue<bool>("noclip") && !isSafeModeActive()) {
            return;
        }
        PlayerObject::destroyPlayer(player, object);
    }
};
