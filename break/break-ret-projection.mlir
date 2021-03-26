vvvMLIR (initial)vvv
module  {
  func @crash(%arg0: !lambdapure.Object) -> !lambdapure.Object {
    %0 = "lambdapure.ConstructorOp"() {tag = 0 : i64} : () -> !lambdapure.Object
    %1 = "lambdapure.ProjectionOp"(%arg0) {index = 4 : i64} : (!lambdapure.Object) -> !lambdapure.Object
    "lambdapure.ReturnOp"(%1) : (!lambdapure.Object) -> ()
  }
}
^^^MLIR (initial)^^^
vvvMLIR (final)vvv
module  {
  func @crash(%arg0: !lambdapure.Object) -> !lambdapure.Object {
    %0 = "lambdapure.ConstructorOp"() {tag = 0 : i64} : () -> !lambdapure.Object
    %1 = "lambdapure.ProjectionOp"(%arg0) {index = 4 : i64} : (!lambdapure.Object) -> !lambdapure.Object
    "lambdapure.ReturnOp"(%1) : (!lambdapure.Object) -> ()
  }
}
