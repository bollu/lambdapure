func @main(%p: !lambdapure.Object) -> !lambdapure.Object {
  %a = "lambdapure.ConstructorOp"(){tag=1} : () -> !lambdapure.Object
  %pIsLargeEnough = "lambdapure.ProjectionOp"(%p){index=1}: (!lambdapure.Object) -> !lambdapure.Object
  return %a : !lambdapure.Object
}
