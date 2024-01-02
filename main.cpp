#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define USE_OPENAL
#define OLC_PGEX_SOUND
#include "olcPGEX_Sound.h"

#include "GameBoy.h"

const int WINDOW_WIDTH = 780;
const int WINDOW_HEIGHT = 480;

class Display : public olc::PixelGameEngine {

private:

    GameBoy* game_boy;

public:

    Display() {
        //TODO
    }

    bool OnUserCreate() override {
        sAppName = "GeeBee";

        game_boy = new GameBoy();

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        if (GetKey(olc::Key::ESCAPE).bPressed) {
            return false;
        }

        return true;
    }

    bool OnUserDestroy() override {
        delete game_boy;

        return true;
    }
};

int main(int argc, char* argv[]) {
    Display display;
    if (display.Construct(WINDOW_WIDTH, WINDOW_HEIGHT, 1, 1, true)) {
        display.Start();
    }

    return 0;
}
