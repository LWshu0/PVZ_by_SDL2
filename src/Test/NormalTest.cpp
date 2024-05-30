#include <iostream>
#include <fstream>

#include "Core/ObjectPool.h"

class MyObjectFactory :public ObjectFactory<int> {
public:
    virtual std::shared_ptr<int> create() override
    {
        return std::make_shared<int>(1);
    }
};


int main()
{
    std::shared_ptr<MyObjectFactory> myObjectFactory = std::make_shared<MyObjectFactory>();

    ObjectPool<int> pool(myObjectFactory, 4);

    auto pp = pool.getReusable();
    std::cout << "pp value: " << *pp << std::endl;
    pool.returnReusable(pp);

    std::cout << "main: " << pp.use_count() << "value:" << pp.get() << std::endl;
}