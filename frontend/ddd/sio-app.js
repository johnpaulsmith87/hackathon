var cors = require('cors')
const app = require('express')();
app.use(cors())
const http = require('http').Server(app);
const io = require('socket.io')(http);

app.get('/', function(req, res){
  res.sendFile(__dirname + '/sio.html');
});

io.on('connection', function(socket){
  console.log('a user connected');
});

http.listen(3031, function(){
  console.log('listening on *:3031');
});