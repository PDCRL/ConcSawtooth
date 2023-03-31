var crypto = require("crypto");
const { createHash } = require('crypto')
const { CryptoFactory, createContext } = require('sawtooth-sdk/signing')
const protobuf = require('sawtooth-sdk/protobuf')
// const fs = require('fs')
const fetch = require('node-fetch');
const { Secp256k1PrivateKey } = require('sawtooth-sdk/signing/secp256k1')
const { TextEncoder, TextDecoder } = require('text-encoding/lib/encoding')
//const reader = require('xlsx')
//const file = reader.readFile('./vmconfiguration.xlsx')

var transaction= new Array();

const FAMILY_NAME = "insurance";
const FAMILY_VERSION = "1.0";

function hash(v) {
  return createHash('sha512').update(v).digest('hex');
}

function sleep(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}


function mapping(transaction){
    return transaction.headerSignature;
    
    }



class InsuranceClient{
  constructor(Key) { 
    var  name ="client";
    var pri_key = crypto.createHash("sha256").update(name).digest('hex');
    
    const context = createContext('secp256k1');
    const secp256k1pk = Secp256k1PrivateKey.fromHex(Key.trim());
    const batch_secp256k1pk = Secp256k1PrivateKey.fromHex(pri_key.trim());

    this.signer = new CryptoFactory(context).newSigner(secp256k1pk);
    this.batch_signer = new CryptoFactory(context).newSigner(batch_secp256k1pk);


    this.publicKey = this.signer.getPublicKey().asHex();
    this.batch_publicKey = this.batch_signer.getPublicKey().asHex();
    
    
    this.address = hash(FAMILY_NAME).substr(0, 6) + hash(this.publicKey).substr(0, 64);
    console.log("Storing at: " + this.address);
    

 //     

//    this.batch_signer = new CryptoFactory(context).newSigner(batch_secp256k1pk);

    
    
  }

  async send_data(action, c, quantity) {

    var inputAddressList = [this.address];
    var outputAddressList = [this.address];
    var payload = action + "," + c + "," + quantity;
    var encode = new TextEncoder('utf8');
    const payloadBytes = encode.encode(payload)
    const transactionHeaderBytes = protobuf.TransactionHeader.encode({
      familyName: FAMILY_NAME,
      familyVersion: FAMILY_VERSION,
      inputs: inputAddressList,
      outputs: outputAddressList,
      signerPublicKey: this.signer.getPublicKey().asHex(),
      nonce: "" + Math.random(),
      batcherPublicKey: this.batch_signer.getPublicKey().asHex(),
      dependencies: [],
      payloadSha512: hash(payloadBytes),
	    //-----------
	   // await sleep(1000);

//  console.log("----------------------------------------------------- send data --------------------------------------------------");
    }).finish();

    const transaction = protobuf.Transaction.create({
      header: transactionHeaderBytes,
      headerSignature: this.signer.sign(transactionHeaderBytes),
      payload: payloadBytes
    });
//    console.log(transaction);
    return Promise.resolve(transaction);}
    

    async batch_send(txn_headers,transactions){
   //console.log(transactions[0]);
      
    const batchHeaderBytes = protobuf.BatchHeader.encode({
      signerPublicKey: this.batch_signer.getPublicKey().asHex(),
      transactionIds: transactions.map(mapping),
      
    }).finish();
   
    const batch = protobuf.Batch.create({
      header: batchHeaderBytes,
      headerSignature: this.batch_signer.sign(batchHeaderBytes),
      transactions: transactions,
    });

    const batchListBytes = protobuf.BatchList.encode({
      batches: [batch]
    }).finish();
    console.log(transactions.length);
    this._send_to_rest_api(batchListBytes);
    
  }

  async _send_to_rest_api(batchListBytes) {

    let response = await fetch('http://rest-api:8008/batches', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/octet-stream'
      },
      body: batchListBytes

	    //-------
	 //   await sleep(1000)
//	   console.log("----------------------------------------------------- rest api ---------------------------------------");
    });
	   console.log("----------------------------------------------------- rest api ---------------------------------------");

    console.log("response", response);
  }

  
    async _get_data(access,keyv) {
    try {
      let response = await fetch('http://rest-api:8008/state/' + this.address, {
        method: 'GET',
      });
      let responseJson = await response.json();
      var data = responseJson.data;

      var cert_data = new Buffer(data, 'base64').toString();

            console.log("\n\n\n");
            var newobj=[];
            var map1 = new Map();
            var newlycreatedobj;
            var newstr = cert_data.split(',,');
            for(var i=0;i<newstr.length;i++){
                if(newstr[i]=="")               {
                        continue;
                        }else{
                                if(i==(newstr.length-1))
                                {
                                var strl = newstr[newstr.length-1].split('},');
                                newstr[i]=strl[0].concat('}');
                                }
                              newlycreatedobj = JSON.parse(newstr[i]);
                                 map1.set(newlycreatedobj.addharid,newlycreatedobj);
                          newobj[newobj.length] = newlycreatedobj;
                        }

            }

             if(access=="get")
      {
        return newobj;
      }else {

          var d=map1.get(access);
          if(d==undefined)
          {
              return "not found";
          }
          else{
            return "found";
          }

        }



    }
    catch (error) {
      console.error(error);
    }

  }


  async _get_batches_data(){

	  console.log("************************************* get method *******************************************");
    try {
      let response = await fetch('http://rest-api:8008/batches/' + this.address, {
        method: 'GET',
      });
      let responseJson = await response.json();
      return responseJson;
    }
    catch (error) {
      console.error(error);
    }

  }
  

}
module.exports.InsuranceClient = InsuranceClient;
