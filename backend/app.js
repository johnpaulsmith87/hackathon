const Server = require("socket.io");
const SerialPort = require("socket.io-serialport");

const io = new Server(8080);
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
