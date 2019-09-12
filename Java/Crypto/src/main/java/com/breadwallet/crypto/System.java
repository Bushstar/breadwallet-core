/*
 * System
 *
 * Created by Ed Gamble <ed@breadwallet.com> on 1/22/18.
 * Copyright (c) 2018 Breadwinner AG.  All right reserved.
 *
 * See the LICENSE file at the project root for license information.
 * See the CONTRIBUTORS file at the project root for a list of contributors.
 */
package com.breadwallet.crypto;

import com.breadwallet.crypto.blockchaindb.BlockchainDb;
import com.breadwallet.crypto.blockchaindb.models.bdb.Currency;
import com.breadwallet.crypto.events.system.SystemListener;

import java.util.List;
import java.util.concurrent.ScheduledExecutorService;

public interface System {

    static System create(ScheduledExecutorService executor, SystemListener listener, Account account, boolean isMainnet, String path, BlockchainDb query) {
        return CryptoApi.getProvider().systemProvider().create(executor, listener, account, isMainnet,path, query);
    }

    void configure(List<Currency> appCurrencies);

    void createWalletManager(Network network, WalletManagerMode mode, AddressScheme addressScheme);

    void stop();

    void subscribe(String subscriptionToken);

    /**
     * Set the network reachable flag for all managers.
     *
     * Setting or clearing this flag will NOT result in a connect/disconnect attempt by a {@link WalletManager}.
     * Callers must use the {@link WalletManager#connect()}/{@link WalletManager#disconnect()} methods to
     * change a WalletManager's connectivity state. Instead, WalletManagers MAY consult this flag when performing
     * network operations to determine viability.
     */
    void setNetworkReachable(boolean isNetworkReachable);

    Account getAccount();

    String getPath();

    List<? extends Network> getNetworks();

    List<? extends WalletManager> getWalletManagers();

    List<? extends Wallet> getWallets();

    AddressScheme getDefaultAddressScheme(Network network);

    List<AddressScheme> getSupportedAddressSchemes(Network network);

    boolean supportsAddressScheme(Network network, AddressScheme addressScheme);

    WalletManagerMode getDefaultWalletManagerMode(Network network);

    List<WalletManagerMode> getSupportedWalletManagerModes(Network network);

    boolean supportsWalletManagerModes(Network network, WalletManagerMode mode);
}
