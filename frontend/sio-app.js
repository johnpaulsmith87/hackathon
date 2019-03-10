var cors = require('cors')
const app = require('express')();
app.use(cors())
const http = require('http').Server(app);
const io = require('socket.io')(http);
io.origins('*:*') //enable cors

const handleCommand = (command) => {
  console.log(command)
}

io.on('connection', function(socket){
  console.log('a user connected');
  socket.emit('connection', "yo")

  socket.on('debug', console.log)

  socket.on('command', handleCommand)
});

http.listen(3031, function(){
  console.log('listening on *:3031');
});