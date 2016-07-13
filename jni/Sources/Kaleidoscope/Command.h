#ifndef COMMAND_H_
#define COMMAND_H_

#include "Global.h"

#include <libeng/Game/2D/Game2D.h>
#include <libeng/Graphic/Object/2D/Element2D.h>

#define HALF_CMD_SIZE           0.15f // In unit
#define MAX_COLOR               255.f
#define CMD_TRANS_Y             0.0004f

#define CMD_TEXTURE_WIDTH       512.f
#define CMD_TEXTURE_HEIGHT      512.f

enum {

    CMD_NONE = 0,

    CMD_SHIFT,
    CMD_COLOR,
    CMD_PHOTO,
    CMD_CHECK,
    CMD_GRADIENT,
    CMD_HIDE,
    CMD_PAUSE,
    CMD_PLAY,
    CMD_TIMER,
    CMD_TRASH
};

using namespace eng;

//////
class Command {

private:
    bool mStarted;

    Element2D mPause2D;
    Element2D mPlay2D;
    Element2D mTimer2D;
    Element2D mTrash2D;

    Element2D mShift2D;
    Element2D mColor2D;
    Element2D mGradient2D;
    Element2D mCheck2D;

    TouchArea mPauseArea;
    TouchArea mPlayArea;
    TouchArea mTimerArea;
    TouchArea mTrashArea;

    TouchArea mShiftArea;
    TouchArea mColorArea;
    TouchArea mHideArea;
    TouchArea mGradientArea;
    TouchArea mCheckArea;

    unsigned char mOrientation;
    float mRotation;

public:
    Command();
    virtual ~Command();

    // Configuration flags
    bool mShift;
    bool mColor;
    bool mGradient;
    bool mCheck;
    bool mPause;
    bool mHidden;
    bool mTimer;

    inline bool isStarted() const { return mStarted; }
    inline void pause() {

        LOGV(LOG_LEVEL_COMMAND, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
        mPause2D.pause();
        mPlay2D.pause();
        mTimer2D.pause();
        mTrash2D.pause();

        mShift2D.pause();
        mColor2D.pause();
        mGradient2D.pause();
        mCheck2D.pause();

        mPause2D.resetY();
        mPlay2D.resetY();
        mTimer2D.resetY();
        mTrash2D.resetY();

        mShift2D.resetY();
        mColor2D.resetY();
        mGradient2D.resetY();
        mCheck2D.resetY();
    };

    inline void display(bool hide) { // Display/Hide with a delay

        LOGV(LOG_LEVEL_COMMAND, 0, LOG_FORMAT(" - h:%s"), __PRETTY_FUNCTION__, __LINE__, (hide)? "true":"false");
        float yTrans = (((PANEL_X2 << 1) / CMD_TEXTURE_HEIGHT) / ((hide)? -DISPLAY_DELAY:DISPLAY_DELAY)) -
                ((hide)? -CMD_TRANS_Y:CMD_TRANS_Y);

        mPause2D.translate(0.f, yTrans);
        mPlay2D.translate(0.f, yTrans);
        mTimer2D.translate(0.f, yTrans);
        mTrash2D.translate(0.f, yTrans);

        mShift2D.translate(0.f, -yTrans);
        mColor2D.translate(0.f, -yTrans);
        mGradient2D.translate(0.f, -yTrans);
        mCheck2D.translate(0.f, -yTrans);
    };
    inline void hide(float bottom) {

        LOGV(LOG_LEVEL_COMMAND, 0, LOG_FORMAT(" - b:%f"), __PRETTY_FUNCTION__, __LINE__, bottom);
        float yTrans = bottom + HALF_CMD_SIZE + ((((PANEL_X2 << 1) / CMD_TEXTURE_HEIGHT) - (2.f * HALF_CMD_SIZE)) / 2.f) -
                ((PANEL_X2 << 1) / CMD_TEXTURE_HEIGHT) + CMD_TRANS_Y;

        mPause2D.translate(0.f, yTrans);
        mPlay2D.translate(0.f, yTrans);
        mTimer2D.translate(0.f, yTrans);
        mTrash2D.translate(0.f, yTrans);

        mShift2D.translate(0.f, -yTrans);
        mColor2D.translate(0.f, -yTrans);
        mGradient2D.translate(0.f, -yTrans);
        mCheck2D.translate(0.f, -yTrans);
    };

    void setConfig();

    //
    void start(const Game2D* game);
    void resume();

    unsigned char update(const Game* game);

    void render() const;

};

#endif // COMMAND_H_
