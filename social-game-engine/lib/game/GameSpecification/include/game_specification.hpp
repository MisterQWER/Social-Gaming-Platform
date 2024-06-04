#ifndef GAME_SPECIFICATION_H
#define GAME_SPECIFICATION_H
#include "Configuration.h"

// configuration {}
// constants {}
// variables {}
// per-player {}
// per-audience {}
// rules {}

class GameSpecification {
    public:
        GameSpecification();
        
        virtual ~GameSpecification() = default;
    private:
        Configuration game_configuration_;
};

#endif  // GAME_SPECIFICATION_H