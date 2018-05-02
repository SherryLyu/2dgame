#include <vector>
#include <SDL.h>
#include "start.h"

class StartEngine {
public:
  StartEngine ();
  ~StartEngine ();
  bool play();

private:
  Clock& clock;
  SDL_Renderer * const renderer;
  Start start;
  int lastoption;

  void draw() const;
  void update(Uint32);

  StartEngine(const StartEngine&);
  StartEngine& operator=(const StartEngine&);
};
