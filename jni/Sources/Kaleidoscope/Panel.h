#ifndef PANEL_H_
#define PANEL_H_

#include "Global.h"

#include <libeng/Game/2D/Game2D.h>
#include <libeng/Graphic/Object/2D/Element2D.h>

using namespace eng;

//////
class Panel {

private:
    bool mStarted;

    Element2D mLeft;
    Element2D mCenter;
    Element2D mRight;

    Element2D mUp;

public:
    Panel();
    virtual ~Panel();

    inline bool isStarted() const { return mStarted; }
    inline void pause() {

        LOGV(LOG_LEVEL_PANEL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
        mLeft.pause();
        mCenter.pause();
        mRight.pause();
        mUp.pause();

        mLeft.resetY();
        mCenter.resetY();
        mRight.resetY();
        mUp.resetY();
    };

    inline void display(float bottom, bool hide) { // Display/Hide with a delay

        LOGV(LOG_LEVEL_PANEL, 0, LOG_FORMAT(" - b:%f; h:%s"), __PRETTY_FUNCTION__, __LINE__, bottom,
                (hide)? "true":"false");
        float yTrans = (mLeft.getTop() - bottom) / ((hide)? -DISPLAY_DELAY:DISPLAY_DELAY);

        mLeft.translate(0.f, yTrans);
        mCenter.translate(0.f, yTrans);
        mRight.translate(0.f, yTrans);

        mUp.translate(0.f, -yTrans);
    };
    inline void hide(float bottom) {

        LOGV(LOG_LEVEL_PANEL, 0, LOG_FORMAT(" - b:%f"), __PRETTY_FUNCTION__, __LINE__, bottom);
        mLeft.translate(0.f, bottom - mLeft.getTop());
        mCenter.translate(0.f, bottom - mCenter.getTop());
        mRight.translate(0.f, bottom - mRight.getTop());

        mUp.translate(0.f, -bottom - mUp.getBottom());
    };

    //
    void start(const Game2D* game);
    void resume();

    void render() const;

};

#endif // PANEL_H_
