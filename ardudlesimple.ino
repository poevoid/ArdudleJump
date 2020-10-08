///ArdudleJump!
///October 5th 2020
///Polaris Void
#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "ext.h"
#include "imgaudio.h"
#include "EEPROMUtils.h"
Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);
uint16_t playerscore = 0;
int16_t highscore = 0;
int gamestate = 0;
Player player = {0, 0, Stance::Left, false, playerright, playerrightmask };
PlatformS platforms = { 0, 0, Type::Solid, smallplatform, smallplatformmask};
PlatformM platformm = { 0, 0, Type::Solid, mediumplatform, mediumplatformmask};
PlatformL platforml = { 0, 0, Type::Solid, largeplatform, largeplatformmask};
const uint8_t *player_images[] = {playerleft, playerright};
const uint8_t *player_masks[] = {playerleftmask, playerrightmask};
const uint8_t *platforms_images[] = {smallplatform};
const uint8_t *platforms_masks[] = {smallplatformmask};
const uint8_t *platformm_images[] = {mediumplatform};
const uint8_t *platformm_masks[] = {mediumplatformmask};
const uint8_t *platforml_images[] = {largeplatform};
const uint8_t *platforml_masks[] = {largeplatformmask};
const uint8_t *background_images[] = {bgtile1, bgtile2};
uint8_t jumpCoords[] = {0, 3, 5, 4, 3, 2, 2, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0};

void updateArdudle() {
  if (player.jumping) {
    player.x = player.x + jumpCoords[player.jumpIndex];
    player.jumpIndex++;
    if (player.jumpIndex == sizeof(jumpCoords)) {
      player.jumping = false;
      player.jumpIndex = 0;
    }
  }
  if (player.y == 0) {
    player.y = 63;
  }
  if (player.y == HEIGHT) {
    player.y = 0;
  }
}
void resetGame() {
  player.x = WIDTH;
  player.y = HEIGHT / 2;
  playerscore = 0;
  platforms.x = 10;
  platforms.y = 10;
  platforml.x = 75;
  platforml.y = 30;
  platformm.x = 40;
  platformm.y = 50;
  player.jumpIndex = 0;
}
void drawPlayer() {
  uint8_t imageIndex = static_cast<uint8_t>(player.stance);
  player.image = player_images[imageIndex];
  player.mask = player_masks[imageIndex];
  Sprites::drawExternalMask(player.x, player.y - getImageHeight(player.image), player.image, player.mask, 0, 0);
}
void drawPlatformS() {
  uint8_t imageIndex = static_cast<uint8_t>(platforms.type);
  platforms.image = platforms_images[imageIndex];
  platforms.mask = platforms_masks[imageIndex];
  Sprites::drawExternalMask(platforms.x, platforms.y - getImageHeight(platforms.image), platforms.image, platforms.mask, 0, 0);
}
void drawPlatformM() {
  uint8_t imageIndex = static_cast<uint8_t>(platformm.type);
  platformm.image = platformm_images[imageIndex];
  platformm.mask = platformm_masks[imageIndex];
  Sprites::drawExternalMask(platformm.x, platformm.y - getImageHeight(platformm.image), platformm.image, platformm.mask, 0, 0);
}
void drawPlatformL() {
  uint8_t imageIndex = static_cast<uint8_t>(platforml.type);
  platforml.image = platforml_images[imageIndex];
  platforml.mask = platforml_masks[imageIndex];
  Sprites::drawExternalMask(platforml.x, platforml.y - getImageHeight(platforml.image), platforml.image, platforml.mask, 0, 0);
}
void setup() {
  // put your setup code here, to run once:
  arduboy.begin();
  arduboy.initRandomSeed();
  arduboy.setFrameRate(60);
  arduboy.clear();
  arduboy.audio.on();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!arduboy.nextFrame()) {
    return;
  }
  arduboy.clear();
  arduboy.pollButtons();
  switch (gamestate) {
    case 0:
      //title
      resetGame();
      arduboy.setCursor(0, 0);
      arduboy.print("ArdudleJump!");
      arduboy.setCursor(10, 10);
      arduboy.print("Press A for 1 Player");
      if (arduboy.justPressed(A_BUTTON)) {
        
        gamestate = 1;
      }
      break;

    case 1:
      //gameplay
      arduboy.clear();
      //gameover if player touches bottom
      if (player.x == 0) {
        gamestate = 2;
      }
      //left & right
      if (arduboy.pressed(UP_BUTTON)) {
        player.y--;
        player.stance = Stance::Left;
      }
      if (arduboy.pressed(DOWN_BUTTON)) {
        player.y++;
        player.stance = Stance::Right;
      }
      
      ///move player down(left) by default (like gravity)
      if (!player.jumping) {
        player.x = player.x - 1;
      }

      ///if player touches platform, playerx + 1 (10 times) to sim jumping. if player jumps above half the screen, move the screen instead of jumping and draw new platforms
      if (player.jumping == false && (collision())) {
        player.jumping = true;
        player.jumpIndex++;
      }
      updateArdudle();
      drawPlayer();
      drawPlatformS();
      drawPlatformM();
      drawPlatformL();

      /*if (player.x >= (WIDTH/2) && (collision())) {
        player.x = WIDTH/2;
        platforms.x -= jumpCoords[platforms.jumpIndex];
        platforms.jumpIndex++;
        platformm.x -= jumpCoords[platformm.jumpIndex];
        platformm.jumpIndex++;
        platforml.x -= jumpCoords[platforml.jumpIndex];
        platforml.jumpIndex++;

        }
      */
      //add change in (x) to score


      //print score
      break;

    case 2:
      //gameover
      if (playerscore > highscore) {
        highscore = playerscore;
        EEPROM.put(EEPROM_SCORE, highscore);
      }
      if (arduboy.justPressed(A_BUTTON)) {
        gamestate = 3;
      }
      arduboy.setCursor(0, 0);
      arduboy.print("Game Over");
      break;

    case 3:
      //highscore
      if (arduboy.justPressed(A_BUTTON)) {
        gamestate = 0;
      }
      arduboy.setCursor(35, 0);
      arduboy.print("Highscore: \n");
      arduboy.print(highscore);
      break;

  }
  arduboy.display();
}
/*bool collision () {

      if (collide(player.x, player.y - getImageHeight(player.image), player.image, platforms.x, platforms.y - getImageHeight(platforms.image), platforms.image)) {
        return true;
      }
      if (collide(player.x, player.y - getImageHeight(player.image), player.image, platformm.x, platformm.y - getImageHeight(platformm.image), platformm.image)) {
        return true;
      }
      if (collide(player.x, player.y - getImageHeight(player.image), player.image, platforml.x, platforml.y - getImageHeight(platforml.image), platforml.image)) {
        return true;
      }

  return false;

  }
*/
/*if ((player.x == platform.platSx + platform.platwidth && platform.platSy < player.y + getImageHeight(player.image) && platform.platSy + platform.platSheight > platform.platSy) && (player.y == platform.platSy + platform.platSheight && platform.platSx < player.x + getImageWidth(player.image) && platform.platSx + platform.platwidth > platform.platSx)) {
        player.jumping = true;
        sound.tone(NOTE_E5, 50);
        }
      if (player.x == platform.platMx + platform.platwidth && platform.platMy < player.y + getImageHeight(player.image) && platform.platMy + platform.platMheight > platform.platMy) {
        player.jumping = true;
        sound.tone(NOTE_E5, 50);
        }
      if (player.x == platform.platLx + platform.platwidth && platform.platLy < player.y + getImageHeight(player.image) && platform.platLy + platform.platLheight > platform.platLy) {
        player.jumping = true;
        sound.tone(NOTE_E5, 50);
        }
*/


bool collision () {
  ///for (byte i = 0; i < NUMBER_OF_OBSTACLES; i++) {
      Rect playerRect = Rect{ player.x,
                             player.y - getImageHeight(player.image),
                             getImageWidth(player.image),
                             getImageHeight(player.image) };
      Rect psRect =
        Rect{ platforms.x,
              platforms.y - getImageHeight(platforms.image),
              getImageWidth(platforms.image),
              getImageHeight(platforms.image) };
      Rect pmRect =
        Rect{ platformm.x,
              platformm.y - getImageHeight(platformm.image),
              getImageWidth(platformm.image),
              getImageHeight(platformm.image) };

      Rect plRect =
        Rect{ platforml.x,
              platforml.y - getImageHeight(platforml.image),
              getImageWidth(platforml.image),
              getImageHeight(platforml.image) };
      if (arduboy.collide(playerRect, psRect)) {
        return true;
      }
      if (arduboy.collide(playerRect, pmRect)) {
        return true;
      }
      if (arduboy.collide(playerRect, plRect)) {
        return true;
      }
  return false;
}
