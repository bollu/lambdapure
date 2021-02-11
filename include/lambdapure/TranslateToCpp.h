#ifndef LAMBDAPURE_TRANSLATE
#define LAMBDAPURE_TRANSLATE

#include "mlir/IR/BuiltinOps.h"

namespace lambdapure {
void translate(mlir::ModuleOp &module);
} // namespace lambdapure

#endif // LAMBDAPURE_TRANSLATE
