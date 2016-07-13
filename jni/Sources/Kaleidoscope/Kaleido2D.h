#ifndef KALEIDO2D_H_
#define KALEIDO2D_H_

#include "Global.h"

#include <libeng/Game/2D/Game2D.h>
#include <libeng/Graphic/Object/2D/Element2D.h>

#include <vector>

#define SCALE_CORRECTION            1.008f // Scale used to hide lags between triangle (bug of precision)
#define SCALE_SHIFT                 0.88f
#define SCALE_CHECK                 3.f

#define SCALE_VELOCITY              (SCALE_CHECK / DISPLAY_DELAY)

using namespace eng;

class Triangle : public Object2D, public Dynamic2D {

public:
    Triangle();
    virtual ~Triangle();

    float mCurAlpha;

    inline void setAlphaColor(float alpha) {

        //setAlpha(mCurAlpha * alpha); // Blending disabled
        setRed(mCurAlpha * alpha);
        setGreen(mCurAlpha * alpha);
        setBlue(mCurAlpha * alpha);
    };

    //
    void start(bool reverse);
    inline void update(const Game* game, const Level* level) { assert(NULL); }

    inline bool checkCollision(const Bounds* bounds, unsigned char boundsId) { assert(NULL); return false; }
    inline bool checkCollision(const Object* object, unsigned char objectId) { assert(NULL); return false; }

    void render() const;

};

//////
class Kaleido2D {

private:
    typedef std::vector<Triangle*> Kaleido;
    Kaleido mKaleido;

    unsigned char mLoadSwap;
    float mLoadShift;
    float mLoadRevShift;
    float mLoadFactorA;
    float mLoadFactorB;

    float mAlpha;

    bool mCheckFlag;
    float mCheckScale;

public:
    Kaleido2D();
    virtual ~Kaleido2D();

    inline float getAlpha() const { return mAlpha; }
    inline void setAlpha(float alpha) {

        LOGV(LOG_LEVEL_KALEIDO2D, 0, LOG_FORMAT(" - a:%f"), __PRETTY_FUNCTION__, __LINE__, alpha);
        for (Kaleido::iterator iter = mKaleido.begin(); iter != mKaleido.end(); ++iter)
            (*iter)->setAlphaColor(alpha);

        mAlpha = alpha;
    };

    void setGradient(bool enable);
    inline void setShift(bool enable) {

        LOGV(LOG_LEVEL_KALEIDO2D, 0, LOG_FORMAT(" - e:%s"), __PRETTY_FUNCTION__, __LINE__, (enable)? "true":"false");
        for (Kaleido::iterator iter = mKaleido.begin(); iter != mKaleido.end(); ++iter)
            if (((*iter) != mKaleido[3]) || (!mCheckFlag)) {

                if ((*iter) == mKaleido[3]) mCheckScale = (enable)? SCALE_SHIFT:SCALE_CORRECTION;
                (enable)? (*iter)->scale(SCALE_SHIFT, SCALE_SHIFT):(*iter)->scale(SCALE_CORRECTION, SCALE_CORRECTION);
            }
            else {

                mCheckScale = (enable)? (SCALE_CHECK + SCALE_SHIFT):(SCALE_CHECK + SCALE_CORRECTION);
                (*iter)->scale(mCheckScale, mCheckScale);
            }
    };

    inline void check() {

        LOGV(LOG_LEVEL_KALEIDO2D, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
        mCheckFlag = true;

        mCheckScale += SCALE_VELOCITY;
        mKaleido[3]->scale(mCheckScale, mCheckScale);
    };
    inline void uncheck(bool flag) {

        LOGV(LOG_LEVEL_KALEIDO2D, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
        mCheckFlag = flag;

        mCheckScale -= SCALE_VELOCITY;
        mKaleido[3]->scale(mCheckScale, mCheckScale);
    };
    inline void setChecked(bool check, bool shift) {

        LOGV(LOG_LEVEL_KALEIDO2D, 0, LOG_FORMAT(" - c:%s; s:%s"), __PRETTY_FUNCTION__, __LINE__, (check)? "true":"false",
                (shift)? "true":"false");
        mCheckFlag = check;
        if (mCheckFlag)
            mCheckScale = (shift)? (SCALE_CHECK + SCALE_SHIFT):(SCALE_CHECK + SCALE_CORRECTION);
        else
            mCheckScale = (shift)? SCALE_SHIFT:SCALE_CORRECTION;

        mKaleido[3]->scale(mCheckScale, mCheckScale);
    };

    //
    bool load(const Game2D* game, unsigned char step);
    inline void pause() {

        LOGV(LOG_LEVEL_KALEIDO2D, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
        for (Kaleido::iterator iter = mKaleido.begin(); iter != mKaleido.end(); ++iter)
            (*iter)->pause();
    };

    void render() const;

};

#endif // KALEIDO2D_H_
