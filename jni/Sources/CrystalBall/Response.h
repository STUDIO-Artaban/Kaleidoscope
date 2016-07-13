#ifndef RESPONSE_H_
#define RESPONSE_H_

#include "Global.h"
#ifndef PAID_VERSION

#ifdef __ANDROID__
#include "CrystalBall/Sphere.h"
#else
#include "Sphere.h"
#endif
#include <libeng/Inputs/Accel/Accel.h>

#define ALPHA_VEL_A         0.005f

// Response item IDs
enum {

    RESPONSE_ID_TITLE = 0,
    RESPONSE_ID_AVAILABLE,
    RESPONSE_ID_STORES

};
#define TEXIDX_BUFFER_SIZE  (RESPONSE_ID_STORES + 1)
#define MAX_ALPHA           0.6f

//////
class Response : public Sphere {

private:
    const float* mFullTexCoords; // Used to display the start message

    bool mToRender;

    unsigned char mToDisplay; // Response item ID to display (0: Nothing to display)
    bool mCancel; // TRUE: Cancel what is displayed; FALSE: Let's displaying (if any)
    bool mForce; // TRUE: Force to display content immediately; FALSE: Use alpha & scale velocity

    float mScale;

    enum {

        PORTRAIT = 1,
        REV_PORTRAIT,
        LANDSCAPE,
        REV_LANDSCAPE,
        FLAT,
        REVERSED
    };
    unsigned char mOrientation;
    AccelData mCurAccel;
    float mRotateVel; // Rotation velocity
    float mRotation;

    unsigned char mTexIdx[TEXIDX_BUFFER_SIZE];

public:
    Response();
    virtual ~Response();

    //
    inline bool toRender() const { return mToRender; } // Check if ready as well (when return false)
    inline unsigned char getDisplaying() const { return mToDisplay; }

    inline void display(unsigned char responseId) {

        LOGV(LOG_LEVEL_RESPONSE, 0, LOG_FORMAT(" - r:%d"), __PRETTY_FUNCTION__, __LINE__, responseId);
        assert((responseId >= RESPONSE_ID_TITLE) && (responseId <= RESPONSE_ID_STORES));
        assert(!mToRender);

        mToDisplay = responseId;
        mTextureIdx = mTexIdx[responseId];
        mTexCoords = const_cast<float*>(mFullTexCoords);
        mToRender = true;

        setAlpha(0.f);
        mScale = 0.00001f; // != 0.f coz using delta can add 0 to this value B4 scaling
    }
    inline void cancel() {

        LOGV(LOG_LEVEL_RESPONSE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
        mCancel = true;
        mForce = false;
    }

    //////
    void start();
    inline void stop() { // pause

        LOGV(LOG_LEVEL_RESPONSE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
        Object2D::pause();

        std::memset(mTexIdx, 0, TEXIDX_BUFFER_SIZE);
    }

    void load(unsigned char responseId);
    void update(const Game* game, const Level* level);

};

#endif // !PAID_VERSION
#endif // RESPONSE_H_
