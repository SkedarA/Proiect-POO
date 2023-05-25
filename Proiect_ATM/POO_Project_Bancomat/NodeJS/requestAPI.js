const request = require('axios');

const apiKey = '8ia57or4ht8246g39h4qpguap1ejimru0pji7itgh6sm2jk8l0t68';

function callApi(base) {
    return new Promise((resolve, reject) => {
        let url = `https://anyapi.io/api/v1/exchange/rates?base=${base}&apiKey=${apiKey}`;

        request.get(url)
            .then(data => {
                let body = data.data;

                const timeNow = new Date();

                body.lastUpdate = {
                    "day": timeNow.getDate(),
                    "month": timeNow.getMonth(),
                    "year": timeNow.getFullYear(),
                    "hours": Math.round(Date.now()/1000/3600)
                };

                resolve(body);
            })
            .catch(err => {
                resolve();
            });
    });
}

module.exports.callApi = callApi;