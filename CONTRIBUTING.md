Contributing to Octcoin Core
============================

The Octcoin Core project operates an open contributor model where anyone is
welcome to contribute in the form of peer review, testing and patches.

Getting Started
---------------

New contributors are very welcome. Reviewing and testing is the most effective
way to contribute as a new contributor.

Before contributing, familiarize yourself with the Octcoin Core build system
and tests. There are many open issues waiting to be fixed at
https://github.com/octcoins/octcoin/issues.

Communication
-------------

All development discussion happens on GitHub issues and pull requests at
https://github.com/octcoins/octcoin.

Contributor Workflow
--------------------

Everyone contributes via pull requests (PRs):

  1. Fork the repository
  2. Create a topic branch
  3. Commit your patches
  4. Push to your fork
  5. Open a pull request against `master`

### Commit Messages

- Short subject line (50 chars max)
- Explain the *why*, not just the *what*
- Reference issues: `fixes #1234`
- No `@` mentions in commit messages

### Pull Request Title Prefixes

  - `consensus` — consensus critical code
  - `qt` / `gui` — GUI changes
  - `mining` — mining code
  - `net` / `p2p` — peer-to-peer network
  - `rpc` — RPC/REST APIs
  - `wallet` — wallet code
  - `build` — CMake/build system
  - `doc` — documentation
  - `test` — unit/functional tests
  - `refactor` — structural changes only

Peer Review
-----------

Anyone may participate in peer review via pull request comments.

- `Concept ACK` — agree with the goal
- `Concept NACK` — disagree (must include reasoning)
- `ACK` — reviewed and tested, ready to merge
- "nit" — minor non-blocking issue

Patches changing consensus-critical code require extensive discussion and
a higher standard of review before merging.

Copyright
---------

By contributing, you agree to license your work under the MIT license.