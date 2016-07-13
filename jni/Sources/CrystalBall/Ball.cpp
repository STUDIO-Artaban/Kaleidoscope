#include "Ball.h"
#include <libeng/Game/2D/Game2D.h>

#ifndef PAID_VERSION
#define LIGHT_BALL_COLOR        2.f
#define LIGHT_END_IDX           INCREASE_BUFFER_SIZE + 5

// Delay(s)
#ifdef DEBUG
#define DDELAY_OF_LIGHT         ((6 * 50) / INCREASE_BUFFER_SIZE) // 6 sceonds: 6 * 60|50 FPS / INCREASE_BUFFER_SIZE
#endif
#define DELAY_OF_LIGHT          50857.f

// Texture IDs
#define TEXTURE_ID_ENERGY       6

//////
Ball::Ball() : Sphere(true), mLightIdx(0) {

    LOGV(LOG_LEVEL_BALL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    mLight = NULL;
}
Ball::~Ball() {

    LOGV(LOG_LEVEL_BALL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    if (mLight) delete mLight;
}

unsigned char Ball::lighten(clock_t delta) {

    LOGV(LOG_LEVEL_BALL, 0, LOG_FORMAT(" - d:%u"), __PRETTY_FUNCTION__, __LINE__, delta);
#ifdef DEBUG
    if (!mDelay.isElapsed(delta, DDELAY_OF_LIGHT, DELAY_OF_LIGHT, __PRETTY_FUNCTION__, __LINE__))
#else
    if (!mDelay.isElapsed(delta, DELAY_OF_LIGHT))
#endif
        return (mLightIdx)? mLightIdx:1;

    ++mLightIdx;

    assert(mLight);
    if (mLightIdx < INCREASE_BUFFER_SIZE)
        mLight->setAlpha(IncreaseBuffer[mLightIdx]);
    else if (mLightIdx == LIGHT_END_IDX) {

        LOGI(LOG_LEVEL_BALL, 0, LOG_FORMAT(" - Lighten"), __PRETTY_FUNCTION__, __LINE__);
        return 0;
    }
    return mLightIdx;
}

void Ball::start(const Game* game) {

    LOGV(LOG_LEVEL_BALL, 0, LOG_FORMAT(" - g:%p"), __PRETTY_FUNCTION__, __LINE__, game);
    assert(Camera::getInstance()->getCamTexIdx() != TEXTURE_IDX_INVALID);

    Sphere::start(Camera::getInstance()->getCamTexIdx());

    mLight = new Static2D;
    mLight->initialize(game2DVia(game));
    mLight->start(TEXTURE_ID_ENERGY);
    mLight->setTexCoords(FULL_TEXCOORD_BUFFER);
    mLight->setVertices(1, (mScreen->height >> 1) + (mScreen->width >> 1) - 1, mScreen->width - 1,
            (mScreen->height >> 1) - (mScreen->width >> 1) + 1);
#ifndef __ANDROID__
    setBGRA(true);
#endif
}
void Ball::resume() {

    LOGV(LOG_LEVEL_BALL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    assert(Camera::getInstance()->getCamTexIdx() != TEXTURE_IDX_INVALID);

    Object2D::resume(Camera::getInstance()->getCamTexIdx());
    if (mLight) mLight->resume(TEXTURE_ID_ENERGY);
}

void Ball::render() const {

    glDisable(GL_BLEND);
    Sphere::render(true);
    glEnable(GL_BLEND);

    if (mLightIdx)
        mLight->render(false);
}

#endif // !PAID_VERSION
