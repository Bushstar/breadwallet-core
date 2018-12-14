//
//  CoreXDemoBitcoinClient.swift
//  CoreXDemo
//
//  Created by Ed Gamble on 11/8/18.
//  Copyright © 2018 breadwallet. All rights reserved.
//

import Foundation
import BRCrypto

protocol WalletListener {
    func announceWalletEvent (manager: WalletManager,
                              wallet: Wallet,
                              event: WalletEvent)
}

protocol TransferListener {
    func announceTransferEvent (manager: WalletManager,
                                wallet: Wallet,
                                transfer: Transfer,
                                event: TransferEvent)
}


class CoreDemoListener : WalletManagerListener, BitcoinListener, EthereumListener {
    var wallets: [Wallet] = []

    public func handleManagerEvent(manager: WalletManager,
                                   event: WalletManagerEvent) {
        print ("TST: UI Handling WalletManager Event")
    }

    var walletListeners = [WalletListener]()

    func addWalletListener (listener: WalletListener) {
        walletListeners.append(listener)
    }

    public func handleWalletEvent (manager: WalletManager,
                                   wallet: Wallet,
                                   event: WalletEvent) -> Void {
        print ("TST: UI Handling Wallet Event: \(wallet.currency.code): \(event)")
        switch event {
        case .created:
            wallets.append (wallet)
            /*
        case .transferAdded(let transfer):
            break
        case .transferChanged(let transfer):
            break
        case .transferDeleted(let transfer):
            break
        case .balanceUpdated(let amount):
            break
        case .feeBasisUpdated(let feeBasis):
            break
 */
        case .deleted:
            if let index = wallets.firstIndex(where: { $0 === wallet}) {
                wallets.remove(at: index)
            }
        default:
            break
        }

        walletListeners.forEach {
            $0.announceWalletEvent(manager: manager, wallet: wallet, event: event)
        }
    }

    public func handleTransferEvent (manager: WalletManager,
                                     wallet: Wallet,
                                     transfer: Transfer,
                                     event: TransferEvent) -> Void {
        print ("TST: UI Handling Transfer Event")
    }

    func handleTokenEvent(manager: WalletManager,
                          token: EthereumToken,
                          event: EthereumTokenEvent) {
        print ("TST: UI Handling Token Event: \(token.currency.code): \(event)")
    }
}
