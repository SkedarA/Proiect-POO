const fs = require('fs');
const callApi = require('./requestAPI.js');
const exchangeReq = require('./JSON/exchangeRequest.json');
const exchangeDBPath = './NodeJS/JSON/exchangeDB.json';
const exchangeRespPath = './NodeJS/JSON/exchangeResponse.json';

const base = exchangeReq.from.base.toUpperCase();
const amount = exchangeReq.from.amount;
const to = exchangeReq.to.toUpperCase();


doExchange(base, to, amount);

async function doExchange(base, to, amount){
    try{
        // if there is not required exchange base in exchangeDB.json
        if (checkExchange(base)){
            await appendExchange();
        }

        const currency = getExchange(base);

        let result = Math.floor(currency.rates[to] * amount);

        fs.writeFileSync(exchangeRespPath, JSON.stringify(result));
    }
    catch(err){
        console.log(err);
    }
}

function getExchange(base){
    const exchangeDB = JSON.parse(fs.readFileSync(exchangeDBPath, 'utf-8'));

    for (let i = 0; i < exchangeDB.exchanges.length; i++){
        if (exchangeDB.exchanges[i].base === base){
            return exchangeDB.exchanges[i];
        }
    }
    return null;
}

function deleteExchange(base){
    const exchangeDB = JSON.parse(fs.readFileSync(exchangeDBPath, 'utf-8'));

    for (let i = 0; i < exchangeDB.exchanges.length; i++){
        if (exchangeDB.exchanges[i].base === base){
            exchangeDB.exchanges.splice(i, 1);
            fs.writeFileSync(exchangeDBPath, JSON.stringify(exchangeDB));
            return;
        }
    }
}

function checkExchange(base){
    try{
        const exchangeDB = getExchange(base);

        const hoursNow = Math.round(Date.now()/1000/3600);

        // if there is no such base in DB
        if (exchangeDB === null){
            return true;
        }

        // checking if in the last 24 hours was exchange with the same base
        if (exchangeDB.lastUpdate.hours + 24 >= hoursNow){
            // console.log(`Exchange with base "${base}" was already did ${hoursNow - exchangeDB.lastUpdate.hours} hours ago.
            //      Required min 24 hours.`);
            return false;

            // if exchange was done more than 24 hours ago then delete it
        } else{
            deleteExchange(base);
        }

        return true;
    }
    catch(err){
        console.log(err);
    }

}

async function appendExchange(){
    try{
        let response = await callApi.callApi(base);

        let content = JSON.parse(fs.readFileSync(exchangeDBPath, 'utf-8'));

        content.exchanges.push(response);

        fs.writeFileSync(exchangeDBPath, JSON.stringify(content));
    }
    catch(e){
        console.log(e);
    }
}