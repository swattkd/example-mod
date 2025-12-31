#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(PlayerObject) {
    void destroyPlayer(PlayerObject* p0, GameObject* p1) {
        if (Mod::get()->getSettingValue<bool>("noclip")) {
            return;
        }
        PlayerObject::destroyPlayer(p0, p1);
    }
};
