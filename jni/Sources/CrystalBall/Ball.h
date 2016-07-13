#ifndef BALL_H_
#define BALL_H_

#include "Global.h"
#ifndef PAID_VERSION

#ifdef __ANDROID__
#include "CrystalBall/Sphere.h"
#else
#include "Sphere.h"
#endif
#include <libeng/Graphic/Object/2D/Static2D.h>
#include <libeng/Tools/Tools.h>

#define MAX_BALL_COLOR      0.8f // Maximum ball color ratio

#define FAST_VELOCITY       2
#define SLOW_VELOCITY       20

using namespace eng;

//////
class Ball : public Sphere {

private:
    Delay mDelay;

    Static2D* mLight;
    unsigned char mLightIdx;

public:
    Ball();
    virtual ~Ball();

    inline void reset() { mLightIdx = 0; }
    unsigned char lighten(clock_t delta); // Return 0 when done

    //////
    void start(const Game* game);
    inline void stop() { // pause

        LOGV(LOG_LEVEL_BALL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
        Object2D::pause();

        if (mLight) mLight->pause();
    }
    void resume();

    void render() const;

};

#endif // !PAID_VERSION
#endif // BALL_H_
