//
//  BREthereumBCS.h
//  Core
//
//  Created by Ed Gamble on 5/24/18.
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

#ifndef BR_Ethereum_BCS_h
#define BR_Ethereum_BCS_h

#include "../base/BREthereumBase.h"
#include "../les/BREthereumLES.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct BREthereumBCSStruct *BREthereumBCS;

//
// BCS Listener
//
typedef void* BREthereumBCSCallbackContext;

/**
 * The BCS block chain has been extended.
 */
typedef void
(*BREthereumBCSCallbackBlockchain) (BREthereumBCSCallbackContext context,
                                    BREthereumHash headBlockHash,
                                    uint64_t headBlockNumber,
                                    uint64_t headBlockTimestamp);


/**
 * The BCS account state has been updated.
 */
typedef void
(*BREthereumBCSCallbackAccountState) (BREthereumBCSCallbackContext context,
                                      BREthereumAccountState accountState);

/**
 * A BCS Transaction (for `account`) have been updated.
 */
typedef enum {
    BCS_CALLBACK_TRANSACTION_ADDED,
    BCS_CALLBACK_TRANSACTION_UPDATED,
    BCS_CALLBACK_TRANSACTION_DELETED
} BREthereumBCSCallbackTransactionType;

#define BCS_CALLBACK_TRANSACTION_TYPE_NAME( type ) \
  ((type) == BCS_CALLBACK_TRANSACTION_UPDATED \
    ? "Updated" \
    : ((type) == BCS_CALLBACK_TRANSACTION_ADDED \
        ? "Added" \
        : "Deleted"))

typedef void
(*BREthereumBCSCallbackTransaction) (BREthereumBCSCallbackContext context,
                                     BREthereumBCSCallbackTransactionType event,
                                     OwnershipGiven BREthereumTransaction transaction);

/**
 * A BCS Log (for `account`) has been updated.
 */
typedef enum {
    BCS_CALLBACK_LOG_ADDED,
    BCS_CALLBACK_LOG_UPDATED,
    BCS_CALLBACK_LOG_DELETED,
} BREthereumBCSCallbackLogType;

typedef void
(*BREthereumBCSCallbackLog) (BREthereumBCSCallbackContext context,
                             BREthereumBCSCallbackLogType event,
                             OwnershipGiven BREthereumLog log);

/**
 * Save Blocks
 */
typedef void
(*BREthereumBCSCallbackSaveBlocks) (BREthereumBCSCallbackContext context,
                                    OwnershipGiven BRArrayOf(BREthereumBlock) blocks);

/**
 * Save Peers
 */
typedef void
(*BREthereumBCSCallbackSavePeers) (BREthereumBCSCallbackContext context,
                                   OwnershipGiven BRArrayOf(BREthereumNodeConfig) peers);

/**
 * Sync
 */
typedef enum {
    BCS_CALLBACK_SYNC_STARTED,
    BCS_CALLBACK_SYNC_UPDATE,
    BCS_CALLBACK_SYNC_STOPPED
} BREthereumBCSCallbackSyncType;

typedef void
(*BREthereumBCSCallbackSync) (BREthereumBCSCallbackContext context,
                              BREthereumBCSCallbackSyncType event,
                              uint64_t blockNumberStart,
                              uint64_t blockNumberCurrent,
                              uint64_t blockNumberStop);

/**
 * Get (Interesting) Blocks.  Depending on the sync-mode we may ask the BRD endpoint for blocks
 * that contain transactions or logs of interest.
 */
typedef void
(*BREthereumBCSCallbackGetBlocks) (BREthereumBCSCallbackContext context,
                                   BREthereumAddress address,
                                   BREthereumSyncInterestSet interests,
                                   uint64_t blockStart,
                                   uint64_t blockStop);

typedef struct {
    BREthereumBCSCallbackContext context;
    BREthereumBCSCallbackBlockchain blockChainCallback;
    BREthereumBCSCallbackAccountState accountStateCallback;
    BREthereumBCSCallbackTransaction transactionCallback;
    BREthereumBCSCallbackLog logCallback;
    BREthereumBCSCallbackSaveBlocks saveBlocksCallback;
    BREthereumBCSCallbackSavePeers savePeersCallback;
    BREthereumBCSCallbackSync syncCallback;
    BREthereumBCSCallbackGetBlocks getBlocksCallback;
} BREthereumBCSListener;

/**
 * Create BCS (a 'BlockChain Slice`) providing a view of the Ethereum blockchain for `network`
 * focused on the `account` primary address.  Initialize the synchronization with the previously
 * saved `headers`.  Provide `listener` to anounce BCS 'events'.
 *
 * @parameters
 * @parameter headers - is this a BRArray; assume so for now.
 */
extern BREthereumBCS
bcsCreate (BREthereumNetwork network,
           BREthereumAddress address,
           BREthereumBCSListener listener,
           BREthereumMode syncMode,
           BRSetOf(BREthereumNodeConfig) peers,
           BRSetOf(BREthereumBlock) blocks,
           BRSetOf(BREthereumTransaction) transactions,
           BRSetOf(BREthereumLog) logs);

extern void
bcsStart (BREthereumBCS bcs);

extern void
bcsStop (BREthereumBCS bcs);

extern BREthereumBoolean
bcsIsStarted (BREthereumBCS bcs);

extern void
bcsDestroy (BREthereumBCS bcs);

extern void
bcsSync (BREthereumBCS bcs,
         uint64_t blockNumber);

extern BREthereumBoolean
bcsSyncInProgress (BREthereumBCS bcs);

/**
 * Submit `transaction` to the Ethereum Network
 *
 * @param bcs
 * @param transaction
 */
extern void
bcsSendTransaction (BREthereumBCS bcs,
                    BREthereumTransaction transaction);

/**
 * Request the status of `transaction` for the given block from the Ethereum Network
 *
 * @param bcs
 * @param transactionHash
 * @param blockNumber
 *@param blockTransactionIndex
 */
extern void
bcsSendTransactionRequest (BREthereumBCS bcs,
                           BREthereumHash transactionHash,
                           uint64_t blockNumber,
                           uint64_t blockTransactionIndex);

/**
 * Request the status of `log` for the given block from the Ethereum Network
 *
 * @param bcs
 * @param transactionHash
 * @param blockNumber
 * @param blockTransactionIndex 
 */
extern void
bcsSendLogRequest (BREthereumBCS bcs,
                   BREthereumHash transactionHash,
                   uint64_t blockNumber,
                   uint64_t blockTransactionIndex);

extern void
bcsReportInterestingBlocks (BREthereumBCS bcs,
                            // interest
                            // request id
                            BRArrayOf(uint64_t) blockNumbers);

#ifdef __cplusplus
}
#endif

#endif /* BR_Ethereum_BCS_h */
