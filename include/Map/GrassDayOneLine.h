#ifndef GRASSDAYONELINE_H
#define GRASSDAYONELINE_H

#include "MapInitilizer.h"

class GrassDayOneLine :public MapInitilizer {
protected:
    
public:
    GrassDayOneLine(
        std::shared_ptr<TextureRes> res
    );
    
    virtual ~GrassDayOneLine();
};

#endif