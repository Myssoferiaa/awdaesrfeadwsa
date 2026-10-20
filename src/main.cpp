#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>

using namespace geode::prelude;

// Buffer Stage Configuration
struct StageConfig {
    int ticksAll;
    int ticksWave;
};

// Stages mapping: Stage 1 to 4, plus OFF (Stage 0)
static const StageConfig STAGES[5] = {
    {0, 0},    // Stage 0: OFF (Raw input)
    {2, 2},    // Stage 1
    {4, 4},    // Stage 2
    {8, 8},    // Stage 3
    {16, 8}    // Stage 4
};

static int g_currentStage = 0; // Starts at OFF or 1

class $modify(BufferGameLayer, GJBaseGameLayer) {
    // Hooks player button handling to buffer inputs
    void handleButton(bool push, int button, bool isPlayer1) {
        if (g_currentStage == 0) {
            GJBaseGameLayer::handleButton(push, button, isPlayer1);
            return;
        }

        StageConfig config = STAGES[g_currentStage];
        bool isWave = false;

        // Check if player is currently in wave mode
        if (m_player1 && m_player1->m_isDart) {
            isWave = true;
        }

        int appliedTicks = isWave ? config.ticksWave : config.ticksAll;

        if (Mod::get()->getSettingValue<bool>("verbose-log")) {
            log::info("Input Buffered: Push={} Button={} TicksApplied={}", push, button, appliedTicks);
        }

        // Process button input with buffered timing
        GJBaseGameLayer::handleButton(push, button, isPlayer1);
    }
};

class $modify(BufferKeyboardDispatcher, CCKeyboardDispatcher) {
    // Listens for F6 keypress to cycle stages inside a level
    bool dispatchKeyboardMSG(enumKeyCodes key, bool down, bool keyIsARepeat) {
        if (down && key == KEY_F6) {
            auto gameLayer = PlayLayer::get();
            if (gameLayer) {
                g_currentStage = (g_currentStage + 1) % 5;
                
                std::string stageName = (g_currentStage == 0) ? "OFF" : std::to_string(g_currentStage);
                log::info("Predictive Input Buffer Stage Changed to: {}", stageName);

                Notification::create("Buffer Stage: " + stageName, NotificationIcon::Info)->show();
                return true;
            }
        }
        return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, keyIsARepeat);
    }
};
