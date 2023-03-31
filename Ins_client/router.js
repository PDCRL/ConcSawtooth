'use strict';

var express = require('express');
var bodyParser = require('body-parser');
const app = express();
const swaggerJsDoc = require('swagger-jsdoc');
const swaggerUi = require('swagger-ui-express');
const swaggerOptions = {
        swaggerDefinition:{
                openapi:'3.0.0',
                info:{
                  title:'Insurance API',
                  description:" Insurance Api ",
                  servers:["http://10.210.0.124:3002"]

                }
        },
        apis:["router.js"]
};


const swaggerDocs = swaggerJsDoc(swaggerOptions);
app.use('/api-docs',swaggerUi.serve,swaggerUi.setup(swaggerDocs));


app.use(bodyParser.json({ limit: "50mb" }));
app.use(bodyParser.urlencoded({ limit: "50mb", extended: true }));

app.use(function(req, res, next) {
    res.setHeader("Content-Type", "application/json");
    next();
});

// app.use(bodyParser.json());
const fs = require('fs');
const md5 = require("md5");

var { InsuranceClient } = require('./InsuranceClient');
var { insuranceLogic } = require('./insuranceLogic');


var crypto = require("crypto");
const e = require('express');

function prettyJSONString(inputString) {
   return JSON.stringify(JSON.parse(inputString), null, 2);
}



app.use(express.json());
/**
 * @swagger
 * definitions:
 *  insurance:
 *   type: object
 *   properties:
 *    id:
 *     type: string
 *     description: id of the certificate
 */
/**
  * @swagger
  * /api/registeruser:
  *  post:
  *   requestBody:
  *    content:
  *     application/json:
  *      schema:
  *       $ref: '#/definitions/insurance'
  *   responses:
  *    200:
  *     description: inserted succesfully
  */
app.post('/api/registeruser', async function (req, res) {
        try {

                try {

                     var data = req.body.user;
                //     var id = req.body.user.userid;
                     
                     var insurancelogic=new insuranceLogic();
                     var resdata =  await insurancelogic.registeruser(data);
                     
                     return res.json(resdata);

                } finally {

                }
        } catch (error) {
                console.error(`******** FAILED to run the application: ${error}`);
        }

})

app.post('/api/count', async function (req, res) {
        try {

                var data = req.body.user;
                console.log("user data @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",+data);
		var insurancelogic=new insuranceLogic();
	        var cert_length = await insurancelogic.countdata(data.userid,"len");
		if(cert_length=="User is not Available")
		{
		   console.log({status:"fail",result: cert_length});
		   return res.json({ status:"fail",result: cert_length });
		//res.json({status:"success",result:alldata});
		}else{ 
	           	console.log({status:"success",result: cert_length });
			res.json({status:"success",result: cert_length });
		}
        } catch (error) {
                console.error(`******** FAILED to run the application: ${error}`);
        }

})

/**
  * @swagger
  * /api/fetchalldata:
  *  post:
  *   requestBody:
  *    content:
  *     application/json:
  *      schema:
  *       $ref: '#/definitions/certificate'
  *   responses:
  *    200:
  *     description: created succesfully
  */
app.post('/api/fetchalldata', async function (req, res) {
        try {

                try {


                        var data = req.body.user;

			var cert = new certificateLogic();
			var alldata = await cert.retrieveCertificate(data);
			
                        if(alldata=="certificate is not available in ledger!"){
			return res.json({ status:"fail",result: alldata });
			}else{
			return res.json({ status:"success",result: alldata });
			
			}
                } finally {

                }
        } catch (error) {
                console.error(`******** FAILED to run the application: ${error}`);
        }

})




console.log("port is listing on " + app.listen(3002));










