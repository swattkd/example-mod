#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/CCScheduler.hpp>

using namespace geode::prelude;

// --- HELPER FUNCTION ---
// Checks if the level gives stars and if the user wants Safe Mode on.
bool isSafeModeActive() {
    auto pl = PlayLayer::get();
    if (!pl || !pl->m_level) return false;
    
    bool masterSwitch = Mod::get()->getSettingValue<bool>("auto-safe-mode");
    // If stars > 0 and safe mode is toggled on, return true (cheats will be blocked)
    return (masterSwitch && pl->m_level->m_stars > 0);
}

// --- NOCLIP FEATURE ---
class $modify(PlayerObject) {
    void destroyPlayer(PlayerObject* player, GameObject* object) {
        bool noclipEnabled = Mod::get()->getSettingValue<bool>("noclip");

        // If noclip is on and Safe Mode is NOT blocking us, ignore the death
        if (noclipEnabled && !isSafeModeActive()) {
            return; 
        }

        // Otherwise, die as normal
        PlayerObject::destroyPlayer(player, object);
    }
};

// --- SPEEDHACK & TPS BYPASS FEATURE ---
class $modify(CCScheduler) {
    void update(float dt) {
        bool safeActive = isSafeModeActive();
        
        // Handle Speedhack
        double speed = Mod::get()->getSettingValue<double>("speedhack");
        float finalDt = dt;

        // Only apply speedhack if safe mode isn't active
        if (!safeActive) {
            finalDt *= static_cast<float>(speed);
        }

        // Handle TPS Bypass (Physics)
        bool tpsEnabled = Mod::get()->getSettingValue<bool>("tps-bypass");
        if (tpsEnabled && !safeActive) {
            // Apply 240 ticks per second physics
            CCScheduler::update(1.0f / 240.0f);
        } else {
            // Apply normal physics (with speedhack if active)
            CCScheduler::update(finalDt);
        }
    }
};

// --- GUI LOGIC ---
class $modify(PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontSave) {
        if (!PlayLayer::init(level, useReplay, dontSave)) return false;
        
        // This makes sure the mod initializes correctly when you start a level
        return true;
    }
};
