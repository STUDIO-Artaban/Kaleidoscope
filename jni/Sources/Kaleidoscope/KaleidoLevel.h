#ifndef KALEIDOLEVEL_H_
#define KALEIDOLEVEL_H_

#include "Global.h"

#include <libeng/Game/Level.h>
#include <libeng/Features/Camera/Camera.h>
#include <libeng/Advertising/Advertising.h>

#ifdef __ANDROID__
#include "Kaleidoscope/Kaleido2D.h"
#include "Kaleidoscope/Panel.h"
#include "Kaleidoscope/Command.h"

#else
#include "Kaleido2D.h"
#include "Panel.h"
#include "Command.h"

#endif

#define ALPHA_VEL_B         (1.f / DISPLAY_DELAY)

using namespace eng;

//////
class KaleidoLevel : public Level {

private:
    Camera* mCamera;
    Kaleido2D mKaleido2D;
#ifndef PAID_VERSION
    Advertising* mAdvertising;
#endif

    Static2D* mHideCam; // BUG: Needed to hide the camera preview surface (when it is displayed during the load)

    Element2D* mPalette;
    TouchArea mColorArea;
    TouchArea mGrayArea;

    Panel mPanel;
    Command mCommand;

    enum {

        DISP_COMMAND = 0,
        DISP_CHECK,
        DISP_UNCHECK,
        DISP_HIDDEN,
        DISP_SHOW,
        DISP_TIMER
    };
    unsigned char mDisplayID;
    unsigned char mCounter;

    // Background colors
    float mRedVel;
    float mGreenVel;
    float mBlueVel;

    float mRed;
    float mGreen;
    float mBlue;
    unsigned char* mPaletteBuffer;

    unsigned char* mPicBuffer;
    mutable bool mTakePic;

    short mPicWidth;
    short mPicHeight;
    std::string mPicFile;

#ifndef PAID_VERSION
    unsigned char mAdDelay;
    unsigned char mTouchDelay;
#else
    void photo(short width, short height);
#endif

public:
    KaleidoLevel(Game* game);
    virtual ~KaleidoLevel();

    //////
    void init() { }
    void pause();

    void wait(float millis) { }

protected:
    bool loaded(const Game* game);
public:
    bool update(const Game* game);

    void renderLoad() const;
    void renderUpdate() const;

};

#endif // KALEIDOLEVEL_H_
