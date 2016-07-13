#include "Panel.h"

#define FLASH_DIAMETER              PANEL_X2
#define PANEL_Y2                    256

#define PANEL_TEXTURE_WIDTH         256.f
#define PANEL_TEXTURE_HEIGHT        512.f

// Texture IDs
#define TEXTURE_ID_FLASH            24
#define TEXTURE_ID_PANEL            27

//////
Panel::Panel() : mStarted(false) { LOGV(LOG_LEVEL_PANEL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__); }
Panel::~Panel() { LOGV(LOG_LEVEL_PANEL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__); }

void Panel::start(const Game2D* game) {

    LOGV(LOG_LEVEL_PANEL, 0, LOG_FORMAT(" - g:%x"), __PRETTY_FUNCTION__, __LINE__, game);
    mLeft.initialize(game2DVia(game));
    mCenter.initialize(game2DVia(game));
    mRight.initialize(game2DVia(game));
    mUp.initialize(game2DVia(game));

    mLeft.start(TEXTURE_ID_PANEL);
    mCenter.start(TEXTURE_ID_FLASH);
    mRight.start(TEXTURE_ID_PANEL);
    mUp.start(TEXTURE_ID_PANEL);

    mCenter.setTexCoords(FULL_TEXCOORD_BUFFER);

    float texCoords[8] = { PANEL_X2 / PANEL_TEXTURE_WIDTH, 0.f, (PANEL_X2 << 1) / PANEL_TEXTURE_WIDTH, 0.f,
            (PANEL_X2 << 1) / PANEL_TEXTURE_WIDTH, 1.f, PANEL_X2 / PANEL_TEXTURE_WIDTH, 1.f };
    mUp.setTexCoords(texCoords);

    texCoords[0] = PANEL_X2 / PANEL_TEXTURE_WIDTH;
    texCoords[1] = PANEL_Y2 / PANEL_TEXTURE_HEIGHT;
    texCoords[2] = 0.f;
    texCoords[3] = texCoords[1];
    texCoords[4] = 0.f;
    texCoords[5] = 0.f;
    texCoords[6] = texCoords[0];
    texCoords[7] = 0.f;
    mRight.setTexCoords(texCoords);

    texCoords[1] = 0.f;
    texCoords[3] = 0.f;
    texCoords[5] = PANEL_Y2 / PANEL_TEXTURE_HEIGHT;
    texCoords[7] = texCoords[5];
    mLeft.setTexCoords(texCoords);

    short height = PANEL_X2 * (game->getScreen()->width >> 1) / static_cast<short>(PANEL_TEXTURE_WIDTH);
    mLeft.setVertices(0, height, game->getScreen()->width >> 1, 0);
    mCenter.setVertices((game->getScreen()->width >> 1) - (height >> 1), height,
            (game->getScreen()->width >> 1) + (height >> 1), 0);
    mRight.setVertices(game->getScreen()->width >> 1, height, game->getScreen()->width, 0);
    mUp.setVertices(0, game->getScreen()->height, game->getScreen()->width, game->getScreen()->height - height);

    mStarted = true;
}
void Panel::resume() {

    LOGV(LOG_LEVEL_PANEL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    mLeft.resume(TEXTURE_ID_PANEL);
    mCenter.resume(TEXTURE_ID_FLASH);
    mRight.resume(TEXTURE_ID_PANEL);

    mUp.resume(TEXTURE_ID_PANEL);
}

void Panel::render() const {

    mLeft.render(true);
    mRight.render(true);
    mCenter.render(true);

    mUp.render(true);
}
