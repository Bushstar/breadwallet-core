//
//  BREthereumClient.h
//  BRCore
//
//  Created by Ed Gamble on 11/20/18.
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

#ifndef BR_Ethereum_Client_H
#define BR_Ethereum_Client_H

#include "BREthereumBase.h"

#ifdef __cplusplus
extern "C" {
#endif

    //
    // BREthereumClient
    //
    // Type definitions for client functions.  When configuring a EWM these functions must be
    // provided.  A EWM has limited cababilities; these callbacks provide data back into the
    // EWM (such as with the 'gas price' or with the BRD-indexed logs for a given address) or they
    // request certain data be saved to reestablish EWM state on start or they announce events
    // signifying changes in EWM state.
    //
    typedef void *BREthereumClientContext;


    ///
    /// MARK: - Balance
    ///
    typedef void
    (*BREthereumClientHandlerGetBalance) (BREthereumClientContext context,
                                          BREthereumEWM ewm,
                                          BREthereumWallet wid,
                                          const char *address,
                                          int rid);

    extern BREthereumStatus
    ewmAnnounceWalletBalance (BREthereumEWM ewm,
                              BREthereumWallet wid,
                              const char *balance,
                              int rid);

    extern void
    ewmUpdateWalletBalance (BREthereumEWM ewm,
                            BREthereumWallet wid);

    ///
    /// MARK: - Gas Price
    ///
    typedef void
    (*BREthereumClientHandlerGetGasPrice) (BREthereumClientContext context,
                                           BREthereumEWM ewm,
                                           BREthereumWallet wid,
                                           int rid);

    extern BREthereumStatus
    ewmAnnounceGasPrice(BREthereumEWM ewm,
                        BREthereumWallet wid,
                        const char *gasEstimate,
                        int rid);

    extern void
    ewmUpdateGasPrice (BREthereumEWM ewm,
                       BREthereumWallet wid);

    ///
    /// MARK: - Gas Estimate
    ///
    typedef void
    (*BREthereumClientHandlerEstimateGas) (BREthereumClientContext context,
                                           BREthereumEWM ewm,
                                           BREthereumWallet wid,
                                           BREthereumTransfer tid,
                                           const char *to,
                                           const char *amount,
                                           const char *data,
                                           int rid);

    extern BREthereumStatus
    ewmAnnounceGasEstimate (BREthereumEWM ewm,
                            BREthereumWallet wid,
                            BREthereumTransfer tid,
                            const char *gasEstimate,
                            int rid);


    extern void
    ewmUpdateGasEstimate (BREthereumEWM ewm,
                          BREthereumWallet wid,
                          BREthereumTransfer tid);

    ///
    /// MARK: Submit Transfer
    ///
    typedef void
    (*BREthereumClientHandlerSubmitTransaction) (BREthereumClientContext context,
                                                 BREthereumEWM ewm,
                                                 BREthereumWallet wid,
                                                 BREthereumTransfer tid,
                                                 const char *transaction,
                                                 int rid);

    extern BREthereumStatus
    ewmAnnounceSubmitTransfer(BREthereumEWM ewm,
                              BREthereumWallet wid,
                              BREthereumTransfer tid,
                              const char *hash,
                              int rid);

    ///
    /// MARK: Get Transactions
    ///
    typedef void
    (*BREthereumClientHandlerGetTransactions) (BREthereumClientContext context,
                                               BREthereumEWM ewm,
                                               const char *address,
                                               int rid);
    extern BREthereumStatus
    ewmAnnounceTransaction (BREthereumEWM ewm,
                            int id,
                            const char *hash,
                            const char *from,
                            const char *to,
                            const char *contract,
                            const char *amount, // value
                            const char *gasLimit,
                            const char *gasPrice,
                            const char *data,
                            const char *nonce,
                            const char *gasUsed,
                            const char *blockNumber,
                            const char *blockHash,
                            const char *blockConfirmations,
                            const char *blockTransactionIndex,
                            const char *blockTimestamp,
                            // cumulative gas used,
                            // confirmations
                            // txreceipt_status
                            const char *isError);

    /**
     * Update the transactions for the ewm's account.  A JSON_RPC EWM will call out to
     * BREthereumClientHandlerGetTransactions which is expected to query all transactions associated with the
     * accounts address and then the call out is to call back the 'announce transaction' callback.
     */
    extern void
    ewmUpdateTransactions (BREthereumEWM ewm);

    ///
    /// MARK: Get Logs
    ///

    typedef void
    (*BREthereumClientHandlerGetLogs) (BREthereumClientContext context,
                                       BREthereumEWM ewm,
                                       const char *contract,
                                       const char *address,
                                       const char *event,
                                       int rid);

    extern BREthereumStatus
    ewmAnnounceLog (BREthereumEWM ewm,
                    int id,
                    const char *strHash,
                    const char *strContract,
                    int topicCount,
                    const char **arrayTopics,
                    const char *strData,
                    const char *strGasPrice,
                    const char *strGasUsed,
                    const char *strLogIndex,
                    const char *strBlockNumber,
                    const char *strBlockTransactionIndex,
                    const char *strBlockTimestamp);

    extern void
    ewmUpdateLogs (BREthereumEWM ewm,
                   BREthereumWallet wid,
                   BREthereumContractEvent event);

    ///
    /// MARK: Get Tokens
    ///
    typedef void
    (*BREthereumClientHandlerGetTokens) (BREthereumClientContext context,
                                         BREthereumEWM ewm,
                                         int rid);

    extern void
    ewmAnnounceToken(BREthereumEWM ewm,
                     const char *address,
                     const char *symbol,
                     const char *name,
                     const char *description,
                     unsigned int decimals,
                     const char *strDefaultGasLimit,
                     const char *strDefaultGasPrice,
                     int rid);

    extern void
    ewmUpdateTokens (BREthereumEWM ewm);

    ///
    /// MARK: - BlockNumber
    ///
    typedef void
    (*BREthereumClientHandlerGetBlockNumber) (BREthereumClientContext context,
                                              BREthereumEWM ewm,
                                              int rid);

    extern BREthereumStatus
    ewmAnnounceBlockNumber (BREthereumEWM ewm,
                            const char *blockNumber,
                            int rid);

    extern void
    ewmUpdateBlockNumber (BREthereumEWM ewm);

    ///
    /// MARK: - Nonce
    ///
    typedef void
    (*BREthereumClientHandlerGetNonce) (BREthereumClientContext context,
                                        BREthereumEWM ewm,
                                        const char *address,
                                        int rid);

    extern BREthereumStatus
    ewmAnnounceNonce (BREthereumEWM ewm,
                      const char *strAddress,
                      const char *strNonce,
                      int rid);

    extern void
    ewmUpdateNonce (BREthereumEWM ewm);

    ///
    /// MARK: - Blocks
    ///
    typedef void
    (*BREthereumClientHandlerGetBlocks) (BREthereumClientContext context,
                                         BREthereumEWM ewm,
                                         const char *address, // disappears immediately
                                         BREthereumSyncInterestSet interests,
                                         uint64_t blockNumberStart,
                                         uint64_t blockNumberStop,
                                         int rid);

    extern BREthereumStatus
    ewmAnnounceBlocks (BREthereumEWM ewm,
                       int id,
                       // const char *strBlockHash,
                       int blockNumbersCount,
                       uint64_t *blockNumbers);

    ///
    /// MARK: - Persistent Changes
    ///
    typedef enum {
        CLIENT_CHANGE_ADD,
        CLIENT_CHANGE_REM,
        CLIENT_CHANGE_UPD
    } BREthereumClientChangeType;

#define CLIENT_CHANGE_TYPE_NAME( ev ) \
(CLIENT_CHANGE_ADD == (ev) ? "Add" \
: (CLIENT_CHANGE_REM == (ev) ? "Rem" : "Upd"))

    typedef void
    (*BREthereumClientHandlerSaveBlocks) (BREthereumClientContext context,
                                          BREthereumEWM ewm,
                                          OwnershipGiven BRSetOf(BREthereumHashDataPair) data);

    typedef void
    (*BREthereumClientHandlerSaveNodes) (BREthereumClientContext context,
                                         BREthereumEWM ewm,
                                         OwnershipGiven BRSetOf(BREthereumHashDataPair) data);

    typedef void
    (*BREthereumClientHandlerChangeTransaction) (BREthereumClientContext context,
                                                 BREthereumEWM ewm,
                                                 BREthereumClientChangeType type,
                                                 OwnershipGiven  BREthereumHashDataPair data);

    typedef void
    (*BREthereumClientHandlerChangeLog) (BREthereumClientContext context,
                                         BREthereumEWM ewm,
                                         BREthereumClientChangeType type,
                                         OwnershipGiven  BREthereumHashDataPair data);

    ///
    /// MARK: - Events
    ///
    typedef enum {
        WALLET_EVENT_CREATED = 0,
        WALLET_EVENT_BALANCE_UPDATED,
        WALLET_EVENT_DEFAULT_GAS_LIMIT_UPDATED,
        WALLET_EVENT_DEFAULT_GAS_PRICE_UPDATED,
        WALLET_EVENT_DELETED
    } BREthereumWalletEvent;

#define WALLET_NUMBER_OF_EVENTS  (1 + WALLET_EVENT_DELETED)

    typedef void (*BREthereumClientHandlerWalletEvent) (BREthereumClientContext context,
                                                        BREthereumEWM ewm,
                                                        BREthereumWallet wid,
                                                        BREthereumWalletEvent event,
                                                        BREthereumStatus status,
                                                        const char *errorDescription);

    typedef enum {
        BLOCK_EVENT_CREATED = 0,

        BLOCK_EVENT_CHAINED,
        BLOCK_EVENT_ORPHANED,

        BLOCK_EVENT_DELETED
    } BREthereumBlockEvent;

#define BLOCK_NUMBER_OF_EVENTS (1 + BLOCK_EVENT_DELETED)

#if defined (NEVER_DEFINED)
    typedef void (*BREthereumClientHandlerBlockEvent) (BREthereumClientContext context,
                                                       BREthereumEWM ewm,
                                                       BREthereumBlock bid,
                                                       BREthereumBlockEvent event,
                                                       BREthereumStatus status,
                                                       const char *errorDescription);
#endif

    typedef enum {
        // Added/Removed from Wallet
        TRANSFER_EVENT_CREATED = 0,

        // Transfer State
        TRANSFER_EVENT_SIGNED,
        TRANSFER_EVENT_SUBMITTED,
        TRANSFER_EVENT_INCLUDED,  // aka confirmed
        TRANSFER_EVENT_ERRORED,


        TRANSFER_EVENT_GAS_ESTIMATE_UPDATED,
        TRANSFER_EVENT_BLOCK_CONFIRMATIONS_UPDATED,

        TRANSFER_EVENT_DELETED

    } BREthereumTransferEvent;

#define TRANSACTION_NUMBER_OF_EVENTS (1 + TRANSACTION_EVENT_DELETED)

    typedef void (*BREthereumClientHandlerTransferEvent) (BREthereumClientContext context,
                                                          BREthereumEWM ewm,
                                                          BREthereumWallet wid,
                                                          BREthereumTransfer tid,
                                                          BREthereumTransferEvent event,
                                                          BREthereumStatus status,
                                                          const char *errorDescription);

    typedef enum {
        PEER_EVENT_CREATED = 0,
        PEER_EVENT_DELETED
        // added/removed/updated
    } BREthereumPeerEvent;

#define PEER_NUMBER_OF_EVENTS   (1 + PEER_EVENT_DELETED)

    typedef void (*BREthereumClientHandlerPeerEvent) (BREthereumClientContext context,
                                                      BREthereumEWM ewm,
                                                      // BREthereumWallet wid,
                                                      // BREthereumTransaction tid,
                                                      BREthereumPeerEvent event,
                                                      BREthereumStatus status,
                                                      const char *errorDescription);

    typedef enum {
        TOKEN_EVENT_CREATED = 0,
        TOKEN_EVENT_DELETED
    } BREthereumTokenEvent;

#define TOKEN_NUMBER_OF_EVENTS  (1 + TOKEN_EVENT_DELETED)

    typedef void (*BREthereumClientHandlerTokenEvent) (BREthereumClientContext context,
                                                       BREthereumEWM ewm,
                                                       BREthereumToken token,
                                                       BREthereumTokenEvent event);

    typedef enum {
        EWM_EVENT_CREATED = 0,
        EWM_EVENT_SYNC_STARTED,
        EWM_EVENT_SYNC_CONTINUES,
        EWM_EVENT_SYNC_STOPPED,
        EWM_EVENT_NETWORK_UNAVAILABLE,
        EWM_EVENT_DELETED
    } BREthereumEWMEvent;

#define EWM_NUMBER_OF_EVENTS   (1 + EWM_EVENT_DELETED)

    typedef void (*BREthereumClientHandlerEWMEvent) (BREthereumClientContext context,
                                                     BREthereumEWM ewm,
                                                     // BREthereumWallet wid,
                                                     // BREthereumTransaction tid,
                                                     BREthereumEWMEvent event,
                                                     BREthereumStatus status,
                                                     const char *errorDescription);

    //
    // EWM Configuration
    //
    // Used to configure a EWM appropriately for BRD or LES.  Starts with a
    // BREthereumNetwork (one of ethereum{Mainnet,Testnet,Rinkeby} and then specifics for the
    // type.
    //
    typedef struct {
        BREthereumClientContext context;

        // Backend Server Support - typically implemented with HTTP requests for JSON_RPC or DB
        // queries of BRD endpoints.  All of these functions *must* callback to the EWM with a
        // corresponding 'announce' function.
        BREthereumClientHandlerGetBalance funcGetBalance;
        BREthereumClientHandlerGetGasPrice funcGetGasPrice;
        BREthereumClientHandlerEstimateGas funcEstimateGas;
        BREthereumClientHandlerSubmitTransaction funcSubmitTransaction;
        BREthereumClientHandlerGetTransactions funcGetTransactions; // announce one-by-one
        BREthereumClientHandlerGetLogs funcGetLogs; // announce one-by-one
        BREthereumClientHandlerGetBlocks funcGetBlocks;
        BREthereumClientHandlerGetTokens funcGetTokens; // announce one-by-one
        BREthereumClientHandlerGetBlockNumber funcGetBlockNumber;
        BREthereumClientHandlerGetNonce funcGetNonce;

        // Save Sync (and other) State - required as Core does not maintain and is not configured to
        // use persistent storage (aka an sqlite DB or simply disk)
        BREthereumClientHandlerSaveNodes funcSaveNodes;
        BREthereumClientHandlerSaveBlocks funcSaveBlocks;
        BREthereumClientHandlerChangeTransaction funcChangeTransaction;
        BREthereumClientHandlerChangeLog funcChangeLog;

        // Events - Announce changes to entities that normally impact the UI.
        BREthereumClientHandlerEWMEvent funcEWMEvent;
        BREthereumClientHandlerPeerEvent funcPeerEvent;
        BREthereumClientHandlerWalletEvent funcWalletEvent;
        BREthereumClientHandlerTokenEvent funcTokenEvent;
        //       BREthereumClientHandlerBlockEvent funcBlockEvent;
        BREthereumClientHandlerTransferEvent funcTransferEvent;

    } BREthereumClient;

#ifdef __cplusplus
}
#endif

#endif //BR_Ethereum_Client_H

