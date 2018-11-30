#ifndef SYNTHASSEMBLAGE_H
#define SYNTHASSEMBLAGE_H
#include "EntityComponentManager.h"
#include <memory>


class SynthAssemblage
{
    public:
        SynthAssemblage();
        virtual ~SynthAssemblage();

        static std::shared_ptr<int> Create(ECS::EntityComponentManager &ecs, int x, int y);
};

#endif // SYNTHASSEMBLAGE_H
