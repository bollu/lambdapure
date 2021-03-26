func @main(%p: !lambdapure.Object) -> !lambdapure.Object {
  %a = "lambdapure.ConstructorOp"(){tag=1} : () -> !lambdapure.Object
  %a1 = "lambdapure.ProjectionOp"(%a){index=1}: (!lambdapure.Object) -> !lambdapure.Object
  return %a : !lambdapure.Object
}
