//
//  TransferCreateController.swift
//  CoreDemo
//
//  Created by Ed Gamble on 8/22/18.
//  Copyright © 2018 breadwallet. All rights reserved.
//

import UIKit
import BRCore

class TransferCreateController: UIViewController, UITextViewDelegate {

    var wallet : EthereumWallet!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated);

        oneEtherButton.setEnabled (wallet.token == nil, forSegmentAt: 0)
        oneEtherButton.selectedSegmentIndex = 1

        amountSlider.minimumValue = 0.0
        amountSlider.maximumValue = 0.001  //  Float (wallet.balance.amount)!
        amountSlider.value = 0.0
        recvField.text = (UIApplication.sharedClient.network == EthereumNetwork.mainnet
            ? "0x19454a70538bfbdbd7abf3ac8d274d5cb2514056" /* "0xb0F225defEc7625C6B5E43126bdDE398bD90eF62" */ 
            : "0xbDFdAd139440D2Db9BA2aa3B7081C2dE39291508");
        updateView()
    }

    var oneEtherSelected = false

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */
    @IBAction func submit(_ sender: UIBarButtonItem) {
        NSLog ("Want to submit")
        let amount = (self.oneEtherSelected
            ? "1"
            : self.amountSlider.value.description)

        let alert = UIAlertController (title: "Submit Transaction for \(amount) \(wallet.name)",
            message: "Are you sure?",
            preferredStyle: UIAlertController.Style.actionSheet)

        alert.addAction(UIAlertAction (title: "Yes", style: UIAlertAction.Style.destructive) { (action) in
            let transfer = self.wallet.createTransfer(recvAddress: self.recvField.text!,
                                                      amount: amount,
                                                      unit: self.wallet.unit, //  EthereumAmountUnit.defaultUnitEther,
                                                      gasPrice: self.gasPrice(),
                                                      gasPriceUnit: EthereumAmountUnit.etherWEI,
                                                      gasLimit: self.gasLimit())

            self.wallet.sign(transfer: transfer,
                             paperKey: UIApplication.sharedClient.paperKey);

            self.wallet.submit(transfer: transfer);
            // Notify, close
            self.dismiss(animated: true) {}
        })
        alert.addAction(UIAlertAction (title: "No", style: UIAlertAction.Style.cancel) { (action) in
            NSLog ("Will Cancel" )
        })
        self.present(alert, animated: true) {}
    }

    @IBAction func cancel(_ sender: UIBarButtonItem) {
        self.dismiss(animated: true) {}
    }

    func canonicalAmount (_ amount: EthereumAmount, sign: String, symbol: String) -> String {
        var result = amount.amount.trimmingCharacters(in: CharacterSet (charactersIn: "0 "))
        if result == "." || result == "" || result == "0." || result == ".0" {
            result = "0.0"
        }
        return sign + result + " " + symbol
    }

    func updateView () {

        let fee = Double (gasPrice() * gasLimit()) / 1e18
        feeLabel.text = "\(fee) ETH"

        amountMinLabel.text = amountSlider.minimumValue.description
        amountMaxLabel.text = amountSlider.maximumValue.description
        amountLabel.text = amountSlider.value.description
        amountSlider.isEnabled = !oneEtherSelected

        submitButton.isEnabled = (recvField.text != "" &&
            (0.0 != amountSlider.value || oneEtherSelected ))
    }

    @IBAction func amountChanged(_ sender: Any) {
        amountLabel.text = amountSlider.value.description
        submitButton.isEnabled = (recvField.text != "" &&
            (0.0 != amountSlider.value || oneEtherSelected))
    }

    // In WEI
    func gasPrice () -> UInt64 {
        switch (gasPriceSegmentedController.selectedSegmentIndex) {
        case 0: return   15 * 1000000000 // 15    GWEI
        case 1: return    5 * 1000000000 //  5    GWEI
        case 2: return 1001 *    1000000 // 1.001 GWEI
        default: return 5
        }
    }

    @IBAction func amountOneEther(_ sender: UISegmentedControl) {
        oneEtherSelected = 0 == oneEtherButton.selectedSegmentIndex
        updateView()
    }

    @IBAction func gasPriceChanged(_ sender: UISegmentedControl) {
        updateView()
    }

    func gasLimit () -> UInt64 {
        switch (gasLimitSegmentedController.selectedSegmentIndex) {
        case 0: return 92000
        case 1: return 21000
        case 2: return  1000
        default: return 21000
        }
    }

    @IBAction func gasLimitChanged(_ sender: UISegmentedControl) {
        updateView()
    }

    @IBOutlet var submitButton: UIBarButtonItem!
    @IBOutlet var feeLabel: UILabel!
    @IBOutlet var amountSlider: UISlider!
    @IBOutlet var recvField: UITextField!
    @IBOutlet var amountMinLabel: UILabel!
    @IBOutlet var amountMaxLabel: UILabel!
    @IBOutlet var amountLabel: UILabel!
    @IBOutlet var gasPriceSegmentedController: UISegmentedControl!
    @IBOutlet var gasLimitSegmentedController: UISegmentedControl!
    @IBOutlet var oneEtherButton: UISegmentedControl!
}
