console.log('index.js started')

const sioUrl = `192.168.43.196:8080`;
const socket = io(sioUrl);

socket.on('connection', function () {
    console.log('connected')
});

const parse = (data) => {
    return JSON.parse(data)
}

const handleData = (data) => {
    document.getElementById('status').innerHTML = data.status
    document.getElementById('velocity').innerHTML = data.velocity
    document.getElementById('ultrasonic').innerHTML = data.ultrasonic
    document.getElementById('angle').innerHTML = data.angle
}

socket.on('data', (data) => { 
    console.log(data)
    handleData(parse(data))
});

const stop = () => {
    console.log('stopping')
    socket.emit('command', 'stop')
}

const start = () => {
    console.log('starting')
    socket.emit('command', 'start')
}