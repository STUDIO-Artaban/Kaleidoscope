#include "Kaleido2D.h"
#include <libeng/Tools/Tools.h>

#define CAM_TEXTURE_WIDTH           1024.f
#define CAM_TEXTURE_HEIGHT          512.f

#define ALPHA_VEL_A                 0.1f
#define ALPHA_VEL_B                 0.088f

#define TRIANGLE_WIDTH              416 // [ <| ]
#define UNIT_TRIANGLE_WIDTH         (2.f / 7.f)
#define TRIANGLE_HEIGHT             0.24743583f // e -> ^
#define CENTER_HEIGHT_LAG           (TRIANGLE_HEIGHT / 3.f)

// Coordinate of the triangle center: x = (x0 + x1 + x2) / 3; y = (y0 + y1 + y2) / 3
static const float g_TriangleVertices[] = { -UNIT_TRIANGLE_WIDTH / 2.f, -TRIANGLE_HEIGHT / 3.f, 0.f,
        (2.f * TRIANGLE_HEIGHT) / 3.f, UNIT_TRIANGLE_WIDTH / 2.f, -TRIANGLE_HEIGHT / 3.f };

static const unsigned short g_TriangleIndices[] = { 0, 1, 2 };
static const float g_TriangleTexCoords[] = { (((CAM_WIDTH - TRIANGLE_WIDTH) >> 1) + TRIANGLE_WIDTH) / CAM_TEXTURE_WIDTH,
        CAM_HEIGHT / CAM_TEXTURE_HEIGHT, ((CAM_WIDTH - TRIANGLE_WIDTH) >> 1) / CAM_TEXTURE_WIDTH,
        (CAM_HEIGHT >> 1) / CAM_TEXTURE_HEIGHT,
        (((CAM_WIDTH - TRIANGLE_WIDTH) >> 1) + TRIANGLE_WIDTH) / CAM_TEXTURE_WIDTH, 0.f };

//////
Triangle::Triangle() : Object2D(false), Dynamic2D(), mCurAlpha(1.f) {

    LOGV(LOG_LEVEL_KALEIDO2D, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    mTransform = new float[TRANSFORM_BUFFER_SIZE];
    std::memset(mTransform, 0, sizeof(float) * TRANSFORM_BUFFER_SIZE);

    mVertices = new float[6];
    std::memset(mVertices, 0, sizeof(float) * 6);

    mTexCoords = new float[6];
    std::memset(mTexCoords, 0, sizeof(float) * 6);
}
Triangle::~Triangle() {

    LOGV(LOG_LEVEL_KALEIDO2D, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    delete [] mVertices;
}

void Triangle::start(bool reverse) {

    LOGV(LOG_LEVEL_KALEIDO2D, 0, LOG_FORMAT(" - r:%s"), __PRETTY_FUNCTION__, __LINE__, (reverse)? "true":"false");
    Object2D::start(Camera::getInstance()->getCamTexIdx());

    std::memcpy(mTransform, Object2D::TransformBuffer, sizeof(float) * TRANSFORM_BUFFER_SIZE);
    if (!reverse) {

        std::memcpy(mVertices, g_TriangleVertices, sizeof(float) * 6);
        std::memcpy(mTexCoords, g_TriangleTexCoords, sizeof(float) * 6);
    }
    else {

        mVertices[0] = g_TriangleVertices[2];
        mVertices[1] = g_TriangleVertices[3];
        mVertices[2] = g_TriangleVertices[0];
        mVertices[3] = g_TriangleVertices[1];
        mVertices[4] = g_TriangleVertices[4];
        mVertices[5] = g_TriangleVertices[5];

        mTexCoords[0] = g_TriangleTexCoords[2];
        mTexCoords[1] = g_TriangleTexCoords[3];
        mTexCoords[2] = g_TriangleTexCoords[4];
        mTexCoords[3] = g_TriangleTexCoords[5];
        mTexCoords[4] = g_TriangleTexCoords[0];
        mTexCoords[5] = g_TriangleTexCoords[1];
    }
}

void Triangle::render() const {

#ifdef DEBUG
    LOGV(LOG_LEVEL_KALEIDO2D, (*mLog % 100), LOG_FORMAT(" - (u:%s)"), __PRETTY_FUNCTION__, __LINE__,
            (mUpdated)? "true":"false");
    Dynamic2D::transform(getShader2D(), mLog);
#else
    Dynamic2D::transform(getShader2D());
#endif
    Object2D::render(false);
    glDrawElements(GL_TRIANGLES, sizeof(g_TriangleIndices) / sizeof(short), GL_UNSIGNED_SHORT, g_TriangleIndices);
}

//////
Kaleido2D::Kaleido2D() : mAlpha(0.f), mCheckScale(SCALE_CORRECTION), mCheckFlag(false), mLoadSwap(0),
        mLoadShift(0.f), mLoadRevShift(TRIANGLE_HEIGHT), mLoadFactorA(2.f), mLoadFactorB(2.f) {

    LOGV(LOG_LEVEL_KALEIDO2D, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
}
Kaleido2D::~Kaleido2D() {

    LOGV(LOG_LEVEL_KALEIDO2D, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    for (Kaleido::iterator iter = mKaleido.begin(); iter != mKaleido.end(); ++iter)
        delete (*iter);
    mKaleido.clear();
}

void Kaleido2D::setGradient(bool enable) {

    LOGV(LOG_LEVEL_KALEIDO2D, 0, LOG_FORMAT(" - e:%s"), __PRETTY_FUNCTION__, __LINE__, (enable)? "true":"false");
    if (!enable) {
        for (Kaleido::iterator iter = mKaleido.begin(); iter != mKaleido.end(); ++iter) {

            (*iter)->mCurAlpha = 1.f;
            (*iter)->setAlphaColor(1.f);
        }
    }
    else {

        unsigned char level = 1;
        for (Kaleido::iterator iter = mKaleido.begin(); iter != mKaleido.end(); ++level) {

            static float factor = 0.f;
            switch (level) {
                case 1: factor = 6.f; break;
                case 2: factor = 5.5f; break;
                default: {

                    if (level % 2)
                        factor -= 1.f;
                    break;
                }
            }
            float alpha = ALPHA_VEL_A * factor;
            for (unsigned char i = 0; i < 7; ++i, ++iter) {
#ifdef DEBUG
                if (iter == mKaleido.end()) {
                    LOGE(LOG_FORMAT(" - Wrong triangle count"), __PRETTY_FUNCTION__, __LINE__);
                    assert(NULL);
                }
#endif
                if (alpha < 0.f)
                    (*iter)->mCurAlpha = 0.f;
                else
                    (*iter)->mCurAlpha = alpha;
                (*iter)->setAlphaColor(1.f);

                if (i < 3)
                    alpha += ALPHA_VEL_A;
                else
                    alpha -= ALPHA_VEL_A;
            }
            alpha = ALPHA_VEL_B * factor;
            for (unsigned char i = 0; i < 8; ++i, ++iter) {
#ifdef DEBUG
                if (iter == mKaleido.end()) {
                    LOGE(LOG_FORMAT(" - Wrong triangle count"), __PRETTY_FUNCTION__, __LINE__);
                    assert(NULL);
                }
#endif
                if (alpha < 0.f)
                    (*iter)->mCurAlpha = 0.f;
                else
                    (*iter)->mCurAlpha = alpha;
                (*iter)->setAlphaColor(1.f);

                if (i < 3)
                    alpha += ALPHA_VEL_B;
                else if (i > 3)
                    alpha -= ALPHA_VEL_B;
            }
        }
    }
}

bool Kaleido2D::load(const Game2D* game, unsigned char step) {

    LOGV(LOG_LEVEL_KALEIDO2D, 0, LOG_FORMAT(" - g:%x; s:%d (c:%d)"), __PRETTY_FUNCTION__, __LINE__, game, step,
            Camera::getInstance()->getCamTexIdx());

    assert(Camera::getInstance()->getCamTexIdx() != TEXTURE_IDX_INVALID);
    if ((step == 1) && (!mKaleido.empty())) { // Resume

        for (Kaleido::iterator iter = mKaleido.begin(); iter != mKaleido.end(); ++iter)
            (*iter)->resume(Camera::getInstance()->getCamTexIdx());
        return false;
    }

    // Start...
    float xLag = (UNIT_TRIANGLE_WIDTH / 2.f) - 1.f;
    float yLag = 0.f;

    bool reverse = static_cast<bool>(mLoadSwap % 3);
    float rotate = 0.f;
    if (reverse) {

        yLag = -2.f * TRIANGLE_HEIGHT / 3.f;
        rotate = PI_F;
    }

    if (step > 2) {
        if (reverse) {
            if (step == 3)
                mLoadShift = TRIANGLE_HEIGHT;
            else {
                if (mLoadShift > 0.f)
                    mLoadShift *= -1.f;
                else {

                    mLoadShift = mLoadFactorA * TRIANGLE_HEIGHT;
                    mLoadFactorA += 1.f;
                }
            }
            yLag += mLoadShift;
        }
        else {

            if (mLoadRevShift > 0.f) {

                mLoadRevShift = -TRIANGLE_HEIGHT * mLoadFactorB;
                mLoadFactorB += 2.f;
            }
            else
                mLoadRevShift *= -1.f;
            yLag = mLoadRevShift;
        }
    }

    // Check no need to add triangle (not visible)
    if (step % 2) {

#ifdef DEBUG
        if (reverse) {
            if ((TRIANGLE_HEIGHT - CENTER_HEIGHT_LAG + (2.f * yLag)) > game->getScreen()->top) {

                LOGI(LOG_LEVEL_KALEIDO2D, 0, LOG_FORMAT(" - Stop at step: %d"), __PRETTY_FUNCTION__, __LINE__, step);
                return false;
            }
        }
        else if ((CENTER_HEIGHT_LAG + yLag) > game->getScreen()->top) {

            LOGI(LOG_LEVEL_KALEIDO2D, 0, LOG_FORMAT(" - Stop at step: %d"), __PRETTY_FUNCTION__, __LINE__, step);
            return false;
        }
#else
        if (reverse) {
            if ((TRIANGLE_HEIGHT - CENTER_HEIGHT_LAG + (2.f * yLag)) > game->getScreen()->top)
                return false;
        }
        else if ((CENTER_HEIGHT_LAG + yLag) > game->getScreen()->top)
            return false;
#endif
    }

    for (unsigned char i = 0; i < 7; ++i) {

        Triangle* triangle = new Triangle;
        triangle->initialize(game2DVia(game));
        triangle->start(reverse);
        triangle->scale(SCALE_CORRECTION, SCALE_CORRECTION);
#ifndef __ANDROID__
        triangle->setBGRA(true);
#endif

        triangle->translate(xLag, CENTER_HEIGHT_LAG + yLag + ((reverse)? mLoadShift:0.f));
        switch (i) {

            case 1:
            case 4: triangle->rotate((-2.f * PI_F / 3.f) + (rotate / 3.f)); break;
            case 2:
            case 5: triangle->rotate((-4.f * PI_F / 3.f) - (rotate / 3.f)); break;
            default: {

                triangle->rotate(rotate);
                break;
            }
        }
        mKaleido.push_back(triangle);
        xLag += UNIT_TRIANGLE_WIDTH;
    }
    xLag = -1.f;
    if (reverse)
        yLag *= 2.f;

    for (unsigned char i = 0; i < 8; ++i) {

        Triangle* triangle = new Triangle;
        triangle->initialize(game2DVia(game));
        triangle->start(!reverse);
        triangle->scale(SCALE_CORRECTION, SCALE_CORRECTION);
#ifndef __ANDROID__
        triangle->setBGRA(true);
#endif

        triangle->translate(xLag, TRIANGLE_HEIGHT - CENTER_HEIGHT_LAG + yLag);
        switch (i) {
            case 1:
            case 4:
            case 7: triangle->rotate((-2.f * PI_F / 3.f) - PI_F + (rotate / 3.f)); break;
            case 2:
            case 5: triangle->rotate((-5.f * PI_F) + rotate); break;
            default: {

                triangle->rotate((-PI_F / 3.f) - (rotate / 3.f));
                break;
            }
        }
        mKaleido.push_back(triangle);
        xLag += UNIT_TRIANGLE_WIDTH;
    }
    if (++mLoadSwap > 3)
        mLoadSwap = 0;

    return true;
}
void Kaleido2D::render() const {

    glDisable(GL_BLEND);
    for (Kaleido::const_iterator iter = mKaleido.begin(); iter != mKaleido.end(); ++iter)
        if (((*iter) != mKaleido[3]) || (!mCheckFlag))
            (*iter)->render();

    if (mCheckFlag)
        mKaleido[3]->render();
    glEnable(GL_BLEND);
}
