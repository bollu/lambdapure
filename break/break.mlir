func @main(%p: !lambdapure.Object) -> !lambdapure.Object {
  %pIsLargeEnough = "lambdapure.ProjectionOp"(%p){index=1}: (!lambdapure.Object) -> !lambdapure.Object
  %a = "lambdapure.ConstructorOp"(){tag=1} : () -> !lambdapure.Object
  "lambdapure.ReturnOp"(%p): (!lambdapure.Object) -> ()
}
