
// Author: Christoph Lassner.
/**
 * This is an automatically generated file!
 */
#include "../global.h"
#include "../matlab_headers/__data_providers.h"
#include <../../include/fertilized/data_providers/iforestdataprovider.h>

using namespace fertilized;


// Destructor.
void delete_IForestDataProvider_d_uint(void *ptr) {
  auto * storage = static_cast<std::shared_ptr<IForestDataProvider<double,uint>>*>(ptr);
  delete storage;
}
