#pragma once
#include "inc.h"

namespace unreal
{
    class c_actors : public c_singleton<c_actors>
    {
    public:
        std::vector<CG::AMM_NetPC*>
                                    get_actors();
        std::vector<CG::AMM_GatherObject*>
                                    get_gather_actors();
        CG::AMM_MainPC*             get_main_pc();
    public:
        void                        teleport(CG::FVector pos);
    };
}