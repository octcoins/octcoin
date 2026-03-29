# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *liboctcoin_cli*         | RPC client functionality used by *octcoin-cli* executable |
| *liboctcoin_common*      | Home for common functionality shared by different executables and libraries. Similar to *liboctcoin_util*, but higher-level (see [Dependencies](#dependencies)). |
| *liboctcoin_consensus*   | Consensus functionality used by *liboctcoin_node* and *liboctcoin_wallet*. |
| *liboctcoin_crypto*      | Hardware-optimized functions for data encryption, hashing, message authentication, and key derivation. |
| *liboctcoin_kernel*      | Consensus engine and support library used for validation by *liboctcoin_node*. |
| *liboctcoinqt*           | GUI functionality used by *octcoin-qt* and *octcoin-gui* executables. |
| *liboctcoin_ipc*         | IPC functionality used by *octcoin-node* and *octcoin-gui* executables to communicate when [`-DENABLE_IPC=ON`](multiprocess.md) is used. |
| *liboctcoin_node*        | P2P and RPC server functionality used by *octcoind* and *octcoin-qt* executables. |
| *liboctcoin_util*        | Home for common functionality shared by different executables and libraries. Similar to *liboctcoin_common*, but lower-level (see [Dependencies](#dependencies)). |
| *liboctcoin_wallet*      | Wallet functionality used by *octcoind* and *octcoin-wallet* executables. |
| *liboctcoin_wallet_tool* | Lower-level wallet functionality used by *octcoin-wallet* executable. |
| *liboctcoin_zmq*         | [ZeroMQ](../zmq.md) functionality used by *octcoind* and *octcoin-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. An exception is *liboctcoin_kernel*, which, at some future point, will have a documented external interface.

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`add_library(octcoin_* ...)`](../../src/CMakeLists.txt) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *liboctcoin_node* code lives in `src/node/` in the `node::` namespace
  - *liboctcoin_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *liboctcoin_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *liboctcoin_util* code lives in `src/util/` in the `util::` namespace
  - *liboctcoin_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "basis" }}}%%

graph TD;

octcoin-cli[octcoin-cli]-->liboctcoin_cli;

octcoind[octcoind]-->liboctcoin_node;
octcoind[octcoind]-->liboctcoin_wallet;

octcoin-qt[octcoin-qt]-->liboctcoin_node;
octcoin-qt[octcoin-qt]-->liboctcoinqt;
octcoin-qt[octcoin-qt]-->liboctcoin_wallet;

octcoin-wallet[octcoin-wallet]-->liboctcoin_wallet;
octcoin-wallet[octcoin-wallet]-->liboctcoin_wallet_tool;

liboctcoin_cli-->liboctcoin_util;
liboctcoin_cli-->liboctcoin_common;

liboctcoin_consensus-->liboctcoin_crypto;

liboctcoin_common-->liboctcoin_consensus;
liboctcoin_common-->liboctcoin_crypto;
liboctcoin_common-->liboctcoin_util;

liboctcoin_kernel-->liboctcoin_consensus;
liboctcoin_kernel-->liboctcoin_crypto;
liboctcoin_kernel-->liboctcoin_util;

liboctcoin_node-->liboctcoin_consensus;
liboctcoin_node-->liboctcoin_crypto;
liboctcoin_node-->liboctcoin_kernel;
liboctcoin_node-->liboctcoin_common;
liboctcoin_node-->liboctcoin_util;

liboctcoinqt-->liboctcoin_common;
liboctcoinqt-->liboctcoin_util;

liboctcoin_util-->liboctcoin_crypto;

liboctcoin_wallet-->liboctcoin_common;
liboctcoin_wallet-->liboctcoin_crypto;
liboctcoin_wallet-->liboctcoin_util;

liboctcoin_wallet_tool-->liboctcoin_wallet;
liboctcoin_wallet_tool-->liboctcoin_util;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class octcoin-qt,octcoind,octcoin-cli,octcoin-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Crypto* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus, crypto, and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *liboctcoin_wallet* and *liboctcoin_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *liboctcoin_crypto* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *liboctcoin_consensus* should only depend on *liboctcoin_crypto*, and all other libraries besides *liboctcoin_crypto* should be allowed to depend on it.

- *liboctcoin_util* should be a standalone dependency that any library can depend on, and it should not depend on other libraries except *liboctcoin_crypto*. It provides basic utilities that fill in gaps in the C++ standard library and provide lightweight abstractions over platform-specific features. Since the util library is distributed with the kernel and is usable by kernel applications, it shouldn't contain functions that external code shouldn't call, like higher level code targeted at the node or wallet. (*liboctcoin_common* is a better place for higher level code, or code that is meant to be used by internal applications only.)

- *liboctcoin_common* is a home for miscellaneous shared code used by different Octcoin Core applications. It should not depend on anything other than *liboctcoin_util*, *liboctcoin_consensus*, and *liboctcoin_crypto*.

- *liboctcoin_kernel* should only depend on *liboctcoin_util*, *liboctcoin_consensus*, and *liboctcoin_crypto*.

- The only thing that should depend on *liboctcoin_kernel* internally should be *liboctcoin_node*. GUI and wallet libraries *liboctcoinqt* and *liboctcoin_wallet* in particular should not depend on *liboctcoin_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be able to get it from *liboctcoin_consensus*, *liboctcoin_common*, *liboctcoin_crypto*, and *liboctcoin_util*, instead of *liboctcoin_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *liboctcoinqt*, *liboctcoin_node*, *liboctcoin_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](../../src/interfaces/) abstract interfaces.

## Work in progress

- Validation code is moving from *liboctcoin_node* to *liboctcoin_kernel* as part of [The liboctcoinkernel Project #27587]( https://github.com/octcoins/octcoin/issues/27587)
