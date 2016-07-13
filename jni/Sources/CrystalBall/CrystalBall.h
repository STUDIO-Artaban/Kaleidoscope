#ifndef CRYSTALBALL_H_
#define CRYSTALBALL_H_

#include "Global.h"
#ifndef PAID_VERSION

#include <libeng/Game/Level.h>
#include <libeng/Graphic/Progress/Progress.h>
#include <libeng/Graphic/Object/2D/Element2D.h>

#include <libeng/Features/Camera/Camera.h>

#ifdef __ANDROID__
#include "CrystalBall/Ball.h"
#include "CrystalBall/Sphere.h"
#include "CrystalBall/Response.h"

#else
#include "Ball.h"
#include "Sphere.h"
#include "Response.h"

#endif

using namespace eng;

//////
class CrystalBall : public Level {

private:
    Player* mPlayer;
    Camera* mCamera;
    Delay mDelay;

#ifdef __ANDROID__
    enum {

        COUNTRY_IDX_EN = 0,
        COUNTRY_IDX_FR,
        COUNTRY_IDX_DE,
        COUNTRY_IDX_ES,
        COUNTRY_IDX_PT,
        COUNTRY_IDX_IT
    };
    unsigned char mAppCountry;
#else
    NSString* mAppCountry;
#endif

    enum { // Advertising step

        WAIT_TITLE = 1,
        DISPLAYING_TITLE,
        TITLE_DISPLAYED,
        CANCELLING_TITLE,
        DISPLAYING_AVAILABLE,
        AVAILABLE_DISPLAYED,
        CANCELLING_AVAILABLE,
        DISPLAYING_STORES,
        STORES_DISPLAYED,
        CLOSE_DISPLAYED
    };
    unsigned char mStep;

    //
    Progress* mProgress;
    Static2D* mClose;

    TouchArea mCloseArea;

    Static2D* mBackground;
    Element2D* mCrystal;
    Response* mResponse;
    Ball* mBall;

    void reduceVolume(unsigned char track, clock_t delta);

public:
    CrystalBall(Game* game);
    virtual ~CrystalBall();

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

#endif // !PAID_VERSION
#endif // CRYSTALBALL_H_
