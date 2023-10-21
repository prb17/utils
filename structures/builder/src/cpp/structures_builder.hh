#pragma once

#include "builder_role.hh"

namespace prb17 {
    namespace utils {
        namespace structures {
            class structures_builder : public prb17::utils::patterns::creational::builder::builder_role {
                private:

                protected:

                public:
                    virtual void build() = 0;
            };
        }
    }
}