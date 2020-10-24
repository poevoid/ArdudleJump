///ArdudleJump!
///October 5th 2020
///Polaris Void
#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "ext.h"
#include "imgaudio.h"
#include "EEPROMUtils.h"
#define NUMBER_OF_PLATFORMS 5
Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);
uint16_t playerscore = 0;
int16_t highscore = 0;
int gamestate = 0;
Player player = {0, 0, Stance::Left, false, playerright, playerrightmask };
Platform platforms[NUMBER_OF_PLATFORMS] = {
  {0, 0, Type::Small, false, smallplatform, smallplatformmask},
  {0, 0, Type::Small, false, smallplatform, smallplatformmask},
  {0, 0, Type::Small, false, smallplatform, smallplatformmask},
  {0, 0, Type::Small, false, smallplatform, smallplatformmask},
  {0, 0, Type::Small, false, smallplatform, smallplatformmask},
};

const uint8_t *player_images[] = {playerleft, playerright};
const uint8_t *player_masks[] = {playerleftmask, playerrightmask};
const uint8_t *platform_images[] = {smallplatform, mediumplatform, largeplatform};
const uint8_t *platform_masks[] = {smallplatformmask, mediumplatformmask, largeplatformmask};
const uint8_t *background_images[] = {bgtile1, bgtile2};
uint8_t jumpCoords[] = {0, 4, 6, 5, 4, 3, 3, 3, 2, 3, 2, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0};
bool platformLaunchCountdown = false;
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
 
void updatePlatforms() {
  for (uint8_t i = 0; i < NUMBER_OF_PLATFORMS; i++) {
    if (platforms[i].enabled == true) {
    switch (platforms[i].type) {
      case Type::Small:
       break;
      case Type::Medium:
       break;
      case Type::Large:
      ///if the player is above half the screen, move the platforms down by how high the player jumps 
      if (player.x > WIDTH/2) {
        playerscore++;
        platforms[i].x = platforms[i].x - jumpCoords[player.jumpIndex];
        }
        break;
    }
    //out of view?
    if (platforms[i].x == 0) {
      platforms[i].enabled = false;
      platformLaunchCountdown = true;
    }
  }
 }
}

void launchPlatform(uint8_t platformNumber) {
  Type randomUpper = Type::Small;
  switch (playerscore) {
    case 0 ... 99:
    randomUpper = Type::Small;
    break;
    case 100 ... 199:
    randomUpper = Type::Medium;
    break;
    case 200 ... 299:
    randomUpper = Type::Large;
    break;
    default:
    randomUpper = Type::Count_All_Platforms;
    break;
  }
  uint8_t randomLowerVal = static_cast<uint8_t>(Type::Small);
  uint8_t randomUpperVal = static_cast <uint8_t>(randomUpper);
  uint8_t randomPlatform = random(randomLowerVal, randomUpperVal + 1);
  Type type = static_cast<Type>(randomPlatform);
  //launch
  platforms[platformNumber].type = type;
  platforms[platformNumber].enabled = true;
  platforms[platformNumber].x = random(HEIGHT);
  platforms[platformNumber].y = WIDTH;
}

void resetGame() {
  player.x = WIDTH/2;
  player.y = HEIGHT / 2;
  platforms[1].x = 12;
  platforms[1].y = 10;
  platforms[2].x = 45;
  platforms[2].y = 15;
  platforms[3].x = 75;
  platforms[3].y = 84;
  platforms[4].x = 30;
  platforms[4].y = 34;
  platforms[5].x = 96;
  platforms[5].y = 17;
  playerscore = 0;
  player.jumpIndex = 0;
}

void drawPlayer() {
  uint8_t imageIndex = static_cast<uint8_t>(player.stance);
  player.image = player_images[imageIndex];
  player.mask = player_masks[imageIndex];
  Sprites::drawExternalMask(player.x, player.y - getImageHeight(player.image), player.image, player.mask, 0, 0);
}
void drawPlatforms() {
  for (uint8_t i = 0; i < NUMBER_OF_PLATFORMS; i++) {
   if (platforms[i].enabled == true) {
    uint8_t imageIndex = static_cast<uint8_t>(platforms[i].type);
    platforms[i].image = platform_images[imageIndex];
    platforms[i].mask = platform_masks[imageIndex];
    Sprites::drawExternalMask(platforms[i].x, platforms[i].y - getImageHeight(platforms[i].image), platforms[i].image, platforms[i].mask, 0, 0);
  }
 }
}
void drawScore() {
   if (playerscore == 0) {
    Sprites::drawOverwrite(122, 0, Zero, 0);
    Sprites::drawOverwrite(122, 5, Zero, 0);
   };
   if (playerscore == 1) Sprites::drawOverwrite(122, 0, One, 0);
   if (playerscore == 2) Sprites::drawOverwrite(122, 0, Two, 0);
   if (playerscore == 3) Sprites::drawOverwrite(122, 0, Three, 0);
   if (playerscore == 4) Sprites::drawOverwrite(122, 0, Four, 0);
   if (playerscore == 5) Sprites::drawOverwrite(122, 0, Five, 0);
   if (playerscore == 6) Sprites::drawOverwrite(122, 0, Six, 0);
   if (playerscore == 7) Sprites::drawOverwrite(122, 0, Seven, 0);
   if (playerscore == 8) Sprites::drawOverwrite(122, 0, Eight, 0);
   if (playerscore == 9) Sprites::drawOverwrite(122, 0, Nine, 0);
   if (playerscore == 10); {
   Sprites::drawOverwrite(122, 0, One, 0); 
   Sprites::drawOverwrite(122, 5, Zero, 0);
   };
   if (20 > playerscore > 10); { 
   Sprites::drawOverwrite(122, 0, One, 0);
      if (playerscore == 11) Sprites::drawOverwrite(122, 5, One, 0);
      if (playerscore == 12) Sprites::drawOverwrite(122, 5, Two, 0);
      if (playerscore == 13) Sprites::drawOverwrite(122, 5, Three, 0);
      if (playerscore == 11) Sprites::drawOverwrite(122, 5, Four, 0);
      if (playerscore == 12) Sprites::drawOverwrite(122, 5, Five, 0);
      if (playerscore == 13) Sprites::drawOverwrite(122, 5, Six, 0);
      if (playerscore == 11) Sprites::drawOverwrite(122, 5, Seven, 0);
      if (playerscore == 12) Sprites::drawOverwrite(122, 5, Eight, 0);
      if (playerscore == 13) Sprites::drawOverwrite(122, 5, Nine, 0);
   }
   if (playerscore == 20); {
   Sprites::drawOverwrite(122, 0, Two, 0); 
   Sprites::drawOverwrite(122, 5, Zero, 0);
   }
   if (30 > playerscore > 20); { 
   Sprites::drawOverwrite(122, 0, Two, 0);
      if (playerscore == 21) Sprites::drawOverwrite(122, 5, One, 0);
      if (playerscore == 22) Sprites::drawOverwrite(122, 5, Two, 0);
      if (playerscore == 23) Sprites::drawOverwrite(122, 5, Three, 0);
      if (playerscore == 24) Sprites::drawOverwrite(122, 5, Four, 0);
      if (playerscore == 25) Sprites::drawOverwrite(122, 5, Five, 0);
      if (playerscore == 26) Sprites::drawOverwrite(122, 5, Six, 0);
      if (playerscore == 27) Sprites::drawOverwrite(122, 5, Seven, 0);
      if (playerscore == 28) Sprites::drawOverwrite(122, 5, Eight, 0);
      if (playerscore == 29) Sprites::drawOverwrite(122, 5, Nine, 0);
   }
   if (playerscore == 30); {
   Sprites::drawOverwrite(122, 0, Three, 0); 
   Sprites::drawOverwrite(122, 5, Zero, 0);
   }
   if (40 > playerscore > 30); { 
   Sprites::drawOverwrite(122, 0, Three, 0);
      if (playerscore == 31) Sprites::drawOverwrite(122, 5, One, 0);
      if (playerscore == 32) Sprites::drawOverwrite(122, 5, Two, 0);
      if (playerscore == 33) Sprites::drawOverwrite(122, 5, Three, 0);
      if (playerscore == 34) Sprites::drawOverwrite(122, 5, Four, 0);
      if (playerscore == 35) Sprites::drawOverwrite(122, 5, Five, 0);
      if (playerscore == 36) Sprites::drawOverwrite(122, 5, Six, 0);
      if (playerscore == 37) Sprites::drawOverwrite(122, 5, Seven, 0);
      if (playerscore == 38) Sprites::drawOverwrite(122, 5, Eight, 0);
      if (playerscore == 39) Sprites::drawOverwrite(122, 5, Nine, 0);
   }
   if (playerscore == 40); {
   Sprites::drawOverwrite(122, 0, Four, 0); 
   Sprites::drawOverwrite(122, 5, Zero, 0);
   }
   if (50 > playerscore > 40); { 
   Sprites::drawOverwrite(122, 0, Four, 0);
      if (playerscore == 41) Sprites::drawOverwrite(122, 5, One, 0);
      if (playerscore == 42) Sprites::drawOverwrite(122, 5, Two, 0);
      if (playerscore == 43) Sprites::drawOverwrite(122, 5, Three, 0);
      if (playerscore == 44) Sprites::drawOverwrite(122, 5, Four, 0);
      if (playerscore == 45) Sprites::drawOverwrite(122, 5, Five, 0);
      if (playerscore == 46) Sprites::drawOverwrite(122, 5, Six, 0);
      if (playerscore == 47) Sprites::drawOverwrite(122, 5, Seven, 0);
      if (playerscore == 48) Sprites::drawOverwrite(122, 5, Eight, 0);
      if (playerscore == 49) Sprites::drawOverwrite(122, 5, Nine, 0);
   }
   if (playerscore == 50); {
   Sprites::drawOverwrite(122, 0, Five, 0); 
   Sprites::drawOverwrite(122, 5, Zero, 0);
   }
   if (60 > playerscore > 50); { 
   Sprites::drawOverwrite(122, 0, Five, 0);
      if (playerscore == 51) Sprites::drawOverwrite(122, 5, One, 0);
      if (playerscore == 52) Sprites::drawOverwrite(122, 5, Two, 0);
      if (playerscore == 53) Sprites::drawOverwrite(122, 5, Three, 0);
      if (playerscore == 54) Sprites::drawOverwrite(122, 5, Four, 0);
      if (playerscore == 55) Sprites::drawOverwrite(122, 5, Five, 0);
      if (playerscore == 56) Sprites::drawOverwrite(122, 5, Six, 0);
      if (playerscore == 57) Sprites::drawOverwrite(122, 5, Seven, 0);
      if (playerscore == 58) Sprites::drawOverwrite(122, 5, Eight, 0);
      if (playerscore == 59) Sprites::drawOverwrite(122, 5, Nine, 0);
   }if (playerscore == 60); {
   Sprites::drawOverwrite(122, 0, Six, 0); 
   Sprites::drawOverwrite(122, 5, Zero, 0);
   }
   if (70 > playerscore > 60); { 
   Sprites::drawOverwrite(122, 0, Six, 0);
      if (playerscore == 61) Sprites::drawOverwrite(122, 5, One, 0);
      if (playerscore == 62) Sprites::drawOverwrite(122, 5, Two, 0);
      if (playerscore == 63) Sprites::drawOverwrite(122, 5, Three, 0);
      if (playerscore == 64) Sprites::drawOverwrite(122, 5, Four, 0);
      if (playerscore == 65) Sprites::drawOverwrite(122, 5, Five, 0);
      if (playerscore == 66) Sprites::drawOverwrite(122, 5, Six, 0);
      if (playerscore == 67) Sprites::drawOverwrite(122, 5, Seven, 0);
      if (playerscore == 68) Sprites::drawOverwrite(122, 5, Eight, 0);
      if (playerscore == 69) Sprites::drawOverwrite(122, 5, Nine, 0);
   }
   if (playerscore == 70); {
   Sprites::drawOverwrite(122, 0, Seven, 0); 
   Sprites::drawOverwrite(122, 5, Zero, 0);
   }
   if (80 > playerscore > 70); { 
   Sprites::drawOverwrite(122, 0, Seven, 0);
      if (playerscore == 71) Sprites::drawOverwrite(122, 5, One, 0);
      if (playerscore == 72) Sprites::drawOverwrite(122, 5, Two, 0);
      if (playerscore == 73) Sprites::drawOverwrite(122, 5, Three, 0);
      if (playerscore == 74) Sprites::drawOverwrite(122, 5, Four, 0);
      if (playerscore == 75) Sprites::drawOverwrite(122, 5, Five, 0);
      if (playerscore == 76) Sprites::drawOverwrite(122, 5, Six, 0);
      if (playerscore == 77) Sprites::drawOverwrite(122, 5, Seven, 0);
      if (playerscore == 78) Sprites::drawOverwrite(122, 5, Eight, 0);
      if (playerscore == 79) Sprites::drawOverwrite(122, 5, Nine, 0);
   }
   if (playerscore == 80); {
   Sprites::drawOverwrite(122, 0, Eight, 0); 
   Sprites::drawOverwrite(122, 5, Zero, 0);
   }
   if (90 > playerscore > 80); { 
   Sprites::drawOverwrite(122, 0, Eight, 0);
      if (playerscore == 81) Sprites::drawOverwrite(122, 5, One, 0);
      if (playerscore == 82) Sprites::drawOverwrite(122, 5, Two, 0);
      if (playerscore == 83) Sprites::drawOverwrite(122, 5, Three, 0);
      if (playerscore == 84) Sprites::drawOverwrite(122, 5, Four, 0);
      if (playerscore == 85) Sprites::drawOverwrite(122, 5, Five, 0);
      if (playerscore == 86) Sprites::drawOverwrite(122, 5, Six, 0);
      if (playerscore == 87) Sprites::drawOverwrite(122, 5, Seven, 0);
      if (playerscore == 88) Sprites::drawOverwrite(122, 5, Eight, 0);
      if (playerscore == 89) Sprites::drawOverwrite(122, 5, Nine, 0);
   }
   if (playerscore == 90); {
   Sprites::drawOverwrite(122, 0, Nine, 0); 
   Sprites::drawOverwrite(122, 5, Zero, 0);
   }
   if (100 > playerscore > 90) { 
   Sprites::drawOverwrite(122, 0, Nine, 0);
      if (playerscore == 91) Sprites::drawOverwrite(122, 5, One, 0);
      if (playerscore == 92) Sprites::drawOverwrite(122, 5, Two, 0);
      if (playerscore == 93) Sprites::drawOverwrite(122, 5, Three, 0);
      if (playerscore == 94) Sprites::drawOverwrite(122, 5, Four, 0);
      if (playerscore == 95) Sprites::drawOverwrite(122, 5, Five, 0);
      if (playerscore == 96) Sprites::drawOverwrite(122, 5, Six, 0);
      if (playerscore == 97) Sprites::drawOverwrite(122, 5, Seven, 0);
      if (playerscore == 98) Sprites::drawOverwrite(122, 5, Eight, 0);
      if (playerscore == 99) Sprites::drawOverwrite(122, 5, Nine, 0);
   }
   if (playerscore == 100) {
   Sprites::drawOverwrite(122, 0, One, 0); 
   Sprites::drawOverwrite(122, 5, Zero, 0);
   Sprites::drawOverwrite(122, 10, Zero, 0);
   }
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
      Sprites::drawOverwrite(WIDTH/2 -7, 15, TITLEsub, 0);
      if (arduboy.justPressed(A_BUTTON)) {
        gamestate = 1;
      }
      break;

    case 1:
      //gameplay
      arduboy.clear();
      //gameover if player touches bottom
      if (player.x <= 0) {
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
        player.x = player.x - 2;
      }

      //if player touches platform, jump! 
     if (player.jumping == false && (collision())) {
        player.jumping = true;
        player.jumpIndex++;
        }
        
     if (platformLaunchCountdown == true) {
      for (uint8_t i = 0; i < NUMBER_OF_PLATFORMS; i++) {
        if (!platforms[i].enabled) {
          launchPlatform(i);
          break;
        }
      }
      platformLaunchCountdown == false;
     }
     
      updateArdudle();
      updatePlatforms();
      drawPlatforms();
      drawPlayer();
      drawScore();
      
      break;

    case 2:
      //gameover
      arduboy.clear();
      /*if (playerscore > highscore) {
        highscore = playerscore;
        EEPROM.put(EEPROM_SCORE, highscore);
     }
     */
      if (arduboy.justPressed(A_BUTTON)) {
        gamestate = 3;
      }
      Sprites::drawOverwrite(WIDTH/2, 10, GAME, 0);
      Sprites::drawOverwrite(WIDTH/2 - 14, 10, OVER, 0);
      break;

    case 3:
      //highscore
      if (arduboy.justPressed(A_BUTTON)) {
        gamestate = 0;
      }
      Sprites::drawOverwrite(WIDTH/2 + 12, 0, HIGHSCORE, 0);
      Sprites::drawOverwrite(WIDTH/2, 0, One, 0);
      Sprites::drawOverwrite(WIDTH/2 -7, 0, Two, 0);
      Sprites::drawOverwrite(WIDTH/2 -14, 0, Three, 0);
      Sprites::drawOverwrite(WIDTH/2 -21, 0, Four, 0);
      Sprites::drawOverwrite(WIDTH/2 -28, 0, Five, 0);
      Sprites::drawOverwrite(WIDTH/2 -35, 0, Six, 0);
      Sprites::drawOverwrite(WIDTH/2 -42, 0, Seven, 0);
      Sprites::drawOverwrite(WIDTH/2 -49, 0, Eight, 0);
      Sprites::drawOverwrite(WIDTH/2 -56, 0, Nine, 0);
      Sprites::drawOverwrite(WIDTH/2 -63, 0, One, 0);
      Sprites::drawOverwrite(WIDTH/2 -63, 4, Zero, 0);
      break;

  }
  arduboy.display();
}


bool collision () {
  for (uint8_t i = 0; i < NUMBER_OF_PLATFORMS; i++) {
    if (platforms[i].enabled == true) {
      if (collide(player.x, player.y -getImageHeight(player.image), player.image, platforms[i].x, platforms[i].y - getImageHeight(platforms[i].image), platforms[i].image)) {
        return true; 
      }
    }
  }
  return false;
}
