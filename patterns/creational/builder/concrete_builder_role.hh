#pragma once

namespace prb17 {
    namespace utils {	
        namespace patterns {
            namespace creational {
                namespace builder {
                    class concrete_builder_role {
                        private:

                        protected:
                            void *product;

                        public:
                            virtual void* get_product();
                    };

                    void* concrete_builder_role::get_product() {
                        return product;
                    }
                }                
            }
        }
    }
}