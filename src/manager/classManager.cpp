#include "manager/classManager.h"

#include "model/entity/classType/classType.h"

namespace ClassManager
{
    std::vector<ClassType*> getClasses()
    {
        return {
            new Mage(),
            new Thief(),
            new Slime()
        };
    }

    ClassType* getClass(const std::string& id)
    {
        for (ClassType* classType: getClasses())
        {
            if (classType->id == id)
            {
                return classType;
            }
        }

        return nullptr;
    }
}