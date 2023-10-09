#pragma once

#include "graph_builder.hh"
#include "director_role.hh"

namespace prb17 {
    namespace utils {
        namespace structures {
            class graph_director : public prb17::utils::patterns::creational::builder::director_role {
                private:

                protected:

                public:
                    void construct(prb17::utils::patterns::creational::builder::builder_role *) override;
            };

            void graph_director::construct(prb17::utils::patterns::creational::builder::builder_role *b) {
                b->build();
            }
        }
    }
}