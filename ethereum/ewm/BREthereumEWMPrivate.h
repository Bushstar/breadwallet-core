//
//  BREthereumEWMPrivate.h
//  BRCore
//
//  Created by Ed Gamble on 5/7/18.
//  Copyright (c) 2018 breadwallet LLC
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#ifndef BR_Ethereum_EWM_Private_H
#define BR_Ethereum_EWM_Private_H

#include <pthread.h>
#include "../blockchain/BREthereumBlockChain.h"
#include "../les/BREthereumLES.h"
#include "../bcs/BREthereumBCS.h"
#include "../event/BREvent.h"

#include "BREthereumEWM.h"
#include "BREthereumWallet.h"
#include "BREthereumTransfer.h"

#ifdef __cplusplus
extern "C" {
#endif

//
// EWM
//

#define DEFAULT_LISTENER_CAPACITY 3
#define DEFAULT_WALLET_CAPACITY 10
#define DEFAULT_BLOCK_CAPACITY 100
#define DEFAULT_TRANSACTION_CAPACITY 1000

typedef enum {
    LIGHT_NODE_CREATED,
    LIGHT_NODE_CONNECTING,
    LIGHT_NODE_CONNECTED,
    LIGHT_NODE_DISCONNECTING,
    LIGHT_NODE_DISCONNECTED,
    LIGHT_NODE_ERRORED
} BREthereumEWMState;

/// MISPLACED
extern void
ewmInsertWallet (BREthereumEWM ewm,
                 BREthereumWallet wallet);

/**
 *
 */
struct BREthereumEWMRecord {
    /**
     * The State
     */
    BREthereumEWMState state;

    /**
     * The Mode of this EWM
     */
    BREthereumMode mode;

    /**
     * The network
     */
    BREthereumNetwork network;

    /**
     * The Client supporting this EWM
     */
    BREthereumClient client;

    /**
     * The account
     */
    BREthereumAccount account;

    /**
     * The wallets 'managed/handled' by this ewm.  There can be only one wallet holding ETHER;
     * all the other wallets hold TOKENs and only one wallet per TOKEN.
     */
    BREthereumWallet *wallets;
    BREthereumWallet  walletHoldingEther;

    /**
     * The BCS Interface
     */
    BREthereumBCS bcs;

    /**
     * The BlockHeight is the largest block number seen or computed.  [Note: the blockHeight may
     * be computed from a Log event as (log block number + log confirmations).
     */
    uint64_t blockHeight;

    /**
     * An identiifer for a LES/BRD Request
     */
    unsigned int requestId;

    /**
     * An EventHandler for Main.  All 'announcements' (via LES (or BRD) hit here.
     */
    BREventHandler handler;

    /**
     * The Lock ensuring single thread access to EWM state.
     */
    pthread_mutex_t lock;

    /**
     * The RLP Coder
     */
    BRRlpCoder coder;

    /**
     * The path for persistent storage or NULL
     */
    const char *storagePath;
};

///
/// MARK: - BCS Callback Interfaces
///
//
// Signal/Handle Block Chain (BCS Callback)
//
extern void
ewmHandleBlockChain (BREthereumEWM ewm,
                     BREthereumHash headBlockHash,
                     uint64_t headBlockNumber,
                     uint64_t headBlockTimestamp);

extern void
ewmSignalBlockChain (BREthereumEWM ewm,
                     BREthereumHash headBlockHash,
                     uint64_t headBlockNumber,
                     uint64_t headBlockTimestamp);

//
// Signal/Handle Balance (BCS Callback)
//
extern void
ewmHandleAccountState (BREthereumEWM ewm,
                       BREthereumAccountState accountState);

extern void
ewmSignalAccountState (BREthereumEWM ewm,
                       BREthereumAccountState accountState);

//
// Signal/Handle Balance (BCS Callback)
//
extern void
ewmHandleBalance (BREthereumEWM ewm,
                  BREthereumAmount balance);

extern void
ewmSignalBalance (BREthereumEWM ewm,
                  BREthereumAmount balance);

//
// Signal/Handle GasPrice (BCS Callback)
//
extern void
ewmHandleGasPrice (BREthereumEWM ewm,
                   BREthereumWallet wallet,
                   BREthereumGasPrice gasPrice);
extern void
ewmSignalGasPrice (BREthereumEWM ewm,
                   BREthereumWallet wallet,
                   BREthereumGasPrice gasPrice);

//
// Signal/Handle GasEstimate (BCS Callback)
//
extern void
ewmHandleGasEstimate (BREthereumEWM ewm,
                      BREthereumWallet wallet,
                      BREthereumTransfer transfer,
                      BREthereumGas gasEstimate);
extern void
ewmSignalGasEstimate (BREthereumEWM ewm,
                      BREthereumWallet wallet,
                      BREthereumTransfer transfer,
                      BREthereumGas gasEstimate);

//
// Signal/Handle Transaction (BCS Callback)
//
extern void
ewmHandleTransaction (BREthereumEWM ewm,
                      BREthereumBCSCallbackTransactionType type,
                      OwnershipGiven BREthereumTransaction transaction);

extern void
ewmSignalTransaction (BREthereumEWM ewm,
                      BREthereumBCSCallbackTransactionType type,
                      OwnershipGiven BREthereumTransaction transaction);

//
// Signal/Handle Log (BCS Callback)
//
extern void
ewmHandleLog (BREthereumEWM ewm,
              BREthereumBCSCallbackLogType type,
              OwnershipGiven BREthereumLog log);

extern void
ewmSignalLog (BREthereumEWM ewm,
              BREthereumBCSCallbackLogType type,
              OwnershipGiven BREthereumLog log);

//
// Signal/Handle Save Blocks (BCS Callback)
//
extern void
ewmHandleSaveBlocks (BREthereumEWM ewm,
                     OwnershipGiven BRArrayOf(BREthereumBlock) blocks);

extern void
ewmSignalSaveBlocks (BREthereumEWM ewm,
                     OwnershipGiven BRArrayOf(BREthereumBlock) blocks);

//
// Signal/Handle Save Peers (BCS Callback)
//
extern void
ewmHandleSaveNodes (BREthereumEWM ewm,
                    OwnershipGiven BRArrayOf(BREthereumNodeConfig) peers);

extern void
ewmSignalSaveNodes (BREthereumEWM ewm,
                    OwnershipGiven BRArrayOf(BREthereumNodeConfig) peers);

//
// Handle Save Transaction
//
extern void
ewmHandleSaveTransaction (BREthereumEWM ewm,
                          BREthereumTransaction transaction,
                          BREthereumClientChangeType type);

//
// Handle Save Log
//
extern void
ewmHandleSaveLog (BREthereumEWM ewm,
                  BREthereumLog log,
                  BREthereumClientChangeType type);

//
// Signal/Handle Sync (BCS Callback)
//
extern void
ewmHandleSync (BREthereumEWM ewm,
               BREthereumBCSCallbackSyncType type,
               uint64_t blockNumberStart,
               uint64_t blockNumberCurrent,
               uint64_t blockNumberStop);

extern void
ewmSignalSync (BREthereumEWM ewm,
               BREthereumBCSCallbackSyncType type,
               uint64_t blockNumberStart,
               uint64_t blockNumberCurrent,
               uint64_t blockNumberStop);

//
// Signal/Handle Get Blocks (BCS Callback)
//
extern void
ewmHandleGetBlocks (BREthereumEWM ewm,
                    BREthereumAddress address,
                    BREthereumSyncInterestSet interests,
                    uint64_t blockStart,
                    uint64_t blockStop);
extern void
ewmSignalGetBlocks (BREthereumEWM ewm,
                    BREthereumAddress address,
                    BREthereumSyncInterestSet interests,
                    uint64_t blockStart,
                    uint64_t blockStop);

///
/// MARK: - (Wallet) Balance
///
extern void
ewmHandleAnnounceBalance (BREthereumEWM ewm,
                                BREthereumWallet wallet,
                                UInt256 amount,
                                int rid);

extern void
ewmSignalAnnounceBalance (BREthereumEWM ewm,
                                BREthereumWallet wallet,
                                UInt256 amount,
                                int rid);

///
/// MARK: - GasPrice
///
extern void
ewmSignalAnnounceGasPrice (BREthereumEWM ewm,
                                 BREthereumWallet wallet,
                                 UInt256 value,
                                 int rid);

extern void
ewmHandleAnnounceGasPrice (BREthereumEWM ewm,
                                 BREthereumWallet wallet,
                                 UInt256 value,
                                 int rid);

///
/// MARK: - Estimate Gas
///

extern void
ewmHandleAnnounceGasEstimate (BREthereumEWM ewm,
                                    BREthereumWallet wallet,
                                    BREthereumTransfer transfer,
                                    UInt256 value,
                                    int rid);

extern void
ewmSignalAnnounceGasEstimate (BREthereumEWM ewm,
                                    BREthereumWallet wallet,
                                    BREthereumTransfer transfer,
                                    UInt256 value,
                                    int rid);

///
/// MARK: - Submit Transaction
///
extern void
ewmSignalAnnounceSubmitTransfer (BREthereumEWM ewm,
                                       BREthereumWallet wallet,
                                       BREthereumTransfer transfer,
                                       int rid);

extern void
ewmHandleAnnounceSubmitTransfer (BREthereumEWM ewm,
                                       BREthereumWallet wallet,
                                       BREthereumTransfer transfer,
                                       int rid);

///
/// MARK: - Transactions
///
typedef struct {
    BREthereumHash hash;
    BREthereumAddress from;
    BREthereumAddress to;
    BREthereumAddress contract;
    UInt256 amount;
    uint64_t gasLimit;
    UInt256 gasPrice;
    char *data;
    uint64_t nonce;
    uint64_t gasUsed;
    uint64_t blockNumber;
    BREthereumHash blockHash;
    uint64_t blockConfirmations;
    uint64_t blockTransactionIndex;
    uint64_t blockTimestamp;
    BREthereumBoolean isError;
} BREthereumEWMClientAnnounceTransactionBundle;

static inline void
ewmClientAnnounceTransactionBundleRelease (BREthereumEWMClientAnnounceTransactionBundle *bundle) {
    free (bundle->data);
    free (bundle);
}

extern void
ewmHandleAnnounceTransaction(BREthereumEWM ewm,
                                   BREthereumEWMClientAnnounceTransactionBundle *bundle,
                                   int id);

extern void
ewmSignalAnnounceTransaction(BREthereumEWM ewm,
                                   BREthereumEWMClientAnnounceTransactionBundle *bundle,
                                   int id);

///
/// MARK: - Logs
///
typedef struct {
    BREthereumHash hash;
    BREthereumAddress contract;
    int topicCount;
    char **arrayTopics;
    char *data;
    UInt256 gasPrice;
    uint64_t gasUsed;
    uint64_t logIndex;
    uint64_t blockNumber;
    uint64_t blockTransactionIndex;
    uint64_t blockTimestamp;
} BREthereumEWMClientAnnounceLogBundle;

static inline void
ewmClientAnnounceLogBundleRelease (BREthereumEWMClientAnnounceLogBundle *bundle) {
    for (int i = 0; i < bundle->topicCount; i++)
        free (bundle->arrayTopics[i]);
    free (bundle->arrayTopics);
    free (bundle->data);
    free (bundle);
}

extern void
ewmSignalAnnounceLog (BREthereumEWM ewm,
                            BREthereumEWMClientAnnounceLogBundle *bundle,
                            int id);

extern void
ewmHandleAnnounceLog (BREthereumEWM ewm,
                            BREthereumEWMClientAnnounceLogBundle *bundle,
                            int id);

///
/// MARK: - Tokens
///
typedef struct {
    char *address;
    char *symbol;
    char *name;
    char *description;
    unsigned int decimals;
    BREthereumGas gasLimit;
    BREthereumGasPrice gasPrice;
} BREthereumEWMClientAnnounceTokenBundle;

static inline void
ewmClientAnnounceTokenBundleRelease (BREthereumEWMClientAnnounceTokenBundle *bundle) {
    free (bundle->address);
    free (bundle->symbol);
    free (bundle->name);
    free (bundle->description);
    free (bundle);
}

extern void
ewmHandleAnnounceToken (BREthereumEWM ewm,
                              BREthereumEWMClientAnnounceTokenBundle *bundle,
                              int id);
extern void
ewmSignalAnnounceToken (BREthereumEWM ewm,
                              BREthereumEWMClientAnnounceTokenBundle *bundle,
                              int id);

///
// MARK: - BlockNumber
///
extern void
ewmHandleAnnounceBlockNumber (BREthereumEWM ewm,
                                    uint64_t blockNumber,
                                    int rid);

extern void
ewmSignalAnnounceBlockNumber (BREthereumEWM ewm,
                                    uint64_t blockNumber,
                                    int rid);

///
/// MARK: - Nonce
///
extern void
ewmHandleAnnounceNonce (BREthereumEWM ewm,
                              BREthereumAddress address,
                              uint64_t nonce,
                              int rid);

extern void
ewmSignalAnnounceNonce (BREthereumEWM ewm,
                              BREthereumAddress address,
                              uint64_t nonce,
                              int rid);


///
// Save Sync (and other) State
//
//
// Wallet Event
//

extern void
ewmHandleWalletEvent(BREthereumEWM ewm,
                           BREthereumWallet wid,
                           BREthereumWalletEvent event,
                           BREthereumStatus status,
                           const char *errorDescription);

extern void
ewmSignalWalletEvent(BREthereumEWM ewm,
                           BREthereumWallet wid,
                           BREthereumWalletEvent event,
                           BREthereumStatus status,
                           const char *errorDescription);

//
// Block Event
//
#if defined (NEVER_DEFINED)
extern void
ewmSignalBlockEvent(BREthereumEWM ewm,
                          BREthereumBlock bid,
                          BREthereumBlockEvent event,
                          BREthereumStatus status,
                          const char *errorDescription);

extern void
ewmHandleBlockEvent(BREthereumEWM ewm,
                          BREthereumBlock bid,
                          BREthereumBlockEvent event,
                          BREthereumStatus status,
                          const char *errorDescription);
#endif
//
// Transfer Event
//

extern void
ewmSignalTransferEvent(BREthereumEWM ewm,
                             BREthereumWallet wid,
                             BREthereumTransfer tid,
                             BREthereumTransferEvent event,
                             BREthereumStatus status,
                             const char *errorDescription);

extern void
ewmHandleTransferEvent(BREthereumEWM ewm,
                             BREthereumWallet wid,
                             BREthereumTransfer tid,
                             BREthereumTransferEvent event,
                             BREthereumStatus status,
                             const char *errorDescription);
//
// Peer Event
//
extern void
ewmSignalPeerEvent(BREthereumEWM ewm,
                         // BREthereumWallet wid,
                         // BREthereumTransaction tid,
                         BREthereumPeerEvent event,
                         BREthereumStatus status,
                         const char *errorDescription);

extern void
ewmHandlePeerEvent(BREthereumEWM ewm,
                         // BREthereumWallet wid,
                         // BREthereumTransaction tid,
                         BREthereumPeerEvent event,
                         BREthereumStatus status,
                         const char *errorDescription);

//
// EWM Event
//
extern void
ewmSignalEWMEvent(BREthereumEWM ewm,
                        // BREthereumWallet wid,
                        // BREthereumTransaction tid,
                        BREthereumEWMEvent event,
                        BREthereumStatus status,
                        const char *errorDescription);

extern void
ewmHandleEWMEvent(BREthereumEWM ewm,
                        // BREthereumWallet wid,
                        // BREthereumTransaction tid,
                        BREthereumEWMEvent event,
                        BREthereumStatus status,
                        const char *errorDescription);

///
/// MARK: - Handler For Main
///
extern const BREventType *ewmEventTypes[];
extern const unsigned int ewmEventTypesCount;

#ifdef __cplusplus
}
#endif

#endif //BR_Ethereum_EWM_Private_H
