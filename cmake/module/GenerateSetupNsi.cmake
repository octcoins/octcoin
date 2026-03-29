# Copyright (c) 2023-present The Octcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or https://opensource.org/license/mit/.

function(generate_setup_nsi)
  set(abs_top_srcdir ${PROJECT_SOURCE_DIR})
  set(abs_top_builddir ${PROJECT_BINARY_DIR})
  set(CLIENT_URL ${PROJECT_HOMEPAGE_URL})
  set(CLIENT_TARNAME "octcoin")
  set(OCTCOIN_WRAPPER_NAME "octcoin")
  set(OCTCOIN_GUI_NAME "octcoin-qt")
  set(OCTCOIN_DAEMON_NAME "octcoind")
  set(OCTCOIN_CLI_NAME "octcoin-cli")
  set(OCTCOIN_TX_NAME "octcoin-tx")
  set(OCTCOIN_WALLET_TOOL_NAME "octcoin-wallet")
  set(OCTCOIN_TEST_NAME "test_octcoin")
  set(EXEEXT ${CMAKE_EXECUTABLE_SUFFIX})
  configure_file(${PROJECT_SOURCE_DIR}/share/setup.nsi.in ${PROJECT_BINARY_DIR}/octcoin-win64-setup.nsi USE_SOURCE_PERMISSIONS @ONLY)
endfunction()
