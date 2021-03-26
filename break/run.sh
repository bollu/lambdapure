#!/usr/bin/env bash
make -C ~/work/lambdapure/build -j && ~/work/lambdapure/build/bin/lambdapure $@
make -C ~/work/lambdapure/build -j && ~/work/lambdapure/build/bin/lambdapure --des $@
