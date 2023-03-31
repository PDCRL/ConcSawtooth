'use strict'

//require the handler module.
const { TransactionHandler } = require('sawtooth-sdk/processor/handler')
const { InvalidTransaction, InternalError } = require('sawtooth-sdk/processor/exceptions')
const { createHash } = require('crypto');
const { TextEncoder, TextDecoder } = require('text-encoding/lib/encoding');
const { count } = require('console');
var alldata=[];

function hash(v) {
  return createHash('sha512').update(v).digest('hex');
}

var encoder = new TextEncoder('utf8');
var decoder = new TextDecoder('utf8');

const CJ_FAMILY = 'insurance';
const CJ_NAMESPACE = hash(CJ_FAMILY).substring(0, 6);

//function to display the errors
var _toInternalError = function (err) {
  console.log(" in error message block");
  var message = err.message ? err.message : err;
  throw new InternalError(message);
};

//function to set the entries in the block using the "SetState" function
const _setEntry = function (context, address, stateValue) {
  let dataBytes = encoder.encode(stateValue)
  let entries = {};
  entries[address] = dataBytes;
console.log("------------------------------------------------- handler end ------------------------------------------------");	
  return context.setState(entries)
}


class InsuranceHandler extends TransactionHandler {
  constructor() {
    super(CJ_FAMILY, ['1.0'], [CJ_NAMESPACE])
  }
  apply(transactionProcessRequest, context) {

    try {
      console.log('trasactionProcessRequest=', transactionProcessRequest);
      const payload = decoder.decode(transactionProcessRequest.payload);
      let payloadData = payload.toString().split(',');
      var header = transactionProcessRequest.header;
      var userPublicKey = header.signerPublicKey;
      var Address = hash(CJ_FAMILY).substr(0, 6) + hash(userPublicKey).substr(0, 64);
      var action = payloadData[0];
      var c=payloadData[1];
   

      // Select the action to be performed
      if (action === 'center') {

        return context.getState([Address]).then(function (stateKeyValueAddress) {
          console.log("State Address value", JSON.stringify(stateKeyValueAddress));
          var previous_data = 0;
          previous_data = stateKeyValueAddress[Address];
         
          return _setEntry(context, Address, strNewCount);
        });
      }
      else if (action === 'useradd') {

        return context.getState([Address]).then(function (stateKeyValueAddress) {
          console.log("State Address value", JSON.stringify(stateKeyValueAddress));
          var previous_data = 0;
          previous_data = stateKeyValueAddress[Address];
            console.log("-----------------------------------handler start -----------------------------------------------"+payloadData[2]);
          
           var objectData=[];


          for(var i=2;i<=payloadData.length;i++){
            objectData[objectData.length]=payloadData[i];
          }
           alldata[c]=objectData;
          //alldata = [...alldata, objectData];
          console.log("objectData : "+alldata);
          console.log("key value is :"+payloadData[1]);


          return _setEntry(context, Address,objectData);
        });
      } else {
        throw new InvalidTransaction('Action must be bake or eat ');
      }
    }
    catch (err) {
      _toInternalError(err);
    }
  }
}

module.exports = InsuranceHandler

