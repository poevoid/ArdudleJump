enum class Stance : uint8_t {
  Left,
  Right,
};
enum class Type : uint8_t {
  Solid,
  Breakable,
  Timer,
};
struct PlatformS {
  int x;
  int y;
  Type type;
  uint8_t jumpIndex;
  const uint8_t *image;
  const uint8_t *mask;
};
struct PlatformM {
  int x;
  int y;
  Type type;
  uint8_t jumpIndex;
  const uint8_t *image;
  const uint8_t *mask;
};
struct PlatformL {
  int x;
  int y;
  Type type;
  uint8_t jumpIndex;
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
