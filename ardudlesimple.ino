///ArdudleJump!
///October 5th 2020
///Polaris Void
#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "ext.h"
#include "imgaudio.h"
#include "EEPROMUtils.h"
#define NUMBER_OF_PLATFORMS 8
#define LAUNCH_DELAY_MIN  2
#define LAUNCH_DELAY_MAX  3
Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);
uint16_t ardudlescore = 0;
uint16_t highscore = 0;
int gamestate = 0;
Ardudle ardudle = {0, 0, Stance::Left, false, ardudleright, ardudlerightmask };
Platform platforms[NUMBER_OF_PLATFORMS] = {
  {0, 0, PlatType::Large, false, largeplatform, largeplatformmask},
  {0, 0, PlatType::Large, false, largeplatform, largeplatformmask},
  {0, 0, PlatType::Large, false, largeplatform, largeplatformmask},
  {0, 0, PlatType::Large, false, largeplatform, largeplatformmask},
  {0, 0, PlatType::Large, false, largeplatform, largeplatformmask},
  {0, 0, PlatType::Large, false, largeplatform, largeplatformmask},
  {0, 0, PlatType::Large, false, largeplatform, largeplatformmask},
  {0, 0, PlatType::Large, false, largeplatform, largeplatformmask},
};

const uint8_t *ardudle_images[] = {ardudleleft, ardudleright};
const uint8_t *ardudle_masks[] = {ardudleleftmask, ardudlerightmask};
const uint8_t *platform_images[] = {smallplatform, mediumplatform, largeplatform};
const uint8_t *platform_masks[] = {smallplatformmask, mediumplatformmask, largeplatformmask};
const uint8_t *background_images[] = {bgtile1, bgtile2};
uint8_t jumpCoords[] = {0, 4, 6, 5, 4, 3, 3, 3, 2, 3, 2, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0};
uint16_t platformLaunchCountdown = LAUNCH_DELAY_MIN;

void updateArdudle() {
  if (ardudle.jumping) {
    ardudle.x = ardudle.x + jumpCoords[ardudle.jumpIndex];
    ardudle.jumpIndex++;
    if (ardudle.jumpIndex == sizeof(jumpCoords)) {
      ardudle.jumping = false;
      ardudle.jumpIndex = 0;
    }
  }
  if (ardudle.y == 0) {
    ardudle.y = 63;
  }
  if (ardudle.y == HEIGHT) {
    ardudle.y = 0;
  }
}

void updatePlatforms() {
  for (uint8_t i = 0; i < NUMBER_OF_PLATFORMS; i++) {
    if (platforms[i].enabled == true) {
      switch (platforms[i].type) {
        case PlatType::Small:
        case PlatType::Medium:
        case PlatType::Large:
          ///if ardudle is above half the screen, move the platforms down by how high ardudle jumps
          if (ardudle.x > WIDTH / 2) {
            //ardudlescore++;
            platforms[i].x -= jumpCoords[ardudle.jumpIndex];
          }
          break;
      }
      //out of view?
      if (platforms[i].x == 0) {
        platforms[i].enabled = false;
      }
    }
  }
}

void launchPlatform(uint8_t platformNumber) {
  PlatType randomUpper = PlatType::Large;
  switch (ardudlescore) {
    case 0 ... 99:
      randomUpper = PlatType::Large;
      break;

    case 100 ... 199:
      randomUpper = PlatType::Medium;
      break;

    case 200 ... 299:
      randomUpper = PlatType::Small;
      break;

    default:
      randomUpper = PlatType::Count_All_Platforms;
      break;
  }

  uint8_t randomLowerVal = static_cast<uint8_t>(PlatType::Large);
  uint8_t randomUpperVal = static_cast <uint8_t>(randomUpper);
  uint8_t randomPlatform = random(randomLowerVal, randomUpperVal + 1);

  PlatType type = static_cast<PlatType>(randomPlatform);
  //launch
  platforms[platformNumber].type = type;
  platforms[platformNumber].enabled = true;
  platforms[platformNumber].x = random(124, 224) ;
  platforms[platformNumber].y = random(1, 64);
}

void resetGame() {
  ardudle.x = WIDTH / 2;
  ardudle.y = HEIGHT / 2;
  ardudlescore = 0;
  platforms[1].x = 20;
  platforms[1].y = 10 + getImageHeight(platforms[1].image);
  platforms[2].x = random(20, 32);
  platforms[2].y = random(HEIGHT -getImageHeight(platforms[2].image)) + getImageHeight(platforms[2].image);
  platforms[3].x = random(32, 44);
  platforms[3].y = random(HEIGHT - getImageHeight(platforms[3].image)) + getImageHeight(platforms[3].image);
  platforms[4].x = random(44, 66);
  platforms[4].y = random(HEIGHT - getImageHeight(platforms[4].image)) + getImageHeight(platforms[4].image);
  ardudle.jumpIndex = 0;
}
void drawArdudle() {
  uint8_t imageIndex = static_cast<uint8_t>(ardudle.stance);
  ardudle.image = ardudle_images[imageIndex];
  ardudle.mask = ardudle_masks[imageIndex];
  Sprites::drawExternalMask(ardudle.x, ardudle.y - getImageHeight(ardudle.image), ardudle.image, ardudle.mask, 0, 0);
}
void drawPlatforms() {
  for (int i = 0; i < NUMBER_OF_PLATFORMS; i++) {
    if (platforms[i].enabled == true) {
      uint8_t imageIndex = static_cast<uint8_t>(platforms[i].type);
      platforms[i].image = platform_images[imageIndex];
      platforms[i].mask = platform_masks[imageIndex];
      Sprites::drawExternalMask(platforms[i].x, platforms[i].y - getImageHeight(platforms[i].image), platforms[i].image, platforms[i].mask, 0, 0);
    }
  }
}
void drawScore() {
  if (ardudlescore == 0) {
    Sprites::drawOverwrite(122, 0, Zero, 0);
    Sprites::drawOverwrite(122, 5, Zero, 0);
  };
  if (ardudlescore == 1) Sprites::drawOverwrite(122, 0, One, 0);
  if (ardudlescore == 2) Sprites::drawOverwrite(122, 0, Two, 0);
  if (ardudlescore == 3) Sprites::drawOverwrite(122, 0, Three, 0);
  if (ardudlescore == 4) Sprites::drawOverwrite(122, 0, Four, 0);
  if (ardudlescore == 5) Sprites::drawOverwrite(122, 0, Five, 0);
  if (ardudlescore == 6) Sprites::drawOverwrite(122, 0, Six, 0);
  if (ardudlescore == 7) Sprites::drawOverwrite(122, 0, Seven, 0);
  if (ardudlescore == 8) Sprites::drawOverwrite(122, 0, Eight, 0);
  if (ardudlescore == 9) Sprites::drawOverwrite(122, 0, Nine, 0);
  if (ardudlescore == 10); {
    Sprites::drawOverwrite(122, 0, One, 0);
    Sprites::drawOverwrite(122, 5, Zero, 0);
  };
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
      Sprites::drawOverwrite(WIDTH - 10, 0, TITLE, 0);
      Sprites::drawOverwrite(WIDTH / 2 - 7, 15, TITLEsub, 0);
      if (arduboy.justPressed(A_BUTTON)) {
        gamestate = 1;
      }
      break;

    case 1:
      //gameplay
      arduboy.clear();
      //gameover if ardudle touches bottom
      if (ardudle.x <= 0) {
        gamestate = 2;
      }
      //left & right
      if (arduboy.pressed(UP_BUTTON)) {
        ardudle.y--;
        ardudle.stance = Stance::Left;
      }
      if (arduboy.pressed(DOWN_BUTTON)) {
        ardudle.y++;
        ardudle.stance = Stance::Right;
      }

      ///move ardudle down(left) by default (like gravity)
      if (!ardudle.jumping) {
        ardudle.x = ardudle.x - 2;
      }

      //if ardudle touches platform, jump!
      if (ardudle.jumping == false && (collision())) {
        ardudle.jumping = true;
        ardudle.jumpIndex++;
      }

      --platformLaunchCountdown;
      if (platformLaunchCountdown == 0) {
      
      for (int i = 0; i < NUMBER_OF_PLATFORMS; i++) {

        if (!platforms[i].enabled) {
          launchPlatform(i);
          break;
        }
      }
      platformLaunchCountdown = random(LAUNCH_DELAY_MIN, LAUNCH_DELAY_MAX);
      
      }
      updateArdudle();
      updatePlatforms();

      drawPlatforms();
      drawArdudle();
      drawScore();

      break;

    case 2:
      //gameover
      arduboy.clear();
      /*if (ardudlescore > highscore) {
        highscore = ardudlescore;
        EEPROM.put(EEPROM_SCORE, highscore);
        }
      */
      if (arduboy.justPressed(A_BUTTON)) {
        gamestate = 3;
      }
      Sprites::drawOverwrite(WIDTH / 2, 10, GAME, 0);
      Sprites::drawOverwrite(WIDTH / 2 - 14, 10, OVER, 0);
      break;

    case 3:
      //highscore
      if (arduboy.justPressed(A_BUTTON)) {
        gamestate = 0;
      }
      Sprites::drawOverwrite(WIDTH / 2 + 12, 0, HIGHSCORE, 0);
      Sprites::drawOverwrite(WIDTH / 2, 0, One, 0);
      Sprites::drawOverwrite(WIDTH / 2 - 7, 0, Two, 0);
      Sprites::drawOverwrite(WIDTH / 2 - 14, 0, Three, 0);
      Sprites::drawOverwrite(WIDTH / 2 - 21, 0, Four, 0);
      Sprites::drawOverwrite(WIDTH / 2 - 28, 0, Five, 0);
      Sprites::drawOverwrite(WIDTH / 2 - 35, 0, Six, 0);
      Sprites::drawOverwrite(WIDTH / 2 - 42, 0, Seven, 0);
      Sprites::drawOverwrite(WIDTH / 2 - 49, 0, Eight, 0);
      Sprites::drawOverwrite(WIDTH / 2 - 56, 0, Nine, 0);
      Sprites::drawOverwrite(WIDTH / 2 - 63, 0, One, 0);
      Sprites::drawOverwrite(WIDTH / 2 - 63, 4, Zero, 0);
      break;

  }
  arduboy.display();
}


bool collision () {
  for (uint8_t i = 0; i < NUMBER_OF_PLATFORMS; i++) {
    if (platforms[i].enabled == true) {
      if (collide(ardudle.x, ardudle.y - getImageHeight(ardudle.image), ardudle.image, platforms[i].x, platforms[i].y - getImageHeight(platforms[i].image), platforms[i].image)) {
        return true;
      }
    }
  }
  return false;
}
