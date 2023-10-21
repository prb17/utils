#pragma once

#include "structures_builder.hh"
#include "director_role.hh"

namespace prb17 {
    namespace utils {
        namespace structures {
            class structures_director : public prb17::utils::patterns::creational::builder::director_role {
                private:

                protected:

                public:
                    void construct(prb17::utils::patterns::creational::builder::builder_role *) override;
            };

            void structures_director::construct(prb17::utils::patterns::creational::builder::builder_role *b) {
                b->build();
            }
        }
    }
}