enum class Stance : uint8_t {
  Left,
  Right,
};
enum class Type : uint8_t {
  Small,
  Medium,
  Large,
  Count_All_Platforms = 3
};

struct Platform {
  int x;
  int y;
  Type type;
  bool enabled;
  const uint8_t *image;
  const uint8_t *mask;
};

struct Player {
  int x;
  int y;
  Stance stance;
  bool jumping;
  uint8_t jumpIndex;
  const uint8_t *image;
  const uint8_t *mask;
};
