
#include "worldobject.h"

class Spline : public WorldObject {
    OBJ_DEFINITION(Spline, WorldObject);

   public:
    Spline();
    virtual ~Spline();

    static void bind_methods();
};
