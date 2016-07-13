#include "Response.h"

#ifndef PAID_VERSION
#include <libeng/Tools/Tools.h>

#define MAX_SCALE               0.8f
#define SCALE_VELOCITY          ((ALPHA_VEL_A * MAX_SCALE) / MAX_ALPHA)
#define DELTA_REF               14068L
#define ROTATION_CORRECTION     ((-52.5f * PI_F) / 180.f)
#define VELOCITY                7.f // Increase this value will decrease speed changes (see use)

//#ifdef __ANDROID__
#define ACCEL_LIMITS_SUP        9.80665f // Android: = 'mAccelSensor.getMaximumRange' / 2  (Samsung Galaxy Tab2)
#define ROTATION_PRECISION      2.f
//#else // iOS
//#endif
#define ACCEL_LIMITS_INF        -ACCEL_LIMITS_SUP
// Accelerometer limits when used in a normal way (without shaking the device)

#define ORIENTATION_SUP         (ACCEL_LIMITS_SUP / 2.f)
#define ORIENTATION_INF         -ORIENTATION_SUP

#define ACCEL_PRECISION         0.2f // Avoid to rotate when the device do not move (too specific)
#define ACCEL_CHANGED(XYZ, xyz) (((XYZ - ACCEL_PRECISION) > xyz) || ((XYZ + ACCEL_PRECISION) < xyz))
#define ACCEL_CLIP(xyz)         ((xyz > ACCEL_LIMITS_SUP)? ACCEL_LIMITS_SUP:(xyz < ACCEL_LIMITS_INF)? ACCEL_LIMITS_INF:xyz)
#define ACCEL_TO_ROTATION       (PI_F / ((4.f + ROTATION_PRECISION) * ORIENTATION_INF))

// Texture IDs
#define TEXTURE_ID_TITLE        3
#define TEXTURE_ID_STORES       5

//////
Response::Response() : Sphere(false), mToRender(false), mToDisplay(0), mCancel(false), mForce(false), mScale(0.f),
        mOrientation(0), mRotateVel(0.f), mRotation(0.f) {

    LOGV(LOG_LEVEL_RESPONSE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    std::memset(mTexIdx, 0, TEXIDX_BUFFER_SIZE);
    std::memset(&mCurAccel, 0, sizeof(AccelData));

    mFullTexCoords = mTexCoords;
}
Response::~Response() {

    LOGV(LOG_LEVEL_RESPONSE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    for (unsigned char i = 0; i < TEXIDX_BUFFER_SIZE; ++i)
        if (mTexIdx[i] != 0)
            mTextures->delTexture(mTexIdx[i]);
}

void Response::start() {

    LOGV(LOG_LEVEL_RESPONSE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    std::memcpy(mTransform, Object2D::TransformBuffer, sizeof(float) * TRANSFORM_BUFFER_SIZE);

    // Make a correction on indices (that start from -50°)
    rotate(ROTATION_CORRECTION); // => -2.5° for texture coordinates correction (that start from -2.5°)
}
void Response::load(unsigned char responseId) {

    LOGV(LOG_LEVEL_RESPONSE, 0, LOG_FORMAT(" - r:%d"), __PRETTY_FUNCTION__, __LINE__, responseId);
    switch (responseId) {

        case RESPONSE_ID_TITLE:     mTexIdx[0] = Textures::loadTexture(TEXTURE_ID_TITLE); break;
        case RESPONSE_ID_AVAILABLE: mTexIdx[1] = Textures::loadTexture(TEXTURE_ID_AVAILABLE); break;
        case RESPONSE_ID_STORES:    mTexIdx[2] = Textures::loadTexture(TEXTURE_ID_STORES); break;
#ifdef DEBUG
        default: {

            LOGW(LOG_FORMAT(" - Unknown response item to load (%d)"), __PRETTY_FUNCTION__, __LINE__, responseId);
            assert(NULL);
            return;
        }
#endif
    }
    assert(mTexIdx[responseId] != 0);
    mTextures->genTexture(mTexIdx[responseId]);

    if (mToDisplay == responseId) // Needed when resume
        mTextureIdx = mTexIdx[responseId];
}

void Response::update(const Game* game, const Level* level) {

#ifdef DEBUG
    LOGV(LOG_LEVEL_RESPONSE, (*game->getLog() % 100), LOG_FORMAT(" - g:%x; l:%x (r:%s)"), __PRETTY_FUNCTION__, __LINE__, game,
            level, (mToRender)? "true":"false");
#endif
    if (!mToRender)
        return;

    // Rotation according device position
    if (game->mAccelData) {

#ifdef __ANDROID__
#ifdef DEBUG
        const Input* accelInput = Inputs::getInstance()->get(Inputs::ACCEL);
        assert((*static_cast<const float*>(accelInput->getConfig())) < ((ACCEL_LIMITS_SUP * 2.f) + 1.f));
        assert((*static_cast<const float*>(accelInput->getConfig())) > ((ACCEL_LIMITS_SUP * 2.f) - 1.f));
#endif
        float x = ACCEL_CLIP(game->mAccelData->X);
        float y = ACCEL_CLIP(game->mAccelData->Y);
        float z = ACCEL_CLIP(game->mAccelData->Z);

#else
        // No maximum accelerometer value available on iOS
        // -> Using a common range of [-11;11] values (after having * 10)

        float x = ACCEL_CLIP(-game->mAccelData->X * 10.f);
        float y = ACCEL_CLIP(-game->mAccelData->Y * 10.f);
        float z = ACCEL_CLIP(-game->mAccelData->Z * 10.f);
#endif
        unsigned char orientation = 0;
        if ((ORIENTATION_INF < z) && (z < ORIENTATION_SUP)) {

            if ((ORIENTATION_INF < x) && (x < ORIENTATION_SUP)) orientation = (ORIENTATION_SUP < y)? PORTRAIT:REV_PORTRAIT;
            else if ((ORIENTATION_INF < y) && (y < ORIENTATION_SUP)) orientation = (ORIENTATION_SUP < x)? LANDSCAPE:REV_LANDSCAPE;
        }
        if ((!orientation) && (ORIENTATION_INF < x) && (x < ORIENTATION_SUP) &&
                (ORIENTATION_INF < y) && (y < ORIENTATION_SUP)) orientation = (ORIENTATION_SUP < z)? FLAT:REVERSED;

        unsigned char prevOrientation = mOrientation;
        if (orientation)
            mOrientation = orientation;

        switch (mOrientation) {
            default: {

                mOrientation = PORTRAIT;
                //break;
            }
            case PORTRAIT: { // Portrait (home button at bottom)

                if (prevOrientation == LANDSCAPE) {

                    mRotation -= (2.f * PI_F);
                    mRotateVel -= (2.f * PI_F);
                }
                else if (ACCEL_CHANGED(mCurAccel.X, x))
                    mRotateVel = x * ACCEL_TO_ROTATION;
                break;
            }
            case REV_PORTRAIT: { // Reversed portrait

                if (ACCEL_CHANGED(mCurAccel.X, x))
                    mRotateVel = (x * -ACCEL_TO_ROTATION) + PI_F;
                break;
            }
            case LANDSCAPE: { // Landscape (home button at right)

                if (prevOrientation == PORTRAIT) {

                    mRotation += (2.f * PI_F);
                    mRotateVel += (2.f * PI_F);
                }
                else if (ACCEL_CHANGED(mCurAccel.Y, y))
                    mRotateVel = (y * -ACCEL_TO_ROTATION) + ((3.f * PI_F) / 2.f);
                break;
            }
            case REV_LANDSCAPE: { // Reversed landscape

                if (ACCEL_CHANGED(mCurAccel.Y, y))
                    mRotateVel = (y * ACCEL_TO_ROTATION) + (PI_F / 2.f);
                break;
            }
            case FLAT: // Flat
            case REVERSED: { // Reversed

                if ((!ACCEL_CHANGED(mCurAccel.X, x)) && (!ACCEL_CHANGED(mCurAccel.Y, y)))
                    break;

                static unsigned char prevPos = 0;
                if ((y > 0.f) && (x < 0.f)) {

                    mRotateVel = std::atan(-x / y);
                    prevPos = 0;
                }
                else if ((y < 0.f) && (x < 0.f)) {

                    mRotateVel = std::atan(y / x) + (PI_F / 2.f);
                    prevPos = 0;
                }
                else if ((y < 0.f) && (x > 0.f)) {

                    if (prevPos == 2)
                        mRotation += (2.f * PI_F);
                    mRotateVel = PI_F - std::atan(x / y);
                    prevPos = 1;
                }
                else if ((y > 0.f) && (x > 0.f)) { // 'else if' instead of 'else' coz 'y' must be > 0

                    if (prevPos == 1)
                        mRotation -= (2.f * PI_F);
                    mRotateVel = -std::atan(x / y);
                    prevPos = 2;
                }
                //else // 'x' & 'y' are equal to 0: Nothing to do
                break;
            }
        }
        if (ACCEL_CHANGED(mCurAccel.X, x)) mCurAccel.X = x;
        if (ACCEL_CHANGED(mCurAccel.Y, y)) mCurAccel.Y = y;
        if (ACCEL_CHANGED(mCurAccel.Z, z)) mCurAccel.Z = z;

        mRotation = mRotation - ((mRotation - mRotateVel) / VELOCITY);
        rotate(ROTATION_CORRECTION + mRotation);
    }

    // Hide & Display response
    if (mCancel) {

        if (getAlpha() < ALPHA_VEL_A) {

            mToDisplay = 0;
            mToRender = false;
            mCancel = false;
            mForce = false;
        }
        else
            setAlpha(getAlpha() - (ALPHA_VEL_A * game->getDelta() / DELTA_REF));

        if (mScale < MAX_SCALE) {

            mScale += (SCALE_VELOCITY * game->getDelta() / DELTA_REF);
            scale(mScale, mScale);
        }
        return;
    }

    if (mForce) {

        mScale = MAX_SCALE;
        //setAlpha(MAX_ALPHA); // Let caller manage this
        scale(MAX_SCALE, MAX_SCALE);
        return;
    }
    if (getAlpha() < ((mToDisplay == RESPONSE_ID_STORES)? 1.f:MAX_ALPHA)) {

        setAlpha(getAlpha() + (ALPHA_VEL_A * game->getDelta() / DELTA_REF));
        if (mScale < MAX_SCALE) {

            mScale += (SCALE_VELOCITY * game->getDelta() / DELTA_REF);
            scale(mScale, mScale);
        }
    }
}

#endif // !PAID_VERSION
