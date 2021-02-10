#include <type_traits>
#include <new>
#include <iostream>

#include "mlir/IR/AsmState.h"
#include "mlir/InitAllDialects.h"
#include "mlir/InitAllPasses.h"
#include "mlir/Support/MlirOptMain.h"

#include "lambdapure/Dialect.h"
#include "lambdapure/MLIRGen.h"
#include "lambdapure/Parser.h"
#include "lambdapure/Lexer.h"
#include "lambdapure/Passes.h"
#include "lambdapure/TranslateToCpp.h"

using namespace lambdapure;

//int main(int argc, char **argv){
//  mlir::MLIRContext context;
//
//  module = mlirGen(context, *ast);
//  mlir::PassManager pm(&context);
//  if(dumpMLIR)
//    module -> dump();
//
//  if(desUpdates){
//    pm.addPass(mlir::lambdapure::createDestructiveUpdatePattern());
//  }
//
//  if(refCount ){
//    pm.addPass(mlir::lambdapure::createReferenceRewriterPattern());
//  }
//
//
//
//  if(runtimeLowering){
//    pm.addPass(mlir::lambdapure::createLambdapureToLeanLowering());
//  }
//
//
//
//
//  pm.run(*module);
//  if(runtimeLowering){
//    auto m = *module;
//    lambdapure::translate(m);
//  }
//  if(runtimeLowering || refCount || desUpdates)
//    module -> dump();
//
//  return 0;
//}

int main(int argc, char **argv) {
  mlir::DialectRegistry registry;
  registerAllDialects(registry);
  registry.insert<::mlir::lambdapure::LambdapureDialect>();

  mlir::registerAllPasses();
  ::mlir::registerPass("lambdapure-des-upd", "Lambdapure Destructive Update Pattern",
                       []() -> std::unique_ptr<::mlir::Pass> {
                         return mlir::lambdapure::createDestructiveUpdatePattern();
                       });
  ::mlir::registerPass("lambdapure-ref-rewrite", "Lamdbapure Reference Rewriter Pattern",
                       []() -> std::unique_ptr<::mlir::Pass> {
                         return mlir::lambdapure::createReferenceRewriterPattern();
                       });
  ::mlir::registerPass("convert-lambdapure-to-lean", "Lambdapure to Lean Lowering",
                       []() -> std::unique_ptr<::mlir::Pass> {
                         return mlir::lambdapure::createLambdapureToLeanLowering();
                       });

  return failed(
    mlir::MlirOptMain(argc, argv, "Quantum dialect driver\n", registry));
}
