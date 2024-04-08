#ifndef GRASSDAYONELINE_H
#define GRASSDAYONELINE_H

#include "MapInitilizer.h"

class GrassDayOneLine :public MapInitilizer {
protected:
    
public:
    GrassDayOneLine();

    virtual int initilizeMapTemplate() override;

    virtual ~GrassDayOneLine();
};

#endif