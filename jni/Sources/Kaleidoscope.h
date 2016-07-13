#ifndef KALEIDOSCOPE_H_
#define KALEIDOSCOPE_H_

#include <libeng/Game/2D/Game2D.h>
#include <libeng/Intro/Intro.h>

#include "Global.h"

#ifdef __ANDROID__
#ifndef PAID_VERSION
#include "CrystalBall/CrystalBall.h"
#include "WitchSPet/WitchSPet.h"
#endif
#include "Kaleidoscope/KaleidoLevel.h"

#else
#ifndef PAID_VERSION
#include "CrystalBall.h"
#include "WitchSPet.h"
#endif
#include "KaleidoLevel.h"

#endif

#define FILE_NAME       "Kaleidoscope.backup"

using namespace eng;

//////
class Kaleidoscope : public Game2D {

private:
    Kaleidoscope();
    virtual ~Kaleidoscope();

#ifndef PAID_VERSION
    CrystalBall* mCrystalBall;
    WitchSPet* mWitchSPet;
#endif

    Static2D* mKaleidoTitle;
    KaleidoLevel* mKaleido;
    unsigned char mTitleStep;

#ifndef PAID_VERSION
    WaitNet* mWaitNet;
#endif

public:
    static Kaleidoscope* getInstance() {
        if (!mThis)
            mThis = new Kaleidoscope;
        return static_cast<Kaleidoscope*>(mThis);
    }
    static void freeInstance() {
        if (mThis) {
            delete mThis;
            mThis = NULL;
        }
    }

    //////
    void init() { }

    bool start();
    void pause();
    void destroy();

    void wait(float millis);
    void update();

    void render() const;

};

#endif // KALEIDOSCOPE_H_
