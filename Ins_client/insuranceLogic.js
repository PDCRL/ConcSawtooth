'use strict';

var express = require('express');
const fs = require('fs');
const md5 = require("md5");
//var isBase64 = require('is-base64');
const readline = require('readline');
var batch_file='./batches.txt';
var { InsuranceClient } = require('./InsuranceClient');
var crypto = require("crypto");
var transactions= new Array();

function prettyJSONString(inputString) {
    return JSON.stringify(JSON.parse(inputString), null, 2);
}


async function mapping(transaction){
    return transaction.headerSignature;
    
    }

function sleep(ms) {
return new Promise((resolve) => setTimeout(resolve, ms));
}
 class insuranceLogic{
    constructor() {
    }
  
   async registeruser(data) {

    //var name = 
      //  var random = Math.floor(Math.random() * 1000000);
//	   var name =data.userid;
	 const sentence = data.split(/[, ]+/)
	 //console.log(sentence[1])
	 var  name =sentence[0]//"reg"+ Math.floor(Math.random() * 1000000);
    var pri_key = crypto.createHash("sha256").update(name).digest('hex');
    console.log("*************************************** START  ***************************************************");
    console.log(pri_key);    
	   var insurance_client = new InsuranceClient(pri_key);
        // var regdata = await this.countdata(pri_key);
         var count =0;// await this.countdata(pri_key);
         console.log("****************************************************************"+count);
         if(count=="User is not Available")
         {
            count = 0;
         }
       //  const insurance_data = await insurance_client._get_cookie_count("get");
	//  await insurance_client._get_cookie_count();
	  try{
         var transaction= await insurance_client.send_data("useradd",count, JSON.stringify(data));
        transactions.push(transaction);
         //return "user registered successfully";
         console.log("successful");
         console.log(name);

         return Promise.resolve(transaction);
	  }catch(err){
           return "fail";
	  }	  
     }
    



async countdata(data)
{
            var insurance_client = new InsuranceClient(data);
             const cert_data = await insurance_client._get_data("get");
                       if (cert_data == undefined) {
                           return "User is not Available";
                       }
       console.log("cert length "+cert_data.length);
                       return cert_data.length;
}

};

async function run() {
    // You can use await inside this function block

module.exports.insuranceLogic = insuranceLogic;
let insurance= new insuranceLogic();
var r =readline.createInterface({input: fs.createReadStream(batch_file)});
await r.on('line', async function(text){
    let payloadData = text.toString().split(' ');
    let action=payloadData[0];
    console.log(payloadData[0]);

    let data= payloadData[1];
    if (action="useradd") {
    console.log(data);


      try{
         const transaction = await insurance.registeruser(data);
         
         
         return "success";
      }catch(err){
           return "fail";
      }	    


    } else {
    console.log("count");
    }   });
    
 
    return Promise.resolve(transactions);

}
;

    
async function main(){
var  name ="client";
var pri_key = crypto.createHash("sha256").update(name).digest('hex');    
var insurance_batch = new InsuranceClient(pri_key);
await run();
await sleep(2000);
let txn_headers=await transactions.map(mapping);

//console.log(txn_headers);

await insurance_batch.batch_send(txn_headers,transactions);
}

main();


