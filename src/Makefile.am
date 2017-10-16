DIST_SUBDIRS = secp256k1 univalue

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS)
AM_CXXFLAGS = $(HARDENED_CXXFLAGS)
AM_CPPFLAGS = $(HARDENED_CPPFLAGS)

if EMBEDDED_LEVELDB
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/include
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/helpers/memenv
LIBLEVELDB += $(builddir)/leveldb/libleveldb.a
LIBMEMENV  += $(builddir)/leveldb/libmemenv.a

# NOTE: This dependency is not strictly necessary, but without it make may try to build both in parallel, which breaks the LevelDB build system in a race
$(LIBLEVELDB): $(LIBMEMENV)

$(LIBLEVELDB) $(LIBMEMENV):
	@echo "Building LevelDB ..." && $(MAKE) -C $(@D) $(@F) CXX="$(CXX)" \
	  CC="$(CC)" PLATFORM=$(TARGET_OS) AR="$(AR)" $(LEVELDB_TARGET_FLAGS) \
          OPT="$(AM_CXXFLAGS) $(PIE_FLAGS) $(CXXFLAGS) $(AM_CPPFLAGS) $(CPPFLAGS) -D__STDC_LIMIT_MACROS"
endif

CIVILBIT_CONFIG_INCLUDES=-I$(builddir)/config
CIVILBIT_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

CIVILBIT_INCLUDES += -I$(srcdir)/secp256k1/include
CIVILBIT_INCLUDES += -I$(srcdir)/univalue/include

LIBCIVILBIT_SERVER=libcivilbit_server.a
LIBCIVILBIT_WALLET=libcivilbit_wallet.a
LIBCIVILBIT_COMMON=libcivilbit_common.a
LIBCIVILBIT_CLI=libcivilbit_cli.a
LIBCIVILBIT_UTIL=libcivilbit_util.a
LIBCIVILBIT_CRYPTO=crypto/libcivilbit_crypto.a
LIBCIVILBITQT=qt/libcivilbitqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la
LIBUNIVALUE=univalue/libunivalue.la

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)
  
$(LIBUNIVALUE): $(wildcard univalue/lib/*) $(wildcard univalue/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES = \
  crypto/libcivilbit_crypto.a \
  libcivilbit_util.a \
  libcivilbit_common.a \
  libcivilbit_server.a \
  libcivilbit_cli.a
if ENABLE_WALLET
CIVILBIT_INCLUDES += $(BDB_CPPFLAGS)
EXTRA_LIBRARIES += libcivilbit_wallet.a
endif
if ENABLE_ZMQ
EXTRA_LIBRARIES += libcivilbit_zmq.a
endif

if BUILD_CIVILBIT_LIBS
lib_LTLIBRARIES = libcivilbitconsensus.la
LIBCIVILBIT_CONSENSUS=libcivilbitconsensus.la
else
LIBCIVILBIT_CONSENSUS=
endif

bin_PROGRAMS =
TESTS =
BENCHMARKS =

if BUILD_CIVILBITD
  bin_PROGRAMS += civilbitd
endif

if BUILD_CIVILBIT_UTILS
  bin_PROGRAMS += civilbit-cli civilbit-tx
if ENABLE_WALLET
  bin_PROGRAMS += wallet-utility
endif
endif

.PHONY: FORCE check-symbols check-security
# civilbit core #
CIVILBIT_CORE_H = \
  addressindex.h \
  spentindex.h \
  addrman.h \
  alert.h \
  amount.h \
  arith_uint256.h \
  base58.h \
  bloom.h \
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coincontrol.h \
  coins.h \
  compat.h \
  compat/byteswap.h \
  compat/endian.h \
  compat/sanity.h \
  compressor.h \
  consensus/consensus.h \
  consensus/merkle.h \
  consensus/params.h \
  consensus/validation.h \
  core_io.h \
  core_memusage.h \
  hash.h \
  httprpc.h \
  httpserver.h \
  init.h \
  key.h \
  keystore.h \
  dbwrapper.h \
  limitedmap.h \
  main.h \
  memusage.h \
  merkleblock.h \
  miner.h \
  net.h \
  netbase.h \
  noui.h \
  policy/fees.h \
  policy/policy.h \
  policy/rbf.h \
  pow.h \
  prevector.h \
  primitives/block.h \
  primitives/transaction.h \
  protocol.h \
  pubkey.h \
  random.h \
  reverselock.h \
  rpcclient.h \
  rpcprotocol.h \
  rpcserver.h \
  scheduler.h \
  script/interpreter.h \
  script/script.h \
  script/script_error.h \
  script/sigcache.h \
  script/sign.h \
  script/standard.h \
  serialize.h \
  streams.h \
  support/allocators/secure.h \
  support/allocators/zeroafterfree.h \
  support/cleanse.h \
  support/pagelocker.h \
  sync.h \
  threadsafety.h \
  timedata.h \
  tinyformat.h \
  torcontrol.h \
  txdb.h \
  txmempool.h \
  ui_interface.h \
  uint256.h \
  undo.h \
  util.h \
  utilmoneystr.h \
  utilstrencodings.h \
  utiltime.h \
  validationinterface.h \
  version.h \
  versionbits.h \
  wallet/crypter.h \
  wallet/db.h \
  wallet/wallet.h \
  wallet/wallet_ismine.h \
  wallet/walletdb.h \
  zmq/zmqabstractnotifier.h \
  zmq/zmqconfig.h\
  zmq/zmqnotificationinterface.h \
  zmq/zmqpublishnotifier.h


obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
	  $(abs_top_srcdir)
libcivilbit_util_a-clientversion.$(OBJEXT): obj/build.h

# server: shared between civilbitd and civilbit-qt
libcivilbit_server_a_CPPFLAGS = $(AM_CPPFLAGS) $(CIVILBIT_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libcivilbit_server_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libcivilbit_server_a_SOURCES = \
  addrman.cpp \
  alert.cpp \
  bloom.cpp \
  chain.cpp \
  checkpoints.cpp \
  httprpc.cpp \
  httpserver.cpp \
  init.cpp \
  dbwrapper.cpp \
  main.cpp \
  merkleblock.cpp \
  miner.cpp \
  net.cpp \
  noui.cpp \
  policy/fees.cpp \
  policy/policy.cpp \
  pow.cpp \
  rest.cpp \
  rpcblockchain.cpp \
  rpcmining.cpp \
  rpcmisc.cpp \
  rpcnet.cpp \
  rpcrawtransaction.cpp \
  rpcserver.cpp \
  script/sigcache.cpp \
  timedata.cpp \
  torcontrol.cpp \
  txdb.cpp \
  txmempool.cpp \
  validationinterface.cpp \
  versionbits.cpp \
  $(CIVILBIT_CORE_H)

if ENABLE_ZMQ
LIBCIVILBIT_ZMQ=libcivilbit_zmq.a

libcivilbit_zmq_a_CPPFLAGS = $(CIVILBIT_INCLUDES) $(ZMQ_CFLAGS)
libcivilbit_zmq_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libcivilbit_zmq_a_SOURCES = \
  zmq/zmqabstractnotifier.cpp \
  zmq/zmqnotificationinterface.cpp \
  zmq/zmqpublishnotifier.cpp
endif


# wallet: shared between civilbitd and civilbit-qt, but only linked
# when wallet enabled
libcivilbit_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(CIVILBIT_INCLUDES)
libcivilbit_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libcivilbit_wallet_a_SOURCES = \
  wallet/crypter.cpp \
  wallet/db.cpp \
  wallet/rpcdump.cpp \
  wallet/rpcwallet.cpp \
  wallet/wallet.cpp \
  wallet/wallet_ismine.cpp \
  wallet/walletdb.cpp \
  policy/rbf.cpp \
  $(CIVILBIT_CORE_H)

# crypto primitives library
crypto_libcivilbit_crypto_a_CPPFLAGS = $(AM_CPPFLAGS) $(CIVILBIT_CONFIG_INCLUDES)
crypto_libcivilbit_crypto_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libcivilbit_crypto_a_SOURCES = \
  crypto/common.h \
  crypto/hmac_sha256.cpp \
  crypto/hmac_sha256.h \
  crypto/hmac_sha512.cpp \
  crypto/hmac_sha512.h \
  crypto/ripemd160.cpp \
  crypto/ripemd160.h \
  crypto/sha1.cpp \
  crypto/sha1.h \
  crypto/sha256.cpp \
  crypto/sha256.h \
  crypto/sha512.cpp \
  crypto/sha512.h

# common: shared between civilbitd, and civilbit-qt and non-server tools
libcivilbit_common_a_CPPFLAGS = $(AM_CPPFLAGS) $(CIVILBIT_INCLUDES)
libcivilbit_common_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libcivilbit_common_a_SOURCES = \
  amount.cpp \
  arith_uint256.cpp \
  base58.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
  consensus/merkle.cpp \
  core_read.cpp \
  core_write.cpp \
  hash.cpp \
  key.cpp \
  keystore.cpp \
  netbase.cpp \
  primitives/block.cpp \
  primitives/transaction.cpp \
  protocol.cpp \
  pubkey.cpp \
  scheduler.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  script/script_error.cpp \
  script/sign.cpp \
  script/standard.cpp \
  $(CIVILBIT_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libcivilbit_util_a_CPPFLAGS = $(AM_CPPFLAGS) $(CIVILBIT_INCLUDES)
libcivilbit_util_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libcivilbit_util_a_SOURCES = \
  support/pagelocker.cpp \
  chainparamsbase.cpp \
  clientversion.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  compat/strnlen.cpp \
  random.cpp \
  rpcprotocol.cpp \
  support/cleanse.cpp \
  sync.cpp \
  uint256.cpp \
  util.cpp \
  utilmoneystr.cpp \
  utilstrencodings.cpp \
  utiltime.cpp \
  $(CIVILBIT_CORE_H)

if GLIBC_BACK_COMPAT
libcivilbit_util_a_SOURCES += compat/glibc_compat.cpp
endif

# cli: shared between civilbit-cli and civilbit-qt
libcivilbit_cli_a_CPPFLAGS = $(AM_CPPFLAGS) $(CIVILBIT_INCLUDES)
libcivilbit_cli_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libcivilbit_cli_a_SOURCES = \
  rpcclient.cpp \
  $(CIVILBIT_CORE_H)

nodist_libcivilbit_util_a_SOURCES = $(srcdir)/obj/build.h
#

# civilbitd binary #
civilbitd_SOURCES = civilbitd.cpp
civilbitd_CPPFLAGS = $(AM_CPPFLAGS) $(CIVILBIT_INCLUDES)
civilbitd_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
civilbitd_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
civilbitd_SOURCES += civilbitd-res.rc
endif

civilbitd_LDADD = \
  $(LIBCIVILBIT_SERVER) \
  $(LIBCIVILBIT_COMMON) \
  $(LIBUNIVALUE) \
  $(LIBCIVILBIT_UTIL) \
  $(LIBCIVILBIT_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

if ENABLE_ZMQ
civilbitd_LDADD += $(LIBCIVILBIT_ZMQ) $(ZMQ_LIBS)
endif

if ENABLE_WALLET
civilbitd_LDADD += libcivilbit_wallet.a
endif

civilbitd_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS)

# civilbit-cli binary #
civilbit_cli_SOURCES = civilbit-cli.cpp
civilbit_cli_CPPFLAGS = $(AM_CPPFLAGS) $(CIVILBIT_INCLUDES) $(EVENT_CFLAGS)
civilbit_cli_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
civilbit_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

# wallet-utility binary #
if ENABLE_WALLET
wallet_utility_SOURCES = wallet-utility.cpp
wallet_utility_CPPFLAGS = $(AM_CPPFLAGS) $(CIVILBIT_INCLUDES) $(EVENT_CFLAG)
wallet_utility_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
wallet_utility_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)
endif

if TARGET_WINDOWS
civilbit_cli_SOURCES += civilbit-cli-res.rc
endif

civilbit_cli_LDADD = \
  $(LIBCIVILBIT_CLI) \
  $(LIBUNIVALUE) \
  $(LIBCIVILBIT_UTIL)

civilbit_cli_LDADD += $(BOOST_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(EVENT_LIBS)
if ENABLE_WALLET
wallet_utility_LDADD = libcivilbit_wallet.a $(LIBCIVILBIT_COMMON) $(LIBCIVILBIT_CRYPTO) $(LIBSECP256K1) $(LIBCIVILBIT_UTIL) $(BOOST_LIBS) $(BDB_LIBS) $(CRYPTO_LIBS)
endif

#

# civilbit-tx binary #
civilbit_tx_SOURCES = civilbit-tx.cpp
civilbit_tx_CPPFLAGS = $(AM_CPPFLAGS) $(CIVILBIT_INCLUDES)
civilbit_tx_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
civilbit_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
civilbit_tx_SOURCES += civilbit-tx-res.rc
endif

civilbit_tx_LDADD = \
  $(LIBUNIVALUE) \
  $(LIBCIVILBIT_COMMON) \
  $(LIBCIVILBIT_UTIL) \
  $(LIBCIVILBIT_CRYPTO) \
  $(LIBSECP256K1)

civilbit_tx_LDADD += $(BOOST_LIBS) $(CRYPTO_LIBS)
#

# civilbitconsensus library #
if BUILD_CIVILBIT_LIBS
include_HEADERS = script/civilbitconsensus.h
libcivilbitconsensus_la_SOURCES = \
  crypto/hmac_sha512.cpp \
  crypto/ripemd160.cpp \
  crypto/sha1.cpp \
  crypto/sha256.cpp \
  crypto/sha512.cpp \
  hash.cpp \
  primitives/transaction.cpp \
  pubkey.cpp \
  script/civilbitconsensus.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  uint256.cpp \
  utilstrencodings.cpp

if GLIBC_BACK_COMPAT
  libcivilbitconsensus_la_SOURCES += compat/glibc_compat.cpp
endif

libcivilbitconsensus_la_LDFLAGS = $(AM_LDFLAGS) -no-undefined $(RELDFLAGS)
libcivilbitconsensus_la_LIBADD = $(LIBSECP256K1)
libcivilbitconsensus_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(builddir)/obj -I$(srcdir)/secp256k1/include -DBUILD_CIVILBIT_INTERNAL
libcivilbitconsensus_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)

endif
#

CLEANFILES = leveldb/libleveldb.a leveldb/libmemenv.a
CLEANFILES += *.gcda *.gcno
CLEANFILES += compat/*.gcda compat/*.gcno
CLEANFILES += consensus/*.gcda consensus/*.gcno
CLEANFILES += crypto/*.gcda crypto/*.gcno
CLEANFILES += policy/*.gcda policy/*.gcno
CLEANFILES += primitives/*.gcda primitives/*.gcno
CLEANFILES += script/*.gcda script/*.gcno
CLEANFILES += support/*.gcda support/*.gcno
CLEANFILES += univalue/*.gcda univalue/*.gcno
CLEANFILES += wallet/*.gcda wallet/*.gcno
CLEANFILES += wallet/test/*.gcda wallet/test/*.gcno
CLEANFILES += zmq/*.gcda zmq/*.gcno

DISTCLEANFILES = obj/build.h

EXTRA_DIST = leveldb

clean-local:
	-$(MAKE) -C leveldb clean
	-$(MAKE) -C secp256k1 clean
	-$(MAKE) -C univalue clean
	-rm -f leveldb/*/*.gcda leveldb/*/*.gcno leveldb/helpers/memenv/*.gcda leveldb/helpers/memenv/*.gcno
	-rm -f config.h

.rc.o:
	@test -f $(WINDRES)
	$(AM_V_GEN) $(WINDRES) -DWINDRES_PREPROC -i $< -o $@

.mm.o:
	$(AM_V_CXX) $(OBJCXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	  $(CPPFLAGS) $(AM_CXXFLAGS) $(QT_INCLUDES) $(AM_CXXFLAGS) $(PIE_FLAGS) $(CXXFLAGS) -c -o $@ $<

check-symbols: $(bin_PROGRAMS)
if GLIBC_BACK_COMPAT
	@echo "Checking glibc back compat..."
	$(AM_V_at) READELF=$(READELF) CPPFILT=$(CPPFILT) $(top_srcdir)/contrib/devtools/symbol-check.py < $(bin_PROGRAMS)
endif

check-security: $(bin_PROGRAMS)
if HARDEN
	@echo "Checking binary security..."
	$(AM_V_at) READELF=$(READELF) OBJDUMP=$(OBJDUMP) $(top_srcdir)/contrib/devtools/security-check.py < $(bin_PROGRAMS)
endif

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(abspath $(<D) $<)

if ENABLE_TESTS
include Makefile.test.include
endif

if ENABLE_BENCH
include Makefile.bench.include
endif

if ENABLE_QT
include Makefile.qt.include
endif

if ENABLE_QT_TESTS
include Makefile.qttest.include
endif
