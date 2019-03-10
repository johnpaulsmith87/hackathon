const SerialPort = require("socket.io-serialport");

const app = require('express')()
const cors = require('cors')
app.use(cors())
const http = require('http').Server(app)
const io = require("socket.io")(http);

io.origins('*:*') //enable cors
 
const serialport = new SerialPort({
  io: io,
  route: "/port/ttyS0",
  captureFile: "/var/log/serialport/ttyS0",
  retryPeriod: 1000,
  device: "/dev/ttyS0",
  options: {
    baudrate: 115200
  }
});

serialport.open();
// .then(() => {
//   console.log('port opened');

//   // And when done (shutting down, etc)
//   // serialport.close()
//   // .then(() => {
//   //   console.log('port closed');
//   // });
// });
