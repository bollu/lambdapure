#include <iostream>
#include <new>
#include <type_traits>

#include "mlir/IR/AsmState.h"
#include "mlir/InitAllDialects.h"
#include "mlir/InitAllPasses.h"
#include "mlir/Pass/PassManager.h"
#include "mlir/Support/LogicalResult.h"
#include "mlir/Support/MlirOptMain.h"

#include "lambdapure/Dialect.h"
#include "lambdapure/Lexer.h"
#include "lambdapure/MLIRGen.h"
#include "lambdapure/Parser.h"
#include "lambdapure/Passes.h"
#include "lambdapure/TranslateToCpp.h"

using namespace lambdapure;
namespace cl = llvm::cl;

// Parse Commandline
//----------------------------------------------------------------------------
static cl::opt<std::string> inputFilename(cl::Positional,
                                          cl::desc("<input lambdapure file>"),
                                          cl::Required,
                                          cl::value_desc("filename"));

static cl::opt<std::string> outputFilename("o", cl::init("a"),
                                           cl::desc("Specify output filename"),
                                           cl::value_desc("filename"));

static cl::opt<bool> dumpAST("ast", cl::desc("AST dump"), cl::init(false));

static cl::opt<bool> dumpMLIR("mlir", cl::desc("MLIR dump"), cl::init(false));

static cl::opt<bool> runtimeLowering("runtime", cl::desc("Runtime dump"),
                                     cl::init(false));

static cl::opt<bool> refCount("ref", cl::desc("Ref Count"), cl::init(false));

static cl::opt<bool> desUpdates("des", cl::desc("DestructiveUpdates"),
                                cl::init(false));

//----------------------------------------------------------------------------

std::unique_ptr<ModuleAST> parseInputFile(llvm::StringRef inputFilename) {
  llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> fileOrErr =
      llvm::MemoryBuffer::getFileOrSTDIN(inputFilename);
  if (std::error_code EC = fileOrErr.getError()) {
    llvm::errs() << "Could not open input file: " << EC.message() << "\n";
    return nullptr;
  }
  if (!llvm::StringRef(inputFilename).endswith(".lambdapure")) {
    llvm::errs() << "Input filetype does not end with lambdapure"
                 << "\n";
    return nullptr;
  }
  llvm::StringRef buffer = fileOrErr.get()->getBuffer();
  std::string filename = inputFilename.str();
  Lexer lexer = Lexer(filename, buffer);
  Parser parser = Parser(lexer);
  return parser.parse();
}

int main(int argc, char **argv) {
  cl::ParseCommandLineOptions(argc, argv);
  mlir::OwningModuleRef module;

  auto ast = parseInputFile(inputFilename);
  if (!ast) {
    return -1;
  }
  if (dumpAST) {
    ast->print();
    return 0;
  }

  mlir::MLIRContext context;
  context.loadDialect<::mlir::lambdapure::LambdapureDialect>();
  // mlir::DialectRegistry registry;
  // registerAllDialects(registry);
  // registry.insert<::mlir::lambdapure::LambdapureDialect>();

  module = mlirGen(context, *ast);
  mlir::PassManager pm(&context);
  if (dumpMLIR) {
    module->dump();
  }


  if (desUpdates) {
    pm.addPass(mlir::lambdapure::createDestructiveUpdatePattern());
  }

  if (refCount) {
    pm.addPass(mlir::lambdapure::createReferenceRewriterPattern());
  }

  if (runtimeLowering) {
    pm.addPass(mlir::lambdapure::createLambdapureToLeanLowering());
  }

  mlir::LogicalResult runResult = pm.run(*module);
  assert(mlir::succeeded(runResult));
  if (runtimeLowering) {
    auto m = *module;
    lambdapure::translate(m);
  }
  if (runtimeLowering || refCount || desUpdates)
    module->dump();

  return 0;
}

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

// int main(int argc, char **argv) {
//   mlir::DialectRegistry registry;
//   registerAllDialects(registry);
//   registry.insert<::mlir::lambdapure::LambdapureDialect>();
//
//   mlir::registerAllPasses();
//   ::mlir::registerPass(
//       "lambdapure-des-upd", "Lambdapure Destructive Update Pattern",
//       []() -> std::unique_ptr<::mlir::Pass> {
//         return mlir::lambdapure::createDestructiveUpdatePattern();
//       });
//   ::mlir::registerPass(
//       "lambdapure-ref-rewrite", "Lamdbapure Reference Rewriter Pattern",
//       []() -> std::unique_ptr<::mlir::Pass> {
//         return mlir::lambdapure::createReferenceRewriterPattern();
//       });
//   ::mlir::registerPass(
//       "convert-lambdapure-to-lean", "Lambdapure to Lean Lowering",
//       []() -> std::unique_ptr<::mlir::Pass> {
//         return mlir::lambdapure::createLambdapureToLeanLowering();
//       });
//
//   return failed(
//       mlir::MlirOptMain(argc, argv, "lambdapure dialect driver\n",
//       registry));
// }
