#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define USE_OPENAL
#define OLC_PGEX_SOUND
#include "olcPGEX_Sound.h"

#include <cstdlib>

const int WINDOW_WIDTH = 780;
const int WINDOW_HEIGHT = 480;

class Display : public olc::PixelGameEngine {

private:

    //TODO

public:

    Display() {
        //TODO
    }

    bool OnUserCreate() override {
        sAppName = "GeeBee";

        //TODO

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        //TODO
        return true;
    }

    bool OnUserDestroy() override {
        //TODO
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
