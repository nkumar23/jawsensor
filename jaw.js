/*
run this in the terminal before running this node script
sqlite3 jaw.db

CREATE TABLE datatable (
    reading NUMERIC(9, 2),
    recorded DATETIME DEFAULT CURRENT_TIMESTAMP
);
*/

const mqtt = require('mqtt');
const sqlite = require('sqlite');

const database = 'jaw.db';
const query = 'INSERT INTO datatable (reading) VALUES (?)';

const mqttConnectionString = "mqtts://nikhil:voucher-paternal-soever@itpdtd.com";

const client  = mqtt.connect(mqttConnectionString);

client.on('connect', function () {
  client.subscribe('jawbone');
});

client.on('message', async function (topic, message) {
  console.log(topic, message.toString());

  try {
    const db = await sqlite.open(database, { cached: true });
    const result = await db.run(query, [message]);
    console.log(result);
  } catch(err) {
    console.log(err.stack)
  }
});


// Make sure to change strings to integers
// SELECT MAX(CAST(reading as INTEGER))
// FROM datatable;