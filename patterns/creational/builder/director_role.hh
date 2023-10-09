#pragma once

#include "builder_role.hh"

namespace prb17 {
    namespace utils {	
        namespace patterns {
            namespace creational {
                namespace builder {
                    class director_role {
                        private:

                        protected:

                        public:
                            virtual void construct(builder_role *) = 0;
                    };
                }                
            }
        }
    }
}