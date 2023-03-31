
/*This code specifies the Transaction Processor part.This code transfers the
  transaction processing requests to a registered handler,the CookieJarHandler.*/

'use strict'
//works in strict mode
const { TransactionProcessor } = require('sawtooth-sdk/processor')
//requires the module specified in ().
const CertificateHandler = require('./InsuranceHandler')
// handler for cookie store

const address = 'tcp://validator:4004';

const transactionProcessor = new TransactionProcessor(address)

transactionProcessor.addHandler(new CertificateHandler())
/*addHandler adds the given handler to the transaction processor so
  it can receive transaction processing requests. All handlers must
  be added prior to starting the processor.
*/

transactionProcessor.start()
/* start connects the transaction processor to a validator and
   starts listening for requests and routing them to an appropriate
   handler.
*/

